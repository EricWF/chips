#ifndef CHIPS_GAME_HPP
#define CHIPS_GAME_HPP

# include "chips/core.hpp"
# include "chips/entity.hpp"
# include <map>
# include <vector>
# include <utility>

namespace chips
{
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
        
        std::vector<entity> actors;
        std::vector<entity> items;
        std::vector<entity> base;
        
    private:
        unsigned m_id;
        unsigned m_chip_count;
    };
    
    struct tile_properties
    {
        entity_id id;
        std::vector< std::pair<std::string, std::string> > properties;
    };
    
    std::map<unsigned, tile_properties>
    parse_tileset(std::string const &);
    
    struct parsed_level
    {
        unsigned level;
        unsigned chip_count;
        
        std::vector<int> base;
        std::vector<int> items;
        std::vector<int> actors;
    };
    
    parsed_level parse_level(std::string const &);
    
    /* create a level from its number
     * and the list of properties to pass to the tile factory
     */
    level create_level(
        unsigned level_number, std::map<unsigned, tile_properties> const &
    );
    
    entity create_entity(
        unsigned gid
      , std::map<unsigned, tile_properties> const &
    );
}                                                           // namespace chips
#endif /* CHIPS_GAME_HPP */