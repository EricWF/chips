#ifndef CHIPS_PLAYER_HPP
#define CHIPS_PLAYER_HPP

#include "chips/gameobject.hpp"
#include "chips/gameobjectfactory.hpp"

namespace chips
{
    class player : public gameObject
    {
    public:
		void draw(SDL_Renderer*);
		void update();
		void clean();
		void load(const loader* param) { gameObject::load(param); }
				
		// needs virtual destructor!
		virtual ~player() = default;

    private:
		void handle_input();
		
    };

    class playerCreator : public baseCreator
    {
		gameObject * create_game_object() const
	    {
			return new player();
	    }
    };
	

}

#endif
