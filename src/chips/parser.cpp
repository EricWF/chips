#include "chips/parser.hpp"
#include "chips/gameobject.hpp"
#include "chips/gameobjectfactory.hpp"
#include "chips/texturemanager.hpp"
#include "chips/resource_manager.hpp"
#include <iostream>

namespace chips
{
  bool parser::parseState(const char* stateFile, std::string stateID,
			  std::vector<gameObject*> objects)
  {
    TiXmlDocument doc;

    if(!doc.LoadFile(stateFile))
      {
	std::cerr << doc.ErrorDesc() << std::endl;
	return false;
      }
		
		
    TiXmlElement *root = doc.RootElement(); /* root of xml doc */
    TiXmlElement *stateRoot = nullptr; /* root of curr state */

    /* Find root of state we're searching for */
    for(auto e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
      if(e->Value() == stateID) stateRoot = e;
		
    TiXmlElement *texRoot = nullptr; /* root of textures */
		
    /* Find root of textures inside state */
    for(auto e = stateRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
      if(e->Value() == std::string("TEXTURES")) texRoot = e;

    TiXmlElement *objRoot = nullptr; /* root of objects */
		
    /* Find root of objects inside state */
    for(auto e = stateRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
      if(e->Value() == std::string("OBJECTS")) objRoot = e;
		
    parseObjects(objRoot, &objects);
		
    return true;

  }


  void parser::parseTextures(TiXmlElement* stateRoot, std::vector<std::string> *textureIDs)
  {
    for(auto e = stateRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
      {
	std::string filenameAttribute = e->Attribute("filename");
	std::string idAttribute = e->Attribute("ID");
	textureIDs->push_back(idAttribute);
	textureManager::instance().load(filenameAttribute, 
					 idAttribute, &resource_manager::get().renderer());

      }
  }

  void parser::parseObjects(TiXmlElement *stateRoot, std::vector<gameObject *> *objects)
  {
    for(TiXmlElement* e = stateRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
      {
	int x, y, width, height;
	std::string textureID;
	e->Attribute("x", &x);
	e->Attribute("y", &y);
	e->Attribute("width",&width);
	e->Attribute("height", &height);
	textureID = e->Attribute("textureID");
	gameObject* obj = gameObjectFactory::instance().create(e->Attribute("type"));
	obj->load(new loader(x,y,width,height,textureID));
	objects->push_back(obj);
      }
  }

}
