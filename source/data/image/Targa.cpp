#include <data/image/Targa.h>
#include <bitset>

using namespace std;

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace data
{
	namespace image
	{
		Targa::Targa()
		{
		}
		
		Targa::~Targa()
		{
		}

		bool Targa::isTopLeftOrigin() const
		{
			bitset<8> idb( this->imageDescriptorByte );
			return idb.test( 5 );
		}
		
		bool Targa::isBottomLeftOrigin() const
		{
			bitset<8> idb( this->imageDescriptorByte );
			return !idb.test( 5 );
		}
		
		Image * Targa::load( const string& filename )
		{
			Image * image = NULL;
			Raw * file = new Raw( filename );
			
			if( file->isOpened() )
			{
				image = new Image();
				Targa * tga = new Targa();
				
				// Reading Targa headers
				tga->identificationFieldLength = file->readUnsignedInteger8();
				tga->colorMapType = file->readUnsignedInteger8();
				tga->imageTypeCode = file->readUnsignedInteger8();
				tga->colorMapOrigin = file->readUnsignedInteger16();
				tga->colorMapLength = file->readUnsignedInteger16();
				tga->colorMapEntrySize = file->readUnsignedInteger8();
				tga->xOrigin = file->readUnsignedInteger16();
				tga->yOrigin = file->readUnsignedInteger16();
				tga->imageWidth = file->readUnsignedInteger16();
				tga->imageHeight = file->readUnsignedInteger16();
				tga->imagePixelSize = file->readUnsignedInteger8();
				tga->imageDescriptorByte = file->readUnsignedInteger8();
				
				/* Targa Header Debugging */
				/*
				#ifdef DEBUG0
				Logger::get() << "Identification Field Length: \t" << static_cast<unsigned int>( tga->identificationFieldLength ) << Logger::endl;
				Logger::get() << "Color Map Type: \t\t" << static_cast<unsigned int>( tga->colorMapType ) << Logger::endl;
				Logger::get() << "Image Type Code: \t\t" << static_cast<unsigned int>( tga->imageTypeCode ) << Logger::endl;
				Logger::get() << "Color Map Origin: \t\t" << static_cast<unsigned int>( tga->colorMapOrigin ) << Logger::endl;
				Logger::get() << "Color Map Length: \t\t" << static_cast<unsigned int>( tga->colorMapLength ) << Logger::endl;
				Logger::get() << "Color Map Entry Size: \t\t" << static_cast<unsigned int>( tga->colorMapEntrySize ) << Logger::endl;
				Logger::get() << "X Origin of Image: \t\t" << static_cast<unsigned int>( tga->xOrigin ) << Logger::endl;
				Logger::get() << "Y Origin of Image: \t\t" << static_cast<unsigned int>( tga->yOrigin ) << Logger::endl;
				Logger::get() << "Image Width: \t\t\t" << static_cast<unsigned int>( tga->imageWidth ) << Logger::endl;
				Logger::get() << "Image Height: \t\t\t" << static_cast<unsigned int>( tga->imageHeight ) << Logger::endl;
				Logger::get() << "Image Pixel Size: \t\t" << static_cast<unsigned int>( tga->imagePixelSize ) << Logger::endl;
				Logger::get() << "Image Descriptor Byte: \t\t" << static_cast<unsigned int>( tga->imageDescriptorByte ) << Logger::endl;
				#endif*/
				
				// Unmapped RGB image
				if( tga->imageTypeCode == 2 )
				{
					// Skipping Image Identification Field
					file->eat( tga->identificationFieldLength );
					
					// Skipping the color map
					if( tga->colorMapType != 0 )
					{
						for( unsigned int i = 0 ; i < static_cast<unsigned int>( tga->colorMapLength ) ; i++ )
						{
							switch( tga->colorMapEntrySize )
							{
								case 16:
								{
									file->readUnsignedInteger16();
									break;
								}
							
								case 24:
								{
									file->readUnsignedInteger24();
									break;
								}
							
								case 32:
								{
									file->readUnsignedInteger32();
									break;
								}
							}
						}
					}
					
					// Image data
					if( tga->imagePixelSize == 24 )
					{
						vector<unsigned char> pixels( tga->imageWidth * tga->imageHeight * 3, '\0' );
						unsigned int i = 0;
						
						if( tga->isBottomLeftOrigin() )
						{
							for( unsigned int y = 0 ; y < static_cast<unsigned int>( tga->imageHeight ) ; y++ )
							{
								for( unsigned int x = 0 ; x < static_cast<unsigned int>( tga->imageWidth ) ; x++, i += 3 )
								{
									for( unsigned int c = 3 ; c > 0 ; c-- )
										pixels[i + c - 1] = file->readUnsignedInteger8();
								}
							}
						}
						else
						{
							for( unsigned int y = static_cast<unsigned int>( tga->imageHeight ) ; y > 0 ; y-- )
							{
								i = (y - 1) * static_cast<unsigned int>( tga->imageWidth ) * 3;
								
								for( unsigned int x = 0 ; x < static_cast<unsigned int>( tga->imageWidth ) ; x++, i += 3 )
								{
									for( unsigned int c = 3 ; c > 0 ; c-- )
										pixels[i + c - 1] = file->readUnsignedInteger8();
								}
							}
						}
						
						image->setData( static_cast<unsigned int>( tga->imageWidth ), static_cast<unsigned int>( tga->imageHeight ), pixels, false, true );
					}
					else
					{
						delete image;
						image = NULL;
						
						#ifdef DEBUG0
						Logger::get() << "[Targa][" << filename << "] " << static_cast<unsigned int>( tga->imagePixelSize / 4 ) << " bytes per pixel loading is not implemented." << Logger::endl;
						#endif
					}
				}
				else
				{
					delete image;
					image = NULL;
					
					#ifdef DEBUG0
					Logger::get() << "[Targa][" << filename << "] Image type code (" << static_cast<unsigned int>( tga->imageTypeCode ) << ") loading is not implemented." << Logger::endl;
					#endif
				}
				
				delete tga;
			}
			
			delete file;
			
			return image;
		}
		
		void Targa::write( const Image& image, const string& filename )
		{
		}
	}
}

