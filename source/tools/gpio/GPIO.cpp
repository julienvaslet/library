#include <tools/gpio/GPIO.h>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#ifdef DEBUG0
#include <cstring>
#include <errno.h>
extern int errno;

#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#define GPIO_PATH(path)		"/sys/class/gpio/" #path ""

namespace tools
{
	namespace gpio
	{
		GPIO * GPIO::instance = NULL;
		
		GPIO::GPIO()
		{
			#ifdef DEBUG0
			Logger::get() << "[GPIO] Initialized." << Logger::endl;
			#endif
		}
		
		GPIO::~GPIO()
		{
			this->closeAll();
			
			#ifdef DEBUG0
			Logger::get() << "[GPIO] Destroyed." << Logger::endl;
			#endif
		}
		
		bool GPIO::open( int pin, Direction direction )
		{
			bool status = true;
			ofstream exportGpio( GPIO_PATH(export) );
			
			if( exportGpio.is_open() )
			{
				exportGpio << pin;
				exportGpio.close();
				
				status = this->setDirection( pin, direction );
				
				#ifdef DEBUG0
				Logger::get() << "[GPIO#" << pin << "] Exported pin." << Logger::endl;
				#endif
			}
			else
			{
				status = false;
				
				#ifdef DEBUG0
				Logger::get() << "[GPIO#" << pin << "] Unable to open export file (" << GPIO_PATH(export) << "): " << strerror(errno) << "." << Logger::endl;
				#endif
			}
			
			return status;
		}
		
		bool GPIO::setDirection( int pin, Direction direction )
		{
			bool status = true;
			stringstream path;
			path << GPIO_PATH(gpio) << pin << "/direction";
			string pathStr = path.str();
			ofstream directionGpio( pathStr.c_str() );
			
			if( directionGpio.is_open() )
			{
				directionGpio << ( direction == Out ? "out" : "in" );
				directionGpio.close();
				
				this->pins[pin] = direction;
				
				#ifdef DEBUG0
				Logger::get() << "[GPIO#" << pin << "] Direction set to \"" << ( direction == Out ? "out" : "in" ) << "\"." << Logger::endl;
				#endif
			}
			else
			{
				status = false;
				
				#ifdef DEBUG0
				Logger::get() << "[GPIO#" << pin << "] Unable to open direction file (" << pathStr << "): " << strerror(errno) << "." << Logger::endl;
				#endif
			}
			
			return status;
		}
		
		bool GPIO::read( int pin )
		{
			bool value = false;
			
			if( this->isOpen( pin, In ) )
			{
				stringstream path;
				path << GPIO_PATH(gpio) << pin << "/value";
				string pathStr = path.str();
				ifstream valueGpio( pathStr.c_str() );
			
				if( valueGpio.is_open() )
				{
					string valueStr;
					valueGpio >> valueStr;
				
					if( valueStr.at(0) == '1' )
						value = true;
				
					valueGpio.close();
				}
				#ifdef DEBUG0
				else
					Logger::get() << "[GPIO#" << pin << "] Unable to opend value file (" << pathStr << "): " << strerror(errno) << "." << Logger::endl;
				#endif
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[GPIO#" << pin << "] Pin is not opened for read operations." << Logger::endl;
			#endif
			
			return value;
		}
		
		void GPIO::write( int pin, bool value )
		{
			if( this->isOpen( pin, Out ) )
			{
				stringstream path;
				path << GPIO_PATH(gpio) << pin << "/value";
				string pathStr = path.str();
				ofstream valueGpio( pathStr.c_str() );
			
				if( valueGpio.is_open() )
				{
					valueGpio << ( value ? "1" : "0" );
					valueGpio.close();
				}
				#ifdef DEBUG0
				else
					Logger::get() << "[GPIO#" << pin << "] Unable to open value file (" << pathStr << "): " << strerror(errno) << "." << Logger::endl;
				#endif
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[GPIO#" << pin << "] Pin is not opened for write operations." << Logger::endl;
			#endif
		}
		
		bool GPIO::close( int pin )
		{
			bool status = true;
			ofstream unexportGpio( GPIO_PATH(unexport) );
			
			if( unexportGpio.is_open() )
			{
				unexportGpio << pin;
				unexportGpio.close();
				
				this->pins.erase( pin );
				
				#ifdef DEBUG0
				Logger::get() << "[GPIO#" << pin << "] Unexported pin." << Logger::endl;
				#endif
			}
			else
			{
				status = false;
				
				#ifdef DEBUG0
				Logger::get() << "[GPIO#" << pin << "] Unable to open unexport file (" << GPIO_PATH(unexport) << "): " << strerror(errno) << "." << Logger::endl;
				#endif
			}
			
			return status;
		}
		
		void GPIO::closeAll()
		{
			vector<int> pinNumbers;
			
			for( map<int, Direction>::iterator it = this->pins.begin() ; it != this->pins.end() ; it++ )
				pinNumbers.push_back( it->first );
				
			for( vector<int>::iterator it = pinNumbers.begin() ; it != pinNumbers.end() ; it++ )
				this->close( *it );
		}
		
		bool GPIO::isOpen( int pin, Direction direction )
		{
			bool status = false;
			
			map<int, Direction>::iterator it = this->pins.find( pin );
			
			if( it != this->pins.end() )
				status = ( direction == it->second );
			
			return status;
		}
		
		void GPIO::logStatus()
		{
			#ifdef DEBUG0
			Logger::get() << "[GPIO] Logging pins' status: " << Logger::endl;
			
			for( map<int, Direction>::iterator it = this->pins.begin() ; it != this->pins.end() ; it++ )
				Logger::get() << "[GPIO#" << it->first << "] Opened for " << ( it->second == Out ? "write" : "read" ) << " operations." << Logger::endl;
			#endif
		}
		
		GPIO * GPIO::initialize()
		{
			if( GPIO::instance != NULL )
				GPIO::destroy();
			
			GPIO::instance = new GPIO();	
			return GPIO::instance;
		}
		
		GPIO * GPIO::get()
		{
			return GPIO::instance;
		}
		
		void GPIO::destroy()
		{
			if( GPIO::instance != NULL )
			{
				delete GPIO::instance;
				GPIO::instance = NULL;
			}
		}
	}
}

