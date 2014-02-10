#include "chips/chips_main.hpp"
#include "chips/game.hpp"

namespace chips
{
    int chips_main(int, char**, char**)
    {
        game m_game;
        
		if(m_game.init("chips", 100, 100, 640, 480, false))
		{
			while(m_game.running())
			{
				m_game.handleEvents();
				m_game.update();
				m_game.render();
				SDL_Delay(10);
			}
			m_game.clean();
		}
		else
		{
			printf("Failure to init game instance\n");
		}
		
        return 0;
    }
}                                                           // namespace chips
