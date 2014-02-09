#ifndef CHIPS_PLAYER_HPP
#define CHIPS_PLAYER_HPP

#include "chips/gameobject.hpp"

namespace chips
{
	class player : public gameObject
	{
	public:
		player(const loader*) {}
		void draw(SDL_Renderer*);
		void update();
		void clean();
			
	};
}

#endif
