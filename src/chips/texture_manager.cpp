#include "chips/texture_manager.hpp"
#include "chips/config.hpp"

namespace chips
{
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    texture_manager & texture_manager::get()
    {
        static texture_manager tm;
        return tm;
    }
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
    
    texture_manager::texture_manager()
    {
        if (!m_tex_map[texture_uid::tiles].loadFromFile(
                CHIPS_RESOURCE_ROOT "/tileset.gif"))
            throw "TODO";
    }
    
    sf::Texture const & texture_manager::operator[](texture_uid id) const
    {
        return m_tex_map.at(id);
    }
}                                                           // namespace chips