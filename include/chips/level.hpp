#ifndef CHIPS_LEVEL_HPP
#define CHIPS_LEVEL_HPP

# include "chips/config.hpp"
# include "chips/entity.hpp"
# include "chips/parse.hpp"
# include <array>
# include <map>
# include <vector>

namespace chips
{
    // just a fancy way to declare an array. its really just entity[32][32] 
    // but with no pointer decay!
    using level_grid_t = 
        std::array<
            std::array<entity, level_height>
          , level_width
        >;
    
    // TODO make this better.
    // only access to id and chip_count are gaurded since
    // they should not be changed.
    class level
    {
    public:
        level(unsigned xid, unsigned xchip_count)
          : m_id(xid), m_chip_count(xchip_count)
        {}
        
        level(level const &) = default;
        level(level &&) = default;
        level & operator=(level const &) = default;
        level & operator=(level &&) = default;
        
        unsigned id() const noexcept { return m_id; }
        unsigned chip_count() const noexcept { return m_chip_count; }
        
        void id(unsigned xid) { m_id = xid; }
        void chip_count(unsigned c) { m_chip_count = c; }
        
        level_grid_t actors;
        level_grid_t items;
        level_grid_t base;
        
    private:
        unsigned m_id;
        unsigned m_chip_count;
    };
	
	/* create a level from its number
     * and the list of properties to pass to the tile factory
     */
    level create_level(
        unsigned level_number, std::map<unsigned, tile_properties> const &
    );
    
}                                                           // namespace chips
#endif /* CHIPS_LEVEL_HPP */
