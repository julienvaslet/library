#ifndef __AUDIO_INSTRUMENT_SAWTOOTH_H
#define __AUDIO_INSTRUMENT_SAWTOOTH_H	1

#include <audio/instrument/Instrument.h>

namespace audio
{
	namespace instrument
	{
		class SawTooth : public Instrument
		{
			public:
				SawTooth( unsigned int samplingFrequency = 44100, unsigned short int channels = 2 );
				virtual ~SawTooth();
				
				virtual Sound * generate( float note, unsigned int duration, float volume = 0.9f );
		};
	}
}

#endif

