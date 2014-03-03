#include "chips/gameobject.hpp"

#include <vector>
#include <boost/numeric/ublas/assignment.hpp>

namespace chips
{
    void gameObject::load(const loader *params)
    {
        _pos.resize(2, false);
        _pos <<= params->getX(), params->getY();
            
        _velocity.resize(2, false);
        _velocity <<= 0, 0;
            
        _h = params->getHeight();
        _w = params->getWidth();
        _id = params->getID();
        _currFrame = 1;
        _currRow = 1;
    }

    void gameObject::draw(SDL_Renderer & renderer)
    {
        textureManager::instance().drawFrame(
            _id, _pos(0), _pos(1), _w, _h 
          , _currRow, _currFrame
          , &renderer
        );
    }

    void gameObject::update()
    {
		
    }
}
