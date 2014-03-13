#ifndef CHIPS_ENTITY_FACTORY_HPP
#define CHIPS_ENTITY_FACTORY_HPP

# include "chips/entity.hpp"
# include "chips/parse.hpp"
# include <map>

namespace chips
{
    entity create_entity(
        unsigned gid
      , std::map<unsigned, tile_properties> const &
    );
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_FACTORY_HPP */