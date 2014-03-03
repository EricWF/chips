#ifndef CHIPS_PARSE_HPP
#define CHIPS_PARSE_HPP

# include "chips/attribute.hpp"
# include "chips/entity.hpp"
# include "chips/level.hpp"
# include "chips/position.hpp"
# include "tinyxml/tinyxml.h"
# include <vector>

namespace chips
{
    void parse(TiXmlElement &, level &);
    void parse(TiXmlElement &, std::vector<entity> &);
    void parse(TiXmlElement &, entity &);
    void parse(TiXmlElement &, entity_id &);
    void parse(TiXmlElement &, direction &);
    void parse(TiXmlElement &, position & );
}                                                           // namespace chips
#endif /* CHIPS_PARSE_HPP */