#include <opengl/Vector.h>
#include <cmath>
#include <iostream>

namespace opengl
{
	Vector::Vector() : x(0.0f), y(0.0f), z(0.0f)
	{
	}
	
	Vector::Vector( float x, float y, float z ) : x(x), y(y), z(z)
	{
	}
	
	Vector::Vector( const Vector& vector ) : x(vector.x), y(vector.y), z(vector.z)
	{
	}
	
	Vector::~Vector()
	{
	}
	
	void Vector::setX( float x )
	{
		this->x = x;
	}
	
	void Vector::setY( float y )
	{
		this->y = y;
	}
	
	void Vector::setZ( float z )
	{
		this->z = z;
	}

	float Vector::getX() const
	{
		return this->x;
	}
	
	float Vector::getY() const
	{
		return this->y;
	}
	
	float Vector::getZ() const
	{
		return this->z;
	}

	void Vector::show() const
	{
		std::cout << "[ " << this->x << "\t]" << std::endl;
		std::cout << "[ " << this->y << "\t]" << std::endl;
		std::cout << "[ " << this->z << "\t]" << std::endl;
	}

	float Vector::norm() const
	{
		return sqrt( this->x * this->x + this->y * this->y + this->z * this->z );
	}
	
	float Vector::dot( const Vector& vector ) const
	{
		return (this->x * vector.x) + (this->y * vector.y) + (this->z * vector.z);
	}
	
	float Vector::getAngle( const Vector& vector ) const
	{
		return acos( this->dot( vector ) / ( this->norm() * vector.norm() ) ) * 180.0f / M_PI;
	}
	
	Vector& Vector::normalize()
	{
		*this /= this->norm();
		return *this;
	}

	Vector& Vector::operator*=( float n )
	{
		this->x *= n;
		this->y *= n;
		this->z *= n;
	
		return *this;
	}
	
	Vector& Vector::operator*=( const Vector& vector )
	{
		float x = this->x;
		float y = this->y;
		float z = this->z;
	
		this->x = y * vector.z - z * vector.y;
		this->y = z * vector.x - x * vector.z;
		this->z = x * vector.y - y * vector.x;
	
		return *this;
	}
	
	Vector Vector::operator*( const Vector& vector )
	{
		Vector v = *this;
		v *= vector;
	
		return v;
	}
	
	Vector& Vector::operator/=( float n )
	{
		this->x /= n;
		this->y /= n;
		this->z /= n;
	
		return *this;
	}
	
	Vector Vector::operator/( float n )
	{
		Vector v = *this;
		v /= n;
	
		return v;
	}
	
	Vector Vector::operator*( const Matrix& matrix )
	{
		// Implements result = matrix * vector
		
		Vector v;
		
		v.x = this->x * matrix.get( 1, 1 ) + this->y * matrix.get( 2, 1 ) + this->z * matrix.get( 3, 1 ) + matrix.get( 4, 1 );
		v.y = this->x * matrix.get( 1, 2 ) + this->y * matrix.get( 2, 2 ) + this->z * matrix.get( 3, 2 ) + matrix.get( 4, 2 );
		v.z = this->x * matrix.get( 1, 3 ) + this->y * matrix.get( 2, 3 ) + this->z * matrix.get( 3, 3 ) + matrix.get( 4, 3 );

		return v;
	}
	
	Vector& Vector::operator*=( const Matrix& matrix )
	{
		Vector v = (*this) * matrix;
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	
		return *this;
	}
}
