#ifndef __AUDIO_INSTRUMENT_SINE_H
#define __AUDIO_INSTRUMENT_SINE_H	1

#include <audio/instrument/Instrument.h>

namespace audio
{
	namespace instrument
	{
		class Sine : public Instrument
		{
			public:
				Sine( unsigned int samplingFrequency = 44100, unsigned short int channels = 2 );
				virtual ~Sine();
				
				virtual Sound * generate( float note, unsigned int duration, float volume = 0.9f );
		};
	}
}

#endif

