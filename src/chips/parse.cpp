#include "chips/parse.hpp"
#include "chips/error.hpp"
#include "chips/log.hpp"
#include "chips/entity_id.hpp"
#include <elib/aux.hpp>
#include <elib/enumeration.hpp>
#include <elib/fmt.hpp>
#include <string>
#include <iostream>

namespace chips
{
    namespace detail { namespace
    {
        std::pair<unsigned, tile_property_list> parse_tile(TiXmlElement & elem)
        {
            std::pair<unsigned, tile_property_list> tp;
            
            ELIB_ASSERT(elem.Value() == std::string{"tile"});
            
            int tmp;
            int ret = elem.QueryIntAttribute("id", &tmp);
            ELIB_ASSERT(ret == TIXML_SUCCESS);
            
            tp.first = static_cast<unsigned>(tmp);
            
            TiXmlElement *child = elem.FirstChildElement();
            ELIB_ASSERT(child && child->Value() == std::string{"properties"});
            
            child = child->FirstChildElement();
            ELIB_ASSERT(child && child->Value() == std::string{"property"});
            
            for (; child ; child = child->NextSiblingElement())
            {
                ELIB_ASSERT(child->Value() == std::string{"property"});
                
                const char *name = child->Attribute("name");
                const char *value = child->Attribute("value");
                ELIB_ASSERT(name && value);
                
                tp.second.emplace_back(name, value);
            }
            
            if (tp.second.size() < 2)
                log::warn("id=%u", tp.first);
            ELIB_ASSERT(tp.second.size() >= 2);
            return tp;
        }
        
        
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
                int tmp = 0;
                int ret = elem->QueryIntAttribute("gid", &tmp);
                ELIB_ASSERT(ret == TIXML_SUCCESS);
                
                tiles.push_back(tmp);
            }
            
            ELIB_ASSERT(tiles.size() == 32 * 32);
            return tiles;
        }
    }}                                                      // namespace detail
    
    tile_property_map parse_tileset(std::string const & fname)
    {
        tile_property_map tile_map;
        
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
    
    parsed_level parse_level(std::string const & fname)
    {
        parsed_level map;
        
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
            
            const char *cname = pelem->Attribute("name");
            ELIB_ASSERT(cname);
            
            std::string name = cname;
        
            if (name == "chip_count")
            {
                int tmp;
                int ret = pelem->QueryIntAttribute("value", &tmp);
                ELIB_ASSERT(ret == TIXML_SUCCESS); ((void)ret);
                
                map.chip_count = static_cast<unsigned>(tmp);
            }
            else if (name == "level")
            {
                int tmp;
                int ret = pelem->QueryIntAttribute("value", &tmp);
                ELIB_ASSERT(ret == TIXML_SUCCESS); ((void)ret);
                
                map.level = static_cast<unsigned>(tmp);
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
        map.base = elib::move( detail::parse_layer(*elem) );
        
        elem = elem->NextSiblingElement();
        ELIB_ASSERT(elem && elem->Attribute("name") == std::string{"Items"});
        map.items = elib::move( detail::parse_layer(*elem) );
        
        elem = elem->NextSiblingElement();
        ELIB_ASSERT(elem && elem->Attribute("name") == std::string{"Actors"});
        map.actors = elib::move( detail::parse_layer(*elem) );
        
        return map;
    }
    
}                                                           // namespace chips
