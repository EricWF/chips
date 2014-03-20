#include "chips/game_handler.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/draw.hpp"
#include "chips/resource_manager.hpp"
# include <elib/fmt.hpp>
# include <string>

namespace chips
{
    namespace detail { namespace
    {
        unsigned top_left_coord(unsigned c)
        {
            if (c < level_view_width) 
                return 0;
            if (c > level_width - level_view_width)
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
        if (m_level.chip.get<chip_at_exit>()) 
        {
            log::info("Game: level passed");
            return game_event_id::level_passed;
        }
        return game_event_id::none;
    }
    
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
        
        chips::draw(
            win, m_level.chip
          , detail::entity_window_position(tl_pos, m_level.chip)
        );
        
        m_draw_scoreboard(win);
        
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

    void game_handler::m_move_chip_event(sf::Event const & ev)
    {
        if (sf::Keyboard::Up == ev.key.code)
            m_move_chip(direction::N);
        else if (sf::Keyboard::Down == ev.key.code)
            m_move_chip(direction::S);
        else if (sf::Keyboard::Right == ev.key.code)
            m_move_chip(direction::E);
        else if (sf::Keyboard::Left)
            m_move_chip(direction::W);
        
    }
    
    void game_handler::m_move_chip(direction d)
    {
        entity & chip = m_level.chip;
        ELIB_ASSERT(chip);
        position old_pos = chip.get<position>();
        
        chip(move_in_, d, 1u);
       
        for (auto & e : AtPosition(chip.get<position>()).filter(m_level.entity_list))
        {
            if (!e) continue;
            if (e && e.has(on_collision_))
                e(on_collision_, chip);
            if (e && e.has(collides_) && e(collides_, chip))
            {
                chip << old_pos;
                break;
            }
        }
    }
}                                                           // namespace chips