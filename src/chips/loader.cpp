#include "chips/loader.hpp"

namespace chips
{
	loader::loader(int x, int y, int w, int h, std::string id)
	{
		_x = x; _y = y; _w = w; _h = h; _id = id;
	}
}
