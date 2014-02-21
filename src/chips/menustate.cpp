#include "chips/menustate.hpp"
#include <stdio.h>

namespace chips
{

	//const std::string _menuID = "MENU";

	void menuState::update(){}
	void menuState::render(){}
	
	bool menuState::onEnter()
	{
		printf("Entering menu state\n");
		return true;
	}
	
	bool menuState::onExit()
	{
		printf("Exiting menu state\n");
		return true;
	}

}
