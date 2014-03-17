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


namespace chips
{
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
    parsed_level parse_level(std::string const & fname)
    {
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
    
    ////////////////////////////////////////////////////////////////////////////
    level create_level(
        unsigned num
      , std::map<unsigned, tile_properties> const & props
      )
    {
        std::string filename = elib::fmt(
            CHIPS_RESOURCE_ROOT "level%u.tmx"
          , num
        );
        
        parsed_level raw = parse_level(filename);
        level l(raw.level, raw.chip_count);
        
        int pos = 0;
        auto create_fn = [&](int gid) { return create_entity(gid, pos++, props); };
        
        std::transform(
            std::begin(raw.base), std::end(raw.base)
          , std::back_inserter(l.base)
          , create_fn
        );
        
        ELIB_ASSERT(pos == level_width * level_height);
        pos = 0;
        
        std::transform(
            std::begin(raw.items), std::end(raw.items)
          , std::back_inserter(l.items)
          , create_fn
        );
        
        ELIB_ASSERT(pos == level_height * level_width);
        pos = 0;
        
        std::transform(
            std::begin(raw.actors), std::end(raw.actors)
          , std::back_inserter(l.actors)
          , create_fn
        );
        
        ELIB_ASSERT(pos == level_height * level_width);
        
        //TODO connect components
        return l;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    entity create_entity(
        unsigned gid, unsigned index
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
        auto props = pm.at(gid);
        entity e(props.id, pos);
        
        for (auto & p : props.properties)
        {
            if (p.first == "tile_id")
            {
                e.insert_attribute(
                    elib::enumeration::enum_cast<tile_id>(p.second)
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