#include <string>
#include <map>
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
		bool registerType(std::string typeID, baseCreator *creator)
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

		gameObject* create(std::string typeID)
		{
			std::map<std::string, baseCreator*>::iterator it = _creators.find(typeID);

			if(it == _creators.end())
			{
				printf("Could not find find: %s \n", typeID);
				return nullptr;
			}

			baseCreator *creator = (*it).second;
		
			return creator->create_game_object();
		}

	private:
		std::map<std::string, baseCreator*> _creators;
		
	};
}


