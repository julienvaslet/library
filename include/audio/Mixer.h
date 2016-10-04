#ifndef __AUDIO_MIXER_H
#define __AUDIO_MIXER_H	1

#include <audio/PlayingSound.h>

#ifdef __NO_X_WINDOW__

extern "C" {
#include <audio/broadcom/ilclient.h>
}

namespace audio
{
	class Mixer;
}

#include <audio/MixerThread.h>

#else

#include <SDL2/SDL.h>

#endif

#include <string>
#include <map>

using namespace std;

namespace audio
{
	class Mixer
	{
		friend class MixerThread;
			
		public:
			static Mixer * instance;
			
			#ifdef __NO_X_WINDOW__
			
			/* Raspberry PI BCM implementation */
			
			static string errorToString( OMX_ERRORTYPE error );
			static void errorCallback( void * userdata, COMPONENT_T * comp, OMX_U32 data );
			bool setDestination( unsigned int destination );
			unsigned int getLatency();
			
			MixerThread * thread;
			ILCLIENT_T * client;
			COMPONENT_T * audioRender;
			OMX_BUFFERHEADERTYPE * userBufferList;
			COMPONENT_T * list[2];
			sem_t semaphore;
			unsigned int numBuffers;
			unsigned int bytesPerSample;
			
			#else
			
			/* SDL implementation */
			
			static void convertStream( const vector<int>& inStream, Uint8 * outStream, SDL_AudioFormat format );
			static unsigned int getRealSamples( unsigned int samples, SDL_AudioFormat format );
			static void callback( void * userdata, Uint8 * stream, int len );
			
			SDL_AudioFormat getAudioFormat() const;
			
			SDL_AudioFormat format;
			SDL_AudioDeviceID device;
			
			#endif
			
			/* Common attributes */
			
			unsigned int samplingFrequency;
			unsigned short int channels;
			unsigned int samples;
			
			map<string, PlayingSound *> sounds;
			
			void clearSounds();
			void lockAudio();
			void unlockAudio();
			Sound * fillBuffer( unsigned int bufferLength );
			
		public:
			static Mixer * get();
			static void destroy();
			
			Mixer( unsigned int samplingFrequency, unsigned short int channels, unsigned int samples );
			virtual ~Mixer();
			
			unsigned int getSamplingFrequency() const;
			unsigned short int getChannels() const;
			
			void add( const string& name, Sound * sound, bool oneTimePlaying = false );
			void play( const string& name, unsigned int ticks = 0 );
			void stop( const string& name );
			void setRepeat( const string& name, bool repeat, unsigned int times = 0 );
			void setPitch( const string& name, double pitch = 1.0 );
			bool isPlaying();
			
			void clean();
	};
}

#endif
