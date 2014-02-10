#include "chips/window.hpp"
#include "chips/chips_error.hpp"

namespace chips
{
    ////////////////////////////////////////////////////////////////////////////
    //
    window & window::init(window_info const & info)
    {
        return window::m_instance(&info);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    window & window::instance()
    {
        return window::m_instance();
    }
    
    window & window::m_instance(window_info const* info_ptr)
    {
        static window *m_win_ptr = nullptr;
        if (info_ptr)
        {
            ELIB_ASSERT(!m_win_ptr);
            static window win{*info_ptr};
            m_win_ptr = &win;
        }
        else
        {
            ELIB_ASSERT(m_win_ptr);
        }
        return *m_win_ptr;
    }
    
    window::window(window_info const & info)
      : m_window{nullptr}
    {
        m_window = SDL_CreateWindow(info.name.c_str(), info.xpos, info.ypos
                                  , info.width, info.height, info.flags);
        if (!m_window)
            throw_and_clear_sdl_error("Failed to open window with error: %s");
    }
    
    window::~window() noexcept
    {
        SDL_DestroyWindow(m_window);
    }
}                                                           // namespace chips