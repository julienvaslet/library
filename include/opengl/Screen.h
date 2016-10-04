#ifndef __OPENGL_SCREEN_H
#define	__OPENGL_SCREEN_H	1

#include <SDL2/SDL.h>
#include <opengl/OpenGL.h>
#include <opengl/Color.h>
#include <opengl/Point2D.h>

#ifdef __PI__
#define OPENGL_DEFAULT_MAJOR_VERSION	2
#define OPENGL_DEFAULT_MINOR_VERSION	0
#else
#define OPENGL_DEFAULT_MAJOR_VERSION	3
#define OPENGL_DEFAULT_MINOR_VERSION	0
#endif

#define LCD_MONITOR		0

#ifdef __NO_X_WINDOW__
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <bcm_host.h>
#endif

namespace opengl
{
	class Screen
	{
		protected:
			static Screen * instance;

#ifdef __NO_X_WINDOW__
			EGL_DISPMANX_WINDOW_T window;
			EGLContext context;
			EGLDisplay display;
			EGLSurface surface;
#else
			SDL_Window * window;
			SDL_GLContext context;
#endif
			
			int x;
			int y;
			int width;
			int height;
			
			Color clearColor;
		
			Screen();
			~Screen();
			bool createWindow( int width, int height, bool windowed = false );
			bool createContext( int majorVersion, int minorVersion );
			
			static void initializeSystem();
			
#ifdef __NO_X_WINDOW__
			static bool checkEglError();
#endif
		
		public:
			static void getDisplaySize( int * width, int * height );
			static bool initialize( int width = 0, int height = 0, bool autoResize = true, bool ignoreOverscan = false, int majorVersion = OPENGL_DEFAULT_MAJOR_VERSION, int minorVersion = OPENGL_DEFAULT_MINOR_VERSION );
			static bool initializeWindowed( int width = 0, int height = 0, bool maximized = true, int majorVersion = OPENGL_DEFAULT_MAJOR_VERSION, int minorVersion = OPENGL_DEFAULT_MINOR_VERSION );
			static Screen * get();
			static void destroy();
		
			void setTitle( const string& title );
			void setClearColor( const Color& color );
			void render();
			void clear( int flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, bool automaticViewport = true );
			int getWidth();
			int getHeight();
			void resize( int width, int height );
			Point2D getCoordinates( int systemX, int systemY );
	};
}

#endif

