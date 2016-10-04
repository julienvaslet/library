#ifndef __AUDIO_SONG_H
#define __AUDIO_SONG_H	1

#include <audio/Sound.h>
#include <audio/Note.h>
#include <audio/instrument/Instrument.h>

namespace audio
{
	class Song : public audio::Sound
	{
		protected:
			unsigned int tempo;
		
		public:
			Song( unsigned int tempo = 120, unsigned int samplingFrequency = 44100, unsigned short int channels = 2 );
			virtual ~Song();
			
			unsigned int getTempo() const;
			unsigned int getNoteLength( Note::Duration duration, bool dotted = false, bool doubleDotted = false, bool tripleDotted = false );
			unsigned int getNoteDuration( Note::Duration duration, bool dotted = false, bool doubleDotted = false, bool tripleDotted = false );
			
			void mixNote( instrument::Instrument& instrument, float note, Note::Duration duration, bool dotted = false, bool doubleDotted = false, bool tripleDotted = false );
			
	};
}

#endif

