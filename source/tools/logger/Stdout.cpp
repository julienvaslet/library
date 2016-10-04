#include <tools/logger/Stdout.h>
#include <iostream>
using namespace std;

namespace tools
{
	namespace logger
	{
		Stdout::Stdout( const string& name, bool defaultLogger ) : Logger( name, defaultLogger )
		{
		}
		
		Stdout::~Stdout()
		{
		}
		
		Logger& Stdout::operator<<( const string& value )
		{
			cout << value;
			return *this;
		}
	}
}

