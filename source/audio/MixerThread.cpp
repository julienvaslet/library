#include <audio/MixerThread.h>
#include <unistd.h>
#include <climits>
#include <vector>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

using namespace std;

#define SLEEP_TIME 10
#define MIN_LATENCY_TIME 20

namespace audio
{
	MixerThread::MixerThread( Mixer * mixer ) : Thread(), mixer(mixer), allowedLatency(mixer->samplingFrequency * (MIN_LATENCY_TIME + SLEEP_TIME) / 1000)
	{
	}
	
	MixerThread::~MixerThread()
	{
	}
	
	int MixerThread::life()
	{
		#ifdef __NO_X_WINDOW__
		while( this->running )
		{
			this->mixer->lockAudio();
			
			bool shouldPlay = false;
			
			for( map<string, PlayingSound *>::iterator it = this->mixer->sounds.begin() ; it != this->mixer->sounds.end() ; it++ )
			{
				if( it->second->isPlaying() )
				{
					shouldPlay = true;
					break;
				}
			}

			if( shouldPlay )
			{
				OMX_BUFFERHEADERTYPE * hdr = ilclient_get_input_buffer( this->mixer->audioRender, 100, 0 );
			
				if( hdr != NULL )
				{			
					// Just storing one buffer (not using the chained-list with ->pAppPrivate)
					this->mixer->userBufferList = hdr;
				
					// this size is the initial buffersize * the number of channels!!!
					unsigned int bufferLength = hdr->nAllocLen / sizeof(uint32_t);
					
					Sound * sound = this->mixer->fillBuffer( bufferLength );
				
					// Filling buffer
					int * buffer = (int *) hdr->pBuffer;
				
					for( unsigned int i = 0 ; i < sound->getRawLength() ; i++ )
						buffer[i] = sound->getRawData( i );
				
					hdr->nFilledLen = (sound->getRawLength() * 32) >> 3;
		  			hdr->pAppPrivate = NULL;
		  			hdr->nOffset = 0;
		  			
					delete sound;
		  			
		  			OMX_ERRORTYPE error = OMX_EmptyThisBuffer( ILC_GET_HANDLE(this->mixer->audioRender), hdr );

		  			if( error == OMX_ErrorNone )
		  				this->mixer->userBufferList = NULL;
		  			#ifdef DEBUG0
		  			else
		  				Logger::get() << "[Mixer] Unable to empty buffer: " << Mixer::errorToString( error ) << "." << Logger::endl;
		  			#endif
				}
			}
			
			this->mixer->unlockAudio();
			
			while( this->mixer->getLatency() > this->allowedLatency )
				usleep( SLEEP_TIME * 1000 );
		}
		#endif
		
		return 0;
	}
	
	void MixerThread::stop()
	{
		this->running = false;
	}
}

