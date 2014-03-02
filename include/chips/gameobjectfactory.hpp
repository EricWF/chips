#include <string>
#include <map>
#include <iostream>
#include "chips/gameobject.hpp"

namespace chips
{
  class baseCreator
  {

  public:
    virtual gameObject * create_game_object() const = 0;
    virtual ~baseCreator() {}
		
  };

  class gameObjectFactory
  {

  public:

    static gameObjectFactory & instance();

    bool registerType(std::string, baseCreator*);

    gameObject* create(std::string);

  private:
    gameObjectFactory() = default;
		
    ~gameObjectFactory() noexcept = default;
		
    gameObjectFactory(gameObjectFactory const &) = delete;
    gameObjectFactory(gameObjectFactory &&) = delete;
    gameObjectFactory & operator=(gameObjectFactory const &) = delete;
    gameObjectFactory & operator=(gameObjectFactory &&) = delete;
	
    std::map<std::string, baseCreator*> _creators;
		
  };
}


