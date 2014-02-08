#include "chips/chips_main.hpp"
#include "chips/game.hpp"

namespace chips
{
    int chips_main(int, char**, char**)
    {
        game g;
        g.init("chips", 100, 100, 640, 480, 0);
        while(g.running())
        {
            g.handleEvents();
            g.update();
            g.render();
			SDL_Delay(10);
        }
        g.clean();
        return 0;
    }
}                                                           // namespace chips
