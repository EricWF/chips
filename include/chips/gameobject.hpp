#ifndef CHIPS_GAMEOBJECT_HPP
#define CHIPS_GAMEOBJECT_HPP

#include "chips/texturemanager.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace chips
{
	class gameObject
	{
	public:
		//gameObject() = default;
		virtual void load(const int, const int, const int,
						  const int, const std::string);
		virtual void draw(SDL_Renderer*);
		virtual void update();
		virtual void clean() {}

	protected:
		virtual ~gameObject() {}
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
