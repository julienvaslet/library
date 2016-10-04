#ifndef __DATA_RAW_H
#define	__DATA_RAW_H

#include <fstream>
#include <string>

using namespace std;

namespace data
{
	class Raw
	{
		protected:
				ifstream file;
				
		public:
			Raw( const string& filename );
			virtual ~Raw();
			
			bool isOpened() const;
			bool eof() const;
			
			unsigned int length();
			void eat( unsigned int length );
			
			string readString( unsigned int length );
			bool checkString( const string& str );
			bool checkString( const char * str );
			
			unsigned int readUnsignedInteger32();
			int readInteger32();
			//bool checkInteger32( int i );
			
			unsigned int readUnsignedInteger24();
			int readInteger24();
			//bool checkInteger24( int i );
			
			unsigned short int readUnsignedInteger16();
			short int readInteger16();
			//bool checkInteger16( short int i );
			
			unsigned char readUnsignedInteger8();
			char readInteger8();
			//bool checkInteger8( char i );
	};
}

#endif

