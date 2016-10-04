#include <audio/Sound.h>

#include <climits>
#include <cmath>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace audio
{
	Sound::Sound( unsigned int frequency, unsigned short int channels ) : frequency(frequency), channels(channels)
	{
	}
	
	Sound::~Sound()
	{
	}
	
	unsigned int Sound::getDuration() const
	{
		return static_cast<unsigned int>( ( static_cast<double>( this->getRawLength() ) * 1000.0 / static_cast<double>( this->frequency ) ) / static_cast<double>( this->channels ) );
	}
	
	unsigned int Sound::getRawLength() const
	{
		return this->data.size();
	}
	
	unsigned int Sound::getChannelLength() const
	{
		return static_cast<unsigned int>( this->data.size() / static_cast<double>( this->channels ) );
	}
	
	unsigned int Sound::getFrequency() const
	{
		return this->frequency;
	}
	
	unsigned short int Sound::getChannels() const
	{
		return this->channels;
	}
	
	const vector<int>& Sound::getRawData() const
	{
		return this->data;
	}
	
	int Sound::getRawData( unsigned int position ) const
	{
		int value = 0;
		
		if( position < this->getRawLength() )
			value = this->data[position];
		
		return value;
	}
	
	Sound * Sound::getRawData( unsigned int startPosition, unsigned int length )
	{
		if( startPosition > this->getRawLength() )
			startPosition = this->getRawLength();
		
		if( startPosition + length > this->getRawLength() )
			length = this->getRawLength() - startPosition;
		
		Sound * sound = new Sound( this->frequency, this->channels );
		vector<int> data;
		
		for( unsigned int i = startPosition ; i < startPosition + length ; i++ )
			data.push_back( this->data[i] );
			
		sound->setRawData( data );
		
		return sound;
	}
	
	void Sound::setRawData( unsigned int position, int value )
	{
		if( position < this->getRawLength() )
			this->data[position] = value;
	}
	
	void Sound::setRawData( const vector<int>& data )
	{
		this->data.clear();
		
		for( vector<int>::const_iterator it = data.begin() ; it != data.end() ; it++ )
			this->data.push_back( *it );
	}
	
	int Sound::getChannelData( unsigned int position, unsigned short int channel ) const
	{
		return getRawData( (position * this->channels) + channel );
	}
	
	int Sound::getChannelData( double position, unsigned short int channel ) const
	{
		int value = 0;
		
		if( position < this->getChannelLength() )
		{
			// Linear interpolation
			unsigned int iNext = ceil( position );
			unsigned int iPrev = floor( position );
			
			value = static_cast<int>( this->getChannelData( iPrev, channel ) + (this->getChannelData( iNext, channel ) - this->getChannelData( iPrev, channel )) * (position - iNext) );
		}
		
		return value;
	}
	
	void Sound::setChannelData( unsigned int position, unsigned short int channel, int value )
	{
		this->setRawData( (position * this->channels) + channel, value );
	}
	
	void Sound::fill( int value, unsigned int length )
	{
		if( this->data.size() < length )
			this->data.resize( length );
	
		for( unsigned int i = 0 ; i < length ; i++ )
			this->data[i] = value;
	}
	
	void Sound::rawMix( const Sound * sound, unsigned int startPosition, float volume, unsigned int fromPosition, unsigned int toPosition, double pitch )
	{
		// TODO: Convert input sound format
		
		if( toPosition == 0 )
			toPosition = sound->getRawLength();
		
		// Prevents from negative and null values
		if( pitch <= 0.0 )
			pitch = 1.0;
		
		unsigned int channelLength = static_cast<unsigned int>( round( (toPosition - fromPosition) / static_cast<double>( this->channels ) ) / pitch );
		unsigned int mixedLength = channelLength * this->channels;
		
		// Reserve sound data vector if it will be too small
		if( startPosition + mixedLength > this->getRawLength() )
			this->data.resize( startPosition + mixedLength );
		
		// Mixing values
		for( unsigned int i = 0 ; i < channelLength ; i++ )
		{
			for( unsigned short int c = 0 ; c < this->channels ; c++ )
				this->setChannelData( startPosition + i, c, static_cast<unsigned int>( this->getChannelData( startPosition + i, c ) / 2.0 + sound->getChannelData( static_cast<double>( fromPosition + (i * pitch) ), c ) / 2.0 * volume ) );
		}
	}
	
	void Sound::mix( const Sound * sound, unsigned int startMs, float volume, unsigned int fromMs, unsigned int toMs, double pitch )
	{
		unsigned int start = static_cast<unsigned int>( static_cast<double>( this->frequency * this->channels * startMs ) / 1000.0 );
		unsigned int from = static_cast<unsigned int>( static_cast<double>( this->frequency * this->channels * fromMs ) / 1000.0 );
		unsigned int to = (toMs == 0) ? sound->getRawLength() : static_cast<unsigned int>( static_cast<double>( this->frequency * this->channels * toMs ) / 1000.0 );

		this->rawMix( sound, start, volume, from, to, pitch );
	}
}

