#include <tools/logger/Logger.h>

#ifdef DEBUG0
#include <iostream>
#endif

using namespace std;

namespace tools
{
	namespace logger
	{
		Logger * Logger::defaultLogger = NULL;
		map<string, Logger*> Logger::instances;
	
		Logger::Logger( const string& name, bool defaultLogger )
		{
			if( defaultLogger )
				Logger::defaultLogger = this;
				
			map<string, Logger*>::iterator it = Logger::instances.find( name );
			
			if( it != Logger::instances.end() )
			{
				delete it->second;
				Logger::instances[name] = this;
				
				#ifdef DEBUG0
				cout << "[Logger#" << name << "] Replaced Logger." << std::endl;
				#endif
			}
			else
			{
				Logger::instances[name] = this;
				
				#ifdef DEBUG0
				cout << "[Logger#" << name << "] Created." << std::endl;
				#endif
			}
			
			#ifdef DEBUG0
			if( defaultLogger )
				cout << "[Logger#" << name << "] Is now the default logger." << std::endl;
			#endif
		}
		
		Logger::~Logger()
		{
			#ifdef DEBUG0
			string name;
			#endif
			
			if( this == Logger::defaultLogger )
				Logger::defaultLogger = NULL;

			for( map<string, Logger*>::iterator it = Logger::instances.begin() ; it != Logger::instances.end() ; it++ )
			{
				if( it->second == this )
				{
					#ifdef DEBUG0
					name = it->first;
					#endif
					
					Logger::instances.erase( it );
					
					#ifdef DEBUG0
					cout << "[Logger#" << name << "] Deleted." << std::endl;
					#endif
					break;
				}
			}
		}
		
		Logger& Logger::get()
		{
			if( Logger::defaultLogger == NULL )
				throw "No default logger has been declared.";
				
			return *(Logger::defaultLogger);
		}
		
		Logger& Logger::get( const string& name )
		{
			map<string, Logger*>::iterator it = Logger::instances.find( name );
			
			if( it == Logger::instances.end() )
			{
				#ifdef DEBUG0
				cout << "[Logger] Logger not found, redirecting to default logger." << std::endl;
				#endif
				
				return Logger::get();
			}
			else
				return *(it->second);
		}
		
		void Logger::destroy()
		{
			for( map<string, Logger*>::iterator it = Logger::instances.begin() ; it != Logger::instances.end() ; it++ )
				delete it->second;

			Logger::instances.clear();
		}
		
		void Logger::printf( const char * format, ... )
		{
			char buffer[512];
			
			va_list args;
			va_start( args, format );
			vsprintf( buffer, format, args );
			va_end( args );
			
			Logger::get() << buffer;
		}
		
		Logger& Logger::operator<<( bool value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( char value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( short value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( unsigned short value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( int value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( unsigned int value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( long value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( unsigned long value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( float value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( double value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( const void * value )
		{
			stringstream ss;
			ss << value;
			return (*this << ss.str());
		}
		
		Logger& Logger::operator<<( const char * value )
		{
			string str(value);
			return (*this << str);
		}
		
		Logger& Logger::operator<<( LoggerFunction func )
		{
			return func(*this);
		}
		
		Logger& Logger::endl( Logger& logger )
		{
			logger << "\n";
			// flush?
			return logger;
		}
	}
}

