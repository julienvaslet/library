#include <tools/logger/Stderr.h>
#include <iostream>
using namespace std;

namespace tools
{
	namespace logger
	{
		Stderr::Stderr( const string& name, bool defaultLogger ) : Logger( name, defaultLogger )
		{
		}
		
		Stderr::~Stderr()
		{
		}
		
		Logger& Stderr::operator<<( const string& value )
		{
			cerr << value;
			return *this;
		}
	}
}

