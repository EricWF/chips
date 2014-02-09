#ifndef CHIPS_LOADER_HPP
#define CHIPS_LOADER_HPP

#include <string>

namespace chips
{
	class loader
	{

	public:
		loader(int, int, int, int, std::string);
		int getX() const { return _x; }
		int getY() const { return _y; }
		int getWidth() const { return _w; }
		int getHeight() const { return _h; }
		std::string getID() const { return _id; }
		
	private:
		std::string _id;
		int _x;
		int _y;
		int _w;
		int _h;

	};
}
#endif
