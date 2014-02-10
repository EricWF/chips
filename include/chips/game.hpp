#ifndef CHIPS_GAME_HPP
#define CHIPS_GAME_HPP

# include "chips/window.hpp"
# include "chips/texturemanager.hpp"
# include "chips/gameobject.hpp"
# include "chips/player.hpp"

# include <string>
# include <vector>
# include <SDL2/SDL.h>

namespace chips
{

	class game
	{

	public:
        game();
		game(game const &) = delete;
		game(game &&) = delete;
		game & operator=(game const &) = delete;
		game & operator=(game &&) = delete;
        ~game();
		
		void render();
		void update();
		void handleEvents();
		void clean();
		bool running() const noexcept { return _running; }

		gameObject *go;
		gameObject *p;
		
	private:
		bool _running{false};
		std::vector<gameObject*> _objs;
		// int _currFrame; unused
	};
}                                                           // namespace chips
#endif /* CHIPS_GAME_HPP */
