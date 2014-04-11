#include "chips/game_handler.hpp"
#include "chips/core.hpp"
#include "chips/draw.hpp"
#include "chips/logic.hpp"
#include "chips/entity.hpp"
#include <iostream>
#include <chrono>

#include <elib/fmt.hpp>
#include <string>

namespace chips
{
    using namespace logic;
    
    namespace detail { namespace
    {
        unsigned top_left_coord(unsigned c)
        {
            if (c < 4) 
                return 0;
            if (c > level_width - 5)
                return level_width - level_view_width;
            return c - 4;
        }

        position top_left_position(level const & l)
        {
            position ep = l.chip.get<position>();
            return position{
                detail::top_left_coord(ep.x)
              , detail::top_left_coord(ep.y)
            };
        }
       
        position entity_window_position(position p, entity const & e)
        {
            position ep = e.get<position>();
            return position{
                ((ep.x - p.x) * tile_width)  + level_window_xpos
              , ((ep.y - p.y) * tile_height) + level_window_ypos
            };
        }
    }}                                                      // namespace detail
    
////////////////////////////////////////////////////////////////////////////////
//                                   GAME_HANDLER  
////////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////
    game_event_id game_handler::update(sf::RenderWindow & win)
    {
        while (not m_try_tick()) 
        {
        }
        m_tick(win);
        draw(win);
        return m_event;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool game_handler::m_try_tick()
    {
        auto tp = tick_clock::now();
        if (tp >= m_last_tick) {
            m_last_tick = tp + tick_dur(1);
            m_tick_count = ++m_tick_count % resolution::den;
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
            if (m_event != game_event_id::none) return;
            if (chip_moved) draw(win);
            need_check = true;
        } 
        
        if (need_check && (m_check_success() || m_check_failure())) return; 
    
        if ((m_tick_count % full_speed.den) == 0) {
            m_update();
            m_check_failure();
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool game_handler::m_slide()
    {
        if (m_event != game_event_id::none) return false;
            
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
        if (m_event == game_event_id::level_passed)
        { return true; }
        if (not m_level.chip || m_event != game_event_id::none)
        { return false; }
    
        if (Concept<>(IsExit(), AtEntity(m_level.chip)).contains(m_level.entity_list)) {
            m_event = game_event_id::level_passed;
        }
        return (m_event == game_event_id::level_passed);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool game_handler::m_check_failure()
    {
        if (not m_level.chip) {
            m_event = game_event_id::level_failed;
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
                m_event = game_event_id::level_failed;
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
                m_event = game_event_id::closed;
                return false;
            }
            else if (m_event != game_event_id::none) {
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
        m_draw(win);
        win.display();
    }

    ////////////////////////////////////////////////////////////////////////////
    void game_handler::m_draw(sf::RenderWindow & win) const
    {
        position tl_pos = detail::top_left_position(m_level);
        
        auto should_draw_pred = 
        [&](entity const & e)
        {
            if (!e) return false;
            position ep = e.get<position>();
            return (ep.x >= tl_pos.x && ep.x < tl_pos.x + level_view_width
                 && ep.y >= tl_pos.y && ep.y < tl_pos.y + level_view_height);
        };
        
        for (auto & e : Predicate(should_draw_pred).filter(m_level.entity_list))
        {
            chips::draw(win, e, detail::entity_window_position(tl_pos, e));
        }
        
        m_draw_chip(win, detail::entity_window_position(tl_pos, m_level.chip));
        m_draw_helptext(win);
        m_draw_scoreboard(win);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void game_handler::m_draw_chip(sf::RenderWindow & win, position win_pos) const
    {
        entity const & chip = m_level.chip;
        
        chips_state st = get_chips_state(m_level);
        tile_id tid = static_cast<tile_id>(st);
        
        if (st == chips_state::normal || st == chips_state::swimming)
        {
            if (m_event == game_event_id::level_passed) {
                tid = tile_id::chip_fake_exit;
            } else {
                tid = directional_tile_id(tid, chip.get<direction>());
            }
        }
        
        chips::draw(win, win_pos, tid);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void game_handler::m_draw_helptext(sf::RenderWindow & win) const
    {
        auto CheckHelp = Concept<EntityIs<entity_id::hint>>(
            AtPosition(m_level.chip.get<position>())
          );
        
        std::string msg;
        if (m_event == game_event_id::level_failed) {
            msg = "Level Failed! Better luck next time";
        }
        else if (m_event == game_event_id::level_passed) {
            msg = "Level Passed!";
        }
        else if (CheckHelp.contains(m_level.entity_list)) {
            msg = m_level.help();
        }
        else { return; }
            
        auto & res = resource_manager::get();
        sf::Text txt(msg, res[font_uid::arial], 25);
        txt.setPosition((float)helptext_xpos, (float)helptext_ypos);
        txt.setColor(sf::Color::Red);
        win.draw(txt);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void game_handler::m_draw_scoreboard(sf::RenderWindow & win) const
    {
		auto & rm = resource_manager::get();
		win.draw(rm.scoreboard());
        m_draw_chip_count(win);
        m_draw_inventory(win);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void game_handler::m_draw_chip_count(sf::RenderWindow & win) const
    {
        auto & res = resource_manager::get();
        auto & inv = m_level.chip.get<inventory>();
        
        unsigned req_chips = m_level.chip_count() 
                           - inv.count(entity_id::computer_chip);
        sf::Text txt(
            "Chips Left:\n       " + std::to_string(req_chips)
          , res[font_uid::arial]
          , 25
        );
        txt.setPosition((float)chip_count_xpos, (float)chip_count_ypos);
        txt.setColor(sf::Color::Red);
        win.draw(txt);
    }
    
    void game_handler::m_draw_inventory(sf::RenderWindow & win) const
    {
        auto & res = resource_manager::get();
        auto & inv = m_level.chip.get<inventory>();
        position p(inventory_xpos, inventory_ypos);
        
        /// Draw keys
        int key_pos = static_cast<int>(entity_id::blue_key);
        for (int i=0; i < 4; ++i)
        {
            chips::draw(win, entity(entity_id::floor), p);
            
            entity_id key_id = static_cast<entity_id>(key_pos + i);
            unsigned count = inv.count(key_id);
            if (count)
            {
                chips::draw(win, entity(key_id), p);
                sf::Text count_str(std::to_string(count), res[font_uid::arial], 15);
                count_str.setColor(sf::Color::Red);
                count_str.setPosition((float)p.x, (float)p.y);
                win.draw(count_str);
            }
                
            p.x += tile_width;
        }
        
        /// Draw Boots
        p = position(inventory_xpos, inventory_ypos + tile_height);
        int boot_pos = static_cast<int>(entity_id::flippers);
        for (int i=0; i < 4; ++i)
        {
            chips::draw(win, entity(entity_id::floor), p);
            entity_id boot_id = static_cast<entity_id>(boot_pos + i);
            if (inv.contains(boot_id))
                chips::draw(win, entity(boot_id), p);
            p.x += tile_width;
        }
        
          // TODO Draw weapons
//         p = position(inventory_xpos, inventory_ypos + (tile_height * 2));
//         for (int i=0; i < 4; ++i)
//         {
//             chips::draw(win, entity(entity_id::floor), p);
//             p.x += tile_width;
//         }
    }
    


    
    
}                                                           // namespace chips
