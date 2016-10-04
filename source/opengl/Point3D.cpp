#include <opengl/Point3D.h>

namespace opengl
{
	Point3D::Point3D( float x, float y, float z ) : x(x), y(y), z(z)
	{
	}
	
	Point3D::Point3D( const Point3D& point ) : x(point.x), y(point.y), z(point.z)
	{
	}
	
	Point3D::~Point3D()
	{
	}
	
	void Point3D::setX( float x )
	{
		this->x = x;
	}
	
	void Point3D::setY( float y )
	{
		this->y = y;
	}
	
	void Point3D::setZ( float z )
	{
		this->z = z;
	}
	
	void Point3D::moveTo( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	void Point3D::moveBy( float dx, float dy, float dz )
	{
		this->x += dx;
		this->y += dy;
		this->z += dz;
	}
	
	float Point3D::getX() const
	{
		return this->x;
	}
	
	float Point3D::getY() const
	{
		return this->y;
	}
	
	float Point3D::getZ() const
	{
		return this->z;
	}
}
