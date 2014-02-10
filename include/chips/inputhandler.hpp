#ifndef CHIPS_INPUTHANDLER_HPP
#define CHIPS_INPUTHANDLER_HPP

#include <stdint.h>

namespace chips
{
	class inputHandler
	{
	public:
		void update() {}
		void clean() {}

		static inputHandler* instance();
		
	private:
		inputHandler() {}
		~inputHandler() {}
		
		static inputHandler* ins;

		uint8_t _keystate;
	};
}

#endif
