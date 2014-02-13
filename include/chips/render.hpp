#ifndef CHIPS_RENDER_HPP
#define CHIPS_RENDER_HPP

# include "chips/resource_manager.hpp"
# include "chips/tiles/fwd.hpp"
# include "chips/tiles/support.hpp"
# include <SDL2/SDL.h>

// helper functions for handling rendering
namespace chips
{
    struct render_request
    {
        texture_index base;
        texture_index overlay;
        position pos;
    };
}                                                           // namespace chips
#endif /* CHIPS_RENDER_HPP */