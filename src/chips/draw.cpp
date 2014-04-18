#include "chips/draw.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/logic.hpp"
#include "chips/resource_manager.hpp"
#include <elib/aux.hpp>
#include <elib/fmt.hpp>

namespace chips
{
    ////////////////////////////////////////////////////////////////////////////
    unsigned top_left_window_coord(unsigned chip_dim)
    {
        if (chip_dim < 4) 
            return 0;
        if (chip_dim > level_width - 5)
            return level_width - level_view_width;
        return chip_dim - 4;
    }

    ////////////////////////////////////////////////////////////////////////////
    position top_left_window_position(level const & l)
    {
        position p = l.chip.get<position>();
        return position{
            top_left_window_coord(p.x)
          , top_left_window_coord(p.y)
        };
    }
       
    ////////////////////////////////////////////////////////////////////////////
    position position_in_window(position top_left, entity const & e)
    {
        position ep = e.get<position>();
        return position{
              ((ep.x - top_left.x) * tile_width)  + level_window_xpos
            , ((ep.y - top_left.y) * tile_height) + level_window_ypos
        };
    }

    ////////////////////////////////////////////////////////////////////////////
    void draw_level(sf::RenderWindow & win, level const & l)
    {
        draw_playing_field(win, l);
        draw_scoreboard(win, l);
        draw_helptext(win, l);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void draw_playing_field(sf::RenderWindow & win, level const & l)
    {
        auto top_left_pos = top_left_window_position(l);
        draw_entities(win, top_left_pos, l);
        draw_chip(win, top_left_pos, l);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void draw_entities(sf::RenderWindow & win, position tl_pos, level const & lvl)
    {
        auto should_draw_pred = 
        [&](entity const & e)
        {
            if (not e) return false;
            position ep = e.get<position>();
            return (ep.x >= tl_pos.x && ep.x < tl_pos.x + level_view_width
                 && ep.y >= tl_pos.y && ep.y < tl_pos.y + level_view_height);
        };
        
        for (auto & e : Predicate(should_draw_pred).filter(lvl.entity_list))
        {
            ELIB_ASSERT(e);
            draw_entity_at(win, e, position_in_window(tl_pos, e));
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void draw_chip(sf::RenderWindow & win, position tl_pos, level const & lvl)
    {
        entity const & chip = lvl.chip;
        
        chips_state st = logic::get_chips_state(lvl);
        tile_id tid = static_cast<tile_id>(st);
        
        if (st == chips_state::normal || st == chips_state::swimming)
        {
            if (lvl.state == level_state::passed) {
                tid = tile_id::chip_fake_exit;
            } else {
                tid = directional_tile_id(tid, chip.get<direction>());
            }
        }
        
        draw_tile_at(win, tid, position_in_window(tl_pos, chip));
    }
    
    
    
    ////////////////////////////////////////////////////////////////////////////
    void draw_scoreboard(sf::RenderWindow & win, level const & lvl)
    {
        auto & rm = resource_manager::get();
        win.draw(rm.scoreboard());
        draw_chip_count(win, lvl);
        draw_inventory(win, lvl);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void draw_chip_count(sf::RenderWindow & win, level const & lvl)
    {
        auto & res = resource_manager::get();
        auto & inv = lvl.chip.get<inventory>();
        
        unsigned req_chips = lvl.chip_count() 
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
    
    ////////////////////////////////////////////////////////////////////////////
    void draw_inventory(sf::RenderWindow & win, level const & lvl)
    {
        auto & res = resource_manager::get();
        auto & inv = lvl.chip.get<inventory>();
        position p(inventory_xpos, inventory_ypos);
        
        /// Draw keys
        int key_pos = static_cast<int>(entity_id::blue_key);
        for (int i=0; i < 4; ++i)
        {
            draw_tile_at(win, tile_id::floor, p);
            
            entity_id key_id = static_cast<entity_id>(key_pos + i);
            unsigned count = inv.count(key_id);
            if (count)
            {
                draw_tile_at(win, key_id, p);
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
            draw_tile_at(win, tile_id::floor, p);
            entity_id boot_id = static_cast<entity_id>(boot_pos + i);
            if (inv.contains(boot_id))
                draw_tile_at(win, boot_id, p);
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
    
    ////////////////////////////////////////////////////////////////////////////
    void draw_helptext(sf::RenderWindow & win, level const & lvl)
    {
        auto CheckHelp = Concept<EntityIs<entity_id::hint>>(
            AtPosition(lvl.chip.get<position>())
          );
        
        std::string msg;
        if (lvl.state == level_state::failed) {
            msg = "Level Failed! Better luck next time";
        }
        else if (lvl.state == level_state::passed) {
            msg = "Level Passed!";
        }
        else if (CheckHelp.contains(lvl.entity_list)) {
            msg = lvl.help();
        }
        else { return; }
            
        auto & res = resource_manager::get();
        sf::Text txt(msg, res[font_uid::arial], 25);
        txt.setPosition((float)helptext_xpos, (float)helptext_ypos);
        txt.setColor(sf::Color::Red);
        win.draw(txt);
    }

    ////////////////////////////////////////////////////////////////////////////
    bool draw_entity_at(sf::RenderWindow & to, entity const & e, position at)
    {
        // Don't draw if the entity is dead
        if (!e) return false;
        
        tile_id tid;
        if (e.has<tile_id>()) {
            tid = e.get<tile_id>();
        }
        else {
            tid = static_cast<tile_id>(e.id());
        }
            
        if (e.has<direction>()){
            tid = directional_tile_id(tid, e.get<direction>());
        }

        draw_tile_at(to, tid, at);
        return true;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void draw_tile_at(sf::RenderWindow & to, tile_id index, position p)
    {
        auto & rm = resource_manager::get();
        sf::Sprite & s = rm[index];
        s.setPosition(
            static_cast<float>(p.x)
          , static_cast<float>(p.y)
        );
        to.draw(s);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void draw_tile_at(sf::RenderWindow & win, entity_id id, position p)
    {
        draw_tile_at(win, static_cast<tile_id>(id), p);
    }
}                                                           // namespace chips
