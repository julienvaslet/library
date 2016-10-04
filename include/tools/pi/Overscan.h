#ifndef __TOOLS_PI_OVERSCAN_H
#define __TOOLS_PI_OVERSCAN_H	1

namespace tools
{
	namespace pi
	{
		class Overscan
		{
			private:
				static int fileDescriptor;
				static bool deleteFileOnDestroy;
				
				Overscan();
				~Overscan();
				
				static bool sendCommand( void * buffer );
			
			public:
				static bool initialize();
				static void get( int * left, int * right, int * top, int * bottom );
				static void set( int * left, int * right, int * top, int * bottom );
				static void destroy();
		};
	}
}

#endif

