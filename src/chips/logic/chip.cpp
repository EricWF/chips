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
    ////////////////////////////////////////////////////////////////////////////
    void inventory::add_item(entity_id item)
    {
        CHIPS_THROW_BAD_ITEM(item);
        m_items[item]++;
    }
    
    ////////////////////////////////////////////////////////////////////////////
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
            m_items.erase(pos);
        else
            pos->second -= ucount;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void inventory::erase_item(entity_id item)
    {
        CHIPS_THROW_BAD_ITEM(item);
        m_items.erase(item);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    unsigned inventory::count(entity_id item) const
    {
        CHIPS_THROW_BAD_ITEM(item);
        auto pos = m_items.find(item);
        if (pos == m_items.end()) return 0;
        return pos->second;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool inventory::contains(entity_id item) const
    {
        CHIPS_THROW_BAD_ITEM(item);
        return count(item);
    }
    
#undef CHIPS_THROW_BAD_ITEM
    
    ////////////////////////////////////////////////////////////////////////////
    chips_state get_chips_state(level const & l)
    {
        entity const & chip = l.chip;
        auto const & el = l.entity_list;
        
        if (OnWater(chip).contains(el))
        {
            if (chip) return chips_state::swimming;
            else      return chips_state::drowned;
        }
        
        if (OnFire(chip).contains(el))
        {
            if (chip) return chips_state::normal;
            else      return chips_state::burned_fire;
        }
        
        if (OnEntity<entity_id::fake_exit>(chip).contains(el))
        {
            return chips_state::fake_exit;
        }
      
        return chips_state::normal;
    }

}                                                           // namespace chips

namespace chips { namespace logic 
{
    ////////////////////////////////////////////////////////////////////////////
    void init_chip(entity & e, level &)
    {
        ELIB_ASSERT(is_chip(e));
        e.remove<tile_id>();
        
        auto chip_update_ =
        [](entity & self, level & l)
        {
            auto end = l.entity_list.end();
            
            if (!self.get<inventory>().contains(entity_id::skates))
            {
                auto ice_pos = OnIce(self).find(l.entity_list);
                if (ice_pos != end)
                {
                    move_on_ice(self, *ice_pos, l);
                    return;
                }
            }
            
            if (!self.get<inventory>().contains(entity_id::suction_boots))
            {
                auto ff_pos = OnForceFloor(self).find(l.entity_list);
                if (ff_pos != end)
                {
                    move_on_force_floor(self, *ff_pos, l);
                    return;
                }
            }
        };
        
        e << inventory() 
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