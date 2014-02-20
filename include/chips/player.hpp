#ifndef CHIPS_PLAYER_HPP
#define CHIPS_PLAYER_HPP

#include "chips/gameobject.hpp"

namespace chips
{
	class player : public gameObject
	{
	public:
		player(const loader* param) : gameObject(param) {}
		void draw(SDL_Renderer*);
		void update();
		void clean();
		
        // needs virtual destructor!
        virtual ~player() = default;

	private:
		void handle_input();
		
	};
}

#endif
