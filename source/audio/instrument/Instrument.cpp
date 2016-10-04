#include <audio/instrument/Instrument.h>

namespace audio
{
	namespace instrument
	{
		Instrument::Instrument( unsigned int samplingFrequency, unsigned short int channels ) : samplingFrequency(samplingFrequency), channels(channels)
		{
		}
	
		Instrument::~Instrument()
		{
		}
	}
}

