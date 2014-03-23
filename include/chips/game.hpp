#ifndef CHIPS_GAME_HPP
#define CHIPS_GAME_HPP

# include "chips/core.hpp"
# include "chips/entity.hpp"
# include "chips/logic/fwd.hpp"
# include <elib/aux.hpp>
# include <elib/enumeration.hpp>
# include <elib/lexical_cast.hpp>
# include <map>
# include <string>
# include <vector>
# include <utility>
# include <cstddef>

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                                LEVEL
////////////////////////////////////////////////////////////////////////////////

    class level
    {
    public:
        level(unsigned xid, unsigned xchip_count, std::string help_str)
          : m_id(xid), m_chip_count(xchip_count), m_help(help_str)
        {}
        
        ELIB_DEFAULT_COPY_MOVE(level);
        
        /// The level number
        unsigned id() const noexcept { return m_id; }
        void id(unsigned xid) { m_id = xid; }
        
        /// The number of computer chips in the level
        unsigned chip_count() const noexcept { return m_chip_count; }
        void chip_count(unsigned c) { m_chip_count = c; }
        
        /// return the help text for the level.
        std::string const & help() const noexcept { return m_help; }
        
        entity chip;
        std::vector<entity> entity_list;
        
    private:
        unsigned m_id;
        unsigned m_chip_count;
        std::string m_help;
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
        std::string help;
        
        std::vector<int> base;
        std::vector<int> items;
        std::vector<int> actors;
    };
    
    /// Parse XML output by tiled into an intermediate representation
    parsed_level parse_level(unsigned level);
    
    enum class action_type
    {
        none,
        /// bind a trigger to another entity
        bind, 
        clone
    };
}                                                           // namespace chips

namespace elib { namespace enumeration
{
    template <>
    struct basic_enum_traits<::chips::action_type>
    {
        static const std::map<::chips::action_type, std::string> name_map;
    };
}}                                                          // namespace elib
    
namespace chips
{
    inline std::string to_string(action_type a) 
    {
        return elib::enumeration::enum_cast<std::string>(a);
    }
    
    inline action_type to_action_type(std::string const & s)
    {
        return elib::enumeration::enum_cast<action_type>(s);
    }  
    
    /// The type of action parsed
    struct parsed_action
    {
        /// An enumeration representing the action
        action_type action;
        
        /// The entity that requested the action
        entity_locator actor;
        
        /// The entities to act on
        /// USED BY:
        ///  - bind (buttons & teleports)
        std::vector<entity_locator> act_on;
    };
    
    std::vector<parsed_action> parse_actions(unsigned level);
    
////////////////////////////////////////////////////////////////////////////////
//                              CREATION
////////////////////////////////////////////////////////////////////////////////
    /// create a level from its number
    /// and the list of properties to pass to the tile factory
    level create_level(
        unsigned level_number
      , std::map<unsigned, tile_properties> const &
    );
    
    /// Create an entity from its tile gid and a list of entity properties
    /// NOTE: gid has not been changed, that means that 0 represents empty
    /// and it must be translated to the coorisponding key for the 
    /// properties map
    entity create_entity(
        int gid, unsigned index
      , std::map<unsigned, tile_properties> const &
    );
    
    void process_level(level &);
}                                                           // namespace chips
#endif /* CHIPS_GAME_HPP */