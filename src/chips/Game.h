#ifndef GAME_H
#define GAME_H

class Game{
 public:
  Game();
  ~Game();

  void init();
  void render();
  void update();
  void handleEvents();
  void clean();
  bool running();

 private:
  bool _running;

  

};

#endif
