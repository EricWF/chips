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
    
    void level::update_all()
    {
        all.clear();
        all.reserve( actors.size() + items.size() + base.size() );
        
        auto transform_fn = [](entity & e) { return &e; };
        
        std::transform(
            std::begin(base), std::end(base)
          , std::back_inserter(all)
          , transform_fn
        );
        
        std::transform(
            std::begin(items), std::end(items)
          , std::back_inserter(all)
          , transform_fn
        );
        
        std::transform(
            std::begin(actors), std::end(actors)
          , std::back_inserter(all)
          , transform_fn
        );
    }
    
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
        
        // Insert each entity for each level
        for (unsigned i=0; i < level_width * level_height; ++i)
        {
            l.base.push_back( create_entity(raw.base[i], i, props) );
            
            if (raw.items[i] != 0)
            {
                l.items.push_back( create_entity(raw.items[i], i, props) );
            }
            
            if (raw.actors[i] != 0)
            {
                entity tmp = create_entity(raw.actors[i], i, props);
                if (is_chip(tmp)) 
                {
                    ELIB_ASSERT(!l.chip);
                    l.chip = tmp;
                }
                else l.actors.push_back(tmp);
            }
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
        e << inventory() << chips_state::normal 
          << method(move_in_, common::move_in_);
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
            ELIB_ASSERT(e.has<direction>() && e.has<position>());
            
            // TODO remove this
            // add a default velocity if none is present.
            if (!e.has<velocity>())
                e << velocity(1);
                
            e << method(move_, common::move_);
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
        entity & find_entity(std::vector<entity> & el, position p)
        {
            auto pos = 
                std::find_if(
                    std::begin(el), std::end(el)
                  , [&](entity const & e)
                    { return bool(e) && e.get<position>() == p; }
                );
                
            if (pos == el.end())
            {
                ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                    "Failed to find entity at %s", to_string(p)
                )));
            }
            return *pos;
        }
        
        entity & find_entity(level & l, entity_location const & loc)
        {
            if      (is_chip(loc.id))  return l.chip;
            else if (is_actor(loc.id)) return find_entity(l.actors, loc.pos);
            else if (is_item(loc.id))  return find_entity(l.items, loc.pos);
            else if (is_base(loc.id))  return find_entity(l.base, loc.pos);
            else
            {
                ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                    "Failed to find entity %s at %s"
                  , to_string(loc.id), to_string(loc.pos)
                )));
            }
        }
        
        void process_bind(level & l, parsed_action const & act)
        {
            ELIB_ASSERT(act.action == action_type::bind);
            
            entity & actor = find_entity(l, act.actor);
            bindings & actor_binds = actor.get<bindings>();
            
            for (auto & epos : act.act_on)
            {
                actor_binds->push_back( &find_entity(l, epos) );
            }
        }
    }}                                                      // namespace detail
        
    void process_level(level & l)
    {
        init_chip(l.chip);
    
        std::for_each(
            std::begin(l.actors), std::end(l.actors)
          , init_actor
        );
        
        std::for_each(
            std::begin(l.items), std::end(l.items)
          , init_item
        );
        
        std::for_each(
            std::begin(l.base), std::end(l.base)
          , init_base
        );
        
        detail::bind_buttons_to_tank(l);
        detail::bind_buttons_to_toggle_wall(l);
        detail::process_actions(l);
    }
        
    namespace detail
    {
        void bind_buttons_to_tank(level & l)
        {
            for (auto & base_e : l.base)
            {
                if (!base_e || base_e.id() != entity_id::blue_button) continue;
                ELIB_ASSERT(base_e.has<bindings>());
                for (auto & actor_e : l.actors)
                {
                    if (!actor_e || actor_e.id() != entity_id::tank) continue;
                    ELIB_ASSERT(actor_e.has<toggle_state>() 
                            && actor_e.has(toggle_));
                            
                    base_e.get<bindings>()->push_back(&actor_e);
                }
            }
        }
        
        void bind_buttons_to_toggle_wall(level & l)
        {
            for (auto & base_e :  l.base)
            {
                if (!base_e || base_e.id() != entity_id::green_button) continue;
                ELIB_ASSERT(base_e.has<bindings>());
                for (auto & other_e : l.base)
                {
                    if (!other_e || other_e.id() != entity_id::toggle_wall) continue;
                    ELIB_ASSERT(other_e.has<toggle_state>() 
                             && other_e.has(toggle_));
                             
                    base_e.get<bindings>()->push_back(&other_e);
                }
            }
        }
        
        void process_actions(level & l)
        {
            std::vector<parsed_action> actions = parse_actions(l.id());
            if (actions.empty()) return;
                
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
    }                                                       // namespace detail
    
}                                                           // namespace chips