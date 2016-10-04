#include <audio/Note.h>
#include <cmath>

namespace audio
{
	Note::Note()
	{
	}
	
	Note::~Note()
	{
	}
	
	float Note::getFrequency( char note, bool sharp, int octave, float A4 )
	{
		float distance = 0.0f;
		
		if( note >= 'A' && note <= 'G' )
		{
			// Computing distance from A4 and translating it to C5
			distance = ( note - 'A' ) * 2.0f - ( note >= 'F' ? 2.0f : ( note >= 'C' ? 1.0f : 0.0f ) ) + ( sharp ? 1.0f : 0.0f ) - 3.0f;
			
			if( distance < 0.0f )
				distance += 12.0f;
			
			// B#4 is C5...
			if( note == 'B' && sharp )
				octave++;
			
			// Considering octaves
			distance += (octave - 5) * 12.0f;
			
			// Translating to A4
			distance += 3.0f;
		}
		
		return A4 * pow( 2.0f, distance / 12.0f );
	}
}

