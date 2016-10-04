#ifndef __AUDIO_PLAYINGSOUND_H
#define __AUDIO_PLAYINGSOUND_H	1

#include <audio/Sound.h>

namespace audio
{
	class PlayingSound
	{
		protected:
			Sound * sound;
			unsigned int startDate;
			unsigned int position;
			
			double pitch;
			bool repeated;
			unsigned int repeatedTimes;
			unsigned int timesToRepeat;
			bool playing;
			//volume
			//balance
			
			bool oneTimePlaying;
			bool oneTimePlayed;
			
		public:
			PlayingSound( Sound * sound, unsigned int frequency, unsigned short int channels );
			virtual ~PlayingSound();
			
			Sound * getSound();
			unsigned int getStartDate() const;
			unsigned int getPosition() const;
			
			double getPitch() const;
			bool isRepeated() const;
			unsigned int getRepeatedTimes();
			bool isPlaying() const;
			
			void play( unsigned int ticks = 0 );
			void stop();
			
			void setOneTimePlaying( bool oneTimePlaying );
			bool hasPlayedOneTime() const;
			
			void setPosition( unsigned int position, bool relative = false );
			void setPitch( double pitch = 1.0 );
			void setRepeat( bool repeat, unsigned int times = 0 );
	};
}

#endif
