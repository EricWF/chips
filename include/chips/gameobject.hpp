#ifndef CHIPS_GAMEOBJECT_HPP
#define CHIPS_GAMEOBJECT_HPP

namespace chips
{
	class gameObject
	{
	public:
		void draw() {}
		void update() {}
		void clean() {}

	protected:
		int x;
		int y;
	};
}

#endif
