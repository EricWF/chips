#include "chips/core/inventory.hpp"
#include "chips/core/error.hpp"
#include "chips/core/id.hpp"
#include <elib/fmt.hpp>


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
   
namespace chips
{

    ////////////////////////////////////////////////////////////////////////////
    void inventory::add_item(entity_id item, unsigned count)
    {
        CHIPS_THROW_BAD_ITEM(item);
        m_items[item] += count;
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
}                                                           // namespace chips