#ifndef __TOOLS_GPIO_GPIO_H
#define __TOOLS_GPIO_GPIO_H	1

#include <map>
using namespace std;

namespace tools
{
	namespace gpio
	{
		class GPIO
		{
			public:
				enum Direction
				{
					In = 0,
					Out = 1
				};
				
			protected:
				static GPIO * instance;
				map<int, Direction> pins;
				
				GPIO();
				virtual ~GPIO();

			public:
				bool open( int pin, Direction direction = In );
				bool setDirection( int pin, Direction direction );
				bool read( int pin );
				void write( int pin, bool value );
				bool close( int pin );
				void closeAll();
				
				bool isOpen( int pin, Direction direction );
				void logStatus();
				
				static GPIO * initialize();
				static GPIO * get();
				static void destroy();
		};
	}
}

#endif

