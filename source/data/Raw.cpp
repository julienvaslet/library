#include <data/Raw.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace data
{
	Raw::Raw( const string& filename )
	{
		#ifdef DEBUG0
		Logger::get() << "[Raw#" << this << "] Loading file \"" << filename << "\"..." << Logger::endl;
		#endif
		
		this->file.open( filename.c_str() );
		
		#ifdef DEBUG0
		#endif
	}
	
	Raw::~Raw()
	{
		if( this->isOpened() )
		{
			this->file.close();
			
			#ifdef DEBUG0
			Logger::get() << "[Raw#" << this << "] Closed file." << Logger::endl;
			#endif
		}
	}
	
	bool Raw::isOpened() const
	{
		return this->file.is_open();
	}
	
	bool Raw::eof() const
	{
		return this->file.eof();
	}
	
	unsigned int Raw::length()
	{
		unsigned int length = 0;
		
		if( this->isOpened() )
		{
			int position = this->file.tellg();
			this->file.seekg( 0, ios::end );
			length = this->file.tellg();
			this->file.seekg( position );
		}
		
		return length;
	}
	
	void Raw::eat( unsigned int length )
	{
		for( unsigned int i = 0 ; i < length ; i++ )
		{
			if( this->file.good() )
				file.get();
			else
				break;
		}
	}
	
	string Raw::readString( unsigned int length )
	{
		// TODO
		return string("");
	}
	
	bool Raw::checkString( const string& str )
	{
		bool found = true;
		int position = this->file.tellg();
		
		for( unsigned int i = 0 ; i < str.length() ; i++ )
		{
			if( !this->file.good() || str.at( i ) != this->file.get() )
			{
				found = false;
				break;
			}
		}
		
		if( !found )
			this->file.seekg( position );
		
		return found;
	}
	
	bool Raw::checkString( const char * str )
	{
		return this->checkString( string(str) );
	}
	
	unsigned int Raw::readUnsignedInteger32()
	{
		unsigned int value = 0;
		this->file.readsome( (char *) &value, 4 );
		
		return value;
	}
	
	int Raw::readInteger32()
	{
		int value = 0;
		this->file.readsome( (char *) &value, 4 );
		
		return value;
	}
	
	unsigned int Raw::readUnsignedInteger24()
	{
		unsigned int value = 0;
		this->file.readsome( ((char *) &value) + 1, 3 );
		
		return value;
	}
	
	int Raw::readInteger24()
	{
		int value = 0;
		this->file.readsome( ((char *) &value) + 1, 3 );
		
		return value;
	}
	
	unsigned short int Raw::readUnsignedInteger16()
	{
		short int value = 0;
		this->file.readsome( (char *) &value, 2 );
		
		return value;
	}
	
	short int Raw::readInteger16()
	{
		short int value = 0;
		this->file.readsome( (char *) &value, 2 );
		
		return value;
	}
	
	unsigned char Raw::readUnsignedInteger8()
	{
		unsigned char value = 0;
		this->file.readsome( (char *) &value, 1 );
		
		return value;
	}
	
	char Raw::readInteger8()
	{
		char value = 0;
		this->file.readsome( (char *) &value, 1 );
		
		return value;
	}
}

