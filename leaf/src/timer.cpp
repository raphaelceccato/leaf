#include "../include/leaf/timer.h"
#include <SDL2/SDL.h>


namespace leaf {
	Timer::Timer() {
		reset();
	}


	void Timer::reset() {
		startTime = SDL_GetTicks();
	}


	int Timer::getElapsedTime() const {
		return (SDL_GetTicks() - startTime);
	}
}