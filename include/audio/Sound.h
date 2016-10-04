#ifndef __AUDIO_SOUND_H
#define __AUDIO_SOUND_H	1

#include <SDL2/SDL.h>
#include <vector>

using namespace std;

namespace audio
{
	class Sound
	{
		protected:
			unsigned int frequency;
			unsigned short int channels;
			vector<int> data;
			
		public:
		
			/**
			 * Create a sound object.
			 * @param frequency	the sampling frequency of the sound, default value is 44100.
			 * @param channels the number of channels of the sound, default value is 2.
			 */
			Sound( unsigned int frequency = 44100, unsigned short int channels = 2 );
			
			/**
			 * Destroy a sound object.
			 */
			virtual ~Sound();
			
			/**
			 * Returns the duration, in milliseconds, of the sound.
			 * @return the sound's duration.
			 */
			unsigned int getDuration() const;
			
			/**
			 * Returns the raw length of the data.
			 * @return the sound's data raw length.
			 */
			unsigned int getRawLength() const;
			
			/**
			 * Returns the length of one channel of the sound.
			 * @return one channel's data length.
			 */
			unsigned int getChannelLength() const;
			
			/**
			 * Returns the sampling frequency of the sound.
			 * @return the sound's frequency.
			 */
			unsigned int getFrequency() const;
			
			/**
			 * Returns the channels' count of the sound.
			 * @return the channels' count.
			 */
			unsigned short int getChannels() const;
			
			/**
			 * Returns a reference to the sound's raw data.
			 * @return the raw data.
			 */
			const vector<int>& getRawData() const;
			
			/**
			 * Returns a raw part of the sound.
			 * @param startPosition
			 * @param length
			 * @return
			 */
			Sound * getRawData( unsigned int startPosition, unsigned int length );
			
			/**
			 * Returns the value from the raw specified position.
			 * @param position the raw position.
			 * @return the value.
			 */
			int getRawData( unsigned int position ) const;
			
			/**
			 * Sets the data at the raw specified position.
			 * @param position the raw position.
			 * @param value the data to set.
			 */
			void setRawData( unsigned int position, int value );
			
			/**
			 * Overwrite the existing sound's data with those specified.
			 * @param data
			 */
			void setRawData( const vector<int>& data );
			
			/**
			 * Returns the channel's data from the specified position.
			 * @param position the position.
			 * @param channel the channel to analyse.
			 * @return the data.
			 */
			int getChannelData( unsigned int position, unsigned short int channel ) const;
			
			/**
			 * Returns the linearly interpolated data of specified channel.
			 * @param position the theoriacal position.
			 * @param channel the channel to analyse.
			 * @return the interpolated data.
			 */
			int getChannelData( double position, unsigned short int channel ) const;

			/*
			 * Sets the channel's data at specified position.
			 * @param position the position.
			 * @param channel the channel to update.
			 * @param value the data.
			 */
			void setChannelData( unsigned int position, unsigned int short channel, int value );
			
			/*
			 * Fill the sound with the specified value.
			 * If the specified length is bigger than the data, the sound is growed.
			 * @param value	the value to set.
			 * @param length the length.
			 */
			void fill( int value, unsigned int length );
			
			/**
			 * Mix the specified sound into this sound's data.
			 * @param sound the specified sound to mix.
			 * @param startMs the time relative to this sound to start mix.
			 * @param volume the volume at which mix the specified sound.
			 * @param fromMs the time relative to the specified sound to start mix.
			 * @param toMs the time relative to the specified sound to end mix.
			 * @param pitch the pitch at which mix the specified sound.
			 */
			void mix( const Sound * sound, unsigned int startMs = 0, float volume = 1.0f, unsigned int fromMs = 0, unsigned int toMs = 0, double pitch = 1.0 );
			
			/**
			 * Mix the specified sound into this sound's data.
			 * @param sound the specified sound to mix.
			 * @param startPosition the position relative to this sound to start mix.
			 * @param volume the volume at which mix the specified sound.
			 * @param fromPosition the position relative to the specified sound to start mix.
			 * @parma toPosition the position relative to the specified sound to end mix.
			 * @param pitch the pitch at which mix the specified sound.
			 */
			void rawMix( const Sound * sound, unsigned int startPosition = 0, float volume = 1.0f, unsigned int fromPosition = 0, unsigned int toPosition = 0, double pitch = 1.0 );
	};
}

#endif
