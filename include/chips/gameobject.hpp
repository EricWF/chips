#ifndef CHIPS_GAMEOBJECT_HPP
#define CHIPS_GAMEOBJECT_HPP

#include <SDL2/SDL.h>
#include <string>

namespace chips
{
	class gameObject
	{
	public:
		void load(const int, const int, const int,
				  const int, const std::string);
		void draw(SDL_Renderer*);
		void update() {}
		void clean() {}

	protected:
		std::string _id;
		int _currFrame;
		int _currRow;
		int _h;
		int _w;
		int _x;
		int _y;

	};
}

#endif
