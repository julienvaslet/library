#ifndef __OPENGL_CAMERA_H
#define __OPENGL_CAMERA_H	1

#include <opengl/Point3D.h>
#include <opengl/Vector.h>

#if defined(__MINGW32__) || defined(__MINGW64__)
#undef far
#undef near
#endif

using namespace std;

namespace opengl
{
	class Camera
	{	
		protected:
			Point3D eye;
			Point3D center;
			Vector up;
			
		public:
			Camera();
			virtual ~Camera();
			
			Point3D& getEye();
			Point3D& getCenter();
			Vector& getUp();

#ifndef __PI__			
			void look() const;

			void setOrthogonal( float left, float right, float bottom, float top, float near, float far );
			void setPerspective( float fovy, float aspect, float near, float far );
			void setFrustum( float left, float right, float bottom, float top, float near, float far );
#endif
	};
}

#endif
