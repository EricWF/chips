#ifndef CHIPS_WINDOW_HPP
#define CHIPS_WINDOW_HPP

# include <string>
# include <SDL2/SDL.h>

namespace chips
{
    ////////////////////////////////////////////////////////////////////////////
    //
    struct window_info
    {
        std::string name;
        int xpos, ypos;
        int width, height;
        Uint32 flags;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    //
    class window
    {
    public:
        static window & init(window_info const &);
        static window & instance();
        
        ~window() noexcept;
        
        ////////////////////////////////////////////////////////////////////////
        // no copy or move
        window(window const &) = delete;
        window & operator=(window const &) = delete;
        window(window &&) = delete;
        window & operator=(window &&) = delete;
        
        ////////////////////////////////////////////////////////////////////////
        // access to raw window
        SDL_Window const & get() const { return *m_window; }
        SDL_Window & get() { return *m_window; }
        
        ////////////////////////////////////////////////////////////////////////
        // conversion operators
        operator SDL_Window*() { return m_window; }
        operator const SDL_Window*() const { return m_window; }
        
    private:
        static window & m_instance(window_info const* = nullptr);
        
        window(window_info const &);
        SDL_Window *m_window;
    };
}                                                           // namespace chips
#endif /* CHIPS_WINDOW_HPP */