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

		static inputHandler & instance();
		
	private:
		inputHandler() noexcept = default;
		
		/* Not copy or move-able */
		inputHandler(inputHandler const &) = delete;
		inputHandler(inputHandler &&) = delete;
		inputHandler & operator=(inputHandler const &) = delete;
		inputHandler & operator=(inputHandler &&) = delete;
		
		~inputHandler() {}

		uint8_t _keystate;
	};
}

#endif
