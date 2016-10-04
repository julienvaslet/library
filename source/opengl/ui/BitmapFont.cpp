#include <opengl/ui/BitmapFont.h>
#include <opengl/ui/UserInterface.h>
#include <game/Resource.h>
#include <sstream>

using namespace game;
using namespace data::image;
using namespace std;

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	namespace ui
	{
		Program * BitmapFont::program = NULL;
		unsigned int BitmapFont::instances = 0;
	
		BitmapFont::BitmapFont( const string& filename, unsigned int characterWidth, unsigned int characterHeight, unsigned int marginWidth, unsigned marginHeight ) : Font(BitmapFont::getFontNameFromPath(filename)), vertices(NULL), textureCoordinates(NULL), colors(NULL), indices(NULL), rectangle(NULL), charactersByLine(0), characterWidth(characterWidth), characterHeight(characterHeight), marginWidth(marginWidth), marginHeight(marginHeight)
		{
			BitmapFont::instances++;
		
			if( BitmapFont::program == NULL )
			{
				BitmapFont::program = new Program();
				#ifdef __PI__
				BitmapFont::program->loadVertexShaderFile( "data/shaders/BitmapFont.es.vs" );
				BitmapFont::program->loadFragmentShaderFile( "data/shaders/BitmapFont.es.fs" );
				#else
				BitmapFont::program->loadVertexShaderFile( "data/shaders/BitmapFont.vs" );
				BitmapFont::program->loadFragmentShaderFile( "data/shaders/BitmapFont.fs" );
				#endif
				BitmapFont::program->link( true );
			}
		
			this->vertices = new ArrayBufferObject();
			this->textureCoordinates = new ArrayBufferObject();
			this->colors = new ArrayBufferObject();
			this->indices = new ElementArrayBufferObject();
		
			stringstream sFontResourceName;
			sFontResourceName << "font." << BitmapFont::getFontNameFromPath( filename );
			string fontResourceName = sFontResourceName.str();
		
			Resource::loadTexture2D( fontResourceName, filename );
			this->rectangle = new TexturedRectangle( this->characterWidth, this->characterHeight, fontResourceName, false );
	
			if( this->rectangle->getTile()->getTexture() != NULL )
			{	
				this->charactersByLine = this->rectangle->getTile()->getTexture()->getWidth() / this->characterWidth;
				this->rectangle->getTile()->getTexture()->setFiltering( GL_NEAREST, GL_NEAREST );
			}
			#ifdef DEBUG1
			else
			{
				Logger::get() << "[BitmapFont#" << BitmapFont::getFontNameFromPath( filename ) << "] Unable to load texture file \"" << filename << "\"." << Logger::endl;
			}
			#endif
		}
	
		BitmapFont::~BitmapFont()
		{
			if( this->vertices != NULL )
				delete this->vertices;
			
			if( this->textureCoordinates != NULL )
				delete this->textureCoordinates;
			
			if( this->colors != NULL )
				delete this->colors;
			
			if( this->indices != NULL )
				delete this->indices;
			
			if( this->rectangle != NULL )
				delete this->rectangle;
		
			BitmapFont::instances--;
		
			if( BitmapFont::instances == 0 )
			{
				delete BitmapFont::program;
				BitmapFont::program = NULL;
			}
		}
	
		void BitmapFont::render()
		{
			this->vertices->setData( this->vVertices );
			this->textureCoordinates->setData( this->vTextureCoordinates );
			this->colors->setData( this->vColors );
			this->indices->setData( this->vIndices );

			BitmapFont::program->use( true );

			BitmapFont::program->sendUniform( "projection_matrix", UserInterface::get()->getProjectionMatrix(), false );
			BitmapFont::program->sendUniform( "modelview_matrix", UserInterface::get()->getModelviewMatrix(), false );
			BitmapFont::program->sendUniform( "texture0", *(this->rectangle->getTile()->getTexture()), 0 );

			BitmapFont::program->sendVertexPointer( "a_Vertex", this->vertices );
			BitmapFont::program->sendColorPointer( "a_Color", this->colors );
			BitmapFont::program->sendTextureCoordinatesPointer( "a_TexCoord0", this->textureCoordinates );

			this->indices->draw( OpenGL::Triangles );
		
			this->vVertices.clear();
			this->vTextureCoordinates.clear();
			this->vColors.clear();
			this->vIndices.clear();
		}
		
		void BitmapFont::write( const Point3D& origin, const string& text, const Color& color, float size )
		{
			if( size == 0.0f ) size = 1.0f;
		
			if( this->rectangle->getTile()->getTexture() != NULL )
			{
				this->rectangle->resize( (this->characterWidth - (2.0f * this->marginWidth)) * size, (this->characterHeight - (2.0f * this->marginHeight)) * size );
				this->rectangle->getOrigin().moveTo( origin.getX(), origin.getY(), origin.getZ() );
		
				if( text.length() > 0 )
				{
					for( unsigned int i = 0 ; i < text.length() ; i++ )
					{
						if( text[i] == '\n' )
						{
							this->rectangle->getOrigin().moveTo( origin.getX(), this->rectangle->getOrigin().getY() + this->rectangle->getHeight(), 0.0f );
							continue;
						}
				
						float x = static_cast<float>( text[i] % this->charactersByLine );
						float y = static_cast<float>( text[i] / this->charactersByLine );
					
						for( unsigned int j = 0 ; j < 4 ; j++ )
							this->vColors.push_back( color );
					
						this->rectangle->getTile()->setView( (x * this->characterWidth) + this->marginWidth, (y * this->characterHeight) + this->marginHeight, this->characterWidth - (2.0f * this->marginWidth), this->characterHeight - (2.0f * this->marginHeight) );
						this->rectangle->prepareRendering( this->vVertices, this->vTextureCoordinates, this->vIndices );
						this->rectangle->getOrigin().moveBy( this->rectangle->getWidth(), 0.0f, 0.0f );
					}
				}
			}
		}
	
		void BitmapFont::getTextSize( Point2D& origin, const string& text, float size ) const
		{
			float maxWidth = 0.0f;
		
			if( size == 0.0f )
				size = 1.0f;
			
			if( text.length() > 0 )
				origin.moveBy( 0, (this->characterHeight - (2.0f * this->marginHeight)) * size );
		
			for( unsigned int i = 0 ; i < text.length() ; i++ )
			{
				if( text[i] == '\n' )
				{
					if( origin.getX() > maxWidth )
						maxWidth = origin.getX();
				
					origin.moveBy( -1 * origin.getX(), (this->characterHeight - (2.0f * this->marginHeight)) * size );
				}
				else
					origin.moveBy( (this->characterWidth - (2.0f * this->marginWidth)) * size, 0 );
			}
		
			if( maxWidth > origin.getX() )
				origin.setX( maxWidth );
		}
	
		float BitmapFont::getTextWidth( const string& text, float size ) const
		{
			Point2D origin( 0, 0 );
			this->getTextSize( origin, text, size );
			return origin.getX();
		}
	
		float BitmapFont::getTextHeight( const string& text, float size ) const
		{
			Point2D origin( 0, 0 );
			this->getTextSize( origin, text, size );
			return origin.getY();
		}
	
		string BitmapFont::getFontNameFromPath( const string& filename )
		{
			unsigned int lastSlash = filename.find_last_of("/");
			if( lastSlash == string::npos ) lastSlash = 0;
		
			unsigned int lastDot = filename.find_last_of(".");
			if( lastDot == string::npos ) lastDot = filename.length();
		
			return filename.substr( lastSlash + 1, lastDot - lastSlash - 1);
		}
	}
}

