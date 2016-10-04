#include <opengl/ColoredRectangle.h>
#include <opengl/Matrix.h>

namespace opengl
{
	unsigned int ColoredRectangle::instances = 0;
	Program * ColoredRectangle::program = NULL;
	ArrayBufferObject * ColoredRectangle::vertices = NULL;
	ArrayBufferObject * ColoredRectangle::colors = NULL;
	ElementArrayBufferObject * ColoredRectangle::indices = NULL;

	ColoredRectangle::ColoredRectangle() : Rectangle(), color()
	{
		this->initializeRendering();
	}
	
	ColoredRectangle::ColoredRectangle( const ColoredRectangle& rectangle ) : Rectangle(rectangle), color(rectangle.color)
	{
		this->initializeRendering();
	}
	
	ColoredRectangle::ColoredRectangle( const Rectangle& rectangle ) : Rectangle(rectangle), color()
	{
		this->initializeRendering();
	}
	
	ColoredRectangle::ColoredRectangle( const Rectangle& rectangle, const Color& color ) : Rectangle(rectangle), color(color)
	{
		this->initializeRendering();
	}
	
	ColoredRectangle::ColoredRectangle( unsigned width, unsigned height ) : Rectangle(width, height), color()
	{
		this->initializeRendering();
	}
	
	ColoredRectangle::ColoredRectangle( unsigned width, unsigned height, const Color& color ) : Rectangle(width, height), color(color)
	{
		this->initializeRendering();
	}
	
	ColoredRectangle::~ColoredRectangle()
	{
		ColoredRectangle::instances--;
		
		if( ColoredRectangle::instances == 0 )
		{
			if( ColoredRectangle::indices != NULL )
			{
				delete ColoredRectangle::indices;
				ColoredRectangle::indices = NULL;
			}
			
			if( ColoredRectangle::colors != NULL )
			{
				delete ColoredRectangle::colors;
				ColoredRectangle::colors = NULL;
			}
			
			if( ColoredRectangle::vertices != NULL )
			{
				delete ColoredRectangle::vertices;
				ColoredRectangle::vertices = NULL;
			}
			
			if( ColoredRectangle::program != NULL )
			{
				delete ColoredRectangle::program;
				ColoredRectangle::program = NULL;
			}
		}
	}
	
	void ColoredRectangle::initializeRendering()
	{
		ColoredRectangle::instances++;
		
		if( ColoredRectangle::program == NULL )
		{
			ColoredRectangle::program = new Program();
			
			#ifdef __PI__
			ColoredRectangle::program->loadVertexShaderFile( "data/shaders/SimpleColor.es.vs" );
			ColoredRectangle::program->loadFragmentShaderFile( "data/shaders/SimpleColor.es.fs" );
			#else
			ColoredRectangle::program->loadVertexShaderFile( "data/shaders/SimpleColor.vs" );
			ColoredRectangle::program->loadFragmentShaderFile( "data/shaders/SimpleColor.fs" );
			#endif
			
			ColoredRectangle::program->link( true );
		}
		
		if( ColoredRectangle::vertices == NULL )
			ColoredRectangle::vertices = new ArrayBufferObject();
			
		if( ColoredRectangle::colors == NULL )
			ColoredRectangle::colors = new ArrayBufferObject();
			
		if( ColoredRectangle::indices == NULL )
			ColoredRectangle::indices = new ElementArrayBufferObject();
	}
	
	Color& ColoredRectangle::getColor()
	{
		return this->color;
	}
	
	void ColoredRectangle::setColor( const Color& color )
	{
		this->color.setColor( color );
	}
	
	void ColoredRectangle::prepareRendering( vector<Point3D>& vertices, vector<Color>& colors, vector<unsigned short int>& indices ) const
	{
		Rectangle::prepareRendering( vertices, indices, true );
		
		// Colors
		colors.push_back( this->color );
		colors.push_back( this->color );
		colors.push_back( this->color );
		colors.push_back( this->color );
	}
	
	void ColoredRectangle::render( vector<Point3D>& vertices, vector<Color>& colors, vector<unsigned short int>& indices )
	{
		if( ColoredRectangle::program != NULL )
		{
			ColoredRectangle::program->use( true );
			
			ColoredRectangle::vertices->setData( vertices );
			ColoredRectangle::colors->setData( colors );
			ColoredRectangle::indices->setData( indices );
			
			ColoredRectangle::program->sendUniform( "projection_matrix", Matrix::projection, false );
			ColoredRectangle::program->sendUniform( "modelview_matrix", Matrix::modelview, false );
			ColoredRectangle::program->sendVertexPointer( "a_Vertex", ColoredRectangle::vertices );
			ColoredRectangle::program->sendColorPointer( "a_Color", ColoredRectangle::colors );

			ColoredRectangle::indices->draw( OpenGL::Triangles );
		}
	}
}

