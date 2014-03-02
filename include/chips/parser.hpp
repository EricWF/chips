#include <iostream>
#include <vector>
#include "tinyxml/tinyxml.h"

namespace chips
{
    class gameObject;

    class parser
    {
    public:
	bool parseState(const char* stateFile, std::string stateID,
			std::vector<gameObject*> objects);
    private:
	void parseObjects(TiXmlElement* stateRoot,
			  std::vector<gameObject*> *objects);

	void parseTextures(TiXmlElement* stateRoot,
			   std::vector<std::string> *textureIDs);
    };

}
