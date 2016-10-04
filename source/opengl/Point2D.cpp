#include <opengl/Point2D.h>

namespace opengl
{
	Point2D::Point2D( float x, float y ) : x(x), y(y)
	{
	}
	
	Point2D::Point2D( const Point2D& point ) : x(point.x), y(point.y)
	{
	}
	
	Point2D::~Point2D()
	{
	}
	
	void Point2D::setX( float x )
	{
		this->x = x;
	}
	
	void Point2D::setY( float y )
	{
		this->y = y;
	}
	
	void Point2D::moveTo( float x, float y )
	{
		this->x = x;
		this->y = y;
	}
	
	void Point2D::moveBy( float dx, float dy )
	{
		this->x += dx;
		this->y += dy;
	}
	
	float Point2D::getX() const
	{
		return this->x;
	}
	
	float Point2D::getY() const
	{
		return this->y;
	}
}

