#ifndef CHIPS_MENUSTATE_HPP
#define CHIPS_MENUSTATE_HPP

#include "chips/gamestate.hpp"
#include <string>

namespace chips
{

	class menuState : public gameState
	{

	public:
		virtual ~menuState();
		
		virtual void update();
		virtual void render();
		virtual bool onEnter();
		virtual bool onExit();

		virtual std::string getStateID() const { return _menuID; }

	private:
		static const std::string _menuID;
		
	};

}

#endif
