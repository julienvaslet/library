#include <audio/instrument/Square.h>
#include <cmath>

namespace audio
{
	namespace instrument
	{
		Square::Square( unsigned int samplingFrequency, unsigned short int channels ) : Instrument(samplingFrequency, channels)
		{
		}
	
		Square::~Square()
		{
		}
		
		Sound * Square::generate( float note, unsigned int duration, float volume )
		{
			Sound * sound = new Sound( this->samplingFrequency, this->channels );
			
			vector<int> noteData;
			
			unsigned int transitionSize = 0.025f * this->samplingFrequency;
			double period = static_cast<double>( this->samplingFrequency ) / note;
			unsigned int dataLength = static_cast<unsigned int>( ceil( (this->samplingFrequency * duration / 1000.0) / period ) * period );
			
			noteData.reserve( dataLength );
	
			unsigned int i = 0;

			for( i = 0 ; i < dataLength ; i++ )
			{
				float transitionVolume = 1.0f;
				
				if( i < transitionSize )
					transitionVolume = static_cast<float>( i ) / static_cast<float>( transitionSize );
					
				else if( dataLength - i < transitionSize )
					transitionVolume = static_cast<float>( dataLength - i ) / static_cast<float>( transitionSize );
					
				if( transitionVolume < 1.0f )
					transitionVolume = (sin( (transitionVolume * M_PI) - (M_PI / 2.0f) ) + 1.0f) / 2.0f;
				
				unsigned int iDegrees = 360.0 * ((i % static_cast<int>(period)) / period);
				int value = ( iDegrees < 180 ? INT_MAX : INT_MIN ) * volume * transitionVolume;
		
				for( unsigned short int c = 0 ; c < this->channels ; c++ )
					noteData.push_back( value );
			}
	
			sound->setRawData( noteData );
			return sound;
		}
	}
}

