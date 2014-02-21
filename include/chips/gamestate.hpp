#ifndef CHIPS_GAMESTATE_HPP
#define CHIPS_GAMESTATE_HPP

#include <string>

namespace chips
{

	class gameState
	{
	public:
		virtual ~gameState();
		
		virtual void update() = 0;
		virtual void render() = 0;
		virtual bool onEnter() = 0;
		virtual bool onExit() = 0;

		virtual std::string getStateID() const = 0;
  
	};

}

#endif
