#include <opengl/TexturedRectangle.h>

namespace opengl
{
	unsigned int TexturedRectangle::instances = 0;
	Program * TexturedRectangle::program = NULL;
	ArrayBufferObject * TexturedRectangle::vertices = NULL;
	ArrayBufferObject * TexturedRectangle::textureCoordinates = NULL;
	ElementArrayBufferObject * TexturedRectangle::indices = NULL;
	
	TexturedRectangle::TexturedRectangle() : Rectangle(), useGenericProgram(true), tile(NULL)
	{
		if( this->useGenericProgram )
			this->initializeRendering();
		
		this->tile = new Tile();
	}
	
	TexturedRectangle::TexturedRectangle( bool useGenericProgram ) : Rectangle(), useGenericProgram(useGenericProgram), tile(NULL)
	{
		if( this->useGenericProgram )
			this->initializeRendering();
		
		this->tile = new Tile();
	}
	
	TexturedRectangle::TexturedRectangle( const TexturedRectangle& rectangle ) : Rectangle(rectangle), useGenericProgram(rectangle.useGenericProgram), tile(NULL)
	{
		if( this->useGenericProgram )
			this->initializeRendering();
			
		this->tile = new Tile( *(rectangle.tile) );
	}
	
	TexturedRectangle::TexturedRectangle( const Rectangle& rectangle, const string& textureResource, bool useGenericProgram ) : Rectangle(rectangle), useGenericProgram(useGenericProgram), tile(NULL)
	{
		if( this->useGenericProgram )
			this->initializeRendering();
			
		this->tile = new Tile( textureResource );
	}
	
	TexturedRectangle::TexturedRectangle( const Rectangle& rectangle, Texture2D * texture, bool useGenericProgram ) : Rectangle(rectangle), useGenericProgram(useGenericProgram), tile(NULL)
	{
		if( this->useGenericProgram )
			this->initializeRendering();
			
		this->tile = new Tile( texture );
	}
	
	TexturedRectangle::TexturedRectangle( unsigned width, unsigned height, bool useGenericProgram ) : Rectangle(width,height), useGenericProgram(useGenericProgram), tile(NULL)
	{
		if( this->useGenericProgram )
			this->initializeRendering();
			
		this->tile = new Tile();
	}
	
	TexturedRectangle::TexturedRectangle( unsigned width, unsigned height, const string& textureResource, bool useGenericProgram ) : Rectangle(width,height), useGenericProgram(useGenericProgram), tile(NULL)
	{
		if( this->useGenericProgram )
			this->initializeRendering();
			
		this->tile = new Tile( textureResource );
	}
	
	TexturedRectangle::TexturedRectangle( unsigned width, unsigned height, Texture2D * texture, bool useGenericProgram ) : Rectangle(width,height), useGenericProgram(useGenericProgram), tile(NULL)
	{
		if( this->useGenericProgram )
			this->initializeRendering();
			
		this->tile = new Tile( texture );
	}
	
	TexturedRectangle::~TexturedRectangle()
	{
		if( this->tile != NULL )
			delete this->tile;
			
		if( this->useGenericProgram )
		{
			TexturedRectangle::instances--;
		
			if( TexturedRectangle::instances == 0 )
			{
				if( TexturedRectangle::indices != NULL )
				{
					delete TexturedRectangle::indices;
					TexturedRectangle::indices = NULL;
				}
			
				if( TexturedRectangle::textureCoordinates != NULL )
				{
					delete TexturedRectangle::textureCoordinates;
					TexturedRectangle::textureCoordinates = NULL;
				}
			
				if( TexturedRectangle::vertices != NULL )
				{
					delete TexturedRectangle::vertices;
					TexturedRectangle::vertices = NULL;
				}
			
				if( TexturedRectangle::program != NULL )
				{
					delete TexturedRectangle::program;
					TexturedRectangle::program = NULL;
				}
			}
		}
	}

	void TexturedRectangle::initializeRendering()
	{
		TexturedRectangle::instances++;
		
		if( TexturedRectangle::program == NULL )
		{
			TexturedRectangle::program = new Program();
			
			#ifdef __PI__
			TexturedRectangle::program->loadVertexShaderFile( "data/shaders/SimpleTexture.es.vs" );
			TexturedRectangle::program->loadFragmentShaderFile( "data/shaders/SimpleTexture.es.fs" );
			#else
			TexturedRectangle::program->loadVertexShaderFile( "data/shaders/SimpleTexture.vs" );
			TexturedRectangle::program->loadFragmentShaderFile( "data/shaders/SimpleTexture.fs" );
			#endif
			
			TexturedRectangle::program->link( true );
		}
		
		if( TexturedRectangle::vertices == NULL )
			TexturedRectangle::vertices = new ArrayBufferObject();
			
		if( TexturedRectangle::textureCoordinates == NULL )
			TexturedRectangle::textureCoordinates = new ArrayBufferObject();
			
		if( TexturedRectangle::indices == NULL )
			TexturedRectangle::indices = new ElementArrayBufferObject();
	}
	
	Tile * TexturedRectangle::getTile()
	{
		return this->tile;
	}
	
	void TexturedRectangle::prepareRendering( vector<Point3D>& vertices, vector<Point2D>& textureCoordinates, vector<unsigned short int>& indices ) const
	{
		Rectangle::prepareRendering( vertices, indices, true );
		
		// Texture Coordinates
		if( this->tile != NULL )
			this->tile->prepareRendering( textureCoordinates );
			
		else
		{
			textureCoordinates.push_back( Point2D( 0.0f, 0.0f ) );
			textureCoordinates.push_back( Point2D( 0.0f, 0.0f ) );
			textureCoordinates.push_back( Point2D( 0.0f, 0.0f ) );
			textureCoordinates.push_back( Point2D( 0.0f, 0.0f ) );
		}
	}
	
	void TexturedRectangle::render( vector<Point3D>& vertices, vector<Point2D>& textureCoordinates, vector<unsigned short int>& indices, Texture2D * texture, unsigned int textureUnit )
	{
		if( TexturedRectangle::program != NULL )
		{
			TexturedRectangle::program->use( true );
			
			TexturedRectangle::vertices->setData( vertices );
			TexturedRectangle::textureCoordinates->setData( textureCoordinates );
			TexturedRectangle::indices->setData( indices );
			
			TexturedRectangle::program->sendUniform( "projection_matrix", Matrix::projection, false );
			TexturedRectangle::program->sendUniform( "modelview_matrix", Matrix::modelview, false );
			TexturedRectangle::program->sendUniform( "texture0", *texture, textureUnit );
			TexturedRectangle::program->sendVertexPointer( "a_Vertex", TexturedRectangle::vertices );
			TexturedRectangle::program->sendTextureCoordinatesPointer( "a_TexCoord0", TexturedRectangle::textureCoordinates );

			TexturedRectangle::indices->draw( OpenGL::Triangles );
		}
	}
}

