#include <controller/VirtualDevice.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

extern int errno;

namespace controller
{
	VirtualDevice::VirtualDevice( const string& name ) : descriptor(-1)
	{
		memset( &(this->device), 0, sizeof(this->device) );
		
		snprintf( this->device.name, UINPUT_MAX_NAME_SIZE, name.c_str() );
		this->device.id.bustype = BUS_USB;
		this->device.id.vendor  = 0x0;
		this->device.id.product = 0x0;
		this->device.id.version = 0;
		
		this->descriptor = open( UINPUT_DEVICE, O_WRONLY | O_NONBLOCK );
		
		#ifdef DEBUG0
		if( this->descriptor == -1 )
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to open uinput device (" << UINPUT_DEVICE << "): " << strerror( errno ) << Logger::endl;
		else
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Initialized." << Logger::endl;
		#endif
	}
	
	VirtualDevice::~VirtualDevice()
	{
		if( this->descriptor > 0 )
		{
			ioctl( this->descriptor, UI_DEV_DESTROY );
			close( this->descriptor );
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Destroyed." << Logger::endl;
			#endif
		}
	}

	const char * VirtualDevice::getName() const
	{
		return this->device.name;
	}
	
	// Global event functions
	
	bool VirtualDevice::enableEventType( unsigned int type )
	{	
		return ( ioctl( this->descriptor, UI_SET_EVBIT, type ) == 0 );
	}
	
	bool VirtualDevice::enableEvent( unsigned int type, unsigned int event )
	{
		return ( ioctl( this->descriptor, type, event ) == 0 );
	}
		
	void VirtualDevice::sendEvent( unsigned int type, unsigned int event, int value, bool sync )
	{
		if( this->descriptor > 0 )
		{
			memset( &(this->event), 0, sizeof(this->event) );
			gettimeofday( &(this->event.time), NULL );
			
			this->event.type = type;
			this->event.code = event;
			this->event.value = value;
			
			write( this->descriptor, &(this->event), sizeof(this->event) );
			
			if( sync && type != EV_SYN )
				this->flush();
		}
	}
	
	// Synchronization events

	void VirtualDevice::sendSynchronizationEvent( unsigned int syn, int value )
	{
		this->sendEvent( EV_SYN, syn, value, false );
	}
	
	void VirtualDevice::flush()
	{
		this->sendSynchronizationEvent( 0, 0 );
	}
	
	// Keyboard events
	
	bool VirtualDevice::enableKey( unsigned int key )
	{
		bool status = true;
		
		if( this->enableEventType( EV_KEY ) )
		{
			if( !this->enableEvent( UI_SET_KEYBIT, key ) )
			{
				status = false;
		
				#ifdef DEBUG0
				Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable the specified keyboard event: " << strerror( errno ) << Logger::endl;
				#endif
			}
		}
		else
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable keyboard events: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	void VirtualDevice::sendKey( unsigned int key, int value, bool sync )
	{
		this->sendEvent( EV_KEY, key, value, sync );
	}
	
	void VirtualDevice::pressKey( unsigned int key, bool sync )
	{
		this->sendKey( key, 1, sync );
	}
	
	void VirtualDevice::releaseKey( unsigned int key, bool sync )
	{
		this->sendKey( key, 0, sync );
	}
	
	// Relative Axis events
	
	bool VirtualDevice::enableRelativeAxis( unsigned int rel )
	{
		bool status = true;
		
		if( this->enableEventType( EV_REL ) )
		{
			if( !this->enableEvent( UI_SET_RELBIT, rel ) )
			{
				status = false;
			
				#ifdef DEBUG0
				Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable the specified relative axis event: " << strerror( errno ) << Logger::endl;
				#endif
			}
		}
		else
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable relative axis events: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	void VirtualDevice::sendRelativeAxis( unsigned int rel, int value, bool sync )
	{
		this->sendEvent( EV_REL, rel, value, sync );
	}
	
	// Absolute Axis events
	
	bool VirtualDevice::enableAbsoluteAxis( unsigned int abs )
	{
		bool status = true;
		
		if( this->enableEventType( EV_ABS ) )
		{
			if( !this->enableEvent( UI_SET_ABSBIT, abs ) )
			{
				status = false;
			
				#ifdef DEBUG0
				Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable the specified absolute axis event: " << strerror( errno ) << Logger::endl;
				#endif
			}
		}
		else
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable absolute axis events: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	void VirtualDevice::sendAbsoluteAxis( unsigned int abs, int value, bool sync )
	{
		this->sendEvent( EV_ABS, abs, value, sync );
	}
	
	bool VirtualDevice::create()
	{
		if( this->descriptor > 0 )
		{
			write( this->descriptor, &device, sizeof(device) );
		
			// Create the virtual device
			if( ioctl( this->descriptor, UI_DEV_CREATE ) == -1 )
			{
				#ifdef DEBUG0
				Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to create the device: " << strerror( errno ) << Logger::endl;
				#endif
				
				close( this->descriptor );
				this->descriptor = -1;
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[VirtualDevice#" << this->getName() << "] Created." << Logger::endl;
			#endif
		}
		
		return false;
	}
	
	bool VirtualDevice::isValid() const
	{
		return this->descriptor > 0;
	}
}

