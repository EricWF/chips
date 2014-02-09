#include "chips/chips_main.hpp"
#include "chips/game.hpp"

namespace chips
{
    int chips_main(int, char**, char**)
    {

		if(game::instance()->init("chips", 100, 100, 640, 480, false))
		{
			while(game::instance()->running())
			{
				game::instance()->handleEvents();
				game::instance()->update();
				game::instance()->render();
				SDL_Delay(10);
			}
			game::instance()->clean();
		}
		else
		{
			printf("Failure to init game instance\n");
		}
		
        return 0;
    }
}                                                           // namespace chips
