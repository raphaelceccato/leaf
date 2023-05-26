#pragma once
#include "export.h"


namespace leaf {
	class EXPORT Timer
	{
	public:
		Timer();
		void reset();
		int getElapsedTime() const;
	private:
		int startTime;
	};
}