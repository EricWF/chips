#ifndef CHIPS_PLAYER_HPP
#define CHIPS_PLAYER_HPP

#include "chips/gameobject.hpp"

namespace chips
{
	class player : public gameObject
	{
	public:
		void draw();
		void update();
		void clean();
			
	};
}

#endif
