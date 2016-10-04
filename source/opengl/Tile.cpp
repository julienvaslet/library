#include <opengl/Tile.h>
#include <game/Resource.h>

using namespace game;

namespace opengl
{
	Tile::Tile() : texture(NULL), view()
	{
	}
	
	Tile::Tile( const Tile& tile ) : texture(tile.texture), view(tile.view)
	{
		if( this->texture != NULL )
			this->texture->use();
	}
	
	Tile::Tile( const string& textureResource ) : texture(NULL), view()
	{
		this->texture = static_cast<Texture2D *>( Resource::get( textureResource ) );
		
		if( this->texture != NULL )
		{
			this->texture->use();
			this->view.resize( this->texture->getWidth(), this->texture->getHeight() );
		}
	}
	
	Tile::Tile( Texture2D * texture ) : texture(texture), view()
	{
		if( this->texture != NULL )
		{
			this->texture->use();
			this->view.resize( this->texture->getWidth(), this->texture->getHeight() );
		}
	}
	
	Tile::~Tile()
	{
		if( this->texture != NULL )
			this->texture->free();
	}
	
	void Tile::setView( unsigned int x, unsigned int y, unsigned int width, unsigned int height )
	{
		this->view.getOrigin().moveTo( x, y, 0.0f );
		this->view.resize( width, height );
	}
	
	const Rectangle& Tile::getView() const
	{
		return this->view;
	}
	
	Texture2D * Tile::getTexture()
	{
		return this->texture;
	}

	void Tile::setTexture( Texture2D * texture )
	{
		if( this->texture != NULL )
			this->texture->free();
			
		this->texture = texture;
		
		if( this->texture != NULL )
		{
			this->texture->use();
			this->view.resize( this->texture->getWidth(), this->texture->getHeight() );
		}
	}
	
	void Tile::setTexture( const string& textureResource )
	{
		Texture2D * texture = static_cast<Texture2D *>( Resource::get( textureResource ) );
		
		if( texture != NULL )
		{
			if( this->texture != NULL )
				this->texture->free();
				
			this->texture = texture;
			this->texture->use();
			this->view.resize( this->texture->getWidth(), this->texture->getHeight() );
		}
	}
	
	void Tile::prepareRendering( vector<Point2D>& textureCoordinates )
	{
		if( this->texture != NULL )
		{
			textureCoordinates.push_back( Point2D( this->view.getOrigin().getX() / static_cast<float>( this->texture->getWidth() ), 1.0f - ( this->view.getOrigin().getY() / static_cast<float>( this->texture->getHeight() ) ) ) );
			textureCoordinates.push_back( Point2D( ( this->view.getOrigin().getX() + static_cast<float>( this->view.getWidth() ) ) / static_cast<float>( this->texture->getWidth() ), 1.0f - ( this->view.getOrigin().getY() / static_cast<float>( this->texture->getHeight() ) ) ) );
			textureCoordinates.push_back( Point2D( ( this->view.getOrigin().getX() + static_cast<float>( this->view.getWidth() ) ) / static_cast<float>( this->texture->getWidth() ), 1.0f - ( (this->view.getOrigin().getY() + static_cast<float>( this->view.getHeight() ) ) / static_cast<float>( this->texture->getHeight() ) ) ) );
			textureCoordinates.push_back( Point2D( this->view.getOrigin().getX() / static_cast<float>( this->texture->getWidth() ), 1.0f - ( (this->view.getOrigin().getY() + static_cast<float>( this->view.getHeight() ) ) / static_cast<float>( this->texture->getHeight() ) ) ) );
		}
		else
		{
			textureCoordinates.push_back( Point2D( 0.0f, 0.0f ) );
			textureCoordinates.push_back( Point2D( 0.0f, 0.0f ) );
			textureCoordinates.push_back( Point2D( 0.0f, 0.0f ) );
			textureCoordinates.push_back( Point2D( 0.0f, 0.0f ) );
		}
	}
}

