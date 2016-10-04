#ifndef __AUDIO_INSTRUMENT_INSTRUMENT_H
#define __AUDIO_INSTRUMENT_INSTRUMENT_H	1

#include <SDL2/SDL.h>
#include <audio/Note.h>
#include <audio/Sound.h>
#include <climits>

namespace audio
{
	namespace instrument
	{
		class Instrument
		{
			protected:			
				unsigned int samplingFrequency;
				unsigned short int channels;
			
			public:
				Instrument( unsigned int samplingFrequency = 44100, unsigned short int channels = 2 );
				virtual ~Instrument();
				
				virtual Sound * generate( float note, unsigned int duration, float volume = 0.9f ) = 0;
		};
	}
}

#endif

