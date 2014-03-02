 CHIPS_LOADER_HPP
#define CHIPS_LOADER_HPP

#include <string>

namespace chips
{
	class loader
	{

	public:
		loader(int x, int y, int w, int h, std::string id)
          : _id{id}, _x{x}, _y{y}, _w{w}, _h{h}
        {}
        
		int getX() const noexcept { return _x; }
		int getY() const noexcept { return _y; }
		int getWidth() const noexcept { return _w; }
		int getHeight() const noexcept { return _h; }
		const std::string & getID() const noexcept { return _id; }
	private:
		std::string _id;
		int _x, _y, _w, _h;
	};
}
#endif
