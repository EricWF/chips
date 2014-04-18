#include "chips/game_handler.hpp"
#include "chips/core.hpp"
#include "chips/draw.hpp"
#include "chips/logic.hpp"
#include "chips/entity.hpp"
#include "chips/resource_manager.hpp"
#include <iostream>
#include <chrono>

#include <elib/fmt.hpp>
#include <string>

namespace chips
{
    using namespace logic;
    
    
    
////////////////////////////////////////////////////////////////////////////////
//                                   GAME_HANDLER  
////////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////
    level_state game_handler::update(sf::RenderWindow & win)
    {
        while (not m_try_tick()) 
        {
        }
        m_tick(win);
        draw(win);
        return m_level.state;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool game_handler::m_try_tick()
    {
        auto tp = tick_clock::now();
        if (tp >= m_last_tick) {
            m_last_tick = tp + tick_dur(1);
            ++m_tick_count;
            m_tick_count = m_tick_count % resolution::den;
            return true;
        }
        return false;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void game_handler::m_tick(sf::RenderWindow & win)
    {
        bool need_check = false;
        bool chip_slid = false;
		
        if (m_tick_count % full_speed.den == 0) {
            bool actor_slid = m_slide();
            chip_slid = m_slide_chip();
            if (actor_slid || chip_slid) draw(win);
            need_check = true;
        }
        
        // Update based on input event, then check state.
        if (not chip_slid || (m_tick_count % half_speed.den) == 0) {
            bool chip_moved = m_handle_event(win);
            if (not good()) return;
            if (chip_moved) this->draw(win);
            need_check = true;
        } 
        
        if (need_check && (m_check_success() || m_check_failure())) return; 
    
        if ((m_tick_count % full_speed.den) == 0) {
            m_update();
            m_check_failure();
        }
        
        m_level.update_lists();
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool game_handler::m_slide()
    {
        if (not good()) return false;
            
        bool needs_redraw = false;
        
        // Handle sliding.
        auto CanSlide = Concept<Alive, EntityMatches<is_actor>>();
        for (auto & e : CanSlide.filter(m_level.entity_list)) 
        {
            auto pos = OnIce(e).find(m_level.entity_list);
            if (pos != m_level.entity_list.end()) {
                logic::move_on_ice(e, *pos, m_level);
                e << update_lock{};
                needs_redraw = true;
            }
            else if ((pos = OnForceFloor(e).find(m_level.entity_list)) 
                    != m_level.entity_list.end() )
            {
                logic::move_on_force_floor(e, *pos, m_level);
                e << update_lock{};
                needs_redraw = true;
            }
        }
        
        return needs_redraw;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool game_handler::m_slide_chip()
    {
        entity & chip = m_level.chip;
        auto & elist = m_level.entity_list;
        auto & inv = chip.get<inventory>();
        
        // remove old force floor move and update lock if present
        chip.remove<update_lock>();
        chip.remove<force_floor_move>();
    
        if (not inv.contains(entity_id::skates)) {
            auto pos = OnIce(chip).find(elist);
            if (pos != elist.end()) {
                logic::move_on_ice(chip, *pos, m_level);
                chip << update_lock{};
                return true;
            }
        }
        
        if (not inv.contains(entity_id::suction_boots)) {
            auto pos = OnForceFloor(chip).find(elist);
            if (pos != elist.end()) {
                force_floor_move mv = 
                    logic::move_on_force_floor(chip, *pos, m_level);
                chip << mv;
                return true;
            } 
        }
        
        return false;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void game_handler::m_update()
    {
        // Either remove an update lock, or update a entity
        // if it is their tick.
        auto UpdateThisTick = UpdateOnTick(m_tick_count);
        auto OnTick = Predicate([&](entity const & en) { 
            return UpdateThisTick.check(en) || en.has<update_lock>();
			});
        
        for (auto & e : OnTick.filter(m_level.entity_list)) {
            if (e.has<update_lock>()) {
                e.remove<update_lock>();
                continue;
            }
            e(update_, m_level);
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool game_handler::m_check_success()
    {
        if (m_level.state == level_state::passed)
        { return true; }
        if (not m_level.chip || m_level.state != level_state::in_play)
        { return false; }
    
        if (Concept<>(IsExit(), AtEntity(m_level.chip)).contains(m_level.entity_list)) {
            m_level.state = level_state::passed;
        }
        return (m_level.state == level_state::passed);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool game_handler::m_check_failure()
    {
        if (not m_level.chip) {
            m_level.state = level_state::failed;
            return true;
        }
            
        auto CollidedMonsters =
            Concept<Alive, EntityMatches<&is_monster>>(
                AtPosition(m_level.chip.get<position>())
            );
                  
        for (auto & e : CollidedMonsters.filter(m_level.entity_list))
        {
            if (e.has(on_collision_))
            e(on_collision_, m_level.chip, m_level);
            if (!m_level.chip) {
                m_level.state = level_state::failed;
                return true;
            }
        }
        
        return false;
    }

    
#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wswitch-enum"
#endif
    bool game_handler::m_handle_event(sf::RenderWindow & win)
    {
        bool chip_moved = false;
        sf::Event e;
        while (win.pollEvent(e))
        {
            if (e.type == sf::Event::Closed) {
                m_level.state = level_state::exited;
                return false;
            }
            else if (m_level.state != level_state::in_play) {
                // nothing
            }
            else if (not chip_moved && e.type == sf::Event::KeyReleased) {
                chip_moved = m_move_chip_event(e);
            } 
        }
        return chip_moved;
    }

    ////////////////////////////////////////////////////////////////////////////
    bool game_handler::m_move_chip_event(sf::Event const & ev)
    {
        if (not m_level.chip 
            || m_level.chip.has<move_lock>()
            || m_level.chip.has<update_lock>()
          )
        { return false; }
            
        direction d;
        switch (ev.key.code)
        {
            case sf::Keyboard::Up:
                d = direction::N; break;
            case sf::Keyboard::Down:
                d = direction::S; break;
            case sf::Keyboard::Right:
                d = direction::E; break;
            case sf::Keyboard::Left:
                d = direction::W; break;
            default: return false;
        }
        
        m_level.chip(move_, d, m_level);
        return true;
    }
#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

    void game_handler::draw(sf::RenderWindow & win) const
    {
        win.clear(sf::Color::Black);
        draw_level(win, m_level);
        win.display();
    }

}                                                           // namespace chips
