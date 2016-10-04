#ifndef __TOOLS_LOGGER_STDOUT_H
#define __TOOLS_LOGGER_STDOUT_H	1

#include <tools/logger/Logger.h>

using namespace std;

namespace tools
{
	namespace logger
	{
		class Stdout : public Logger
		{
			protected:
			
			public:
				Stdout( const string& name, bool defaultLogger = false );
				virtual ~Stdout();
			
				virtual Logger& operator<<( const string& value );
		};
	}
}

#endif

