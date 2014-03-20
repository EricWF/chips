#include "chips/logic.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/game.hpp"
#include <elib/aux.hpp>
#include <elib/fmt.hpp>

namespace chips
{ 
  
////////////////////////////////////////////////////////////////////////////////
//                              INVENTORY
////////////////////////////////////////////////////////////////////////////////
    
#define CHIPS_THROW_BAD_ITEM(Item)                    \
do {                                                  \
    if (!is_item(Item))                               \
    {                                                 \
        ELIB_THROW_EXCEPTION(chips_error(elib::fmt(   \
            "Item %s cannot be added to an inventory" \
          , to_string(Item)                           \
        )));                                          \
    }                                                 \
} while (false)
#
    void inventory::add_item(entity_id item)
    {
        CHIPS_THROW_BAD_ITEM(item);
        m_items[item]++;
    }
    
    void inventory::use_item(entity_id item, unsigned ucount)
    {
        CHIPS_THROW_BAD_ITEM(item);
        
        auto pos = m_items.find(item);
        if (pos == m_items.end())
        {
            ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                "Inventory has no instances of item %s"
              , to_string(item)
            )));
        }
        
        ELIB_ASSERT(pos->second > 0);
        
        if (pos->second <= ucount)
        {
            m_items.erase(pos);
        }
        else
        {
            pos->second -= ucount;
        }
    }
    
    void inventory::erase_item(entity_id item)
    {
        CHIPS_THROW_BAD_ITEM(item);
        m_items.erase(item);
    }
    
    unsigned inventory::count(entity_id item) const
    {
        CHIPS_THROW_BAD_ITEM(item);
        auto pos = m_items.find(item);
        if (pos == m_items.end()) return 0;
        return pos->second;
    }
    
    bool inventory::contains(entity_id item) const
    {
        CHIPS_THROW_BAD_ITEM(item);
        return count(item);
    }
    
#undef CHIPS_THROW_BAD_ITEM
}                                                           // namespace chips

namespace chips { namespace logic 
{

    void init_chip(entity & e, level &)
    {
        ELIB_ASSERT(is_chip(e));
        e.remove<tile_id>();
        
        auto chip_update_ =
        [](entity & self, level & l)
        {
            auto & inv = self.get<inventory>();
            
            auto InWater = Concept<EntityIs<entity_id::water>>(
                AtPosition(self.get<position>())
            );
            
            auto InFire = Concept<EntityIs<entity_id::fire>>(
                AtPosition(self.get<position>())
            );
            
            auto OnIce = Concept<EntityIs<entity_id::ice>>(
                AtPosition(self.get<position>())
            );
            
            auto OnForceFloor = Concept<EntityIs<entity_id::force_floor>>(
                AtPosition(self.get<position>())
            );
            
            self << moveable(true);
            if (InWater.contains(l.entity_list))
            {
                self.remove<texture_type>();
                if (self)
                    self << static_cast<tile_id>(chips_state::swimming);
                else
                {
                    self << static_cast<tile_id>(chips_state::drowned);
                    self.remove<direction>();
                }
            }
            else if (InFire.contains(l.entity_list))
            {
                if (!self)
                {
                    self << static_cast<tile_id>(chips_state::burned_fire);
                    self.remove<texture_type>();
                    self.remove<direction>();
                }
            }
            else if (OnIce.contains(l.entity_list) && !inv.contains(entity_id::skates))
            {
                self << moveable(false);
                auto & ice = OnIce.find(l.entity_list);
                tile_id tid = ice.get<tile_id>();
                direction d = self.get<direction>();
                if (tid == tile_id::ice_NW)
                {
                    if (d == direction::N)
                        self(move_, direction::E, l);
                    else
                        self(move_, direction::S, l);
                }
                else if (tid == tile_id::ice_NE)
                {
                    if (d == direction::N)
                        self(move_, direction::W, l);
                    else
                        self(move_, direction::S, l);
                }
                else if (tid == tile_id::ice_SW)
                {
                    if (d == direction::S)
                        self(move_, direction::E, l);
                    else
                        self(move_, direction::N, l);
                }
                else if (tid == tile_id::ice_SE)
                {
                    if (d == direction::S)
                        self(move_, direction::W, l);
                    else
                        self(move_, direction::N, l);
                }
                else
                {
                    self(move_, self.get<direction>(), l);
                }
            }
            else if (OnForceFloor.contains(l.entity_list))
            {
                for(auto & ff : OnForceFloor.filter(l.entity_list))
                {
                    if (!inv.contains(entity_id::suction_boots))
                    {
                        if (ff.get<tile_id>() == tile_id::force_floor_N)
                            self(move_, direction::N, l);
                        else if (ff.get<tile_id>() == tile_id::force_floor_S)
                            self(move_, direction::S, l);
                        else if (ff.get<tile_id>() == tile_id::force_floor_E)
                            self(move_, direction::E, l);
                        else if (ff.get<tile_id>() == tile_id::force_floor_W)
                            self(move_, direction::W, l);
                        self << moveable(false);
                    }
                }
            }
            else
            {
                self.remove<tile_id>();
                self << texture_type::cutout;
            }
        };
        
        e << texture_type::cutout << chips_state::normal 
          << inventory() << chip_at_exit(false)
          << method(update_, chip_update_)
          << method(move_, common::move_);
          
        
    }
    
    void process_chip(entity &, level &)
    {
    }
    
    void finalize_chip(entity &, level &)
    {
    }
}}                                                          // namespace chips