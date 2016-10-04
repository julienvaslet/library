#include <opengl/Rectangle.h>

namespace opengl
{
	Rectangle::Rectangle() : origin(), anchor(), width(0), height(0)
	{
	}
	
	Rectangle::Rectangle( const Rectangle& rectangle ) : origin(rectangle.origin), anchor(rectangle.anchor), width(rectangle.width), height(rectangle.height)
	{
	}
	
	Rectangle::Rectangle( unsigned width, unsigned height ) : origin(), anchor(), width(width), height(height)
	{
	}
	
	Rectangle::~Rectangle()
	{
	}
	
	Point3D& Rectangle::getOrigin()
	{
		return this->origin;
	}
	
	Point2D& Rectangle::getAnchor()
	{
		return this->anchor;
	}
	
	float Rectangle::getX() const
	{
		return this->origin.getX() - this->anchor.getX();
	}
	
	float Rectangle::getY() const
	{
		return this->origin.getY() - this->anchor.getY();
	}
	
	float Rectangle::getZ() const
	{
		return this->origin.getZ();
	}
	
	unsigned int Rectangle::getWidth() const
	{
		return this->width;
	}
	
	unsigned int Rectangle::getHeight() const
	{
		return this->height;
	}
	
	void Rectangle::setWidth( unsigned int width )
	{
		this->width = width;
	}
	
	void Rectangle::setHeight( unsigned int height )
	{
		this->height = height;
	}
	
	void Rectangle::resize( unsigned int width, unsigned int height )
	{
		this->width = width;
		this->height = height;
	}
	
	void Rectangle::resizeBy( unsigned int deltaWidth, unsigned int deltaHeight )
	{
		this->width += deltaWidth;
		this->height += deltaHeight;
	}
	
	bool Rectangle::isInCollision( const Rectangle& rectangle ) const
	{
		bool hasCollision = false;
		
		// X axis collision
		if( ( this->getX() <= rectangle.getX() && this->getX() + this->width > rectangle.getX() )
		 || ( rectangle.getX() <= this->getX() && rectangle.getX() + rectangle.width > this->getX() )
		 || ( this->getX() >= rectangle.getX() && this->getX() + this->width <= rectangle.getX() + rectangle.width ) )
		{
			// Y axis collision
			if( ( this->getY() <= rectangle.getY() && this->getY() + this->height > rectangle.getY() )
			 || ( rectangle.getY() <= this->getY() && rectangle.getY() + rectangle.height > this->getY() )
	 		 || ( this->getY() >= rectangle.getY() && this->getY() + this->height <= rectangle.getY() + rectangle.height ))
			{
				// Z axis collision
				// No depth for rectangle, simply compare Z axis
				if( this->getZ() == rectangle.getZ() )
					hasCollision = true;
			}
		}
		
		return hasCollision;
	}
	
	bool Rectangle::isInCollision( const Point2D& point ) const
	{
		bool hasCollision = false;
		
		if( point.getX() >= this->getX() && point.getX() <= this->getX()+ this->width )
		{
			if( point.getY() >= this->getY() && point.getY() <= this->getY() + this->height )
			{
				hasCollision = true;
			}
		}
		
		return hasCollision;
	}
	
	bool Rectangle::isInCollision( const Point3D& point ) const
	{
		bool hasCollision = false;
		
		// Rectangle has no depth, simply compare Z axis
		if( point.getZ() == this->getZ() )
		{
			if( point.getX() >= this->getX() && point.getX() <= this->getX()+ this->width )
			{
				if( point.getY() >= this->getY() && point.getY() <= this->getY() + this->height )
				{
					hasCollision = true;
				}
			}
		}
		
		return hasCollision;
	}
	
	void Rectangle::prepareRendering( vector<Point3D>& vertices, vector<unsigned short int>& indices, bool clockwise ) const
	{
		unsigned short int j = static_cast<unsigned short int>( vertices.size() );
		 
		// Vertices
		vertices.push_back( Point3D( this->origin.getX() - this->anchor.getX(), this->origin.getY() - this->anchor.getY(), this->origin.getZ() ) );
		vertices.push_back( Point3D( this->origin.getX() - this->anchor.getX() + width, this->origin.getY() - this->anchor.getY(), this->origin.getZ() ) );
		vertices.push_back( Point3D( this->origin.getX() - this->anchor.getX() + width, this->origin.getY() - this->anchor.getY() + height, this->origin.getZ() ) );
		vertices.push_back( Point3D( this->origin.getX() - this->anchor.getX(), this->origin.getY() - this->anchor.getY() + height, this->origin.getZ() ) );

		// Indices		
		if( clockwise )
		{
			indices.push_back( j );
			indices.push_back( j + 1 );
			indices.push_back( j + 2 );
			indices.push_back( j );
			indices.push_back( j + 2 );
			indices.push_back( j + 3 );
		}
		else
		{
			indices.push_back( j );
			indices.push_back( j + 3 );
			indices.push_back( j + 2 );
			indices.push_back( j );
			indices.push_back( j + 2 );
			indices.push_back( j + 1 );
		}
	}
}

