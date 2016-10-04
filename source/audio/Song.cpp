#include <audio/Song.h>
#include <cmath>

#include <iostream>

namespace audio
{
	Song::Song( unsigned int tempo, unsigned int frequency, unsigned short int channels ) : Sound(frequency,channels), tempo(tempo)
	{
	}
	
	Song::~Song()
	{
	}

	unsigned int Song::getTempo() const
	{
		return this->tempo;
	}
	
	unsigned int Song::getNoteLength( Note::Duration duration, bool dotted, bool doubleDotted, bool tripleDotted )
	{
		return static_cast<unsigned int>( static_cast<double>( this->getNoteDuration( duration, dotted, doubleDotted, tripleDotted ) ) / 1000.0 * this->frequency * this->channels );
	}
	
	unsigned int Song::getNoteDuration( Note::Duration duration, bool dotted, bool doubleDotted, bool tripleDotted )
	{
		if( tripleDotted )
		{
			dotted = true;
			doubleDotted = true;
		}
		else if( doubleDotted )
			dotted = true;
		
		double baseDuration = pow( 2.0, static_cast<double>( duration ) );
		double relativeDuration = baseDuration;
		
		if( dotted )
		{
			relativeDuration += baseDuration / 2.0;
			
			if( doubleDotted )
			{
				relativeDuration += baseDuration / 4.0;
				
				if( tripleDotted )
				{
					relativeDuration += baseDuration / 8.0;
				}
			}
		}
			
		return static_cast<unsigned int>( relativeDuration * ( 60000.0 / static_cast<double>( this->tempo ) ) );
	}
	
	void Song::mixNote( instrument::Instrument& instrument, float note, Note::Duration duration, bool dotted, bool doubleDotted, bool tripleDotted )
	{
		Sound * sound = instrument.generate( note, this->getNoteDuration( duration, dotted, doubleDotted, tripleDotted ), 0.9f );
		this->rawMix( sound, this->getRawLength() );
		delete sound;
	}
}

