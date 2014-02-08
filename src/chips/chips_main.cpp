
#include "chips/chips_main.hpp"

#include "Game.h"

static Game* game = nullptr;

int chips_main(int, char**, char**){

  game = new Game();

  game->init("chips", 100, 100, 640, 480, 0);

  while(game->running())
    {
      game->handleEvents();
      game->update();
      game->render();
    }

  game->clean();

  return 0;
}
