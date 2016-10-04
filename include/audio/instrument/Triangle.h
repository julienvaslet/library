#ifndef __AUDIO_INSTRUMENT_TRIANGLE_H
#define __AUDIO_INSTRUMENT_TRIANGLE_H	1

#include <audio/instrument/Instrument.h>

namespace audio
{
	namespace instrument
	{
		class Triangle : public Instrument
		{
			public:
				Triangle( unsigned int samplingFrequency = 44100, unsigned short int channels = 2 );
				virtual ~Triangle();
				
				virtual Sound * generate( float note, unsigned int duration, float volume = 0.9f );
		};
	}
}

#endif

