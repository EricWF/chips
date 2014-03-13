#ifndef CHIPS_PARSE_HPP
#define CHIPS_PARSE_HPP

# include "chips/chips_state.hpp"
# include "chips/entity.hpp"
# include "chips/entity_id.hpp"
# include "chips/level.hpp"
# include "chips/position.hpp"
# include "chips/texture_id.hpp"
# include "tinyxml/tinyxml.h"
# include <string>
# include <map>
# include <vector>
# include <utility>

namespace chips
{
    
    using tile_property = std::pair<std::string, std::string>;
    using tile_property_list = std::vector<tile_property>;
    using tile_property_map = std::map<unsigned, tile_property_list>;
    
    struct parsed_level
    {
        unsigned level;
        unsigned chip_count;
        
        std::vector<int> base;
        std::vector<int> items;
        std::vector<int> actors;
    };
    
    tile_property_map parse_tileset(std::string const &);
    
    parsed_level parse_level(std::string const &);
    

}                                                           // namespace chips
#endif /* CHIPS_PARSE_HPP */
