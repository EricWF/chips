#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdio.h>

class Game{
 public:
  Game();
  ~Game();

  bool init(const char*, int, int, int, int, Uint32);
  void render();
  void update();
  void handleEvents();
  void clean();
  bool running();

 private:
  bool _running;
  SDL_Window* _window;
  SDL_Renderer* _renderer;

  

};

#endif
