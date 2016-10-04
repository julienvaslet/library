#include <data/image/Image.h>
#include <algorithm>

#include <data/image/Targa.h>
#include <data/image/Jpeg.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

using namespace std;

namespace data
{
	namespace image
	{
		Image::Image() : width(0), height(0), bytesPerPixel(0)
		{
		}
	
		Image::~Image()
		{
		}
	
		unsigned int Image::getWidth() const
		{
			return this->width;
		}
	
		unsigned int Image::getHeight() const
		{
			return this->height;
		}
	
		unsigned int Image::getBytesPerPixel() const
		{
			return this->bytesPerPixel;
		}
	
		const unsigned char * Image::getData() const
		{
			return &(this->data[0]);
		}

		void Image::setData( unsigned int width, unsigned int height, const vector<unsigned char>& data, bool hasAlpha, bool setAlpha )
		{
			this->setData( width, height, &(data[0]), hasAlpha, setAlpha );
		}
	
		void Image::setData( unsigned int width, unsigned int height, const unsigned char * data, bool hasAlpha, bool setAlpha )
		{
			this->width = width;
			this->height = height;
			this->bytesPerPixel = hasAlpha ? 4 : ( setAlpha ? 4 : 3 );
			
			this->data.clear();
			this->data.resize( this->width * this->height * this->bytesPerPixel );
		
			for( unsigned int i = 0, j = 0 ; i < this->data.size() ; i++, j++ )
			{
				this->data[i] = data[j];
				
				if( !hasAlpha && setAlpha && j % 3 == 2 )
					this->data[++i] = 255;
			}
		}
		
		Image * Image::load( const string& filename )
		{
			Image * image = NULL;
			string extension = filename.substr( filename.find_last_of( "." ) + 1 );
			transform( extension.begin(), extension.end(), extension.begin(), ::tolower );
			
			if( extension == "tga" )
				image = Targa::load( filename );
				
			else if( extension == "jpg" || extension == "jpeg" )
				image = Jpeg::load( filename );

			#ifdef DEBUG0
			else
			{
				Logger::get() << "[Image][" << filename << "] Loading \"" << extension << "\" is not implemented." << Logger::endl;
			}
			#endif
			
			return image;
		}
	}
}

