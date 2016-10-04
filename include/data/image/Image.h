#ifndef __DATA_IMAGE_IMAGE_H
#define __DATA_IMAGE_IMAGE_H	1

#include <vector>
#include <string>
using namespace std;

namespace data
{
	namespace image
	{
		class Image
		{
			protected:
				unsigned int width;
				unsigned int height;
				unsigned int bytesPerPixel;
			
				vector<unsigned char> data;
			
			public:
				Image();
				virtual ~Image();
			
				unsigned int getWidth() const;
				unsigned int getHeight() const;
				unsigned int getBytesPerPixel() const;
				const unsigned char * getData() const;
			
				void setData( unsigned int width, unsigned int height, const vector<unsigned char>& data, bool hasAlpha = false, bool setAlpha = true );
				void setData( unsigned int width, unsigned int height, const unsigned char * data, bool hasAlpha = false, bool setAlpha = true );
				
				static Image * load( const string& filename );
		};
	}
}

#endif

