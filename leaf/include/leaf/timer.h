#pragma once
#include <SDL2/SDL_timer.h>


namespace leaf {
	class Timer
	{
	public:
		Timer() {
			reset();
		}


		void reset() {
			startTime = SDL_GetTicks();
		}


		int getElapsedTime() const {
			return (SDL_GetTicks() - startTime);
		}
	private:
		int startTime;
	};
}