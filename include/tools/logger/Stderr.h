#ifndef __TOOLS_LOGGER_STDERR_H
#define __TOOLS_LOGGER_STDERR_H	1

#include <tools/logger/Logger.h>

using namespace std;

namespace tools
{
	namespace logger
	{
		class Stderr : public Logger
		{
			protected:
			
			public:
				Stderr( const string& name, bool defaultLogger = false );
				virtual ~Stderr();
			
				virtual Logger& operator<<( const string& value );
		};
	}
}

#endif

