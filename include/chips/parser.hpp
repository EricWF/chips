
#include "tinyxml/tinyxml.h"
#include <memory>
#include <vector>

namespace chips
{
    class gameObject;

    std::vector< std::shared_ptr<gameObject> >
	bool parseState(std::string const & stateFile, std::string const & stateID);
    
	void parseObjects(TiXmlElement* stateRoot,
                      std::vector<gameObject*> *objects);

	void parseTextures(TiXmlElement* stateRoot,
                       std::vector<std::string> *textureIDs);

}
