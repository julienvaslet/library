#include <tools/thread/Thread.h>

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace tools
{
	namespace thread
	{
		void * Thread::threadLauncher( void * data )
		{
			Thread * thread = static_cast<Thread *>( data );
			int returnCode = thread->life();
			
			#ifdef DEBUG1
			Logger::get() << "[Thread#" << thread->id << "] Stopped, return code: " << returnCode << Logger::endl;
			#endif
			
			thread->running = false;
			thread->id = 0;
			
			pthread_exit( (void *) returnCode );
		}
	
		Thread::Thread() : id(0), running(false)
		{
		}
	
		Thread::~Thread()
		{
			if( this->id != 0 )
			{
				#ifdef DEBUG1
				Logger::get() << "[Thread#" << this->id << "] WARNING: Cancelling thread." << Logger::endl;
				#endif
				
				pthread_cancel( this->id );
			}		
		}
	
		bool Thread::start()
		{
			if( pthread_create( &(this->id), NULL, &(Thread::threadLauncher), this ) == 0 )
			{
				#ifdef DEBUG1
				Logger::get() << "[Thread#" << this->id << "] Started." << Logger::endl;
				#endif
			
				this->running = true;
			}
			else
			{
				#ifdef DEBUG1
				Logger::get() << "[Thread] Could not be started." << Logger::endl;
				#endif

				this->running = false;
			}
		
			return this->running;
		}
	
		bool Thread::isRunning()
		{
			return this->running;
		}
	
		void Thread::join()
		{
			pthread_t thisId = this->id;
			
			if( thisId != 0 )
			{
				#ifdef DEBUG1
				Logger::get() << "[Thread#" << thisId << "] Waiting for thread." << Logger::endl;
				#endif
			
				if( pthread_join( thisId, NULL ) != 0 )
				{
					#ifdef DEBUG1
					Logger::get() << "[Thread#" << thisId << "] Could not wait for the thread." << Logger::endl;
					#endif
				}
			}
		}
	}
}

