#ifndef __TOOLS_TIME_TIME_H
#define	__TOOLS_TIME_TIME_H	1

#include <ctime>

namespace tools
{
	namespace time
	{
		class Time
		{
			protected:
				static struct timespec startTime;
			
			public:
				static unsigned int getTicks();
		};
	}
}

#endif

