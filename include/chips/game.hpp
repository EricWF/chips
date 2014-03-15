#ifndef CHIPS_GAME_HPP
#define CHIPS_GAME_HPP

# include "chips/core.hpp"
# include "chips/entity.hpp"
# include <map>
# include <string>
# include <utility>
# include <vector>

namespace chips
{
    /// TODO make this better.
    /// only access to id and chip_count are guarded since
    /// they should not be changed.
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
        
        /// TODO: maybe? It makes iteration easier.
        std::vector<entity*> all;
    private:
        unsigned m_id;
        unsigned m_chip_count;
    };
    
////////////////////////////////////////////////////////////////////////////////
//                              PARSING
////////////////////////////////////////////////////////////////////////////////
    
    /// Parsed from XML. The entity ID is required
    /// the rest of the properties are left as name value pairs to be parsed
    /// elsewhere 
    struct tile_properties
    {
        entity_id id;
        std::vector< std::pair<std::string, std::string> > properties;
    };
    
    /// Parse the tileset information into a [UID -> Properties] map
    std::map<unsigned, tile_properties> 
    parse_tileset(std::string const &);
    
    /// The raw level data as parsed from the XML. 
    struct parsed_level
    {
        unsigned level;
        unsigned chip_count;
        
        std::vector<int> base;
        std::vector<int> items;
        std::vector<int> actors;
    };
    
    /// Parse XML output by tiled into an intermediate representation
    parsed_level parse_level(std::string const &);
    
////////////////////////////////////////////////////////////////////////////////
//                           CREATION
////////////////////////////////////////////////////////////////////////////////
    /// create a level from its number
    /// and the list of properties to pass to the tile factory
    level create_level(
        unsigned level_number, std::map<unsigned, tile_properties> const &
    );
    
    /// Create an entity from its tile gid and a list of entity properties
    /// NOTE: gid has not been changed, that means that 0 represents empty
    /// and it must be translated to the coorisponding key for the 
    /// properties map
    entity create_entity(
        unsigned gid
      , std::map<unsigned, tile_properties> const &
    );
}                                                           // namespace chips
#endif /* CHIPS_GAME_HPP */