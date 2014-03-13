#ifndef CHIPS_PARSE_HPP
#define CHIPS_PARSE_HPP

# include "chips/chips_state.hpp"
# include "chips/entity.hpp"
# include "chips/position.hpp"
# include "chips/texture_id.hpp"
# include "tinyxml/tinyxml.h"
# include <string>
# include <map>
# include <vector>
# include <utility>

namespace chips
{
    struct tile_properties
    {
        entity_id id;
        std::vector< std::pair<std::string, std::string> > properties;
    };
    
    struct parsed_level
    {
        unsigned level;
        unsigned chip_count;
        
        std::vector<int> base;
        std::vector<int> items;
        std::vector<int> actors;
    };
    
    std::map<unsigned, tile_properties>
    parse_tileset(std::string const &);
    
    parsed_level parse_level(std::string const &);
    

}                                                           // namespace chips
#endif /* CHIPS_PARSE_HPP */
