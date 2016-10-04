#ifndef __TOOLS_THREAD_THREAD_H
#define	__TOOLS_THREAD_THREAD_H	1

#include <pthread.h>

namespace tools
{
	namespace thread
	{
		class Thread
		{
			protected:
				static void * threadLauncher( void * data );
		
				pthread_t id;
				bool running;
			
				virtual int life() = 0;
		
			public:
				Thread();
				virtual ~Thread();
			
				bool start();
				bool isRunning();
				void join();
		};
	}
}

#endif

