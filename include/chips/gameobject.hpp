#ifndef CHIPS_GAMEOBJECT_HPP
#define CHIPS_GAMEOBJECT_HPP

#include "chips/texturemanager.hpp"
#include "chips/inputhandler.hpp"
# include "chips/position.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace chips
{
	class gameObject
	{
	public:
		gameObject()
          : _id(), _w(0), _h(0), _pos(2), _velocity(0)
        {
            _pos <<= 0, 0;
            _velocity <<= 0, 0;
        }
        
		gameObject(std::string id, int x, int y, int w, int h)
          : _id(id), _w(w), _h(h), _pos(2), _velocity(2)
        {
            _pos <<= x, y;
            _velocity <<= 0, 0;
        }
		
		gameObject(gameObject const &) = default;
        gameObject(gameObject &&) = default;
        gameObject & operator=(gameObject const &) = default;
        gameObject & operator=(gameObject &&) = default;
		
		virtual ~gameObject() {}
		
		std::string const & id() const noexcept { return m_id; }
		
		int const & x() const noexcept  { return m_pos[0]; }
        int const & y() const noexcept  { return m_pos[1]; }
		
		int width()  const noexcept  { return m_width; }
		int height() const noexcept  { return m_height; }
		
		
		virtual void draw(SDL_Renderer &);
		virtual void update();
		virtual void clean() {}

	protected:
		
		std::string _id;
        int _w, _h;
		int m_tex_row, m_tex_col
		
		boost::numeric::ublas::vector<int> _pos;
		
	};
}

#endif
