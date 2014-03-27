#ifndef CHIPS_CORE_INVENTORY_HPP
#define CHIPS_CORE_INVENTORY_HPP

# include "chips/core/fwd.hpp"
# include "chips/entity/fwd.hpp" /* for attribute_base */
# include <map>

namespace chips
{
    /// an inventory is an attribute that chip has.
    /// It can hold boots and keys. Keys have an associated count.
    /// It also holds the number of computer chips that chip has collected
    class inventory : attribute_base
    {
    public:
        using item_map = std::map<entity_id, unsigned>;
        
    public:
        inventory() = default;
        
        inventory(inventory const &) = default;
        inventory(inventory &&) = default;
        inventory & operator=(inventory const &) = default;
        inventory & operator=(inventory &&) = default;
        
        /// insert the item in non-existent, otherwise increase its count
        void add_item(entity_id item);
        
        /// Decrease an items count, throw if it isn't contained
        void use_item(entity_id item, unsigned count = 1);
        
        /// Remove all instances of an item
        void erase_item(entity_id item);
        
        /// Return the number of a certain item that are held
        unsigned count(entity_id item) const;
        
        /// Return if the inventory contains an item
        bool contains(entity_id item) const;
        
        /// clear the entire inventory
        void clear() { m_items.clear(); }
        
        item_map::iterator begin() { return m_items.begin(); }
        item_map::iterator end()   { return m_items.end();   }
        
        item_map::const_iterator begin() const { return m_items.begin(); }
        item_map::const_iterator end()   const { return m_items.end();   }
        
    private:
        std::map<entity_id, unsigned> m_items;
    };
}                                                           // namespace chips
#endif /* CHIPS_GAME_INVENTORY_HPP */