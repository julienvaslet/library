#ifndef __AUDIO_INSTRUMENT_SILENCE_H
#define __AUDIO_INSTRUMENT_SILENCE_H	1

#include <audio/instrument/Instrument.h>

namespace audio
{
	namespace instrument
	{
		class Silence : public Instrument
		{
			public:
				Silence( unsigned int samplingFrequency = 44100, unsigned short int channels = 2 );
				virtual ~Silence();
				
				virtual Sound * generate( float note, unsigned int duration, float volume = 0.9f );
		};
	}
}

#endif

