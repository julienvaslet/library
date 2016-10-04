#include <tools/pi/Overscan.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#ifdef __NO_X_WINDOW__
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define	GET_OVERSCAN		0x0004000a
#define	SET_OVERSCAN		0x0004800a
#define END_TAG				0x00000000
#define PROCESS_REQUEST		0x00000000
#define REQUEST_SUCCESS		0x80000000
#define REQUEST_ERROR		0x80000001

#define MAILBOX_FILE		"/dev/vcio"
//#define MAILBOX_FILE		"/dev/mailbox"
#define DELTA_VALUE			32

extern int errno;
#endif

// Based on https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface

namespace tools
{
	namespace pi
	{
		int Overscan::fileDescriptor = -1;
		bool Overscan::deleteFileOnDestroy = false;
		
		Overscan::Overscan()
		{
		}
		
		Overscan::~Overscan()
		{
		}
		
		bool Overscan::initialize()
		{
			#ifdef __NO_X_WINDOW__
			if( Overscan::fileDescriptor == -1 )
			{
				if( access( MAILBOX_FILE, F_OK ) != 0 )
				{
					if( mknod( MAILBOX_FILE, S_IFCHR, makedev( 100, 0 ) ) == 0 )
					{
						Overscan::deleteFileOnDestroy = true;
						
						#ifdef DEBUG0
						Logger::get() << "[Overscan] \"" MAILBOX_FILE "\" created." << Logger::endl;
						#endif
					}
					#ifdef DEBUG0
					else
						Logger::get() << "[Overscan] Unable to create node \"" MAILBOX_FILE "\": " << strerror( errno ) << Logger::endl;
					#endif
				}
				
				Overscan::fileDescriptor = open( MAILBOX_FILE, 0 );
				
				#ifdef DEBUG0
				if( Overscan::fileDescriptor == -1 )
					Logger::get() << "[Overscan] Unable to open node \"" MAILBOX_FILE "\": " << strerror( errno ) << Logger::endl;
				#endif
			}
			
			return Overscan::fileDescriptor > -1;
			#else
			return true;
			#endif
		}
		
		bool Overscan::sendCommand( void * buffer )
		{
			#ifdef __NO_X_WINDOW__
			bool success = false;
			
			if( Overscan::fileDescriptor > -1 )
			{
				if( ioctl( Overscan::fileDescriptor, _IOWR(100, 0, char *), buffer ) == 0 )
					success = true;
				else
				{
					#ifdef DEBUG1
					Logger::get() << "Unable to send command via ioctl: " << strerror( errno ) << Logger::endl;
					#endif
				}
			}
			
			return success;
			#else
			return true;
			#endif
		}
		
		void Overscan::get( int * left, int * right, int * top, int * bottom )
		{
			#ifdef __NO_X_WINDOW__
			int i = 0;
			unsigned int property[32];
			property[i++] = 0;
			property[i++] = PROCESS_REQUEST;

			property[i++] = GET_OVERSCAN; 
			property[i++] = 0x00000010; 
			property[i++] = 0x00000000;
			property[i++] = 0x00000000; 
			property[i++] = 0x00000000;
			property[i++] = 0x00000000;
			property[i++] = 0x00000000;
			property[i++] = END_TAG;
			property[0] = i * sizeof( *property );

			if( Overscan::sendCommand( property ) )
			{
				if( property[1] == REQUEST_SUCCESS )
				{
					*top = static_cast<int>( property[5] ) - DELTA_VALUE;
					*bottom = static_cast<int>( property[6] ) - DELTA_VALUE;
					*left = static_cast<int>( property[7] ) - DELTA_VALUE;
					*right = static_cast<int>( property[8] ) - DELTA_VALUE;
				}
				#ifdef DEBUG0
				else
					Logger::get() << "[Overscan] Get overscan request failed." << Logger::endl;
				#endif
			}
			#endif
		}
		
		void Overscan::set( int * left, int * right, int * top, int * bottom )
		{
			#ifdef __NO_X_WINDOW__
			int i = 0;
			unsigned int property[32];
			property[i++] = 0;
			property[i++] = PROCESS_REQUEST;
			property[i++] = SET_OVERSCAN;
			property[i++] = 0x00000010;
			property[i++] = 0x00000010;
			property[i++] = static_cast<unsigned int>( *top ) + DELTA_VALUE;
			property[i++] = static_cast<unsigned int>( *bottom ) + DELTA_VALUE;
			property[i++] = static_cast<unsigned int>( *left ) + DELTA_VALUE;
			property[i++] = static_cast<unsigned int>( *right ) + DELTA_VALUE;
			property[i++] = END_TAG; 
			property[0] = i * sizeof( *property );

			if( Overscan::sendCommand( property ) )
			{
				if( property[1] == REQUEST_SUCCESS )
				{
					*top = static_cast<int>( property[5] ) - DELTA_VALUE;
					*bottom = static_cast<int>( property[6] ) - DELTA_VALUE;
					*left = static_cast<int>( property[7] ) - DELTA_VALUE;
					*right = static_cast<int>( property[8] ) - DELTA_VALUE;
				}
				#ifdef DEBUG0
				else
					Logger::get() << "[Overscan] Set overscan request failed." << Logger::endl;
				#endif
			}
			#endif
		}
		
		void Overscan::destroy()
		{
			#ifdef __NO_X_WINDOW__
			if( Overscan::fileDescriptor > -1 )
			{
				close( Overscan::fileDescriptor );
				Overscan::fileDescriptor = -1;
				
				if( deleteFileOnDestroy )
				{
					remove( MAILBOX_FILE );
					deleteFileOnDestroy = false;
					
					#ifdef DEBUG0
					Logger::get() << "[Overscan] Deleted file \"" MAILBOX_FILE "\"." << Logger::endl;
					#endif
				}
			}
			#endif
		}
	}
}

