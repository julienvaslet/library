#ifndef __CONTROLLER_VIRTUALDEVICE_H
#define __CONTROLLER_VIRTUALDEVICE_H	1

#include <string>

#include <linux/input.h>
#include <linux/uinput.h>

using namespace std;

#define UINPUT_DEVICE	"/dev/uinput"

namespace controller
{
	class VirtualDevice
	{
		protected:
			int descriptor;
			uinput_user_dev device;
			input_event event;
		
		public:
			VirtualDevice( const string& name );
			virtual ~VirtualDevice();
			
			const char * getName() const;
			
			bool enableEventType( unsigned int type );
			bool enableEvent( unsigned int type, unsigned int event );
			void sendEvent( unsigned int type, unsigned int event, int value, bool sync = true );
			
			void sendSynchronizationEvent( unsigned int syn, int value );
			void flush();
			
			bool enableKey( unsigned int key );
			void sendKey( unsigned int key, int value, bool sync = true );
			void pressKey( unsigned int key, bool sync = true );
			void releaseKey( unsigned int key, bool sync = true );
			
			bool enableRelativeAxis( unsigned int rel );
			void sendRelativeAxis( unsigned int rel, int value, bool sync = true );
			
			bool enableAbsoluteAxis( unsigned int abs );
			void sendAbsoluteAxis( unsigned int abs, int value, bool sync = true );

			bool create();
			
			bool isValid() const;
	};
}

#endif
