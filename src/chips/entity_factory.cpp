#include "chips/entity_factory.hpp"
#include "chips/core.hpp"
#include "chips/error.hpp"
#include "chips/parse.hpp"
#include <elib/fmt.hpp>
#include <elib/enumeration.hpp>
#include <elib/lexical_cast.hpp>
#include <string>
#include <utility>

namespace chips
{
   
    entity create_entity(
        unsigned gid
      , std::map<unsigned, tile_properties> const & pm
      )
    {
        auto props = pm.at(gid);
        entity e(props.id);
        
        for (auto & p : props.properties)
        {
            if (p.first == "tile_id")
            {
                e.insert_attribute(
                    elib::enumeration::enum_cast<texture_id>(p.second)
                );
            }
            else if (p.first == "direction")
            {
                direction d;
                if      (p.second == "N") d = direction::N;
                else if (p.second == "W") d = direction::W;
                else if (p.second == "S") d = direction::S;
                else if (p.second == "E") d = direction::E;
                else
                {
                    ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                        "unknown direction %s", p.second
                    )));
                }
                e.insert_attribute(d);
            }
            else
            {
                ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                    "unknown tile property <property name=\"%s\" value=\"%s\" />"
                  , p.first, p.second
                )));
            }
        }                                                   // end for
        
        return e;
    }
}                                                           // namespace chips