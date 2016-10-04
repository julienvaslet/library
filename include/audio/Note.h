#ifndef __AUDIO_NOTE_H
#define __AUDIO_NOTE_H 1

namespace audio
{
	class Note
	{
		public:
			// French terms
			enum Duration
			{
				QuadrupleCroche = -4,
				TripleCroche = -3,
				DoubleCroche = -2,
				Croche = -1,
				Noire = 0,
				Blanche = 1,
				Ronde = 2,
				Carree = 3
			};
		
		private:
			Note();
			~Note();
			
		public:
			static float getFrequency( char note, bool sharp = false, int octave = 4, float A4 = 440.00f );
	};
}
				
#endif

