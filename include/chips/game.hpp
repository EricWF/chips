#ifndef CHIPS_GAME_HPP
#define CHIPS_GAME_HPP

#include <SDL2/SDL.h>
#include <stdio.h>

namespace chips
{
    class game
    {
    public:
        game() noexcept {}
        ~game() noexcept {}
        
        game(game const &) = delete;
        game & operator=(game const &) = delete;

        bool init(const char*, int, int, int, int, Uint32);
        void render();
        void update();
        void handleEvents();
        void clean();
        bool running() const noexcept { return _running; }

    private:
        bool _running;
        // TODO these should be released in the destructor
        // class should not be copyable (see RAII)
        SDL_Window* _window{nullptr};
        SDL_Renderer* _renderer{nullptr};
        SDL_Texture* _texture{nullptr};
        
        SDL_Rect _sourceRect;
        SDL_Rect _destRect;
    };
}                                                           // namespace chips
#endif /* CHIPS_GAME_HPP */
