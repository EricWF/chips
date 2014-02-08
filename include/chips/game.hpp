#ifndef CHIPS_GAME_HPP
#define CHIPS_GAME_HPP

#include "chips/texturemanager.hpp"

#include <SDL2/SDL.h>

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

		int _currFrame;
		textureManager _tm;
      
	};
}                                                           // namespace chips
#endif /* CHIPS_GAME_HPP */
