#ifndef __TOOLS_LOGGER_LOGGER_H
#define __TOOLS_LOGGER_LOGGER_H	1

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdarg>

using namespace std;

namespace tools
{
	namespace logger
	{
		class Logger;
		typedef Logger& (*LoggerFunction)( Logger& );
		
		class Logger
		{
			protected:
				static Logger * defaultLogger;
				static map<string,Logger *> instances;
		
				Logger( const string& name, bool defaultLogger = false );
				virtual ~Logger();
			
			public:
				void printf( const char * format, ... );
			
				static Logger& get();
				static Logger& get( const string& name );
				static void destroy();
				
				static Logger& endl( Logger& logger );
			
				virtual Logger& operator<<( const string& value ) = 0;
				virtual Logger& operator<<( bool value );
				virtual Logger& operator<<( char value );
				virtual Logger& operator<<( short value );
				virtual Logger& operator<<( unsigned short value );
				virtual Logger& operator<<( int value );
				virtual Logger& operator<<( unsigned int value );
				virtual Logger& operator<<( long value );
				virtual Logger& operator<<( unsigned long value );
				virtual Logger& operator<<( float value );
				virtual Logger& operator<<( double value );
				virtual Logger& operator<<( const void * value );
				virtual Logger& operator<<( const char * value );
				
				template<class T> Logger& operator<<( T& value )
				{
					stringstream ss;
					ss << value;
					*this << ss.str();
					return *this;
				}
				
				Logger& operator<<( LoggerFunction func );
		};
	}
}

#endif

