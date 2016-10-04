#ifndef __OPENGL_VECTOR_H
#define __OPENGL_VECTOR_H	1

#include <opengl/Matrix.h>

using namespace std;

namespace opengl
{
	class Vector
	{	
		protected:
			float x;
			float y;
			float z;
			
		public:
			Vector();
			Vector( const Vector& vector );
			Vector( float x, float y, float z );
			~Vector();
		
			void setX( float x );
			void setY( float y );
			void setZ( float z );
			float getX() const;
			float getY() const;
			float getZ() const;
		
			void show() const;
		
			float norm() const;
			Vector& normalize();
			
			float dot( const Vector& vector ) const;
			float getAngle( const Vector& vector ) const;
		
			Vector& operator*=( float n );
			Vector& operator*=( const Vector& vector );
			Vector operator*( const Vector& vector );
			Vector& operator/=( float n );
			Vector operator/( float n );
			Vector operator*( const Matrix& matrix );
			Vector& operator*=( const Matrix& matrix );
	};
}

#endif
