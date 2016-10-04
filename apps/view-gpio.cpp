#include <tools/logger/Stdout.h>
#include <tools/gpio/GPIO.h>

#include <cstdlib>
#include <unistd.h>
#include <signal.h>

#include <vector>

using namespace std;
using namespace tools::logger;
using namespace tools::gpio;

bool running = true;

void signalHandler( int signo )
{
	if( signo == SIGTERM )
	{
		#ifdef DEBUG0
		Logger::get() << "Received SIGTERM." << Logger::endl;
		#endif
		
		running = false;
	}
	else if( signo == SIGINT )
	{
		#ifdef DEBUG0
		Logger::get() << "Received SIGINT." << Logger::endl;
		#endif
		
		running = false;
	}
}

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( signal( SIGTERM, signalHandler ) == SIG_ERR )
	{
		Logger::get() << "Unable to set handle for SIGTERM signals." << Logger::endl;
		Logger::destroy();
		exit( 1 );
	}
	
	if( signal( SIGINT, signalHandler ) == SIG_ERR )
	{
		Logger::get() << "Unable to set handle for SIGKILL signals." << Logger::endl;
		Logger::destroy();
		exit( 1 );
	}
	
	vector<int> ports;
	ports.push_back( 4 );
	ports.push_back( 17 );
	ports.push_back( 27 );
	ports.push_back( 22 );
	ports.push_back( 10 );
	ports.push_back( 9 );
	ports.push_back( 11 );
	ports.push_back( 5 );
	ports.push_back( 6 );
	ports.push_back( 13 );
	ports.push_back( 19 );
	ports.push_back( 26 );
	ports.push_back( 14 );
	ports.push_back( 15 );
	ports.push_back( 18 );
	ports.push_back( 23 );
	ports.push_back( 24 );
	ports.push_back( 25 );
	ports.push_back( 8 );
	ports.push_back( 7 );
	ports.push_back( 12 );
	ports.push_back( 16 );
	ports.push_back( 20 );
	ports.push_back( 21 );

	GPIO::initialize();
	
	for( vector<int>::iterator it = ports.begin() ; it != ports.end() ; it++ )
		GPIO::get()->open( *it, GPIO::In );

	while( running )
	{
		for( vector<int>::iterator it = ports.begin() ; it != ports.end() ; it++ )
		{
			bool status = GPIO::get()->read( *it );
			
			if( status )
			{
				Logger::get() << "GPIO#" << *it << " pushed." << Logger::endl;
			}
		}
		
		// Wait 50 ms
		usleep( 50000 );
	}
	
	GPIO::destroy();
	Logger::destroy();
	
	return 0;
}

