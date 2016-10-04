#ifndef __DATA_IMAGE_TARGA_H
#define __DATA_IMAGE_TARGA_H	1

#include <data/image/Image.h>
#include <data/Raw.h>

namespace data
{
	namespace image
	{
		class Targa
		{
			protected:
				unsigned char identificationFieldLength;
				unsigned char colorMapType;
				unsigned char imageTypeCode;
				unsigned short int colorMapOrigin;
				unsigned short int colorMapLength;
				unsigned char colorMapEntrySize;
				unsigned short int xOrigin;
				unsigned short int yOrigin;
				unsigned short int imageWidth;
				unsigned short int imageHeight;
				unsigned char imagePixelSize;
				unsigned char imageDescriptorByte;
			
			public:
				Targa();
				virtual ~Targa();
				
				bool isTopLeftOrigin() const;
				bool isBottomLeftOrigin() const;
				
				static Image * load( const string& filename );
				static void write( const Image& image, const string& filename );
		};
	}
}

#endif

