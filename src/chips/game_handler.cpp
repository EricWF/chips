#include "chips/game_handler.hpp"
#include "chips/core.hpp"
#include "chips/draw.hpp"
#include "chips/entity.hpp"
#include "chips/logic.hpp"
#include "chips/resource_manager.hpp"
#include <elib/fmt.hpp>
#include <string>

namespace chips
{
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
    
    ////////////////////////////////////////////////////////////////////////////
    //                           GAME_HANDLER  
    ////////////////////////////////////////////////////////////////////////////
    
    game_event_id game_handler::update(sf::RenderWindow & win)
    {
        if (m_tick < std::chrono::high_resolution_clock::now())
        {
            for (auto & e : Concept<Alive, EntityHas<update_m>>()
                           .filter(m_level.entity_list))
            {
                e(update_, m_level);
            }
            
            auto CollidedMonsters =
              Concept<Alive, EntityMatches<&is_monster>>(
                 AtPosition(m_level.chip.get<position>())
                );
                  
            for (auto & e : CollidedMonsters.filter(m_level.entity_list))
            {
              if (e.has(on_collision_))
                e(on_collision_, m_level.chip, m_level);
              if (!m_level.chip) return game_event_id::level_failed;
            }
              
            m_level.chip(update_, m_level);
            
            m_tick = std::chrono::high_resolution_clock::now() 
                    + std::chrono::milliseconds(200);
        }
        
        
        game_event_id ev_id = m_handle_event(win);
        if (ev_id == game_event_id::closed) 
        {
            log::info("Game: Window closed");
            return ev_id;
        }
        if (!m_level.chip) 
        {
            log::info("Game: level failed");
            return game_event_id::level_failed;
        }
        
        if (Concept<>(IsExit(), AtEntity(m_level.chip)).contains(m_level.entity_list))
        {
            log::info("Game: level passed");
            return game_event_id::level_passed;
        }
            
        return game_event_id::none;
    }
    
#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wswitch-enum"
#endif
    void game_handler::m_move_chip_event(sf::Event const & ev)
    {
        if (!CanMove()(m_level.chip)) return;
            
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
            default: return;
        }
        
        m_level.chip(move_, d, m_level);
    }
#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif
    void game_handler::draw(sf::RenderWindow & win) const
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
        
        m_draw_scoreboard(win);
        
    }
    
    void game_handler::m_draw_chip(sf::RenderWindow & win, position win_pos) const
    {
        entity const & chip = m_level.chip;
        
        entity chip_dummy(entity_id::chip);
        
        chips_state st = get_chips_state(m_level);
        chip_dummy << static_cast<tile_id>(st);
        
        if (st == chips_state::normal)
        {
            chip_dummy << chip.get<direction>()
                       << texture_type::cutout; 
        }
        else if (st == chips_state::swimming) 
        {
            chip_dummy << chip.get<direction>();
        }
        
        chips::draw(win, chip_dummy, win_pos);
    }
    
    void game_handler::m_draw_scoreboard(sf::RenderWindow & win) const
    {
        sf::RectangleShape rect{
            sf::Vector2f((float)scoreboard_width, (float)scoreboard_height)
        };
        rect.setPosition((float)scoreboard_xpos, (float)scoreboard_ypos);
        rect.setFillColor(sf::Color(192, 192, 192));
        win.draw(rect);
        
        m_draw_chip_count(win);
        m_draw_inventory(win);
    }
    
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
        
        int key_pos = static_cast<int>(entity_id::blue_key);
        for (int i=0; i < 4; ++i)
        {
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
            else
                chips::draw(win, entity(entity_id::floor), p);
            p.x += tile_width;
        }
        
        p = position(inventory_xpos, inventory_ypos + tile_height);
        int boot_pos = static_cast<int>(entity_id::flippers);
        for (int i=0; i < 4; ++i)
        {
            entity_id boot_id = static_cast<entity_id>(boot_pos + i);
            if (inv.contains(boot_id))
                chips::draw(win, entity(boot_id), p);
            else
                chips::draw(win, entity(entity_id::floor), p);
            p.x += tile_width;
        }
    }
    
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wswitch-enum"
#elif defined(ELIB_CONFIG_GCC)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wswitch-enum"
#endif
    game_event_id game_handler::m_handle_event(sf::RenderWindow & win)
    {
        sf::Event e;
        while (win.pollEvent(e))
        {
            switch (e.type)
            {
                case sf::Event::Closed:
                    return game_event_id::closed;
                case sf::Event::KeyPressed:
                    m_move_chip_event(e);
                    break;
                default: break;
            }
        }
        return game_event_id::none;
    }
#if defined(__clang__)
# pragma clang diagnostic pop
#elif defined(ELIB_CONFIG_GCC)
# pragma GCC diagnostic pop
#endif

    
    
}                                                           // namespace chips