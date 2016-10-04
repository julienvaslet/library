#include <tools/time/Time.h>

namespace tools
{
	namespace time
	{
		struct timespec Time::startTime = {0};
		
		unsigned int Time::getTicks()
		{
			unsigned int ticks = 0;
			
			struct timespec now;
			clock_gettime( CLOCK_MONOTONIC, &now );
			
			// First call
			if( Time::startTime.tv_sec == 0 )
			{
				Time::startTime.tv_sec = now.tv_sec;
				Time::startTime.tv_nsec = now.tv_nsec;
			}
			
			ticks = (now.tv_sec - Time::startTime.tv_sec) * 1000 + (now.tv_nsec - Time::startTime.tv_nsec) / 1000000;
			
			return ticks;
		}
	}
}

