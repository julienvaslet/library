#ifndef __DATA_IMAGE_JPEG_H
#define __DATA_IMAGE_JPEG_H	1

#include <data/image/Image.h>
#include <data/Raw.h>
#include <jpeglib.h>
#include <setjmp.h>

namespace data
{
	namespace image
	{
		struct JpegErrorManager
		{
			struct jpeg_error_mgr pub;
			jmp_buf setjmp_buffer;
		};
		
		typedef JpegErrorManager * JpegErrorPointer;
		
		class Jpeg
		{
			protected:
				static void exitOnError( j_common_ptr cinfo );
			
			public:
				Jpeg();
				virtual ~Jpeg();
				
				static Image * load( const string& filename );
				static void write( const Image& image, const string& filename );
		};
	}
}

#endif
