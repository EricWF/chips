#include "chips/game_handler.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/draw.hpp"
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
            return c - (level_view_width / 2) - 1;
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
        
        if (ev_id == game_event_id::closed) return ev_id;
        if (!m_level.chip) return game_event_id::level_failed;
        
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