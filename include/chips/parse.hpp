#ifndef CHIPS_PARSE_HPP
#define CHIPS_PARSE_HPP

# include "chips/attribute.hpp"
# include "chips/chips_state.hpp"
# include "chips/entity.hpp"
# include "chips/entity_id.hpp"
# include "chips/level.hpp"
# include "chips/position.hpp"
# include "chips/texture_id.hpp"
# include "tinyxml/tinyxml.h"
# include <vector>

namespace chips
{
    void parse(TiXmlElement &, chips_state &);
    void parse(TiXmlElement &, direction &);
    void parse(TiXmlElement &, entity &);
    void parse(TiXmlElement &, entity_id &);
    void parse(TiXmlElement &, std::vector<entity> &);
    void parse(TiXmlElement &, level &);
    void parse(TiXmlElement &, position &);
    void parse(TiXmlElement &, texture_id &);
}                                                           // namespace chips
#endif /* CHIPS_PARSE_HPP */