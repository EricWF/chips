#ifndef CHIPS_INPUTHANDLER_HPP
#define CHIPS_INPUTHANDLER_HPP

#include <SDL2/SDL.h>
#include <stdint.h>

namespace chips
{
	class inputHandler
	{
	public:
		void update();
		void clean() {}
		bool isKeyDown(SDL_Scancode);
		
		static inputHandler & instance();
		
	private:
		inputHandler() noexcept = default;
		
		/* Not copy or move-able */
		inputHandler(inputHandler const &) = delete;
		inputHandler(inputHandler &&) = delete;
		inputHandler & operator=(inputHandler const &) = delete;
		inputHandler & operator=(inputHandler &&) = delete;
		
		/* The destructor can never be called in this singleton pattern */
		~inputHandler() = delete;

		const uint8_t *_keystates;
	};
}

#endif
