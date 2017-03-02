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
		
		void Jpeg::write( const Image& image, const string& filename, int quality )
		{
			FILE * fd = fopen( filename.c_str(), "wb" );
			
			if( fd != NULL )
			{
				struct jpeg_compress_struct cinfo;
				struct jpeg_error_mgr jerr;
				
				cinfo.err = jpeg_std_error( &jerr );
				jpeg_create_compress( &cinfo );
				jpeg_stdio_dest( &cinfo, fd );
				
				cinfo.image_width = image.getWidth();
				cinfo.image_height = image.getHeight();
				cinfo.input_components = 3;
				cinfo.in_color_space = JCS_RGB;
				
				jpeg_set_defaults( &cinfo );
				jpeg_set_quality( &cinfo, quality, true );
				jpeg_start_compress( &cinfo, true );
				
				JSAMPROW row_pointer;
				const unsigned char * buffer = image.getData();
				unsigned int row_size = cinfo.image_width * 3;
				unsigned int row_size_alpha = cinfo.image_width * 4;
				vector<unsigned char> row( row_size );
				
				while( cinfo.next_scanline < cinfo.image_height )
				{
					for( unsigned int i = 0, j = cinfo.next_scanline * row_size_alpha ; i < row_size ; i++ )
					{
						row[i] = buffer[j++];
						
						// Skip alpha.
						if( i % 3 == 2 ) j++;
					}
						
					row_pointer = (JSAMPROW) &(row[0]);
					jpeg_write_scanlines( &cinfo, &row_pointer, 1 );
				}
				
				jpeg_finish_compress( &cinfo );
				fclose( fd );
				
				jpeg_destroy_compress( &cinfo );
			}
			
			#ifdef DEBUG0
			else
				Logger::get() << "[Jpeg][" << filename << "] Can not open file for writing." << Logger::endl;
			#endif
		}
	}
}

