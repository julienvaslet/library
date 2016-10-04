#ifndef __OPENGL_POINT2D_H
#define __OPENGL_POINT2D_H	1

using namespace std;

namespace opengl
{
	class Point2D
	{	
		protected:
			float x;
			float y;
			
		public:
			Point2D( float x = 0.0f, float y = 0.0f );
			Point2D( const Point2D& point );
			virtual ~Point2D();
			
			void setX( float x );
			void setY( float y );
			
			void moveTo( float x, float y );
			void moveBy( float dx, float dy );
			
			float getX() const;
			float getY() const;
	};
}

#endif
