#include "chips/gameobjectfactory.hpp"

namespace chips
{
  
#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
  gameObjectFactory & gameObjectFactory::instance()
  {
    static gameObjectFactory factory;		
    return factory;
  }
#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

  bool gameObjectFactory::registerType(std::string typeID, baseCreator *creator)
  {
    std::map<std::string, baseCreator*>::iterator it = _creators.find(typeID);

    if(it != _creators.end())
      {
	delete creator;
	return false;
      }

    _creators[typeID] = creator;

    return true;
  }

  gameObject* gameObjectFactory::create(std::string typeID)
  {
    std::map<std::string, baseCreator*>::iterator it = _creators.find(typeID);

    if(it == _creators.end())
      {
	std::cout << "Could not find find: " << typeID << std::endl;
	return nullptr;
      }

    baseCreator *creator = (*it).second;
		
    return creator->create_game_object();
  }
  
}
