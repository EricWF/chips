#include "chips/game.hpp"
#include "chips/entity.hpp"
#include "tinyxml/tinyxml.h"
#include <elib/aux.hpp>
#include <elib/enumeration.hpp>
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
        };
#undef _
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
}}                                                          // namespace elib

namespace chips
{
    
    
////////////////////////////////////////////////////////////////////////////////
//                              INVENTORY
////////////////////////////////////////////////////////////////////////////////
    
#define CHIPS_THROW_BAD_ITEM(Item)                    \
do {                                                  \
    if (!is_item(Item))                               \
    {                                                 \
        ELIB_THROW_EXCEPTION(chips_error(elib::fmt(   \
            "Item %s cannot be added to an inventory" \
          , to_string(Item)                           \
        )));                                          \
    }                                                 \
} while (false)
#
    void inventory::add_item(entity_id item)
    {
        CHIPS_THROW_BAD_ITEM(item);
        m_items[item]++;
    }
    
    void inventory::use_item(entity_id item)
    {
        CHIPS_THROW_BAD_ITEM(item);
        
        auto pos = m_items.find(item);
        if (pos == m_items.end())
        {
            ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                "Inventory has no instances of item %s"
              , to_string(item)
            )));
        }
        
        ELIB_ASSERT(pos->second > 0);
        
        pos->second--;
        if (pos->second == 0) m_items.erase(pos);
    }
    
    void inventory::erase_item(entity_id item)
    {
        CHIPS_THROW_BAD_ITEM(item);
        m_items.erase(item);
    }
    
    unsigned inventory::count(entity_id item) const
    {
        CHIPS_THROW_BAD_ITEM(item);
        auto pos = m_items.find(item);
        if (pos == m_items.end()) return 0;
        return pos->second;
    }
    
    bool inventory::contains(entity_id item) const
    {
        CHIPS_THROW_BAD_ITEM(item);
        return count(item);
    }
    
#undef CHIPS_THROW_BAD_ITEM
////////////////////////////////////////////////////////////////////////////////
//                               LEVEL
////////////////////////////////////////////////////////////////////////////////
    

    
////////////////////////////////////////////////////////////////////////////////
//                               PARSING
////////////////////////////////////////////////////////////////////////////////
    
    namespace detail { namespace 
    {
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
            
            ELIB_ASSERT(has_id && tp.second.properties.size() >= 1);
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
            CHIPS_RESOURCE_ROOT "/level%u.tmx"
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
        entity_location parse_entity_location(TiXmlElement & elem)
        {
            entity_location loc;
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
            CHIPS_RESOURCE_ROOT "level%u.actions", level_num
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
    
        process_level(l);
    
        return l;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    entity create_entity(
        int gid, unsigned index
      , std::map<unsigned, tile_properties> const & pm
      )
    {
        // get the position of the entity via the index.
        position pos = to_position(index);
        
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
            else if (p.first == "velocity")
            {
                e.set( to_velocity(p.second) );
            }
            else if (p.first == "toggle_state")
            {
                e << to_toggle_state(p.second) 
                  << method(toggle_, common::toggle_);
            }
            else if (p.first == "bindings")
            {
                ELIB_ASSERT(p.second == "");
                e << bindings() << method(notify_, common::notify_);
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
    
    void init_chip(entity & e)
    {
        ELIB_ASSERT(is_chip(e));
        e.remove<tile_id>();
        e << inventory() << chips_state::normal 
          << method(move_in_, common::move_in_)
          << texture_type::cutout;
    }
    
    void init_actor(entity & e)
    {
        ELIB_ASSERT(is_actor(e) && !is_chip(e));
        if (is_monster(e)) detail::init_monster(e);
    }
    
    void init_item(entity & e)
    {
        ELIB_ASSERT(is_item(e)); ((void)e);
    }
    
    void init_base(entity & e)
    {
        ELIB_ASSERT(is_base(e));
        if (is_floor(e)) detail::init_floor(e);
        if (is_wall(e))  detail::init_wall(e);
    }
    
    namespace detail
    {
        void init_monster(entity & e)
        {
            ELIB_ASSERT(is_monster(e));
            REQUIRE_CONCEPT(e, Directional);
            e.remove<tile_id>();
            // TODO remove this
            // add a default velocity if none is present.
            if (!e.has<velocity>())
                e << velocity(1);
                
            e << method(move_, common::move_)
              << texture_type::cutout;
        }
        
        void init_wall(entity & e)
        {
            ELIB_ASSERT(is_wall(e));
            if (!is_acting_wall(e)) return;
        }
        
        void init_floor(entity & e)
        {
            ELIB_ASSERT(is_floor(e));
            if (!is_acting_floor(e)) return;
        }
    }                                                       // namespace detail
    
    namespace detail { namespace
    {
        
        void process_bind(level & l, parsed_action const & act)
        {
            ELIB_ASSERT(act.action == action_type::bind);
            
            
            entity & actor = Concept<>(
                AtPosition(act.actor.pos), HasId(act.actor.id)
            ).find(l.entity_list);
            
            REQUIRE_CONCEPT(actor, Bindable);
            bindings & actor_binds = actor.get<bindings>();
            
            for (auto & epos : act.act_on)
            {
                entity & act_on = Concept<>(
                    AtPosition(epos.pos), HasId(epos.id)
                ).find(l.entity_list);
                
                actor_binds->push_back( &act_on );
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
        
    }}                                                      // namespace detail
        
    void process_level(level & l)
    {
        init_chip(l.chip);

        for (auto & e : l.entity_list)
        {
            log::debug("entity: %s", to_string(e.id()));
        }
        for (auto & e : EntityMatches<&is_actor>().filter(l.entity_list))
        {
            init_actor(e);
        }
        for (auto & e : EntityMatches<&is_item>().filter(l.entity_list))
        {
           init_item(e);
        }
        for (auto & e : EntityMatches<&is_base>().filter(l.entity_list))
        {
            init_base(e);
        }
       
        
        for (auto & butt : IsBlueButton().filter(l.entity_list))
        {
            REQUIRE_CONCEPT(butt, Bindable);
            for (auto & tank : IsTank().filter(l.entity_list))
            {
                REQUIRE_CONCEPT(tank, Toggleable);
                butt.get<bindings>()->push_back( &tank );
            }
        }
        
        for (auto & butt : IsGreenButton().filter(l.entity_list))
        {
            REQUIRE_CONCEPT(butt, Bindable);
            for (auto & wall : IsToggleWall().filter(l.entity_list))
            {
                REQUIRE_CONCEPT(wall, Toggleable);
                butt.get<bindings>()->push_back(&wall);
            }
        }
        
        detail::process_actions(l);
    }
}                                                           // namespace chips