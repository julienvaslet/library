#include <audio/instrument/Silence.h>

namespace audio
{
	namespace instrument
	{
		Silence::Silence( unsigned int samplingFrequency, unsigned short int channels ) : Instrument(samplingFrequency, channels)
		{
		}
	
		Silence::~Silence()
		{
		}
		
		Sound * Silence::generate( float note, unsigned int duration, float volume )
		{
			Sound * sound = new Sound( this->samplingFrequency, this->channels );
			
			double period = static_cast<double>( this->samplingFrequency ) / note;
			unsigned int dataLength = static_cast<unsigned int>( ceil( (this->samplingFrequency * duration / 1000.0) / period ) * period );
			
			vector<int> noteData;
			noteData.reserve( dataLength );
	
			unsigned int i = 0;

			for( i = 0 ; i < dataLength ; i++ )
			{
				for( unsigned short int c = 0 ; c < this->channels ; c++ )
					noteData.push_back( 0 );
			}
	
			sound->setRawData( noteData );
			return sound;
		}
	}
}

