#include "chips/font_manager.hpp"
#include "chips/error.hpp"
#include <algorithm>
#include <utility>

namespace chips
{

#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wcovered-switch-default"
#endif
    const char* get_font_id_path(font_id id)
    {
        switch (id)
        {
            case font_id::arial: return CHIPS_RESOURCE_ROOT "/arial.ttf";
            case font_id::none:
            default: throw chips_error{"Invalid font_id"};
        }
    }
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
    
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    font_manager & font_manager::get()
    {
        static font_manager fh;
        return fh;
    }
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
    
    const sf::Font & font_manager::operator[](font_id id)
    {
        auto found = m_font_map.find(id);
        if (found != m_font_map.end())
            return found->second;
        
        sf::Font tmp;
        if (!tmp.loadFromFile(get_font_id_path(id)))
            throw "TODO";
            
        auto pos = m_font_map.insert(std::make_pair(id, tmp));
        
        return pos.first->second;
    }
    
    const sf::Font & font_manager::at(font_id id) const
    {
        auto found = m_font_map.find(id);
        if (found == m_font_map.end())
            throw chips_error{"Failed to find font!"};
        return found->second;
    }
}                                                           // namespace chips