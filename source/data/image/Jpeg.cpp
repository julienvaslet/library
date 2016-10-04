#include <data/image/Jpeg.h>
#include <cstdio>
#include <algorithm>

using namespace std;

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace data
{
	namespace image
	{
		Jpeg::Jpeg()
		{
		}
		
		Jpeg::~Jpeg()
		{
		}
		
		void Jpeg::exitOnError( j_common_ptr cinfo )
		{
			JpegErrorPointer err = (JpegErrorPointer) cinfo->err;
			(*cinfo->err->output_message)( cinfo );
			
			longjmp( err->setjmp_buffer, 1 );
		}
		
		Image * Jpeg::load( const string& filename )
		{
			Image * image = NULL;
			vector<unsigned char> pixels;
			vector<unsigned char> data;
			
			struct jpeg_decompress_struct cinfo;
			struct JpegErrorManager jerr;
			
			JSAMPARRAY buffer;
			int row_stride;
			
			FILE * fd = fopen( filename.c_str(), "rb" );
			
			if( fd != NULL )
			{
				cinfo.err = jpeg_std_error( &jerr.pub );
				jerr.pub.error_exit = Jpeg::exitOnError;
				
				if( setjmp( jerr.setjmp_buffer ) )
				{
					jpeg_destroy_decompress( &cinfo );
					
					#ifdef DEBUG0
					Logger::get() << "[Jpeg] An error has occured during libjpeg initialization." << Logger::endl;
					#endif	
				}
				else
				{
					image = new Image();
					
					jpeg_create_decompress( &cinfo );
					jpeg_stdio_src( &cinfo, fd );
					
					jpeg_read_header( &cinfo, true );
					jpeg_start_decompress( &cinfo );
					
					#ifdef DEBUG0
					Logger::get() << "[Jpeg][" << filename << "] Reading image (" << cinfo.output_width << "x" << cinfo.output_height << ")..." << Logger::endl;
					#endif
					
					pixels.reserve( cinfo.output_width * cinfo.output_height );
					
					row_stride = cinfo.output_width * cinfo.output_components;
					buffer = (*cinfo.mem->alloc_sarray)( (j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1 );
					
					while( cinfo.output_scanline < cinfo.output_height )
					{
						jpeg_read_scanlines( &cinfo, buffer, 1 );
						
						// Pixels are read reversed to reverse lines later.
						for( int i = row_stride - 1 ; i >= 0 ; i-- )
							pixels.push_back( buffer[0][i] );
					}
					
					jpeg_finish_decompress( &cinfo );
					
					std::reverse( pixels.begin(), pixels.end() );
					image->setData( (unsigned int) cinfo.output_width, (unsigned int) cinfo.output_height, pixels );
					
					jpeg_destroy_decompress( &cinfo );
					
					#ifdef DEBUG0
					Logger::get() << "[Jpeg][" << filename << "] Image read." << Logger::endl;
					#endif
				}
			
				fclose( fd );
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[Jpeg][" << filename << "] Can not open file." << Logger::endl;
			#endif
			
			return image;
		}
		
		void Jpeg::write( const Image& image, const string& filename )
		{
		}
	}
}

