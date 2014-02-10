#ifndef CHIPS_GAMEOBJECT_HPP
#define CHIPS_GAMEOBJECT_HPP

#include "chips/texturemanager.hpp"
#include "chips/loader.hpp"
#include <boost/numeric/ublas/vector.hpp>
#include <SDL2/SDL.h>
#include <string>

namespace chips
{
	class gameObject
	{
	public:
		gameObject() = default;
		gameObject(const loader*);
		virtual void draw(SDL_Renderer*);
		
		virtual void update();
		
		virtual void clean() {};

	protected:
		virtual ~gameObject() {}
		std::string _id;
		int _currFrame;
		int _currRow;
		int _h;
		int _w;
		
		int _x; // TODO: _pos replaces these
		int _y; 

		boost::numeric::ublas::vector<int> _pos;

    private:
        
        gameObject(gameObject const &) = delete;
        gameObject(gameObject &&) = delete;
        gameObject & operator=(gameObject const &) = delete;
        gameObject & operator=(gameObject &&) = delete;
	};
}

#endif
