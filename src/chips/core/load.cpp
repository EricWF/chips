#include "chips/core/load.hpp"
#include "chips/core/direction.hpp"
#include "chips/core/entity_locator.hpp"
#include "chips/core/level.hpp"
#include "chips/core/position.hpp"
#include "chips/logic.hpp"
#include "chips/entity.hpp"
#include "tinyxml/tinyxml.h"
#include <elib/aux.hpp>
#include <elib/fmt.hpp>
#include <elib/lexical_cast.hpp>
#include <algorithm>
#include <iterator>
#include <string>


namespace elib { namespace enumeration
{
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wglobal-constructors"
# pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
#define _(Name) { ::chips::action_type::Name, #Name }
    const std::map<::chips::action_type, std::string>
    basic_enum_traits<::chips::action_type>::name_map =
        {
            _(bind)
          , _(clone)
        };
#undef _
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
}}                                                          // namespace elib

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                               PARSING
////////////////////////////////////////////////////////////////////////////////
    
    namespace detail { namespace 
    {
# if 0
        /// Convert a position on the 32x32 grid to an index in an array
        constexpr std::size_t 
        to_index(position p) noexcept
        {
            return static_cast<std::size_t>(
                (p.y * level_height) + p.x
            );
        }
# endif
    
        /// Convert an index in an array to a position on the 32x32 grid
        constexpr position 
        to_position(std::size_t index) noexcept
        {
            return position{
                static_cast<unsigned>(index % level_height)
              , static_cast<unsigned>(index / level_height)
            };
        }
        
        ////////////////////////////////////////////////////////////////////////
        /// A guarded access wrapper for XML Element Access
        template <class T>
        T query_attr(TiXmlElement const & elem, std::string const & key)
        {
            const char *raw_attr = elem.Attribute(key.c_str());
            if (!raw_attr)
            {
                ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                    "failed to query attribute %s", key
                )));
            }
            
            return elib::lexical_cast<T>(raw_attr);
        }
        
        ////////////////////////////////////////////////////////////////////////
        /// Parse a tiles properties
        std::pair<unsigned, tile_properties> parse_tile(TiXmlElement & elem)
        {
            std::pair<unsigned, tile_properties> tp;
            
            ELIB_ASSERT(elem.Value() == std::string{"tile"});

            tp.first = query_attr<unsigned>(elem, "id");
            
            TiXmlElement *child = elem.FirstChildElement();
            ELIB_ASSERT(child && child->Value() == std::string{"properties"});
            
            child = child->FirstChildElement();
            ELIB_ASSERT(child && child->Value() == std::string{"property"});
            
            bool has_id = false;
            for (; child ; child = child->NextSiblingElement())
            {
                ELIB_ASSERT(child->Value() == std::string{"property"});
                
                std::string name = query_attr<std::string>(*child, "name");
                std::string value = query_attr<std::string>(*child, "value");
                
                // special case
                if (name == "entity_id")
                {
                    tp.second.id = 
                        elib::enumeration::enum_cast<entity_id>(value);
                    has_id = true;
                }
                else
                {
                    tp.second.properties.emplace_back(
                        elib::move(name), elib::move(value)
                    );
                }
            }
            
            ELIB_ASSERT(has_id);
            return tp;
        }
        
        ////////////////////////////////////////////////////////////////////////
        /// Parse a layer of tiled data
        std::vector<int> parse_layer(TiXmlElement & root)
        {
            std::vector<int> tiles;
            
            int test = 0; ((void)test);
            ELIB_ASSERT(root.Attribute("width", &test) && test == 32);
            ELIB_ASSERT(root.Attribute("height", &test) && test == 32);
            
            TiXmlElement *elem = root.FirstChildElement();
            ELIB_ASSERT(elem && elem->Value() == std::string{"data"});
            
            for (elem = elem->FirstChildElement(); elem; elem = elem->NextSiblingElement())
            {
                ELIB_ASSERT(elem->Value() == std::string{"tile"});
                tiles.push_back(query_attr<int>(*elem, "gid"));
            }
            
            ELIB_ASSERT(tiles.size() == 32 * 32);
            return tiles;
        }
        
    }}                                                      // namespace detail
    
    ////////////////////////////////////////////////////////////////////////////
    std::map<unsigned, tile_properties>
    parse_tileset(std::string const & fname)
    {
        std::map<unsigned, tile_properties> tile_map;
        
        TiXmlDocument doc(fname.c_str());
        if (!doc.LoadFile())
        {
            ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                "Failed to open tile properties file %s"
              , fname
            )));
        }
        
        TiXmlHandle root_handle = TiXmlHandle(doc.RootElement());
        TiXmlElement *elem = root_handle.FirstChildElement("tile").Element();
        ELIB_ASSERT(elem);
        
        for (; elem; elem = elem->NextSiblingElement())
            tile_map.insert( detail::parse_tile(*elem) );
            
        return tile_map;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    parsed_level parse_level(unsigned level_num)
    {
        std::string fname = elib::fmt(
            CHIPS_RESOURCE_ROOT "/new_level%u.tmx"
          , level_num
        );
        
        parsed_level lv;
        
        TiXmlDocument doc(fname.c_str());
        if (!doc.LoadFile())
        {
            ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                "Failed to open map file %s"
              , fname
            )));
        }
        
        TiXmlHandle root_handle = TiXmlHandle(doc.RootElement());
        TiXmlElement *elem = root_handle.FirstChildElement("properties").Element();
        ELIB_ASSERT(elem);
        
        for (TiXmlElement *pelem = elem->FirstChildElement()
            ; pelem; pelem = pelem->NextSiblingElement())
        {
            ELIB_ASSERT(pelem->Value() == std::string{"property"});
        
            std::string name = detail::query_attr<std::string>(*pelem, "name");
        
            if (name == "chip_count")
            {
                lv.chip_count = detail::query_attr<unsigned>(*pelem, "value");
            }
            else if (name == "level")
            {
                lv.level = detail::query_attr<unsigned>(*pelem, "value");
            }
            else if (name == "help")
            {
                lv.help = detail::query_attr<std::string>(*pelem, "value");
            }
            else
            {
                ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                    "unknown property %s for level %s"
                  , name, fname
                )));
            }
        }
        
        elem = elem->NextSiblingElement("layer");
        ELIB_ASSERT(elem && elem->Attribute("name") == std::string{"Base"});
        lv.base = elib::move( detail::parse_layer(*elem) );
        
        elem = elem->NextSiblingElement();
        ELIB_ASSERT(elem && elem->Attribute("name") == std::string{"Items"});
        lv.items = elib::move( detail::parse_layer(*elem) );
        
        elem = elem->NextSiblingElement();
        ELIB_ASSERT(elem && elem->Attribute("name") == std::string{"Actors"});
        lv.actors = elib::move( detail::parse_layer(*elem) );
        
        return lv;
    }
    
    namespace detail { namespace 
    {
        entity_locator parse_entity_location(TiXmlElement & elem)
        {
            entity_locator loc;
            loc.id = to_entity_id( query_attr<std::string>(elem, "entity_id") );
            loc.pos.x = query_attr<unsigned>(elem, "x");
            loc.pos.y = query_attr<unsigned>(elem, "y");
            return loc;
        }
        
        parsed_action parse_bind(TiXmlElement & root)
        {
            parsed_action a;
            a.action = action_type::bind;
            
            TiXmlElement *elem = root.FirstChildElement();
            ELIB_ASSERT(elem && elem->Value() == std::string{"source"});
            
            a.actor = parse_entity_location(*elem);
            for (elem = elem->NextSiblingElement()
                ; elem; elem = elem->NextSiblingElement())
            {
                ELIB_ASSERT(elem->Value() == std::string{"to"});
                a.act_on.push_back( parse_entity_location(*elem) );
            }
            
            ELIB_ASSERT(a.act_on.size() > 0);
            return a;
        }
    }}                                                      // namespace detail
    
    std::vector<parsed_action> parse_actions(unsigned level_num)
    {
        std::vector<parsed_action> actions;
        
        std::string fname = elib::fmt(
            CHIPS_RESOURCE_ROOT "actions%u.xml", level_num
        );
        
        TiXmlDocument doc(fname.c_str());
        if (!doc.LoadFile()) return actions;
        
        TiXmlHandle root_handle = TiXmlHandle(doc.RootElement());
        TiXmlElement *elem = root_handle.FirstChildElement("action").Element();
        
        for (; elem; elem = elem->NextSiblingElement())
        {
            ELIB_ASSERT(elem->Value() == std::string{"action"});
            std::string type = detail::query_attr<std::string>(*elem, "type");
            
            parsed_action tmp_action;
            
            if (type == "bind")
            {
                tmp_action = detail::parse_bind(*elem);
            }
            else
            {
                ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                    "Unknown action type %s", type
                )));
            }
            
            actions.push_back(tmp_action);
        }                                                   // end for
        
        return actions;
    }

     namespace detail { namespace
    {
        
        void process_bind(level & l, parsed_action & act)
        {
            auto pos = AtLocation(act.actor).find(l.entity_list);
            ELIB_ASSERT(pos != l.entity_list.end());
            
            entity & actor = *pos;
            REQUIRE_CONCEPT(actor, EntityHas<entity_list>);
            entity_list & elist = actor.get<entity_list>();
            
            for (auto & dest : act.act_on)
            {
                elist->push_back(dest);
            }
        }
        
        void process_actions(level & l)
        {
            std::vector<parsed_action> actions = parse_actions(l.id());
         
            for (auto & act : actions)
            {
                if (act.action == action_type::bind)
                {
                    detail::process_bind(l, act);
                }
                else
                {
                    ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                        "Unknown action type %s", to_string(act.action)
                    )));
                }
            }
        }
        
        void process_level(level & l)
        {
            logic::init(l.chip, l);
            for (auto & e : l.entity_list)
            {
                log::debug("entity: %s", to_string(e.id()));
                logic::init(e, l);
            }
            detail::process_actions(l);
        }
    }}                                                      // namespace detail
        
   
    
