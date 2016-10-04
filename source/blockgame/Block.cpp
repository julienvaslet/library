#include <blockgame/Block.h>
#include <opengl/Matrix.h>

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

using namespace opengl;

#define BLOCK_SIZE	25.0f

namespace blockgame
{
	unsigned int Block::instances = 0;
	Program * Block::program = NULL;
	Texture2D * Block::texture = NULL;
	ArrayBufferObject * Block::vertices = NULL;
	ArrayBufferObject * Block::textureCoordinates = NULL;
	ArrayBufferObject * Block::colors = NULL;
	ElementArrayBufferObject * Block::indices = NULL;
	
	Block::Block( const Point2D& position, const Color& color ) : position(position), rectangle(NULL), color(color)
	{
		Block::instances++;
		
		if( Block::program == NULL )
		{
			Block::program = new Program();
			#ifdef __PI__
			Block::program->loadVertexShaderFile( "data/blockgame/block.es.vs" );
			Block::program->loadFragmentShaderFile( "data/blockgame/block.es.fs" );
			#else
			Block::program->loadVertexShaderFile( "data/blockgame/block.vs" );
			Block::program->loadFragmentShaderFile( "data/blockgame/block.fs" );
			#endif
			Block::program->link( true );
		}
		
		if( Block::texture == NULL )
		{
			Block::texture = (Texture2D *) Resource::get("texture.block");
			
			if( Block::texture != NULL )
				Block::texture->use();
		}
		
		if( Block::vertices == NULL )
			Block::vertices = new ArrayBufferObject();
			
		if( Block::textureCoordinates == NULL )
			Block::textureCoordinates = new ArrayBufferObject();
			
		if( Block::colors == NULL )
			Block::colors = new ArrayBufferObject();
			
		if( Block::indices == NULL )
			Block::indices = new ElementArrayBufferObject();
			
		this->rectangle = new TexturedRectangle( BLOCK_SIZE, BLOCK_SIZE, "texture.block", false );
	}
	
	Block::~Block()
	{
		Block::instances--;
		
		if( this->rectangle != NULL )
			delete this->rectangle;
		
		if( Block::instances == 0 )
		{
			if( Block::program != NULL )
			{
				delete Block::program;
				Block::program = NULL;
			}
			
			if( Block::texture != NULL )
			{
				Block::texture->free();
				Block::texture = NULL;
			}
			
			if( Block::vertices != NULL )
			{
				delete Block::vertices;
				Block::vertices = NULL;
			}
			
			if( Block::textureCoordinates != NULL )
			{
				delete Block::textureCoordinates;
				Block::textureCoordinates = NULL;
			}
			
			if( Block::colors != NULL )
			{
				delete Block::colors;
				Block::colors = NULL;
			}
			
			if( Block::indices != NULL )
			{
				delete Block::indices;
				Block::indices = NULL;
			}
		}
	}
	
	Point2D& Block::getPosition()
	{
		return this->position;
	}
	
	void Block::prepareRendering( Point3D& origin, vector<Point3D>& vPoints, vector<Point2D>& vTexCoords, vector<Color>& vColors, vector<unsigned short int>& vIndices )
	{
		this->rectangle->getOrigin().moveTo( origin.getX() + this->position.getX() * BLOCK_SIZE, origin.getY() + this->position.getY() * BLOCK_SIZE, origin.getZ() );
		this->rectangle->prepareRendering( vPoints, vTexCoords, vIndices );
		
		// Colors
		vColors.push_back( this->color );
		vColors.push_back( this->color );
		vColors.push_back( this->color );
		vColors.push_back( this->color );
	}
	
	void Block::render( vector<Point3D>& vPoints, vector<Point2D>& vTexCoords, vector<Color>& vColors, vector<unsigned short int>& vIndices )
	{
		Block::program->use( true );

		Block::vertices->setData( vPoints );
		Block::textureCoordinates->setData( vTexCoords );
		Block::colors->setData( vColors );
		Block::indices->setData( vIndices );

		Block::program->sendUniform( "projection_matrix", Matrix::projection, false );
		Block::program->sendUniform( "modelview_matrix", Matrix::modelview, false );
		Block::program->sendUniform( "texture", *(Block::texture), 0 );
		Block::program->sendVertexPointer( "a_Vertex", Block::vertices );
		Block::program->sendColorPointer( "a_Color", Block::colors );
		Block::program->sendAttributePointer( "a_TexCoord", Block::textureCoordinates, 2 );

		Block::indices->draw( OpenGL::Triangles );
	}
}

