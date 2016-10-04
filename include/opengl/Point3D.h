#ifndef __OPENGL_POINT3D_H
#define __OPENGL_POINT3D_H	1

using namespace std;

namespace opengl
{
	class Point3D
	{	
		protected:
			float x;
			float y;
			float z;
			
		public:
			Point3D( float x = 0.0f, float y = 0.0f, float z = 0.0f );
			Point3D( const Point3D& point );
			virtual ~Point3D();
			
			void setX( float x );
			void setY( float y );
			void setZ( float z );
			
			void moveTo( float x, float y, float z );
			void moveBy( float dx, float dy, float dz );
			
			float getX() const;
			float getY() const;
			float getZ() const;
	};
}

#endif
