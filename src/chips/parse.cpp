#include "chips/parse.hpp"
#include "chips/error.hpp"
#include <elib/aux.hpp>
#include <elib/enumeration.hpp>
#include <string>

namespace chips
{
    void parse(TiXmlElement & e, chips_state & d)
    {
        const char *cstr = e.Attribute("value");
        ELIB_ASSERT(cstr);
        d = elib::enumeration::enum_cast<chips_state>(cstr);
    }
    
    void parse(TiXmlElement & e, direction & d)
    {
        const char *cstr = e.Attribute("value");
        ELIB_ASSERT(cstr);
        std::string s = cstr;
        if (s == "N") d = direction::N;
        if (s == "S") d = direction::S;
        if (s == "E") d = direction::E;
        if (s == "W") d = direction::W;
    }
    
    void parse(TiXmlElement & e, entity & dest)
    {
        ELIB_ASSERT(e.Value() == std::string("entity"));
        {
            const char *cstr = e.Attribute("entity_id");
            ELIB_ASSERT(cstr);
            dest = entity(elib::enumeration::enum_cast<entity_id>(cstr));
        }
        {
            TiXmlHandle root = &e;
            TiXmlElement *child = root.FirstChild().Element();
            for (((void)child); child; child = child->NextSiblingElement())
            {
                const char *cstr = child->Attribute("type");
                ELIB_ASSERT(cstr);
                std::string type_str = cstr;
                if (type_str == "chips_state")
                {
                    chips_state s;
                    parse(*child, s);
                    dest << s;
                }
                else if (type_str == "direction")
                {
                    direction d;
                    parse(*child, d);
                    dest << d;
                }
                else if (type_str == "position")
                {
                    position p;
                    parse(*child, p);
                    dest << p;
                }
                else if (type_str == "texture_id")
                {
                    texture_id id;
                    parse(*child, id);
                    dest << id;
                }
            }
        }
    }
    
    void parse(TiXmlElement & e, entity_id & dest)
    {
        const char *cstr = e.Attribute("value");
        ELIB_ASSERT(cstr);
        dest = elib::enumeration::enum_cast<entity_id>(cstr);
    }
    
    void parse(TiXmlElement & e, std::vector<entity> & v)
    {
        for (TiXmlElement *elem = &e; elem; elem = elem->NextSiblingElement())
        {
            ELIB_ASSERT(elem->Value() == std::string("entity"));
            entity tmp;
            parse(*elem, tmp);
            v.push_back(tmp);
        }
    }
    
    //TODO
    void parse(TiXmlElement &, level & e);
    
    void parse(TiXmlElement & e, position & p)
    {
        int ret = e.QueryIntAttribute("x", &p.x);
        ELIB_ASSERT(ret == TIXML_SUCCESS);
        ret = e.QueryIntAttribute("y", &p.y);
        ELIB_ASSERT(ret == TIXML_SUCCESS);
    }
    
    void parse(TiXmlElement & e, texture_id & id)
    {
        const char *cstr = e.Attribute("value");
        ELIB_ASSERT(cstr);
        id = elib::enumeration::enum_cast<texture_id>(cstr);
    }
}                                                           // namespace chips