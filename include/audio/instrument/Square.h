#ifndef __AUDIO_INSTRUMENT_SQUARE_H
#define __AUDIO_INSTRUMENT_SQUARE_H	1

#include <audio/instrument/Instrument.h>

namespace audio
{
	namespace instrument
	{
		class Square : public Instrument
		{
			public:
				Square( unsigned int samplingFrequency = 44100, unsigned short int channels = 2 );
				virtual ~Square();
				
				virtual Sound * generate( float note, unsigned int duration, float volume = 0.9f );
		};
	}
}

#endif

