#include "chips/menu/parse.hpp"

#include "chips/error.hpp"
#include "chips/log.hpp"
#include "tinyxml/tinyxml.h"

#include <elib/aux.hpp>
#include <elib/enumeration.hpp>
#include <elib/lexical_cast.hpp>

#include <exception>

namespace chips
{
    namespace detail
    {
        inline button_info parse_button(TiXmlElement & elem)
        {
            button_info info;
            
            info.id = elib::enumeration::enum_cast<menu_item_id>(elem.Attribute("id"));
            elem.QueryFloatAttribute("x", &info.hpos);
            info.halign = elib::enumeration::enum_cast<align_h>(elem.Attribute("align_x"));
            elem.QueryFloatAttribute("y", &info.vpos);
            info.valign = elib::enumeration::enum_cast<align_v>(elem.Attribute("align_y"));
            info.value = elem.GetText();
            
            return info;
        }
    }                                                       // namespace detail
    
    std::vector<button_info>
    parse_menu(std::string const & filename)
    {
        std::vector<button_info> button_list;
        TiXmlDocument doc(filename.c_str());
        
        if (!doc.LoadFile())
        {
            log::err("Failed to open menu file %s", filename.c_str());
            chips_error e("Failed to open menu file");
            ELIB_THROW_EXCEPTION(e);
        }
        
        TiXmlHandle doc_handle(&doc);
        TiXmlHandle root_handle(nullptr);
        
        TiXmlElement *elem = nullptr;
        
        // find menu root
        {
            elem = doc_handle.FirstChildElement().Element();
            ELIB_ASSERT(elem != nullptr);
            root_handle = TiXmlHandle(elem);
        }
        
        {
            elem = root_handle.FirstChild("button").Element();
            for (((void)elem); elem; elem = elem->NextSiblingElement())
                button_list.push_back(detail::parse_button(*elem));
        }
        
        return button_list;
    }
}                                                           // namespace chips