////////////////////////////////////////////////////////////////////////////////
//                             CREATION
////////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////
    level create_level(
        unsigned num
      , std::map<unsigned, tile_properties> const & props
      )
    {        
        parsed_level raw = parse_level(num);
        ELIB_ASSERT(raw.base.size()   == level_width * level_height);
        ELIB_ASSERT(raw.items.size()  == level_width * level_height);
        ELIB_ASSERT(raw.actors.size() == level_width * level_height);
        
        // Create the level
        level l(raw.level, raw.chip_count, raw.help);
        
        unsigned index = 0;
        for (auto & gid : raw.base)
        {
            l.entity_list.push_back( 
                create_entity(gid, index, props)
            );
            ++index;
        }
        
        index = 0;
        for (auto & gid : raw.items)
        {
            ++index;
            if (gid == 0) continue;
            l.entity_list.push_back(create_entity(gid, index - 1, props));
        }
        
        index = 0;
        for (auto & gid : raw.actors)
        {
            entity tmp = create_entity(gid, index, props);
            if (is_chip(tmp))
            {
                ELIB_ASSERT(!l.chip);
                l.chip = tmp;
            }
            else if (tmp)
            {
                l.entity_list.push_back( tmp );
            }
            ++index;
        }
    
        ELIB_ASSERT(l.chip);
    
        detail::process_level(l);
    
        return l;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    entity create_entity(
        int gid, unsigned index
      , std::map<unsigned, tile_properties> const & pm
      )
    {
        // get the position of the entity via the index.
        position pos = detail::to_position(index);
        
        // if the gid is zero there is no entity at that location
        // create a dead entity and return it.
        if (gid == 0)
        {
            entity e;
            e << pos;
            return e;
        }
        
        gid -= 1;
        auto props = pm.at(static_cast<unsigned>(gid));
        
        entity e(props.id, pos);
        
        for (auto & p : props.properties)
        {
            if (p.first == "tile_id")
            {
                e.set( to_tile_id(p.second) );
            }
            else if (p.first == "direction")
            {
                e.set( to_direction(p.second) );
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