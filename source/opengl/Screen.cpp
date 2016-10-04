#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <tools/pi/Overscan.h>

using namespace tools::pi;

#ifdef DEBUG0
#include <tools/logger/Logger.h>
#include <iostream>
using namespace std;
using namespace tools::logger;
#endif

namespace opengl
{
	Screen * Screen::instance = NULL;

	Screen::Screen() : x(0), y(0), width(0), height(0), clearColor(0.0,0.0,0.0,1.0)
	{
	}

	Screen::~Screen()
	{
		#ifdef __NO_X_WINDOW__
		eglMakeCurrent( this->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
		
		if( this->surface != EGL_NO_SURFACE )
		{
			eglDestroySurface( this->display, this->surface );
			
			#ifdef DEBUG0
			if( Screen::checkEglError() )
				Logger::get() << "[Screen] Surface destroyed." << Logger::endl;
			else
				Logger::get() << "[Screen] Surface could not be destroyed." << Logger::endl;
			#endif
		}
		
		if( this->context != EGL_NO_CONTEXT )
		{
			eglDestroyContext( this->display, this->context );
		
			#ifdef DEBUG0
			if( Screen::checkEglError() )
				Logger::get() << "[Screen] Context destroyed." << Logger::endl;
			else
				Logger::get() << "[Screen] Context could not be destroyed." << Logger::endl;
			#endif
		}
		
		if( this->display != EGL_NO_DISPLAY )
		{
			eglTerminate( this->display );
			
			#ifdef DEBUG0
			if( Screen::checkEglError() )
				Logger::get() << "[Screen] Display session terminated." << Logger::endl;
			else
				Logger::get() << "[Screen] Display session could not be terminated." << Logger::endl;
			#endif
		}
		
		#else
		if( this->context != NULL )
		{
			SDL_GL_DeleteContext( this->context );
			
			#ifdef DEBUG0
			Logger::get() << "[Screen] Context destroyed." << Logger::endl;
			#endif
		}
		
		if( this->window != NULL )
		{
			SDL_DestroyWindow( this->window );
		
			#ifdef DEBUG0
			Logger::get() << "[Screen] Window destroyed." << Logger::endl;
			#endif
		}
		#endif
	}
	
	void Screen::getDisplaySize( int * width, int * height )
	{
		#ifdef __NO_X_WINDOW__
		unsigned int uWidth = 0;
		unsigned int uHeight = 0;

		if( graphics_get_display_size( LCD_MONITOR, &uWidth, &uHeight ) < 0 )
		{
			*width = 0;
			*height = 0;
			
			#ifdef DEBUG0
			Logger::get() << "[Screen] Unable to get the display size." << Logger::endl;
			#endif
		}
		else
		{
			*width = static_cast<int>( uWidth );
			*height = static_cast<int>( uHeight );
		}
		
		#else

		SDL_DisplayMode displayMode;
		SDL_GetDesktopDisplayMode( LCD_MONITOR, &displayMode );
		*width = displayMode.w;
		*height = displayMode.h;
		
		#endif
	}
	
	void Screen::initializeSystem()
	{
		#ifdef __NO_X_WINDOW__
		bcm_host_init();
		SDL_Init( SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO );
		#else
		SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO );
		#endif
	}
	
	bool Screen::createWindow( int width, int height, bool windowed )
	{
		bool success = true;
		
		#ifdef __NO_X_WINDOW__
		
		VC_RECT_T srcRect = { 0, 0, width << 16, height << 16 };
		VC_RECT_T dstRect = { 0, 0, width, height };
		
		DISPMANX_DISPLAY_HANDLE_T dispmanDisplay = vc_dispmanx_display_open( LCD_MONITOR );
		DISPMANX_UPDATE_HANDLE_T dispmanUpdate = vc_dispmanx_update_start( 0 );
		VC_DISPMANX_ALPHA_T alpha = { DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS, 0xff, 0 };
		
		vc_dispmanx_display_set_background( dispmanUpdate, dispmanDisplay, 0, 0, 0 );
		
		this->window.element = vc_dispmanx_element_add( dispmanUpdate, dispmanDisplay, 0, &dstRect, 0, &srcRect, DISPMANX_PROTECTION_NONE, &alpha, 0, static_cast<DISPMANX_TRANSFORM_T>( 0 ) );
		this->window.width = width;
		this->window.height = height;
		
		vc_dispmanx_update_submit_sync( dispmanUpdate );
		
		#else
		
		// BUG: Multiple display is not correctly handled
		
		this->window = SDL_CreateWindow(
			NULL,
			0,
			0,
			width,
			height,
			windowed ? SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED : SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL
		);
		
		if( this->window == NULL )
		{
			success = false;
			
			#ifdef DEBUG0
			Logger::get() << "[Screen] Unable to create window: " << SDL_GetError() << Logger::endl;
			#endif
		}
		
		#endif

		return success;
	}
	
	bool Screen::createContext( int majorVersion, int minorVersion )
	{
		bool success = true;
		
		#ifdef __NO_X_WINDOW__
		this->display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
		Screen::checkEglError();
		
		if( display == EGL_NO_DISPLAY )
		{
			success = false;
			
			#ifdef DEBUG0
			Logger::get() << "[Screen] Unable to open default display." << Logger::endl;
			#endif
		}
		else
		{
			eglInitialize( this->display, NULL, NULL );
			if( !Screen::checkEglError() )
			{
				success = false;
				
				#ifdef DEBUG0
				Logger::get() << "[Screen] Unable to initialize EGL." << Logger::endl;
				#endif
			}
			else
			{
				EGLint numConfigs = 0;
				
				// seems to be useless call
				eglGetConfigs( this->display, NULL, 0, &numConfigs );
				
				if( !Screen::checkEglError() )
				{
					success = false;
					
					#ifdef DEBUG0
					Logger::get() << "[Screen] Unable to get EGL configurations." << Logger::endl;
					#endif
				}
				else
				{
					EGLint attributes[] =
					{
						EGL_RED_SIZE,		8,
						EGL_GREEN_SIZE,		8,
						EGL_BLUE_SIZE,		8,
						EGL_ALPHA_SIZE,		8,
						EGL_SURFACE_TYPE, 	EGL_WINDOW_BIT,
						EGL_NONE
					};
					
					EGLConfig config;
					
					eglChooseConfig( this->display, attributes, &config, 1, &numConfigs );
					
					if( !Screen::checkEglError() )
					{
						success = false;
						
						#ifdef DEBUG0
						Logger::get() << "[Screen] Unable to choose EGL configuration." << Logger::endl;
						#endif
					}
					else
					{
						// majorVersion & minorVersion are ignored
						EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
					
						this->context = eglCreateContext( this->display, config, EGL_NO_CONTEXT, contextAttributes );
						Screen::checkEglError();
						
						if( this->context == EGL_NO_CONTEXT )
						{
							success = false;
							
							#ifdef DEBUG0
							Logger::get() << "[Screen] Unable to create the context." << Logger::endl;
							#endif
						}
						else
						{
							this->surface = eglCreateWindowSurface( this->display, config, &(this->window), NULL );
							Screen::checkEglError();
						
							if( surface == EGL_NO_SURFACE )
							{
								success = false;
							
								#ifdef DEBUG0
								Logger::get() << "[Screen] Unable to create window surface." << Logger::endl;
								#endif
							}
							else
							{
								eglMakeCurrent( this->display, this->surface, this->surface, this->context );
								if( !Screen::checkEglError() )
								{
									success = false;
									
									#ifdef DEBUG0
									Logger::get() << "[Screen] Unable to make the context current." << Logger::endl;
									#endif
								}
								else
								{
									eglSwapInterval( this->display, 0 );
								}
							}
						}
					}
				}
			}
		}
		
		#else
	
		// Set the OpenGL version		
		#ifdef __PI__
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
		#else
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		#endif

		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minorVersion );

		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
	
		this->context = SDL_GL_CreateContext( this->window );
		
		if( this->context == NULL )
		{
			success = false;
			
			#ifdef DEBUG0
			Logger::get() << "[Screen] Unable to create the context: " << SDL_GetError() << Logger::endl;
			#endif
		}
		else
		{
			// Do not synchronize framerate with monitor frequency (processor more used)
			SDL_GL_SetSwapInterval( 0 );
			
			// Synchronize framerate with monitor frequency (processor less used)
			//SDL_GL_SetSwapInterval( 1 );
		}
		#endif

		return success;
	}
		
	bool Screen::initialize( int width, int height, bool autoResize, bool ignoreOverscan, int majorVersion, int minorVersion )
	{
		bool success = true;
		
		Screen::initializeSystem();
		
		int overscanTop = 0;
		int overscanBottom = 0;
		int overscanLeft = 0;
		int overscanRight = 0;
		
		if( !ignoreOverscan )
		{
			Overscan::initialize();
			Overscan::get( &overscanLeft, &overscanRight, &overscanTop, &overscanBottom );
		}

		int displayWidth = 0;
		int displayHeight = 0;
		Screen::getDisplaySize( &displayWidth, &displayHeight );
		
		if( displayWidth == 0 || displayHeight == 0 )
		{
			displayWidth = width;
			displayHeight = height;
		}
		
		int viewWidth = displayWidth - overscanLeft - overscanRight;
		int viewHeight = displayHeight - overscanTop - overscanBottom;
		
		Screen * screen = new Screen();
		
		if( width == 0 || height == 0 )
		{
			width = viewWidth;
			height = viewHeight;
			
			#ifdef DEBUG0
			Logger::get() << "[Screen] Auto-sized window: " << width << "x" << height << "." << Logger::endl;
			#endif
		}
		else if( autoResize )
		{
			double widthRatio = static_cast<double>( viewWidth ) / static_cast<double>( width );
			double heightRatio = static_cast<double>( viewHeight ) / static_cast<double>( height );
			
			#ifdef DEBUG0
			Logger::get() << "[Screen] Original window size: " << width << "x" << height << "." << Logger::endl;
			#endif
			
			if( widthRatio < heightRatio )
			{
				width = viewWidth;
				height = ceil( height * widthRatio );
			}
			else
			{
				width = ceil( width * heightRatio );
				height = viewHeight;
			}
			
			#ifdef DEBUG0
			Logger::get() << "[Screen] Auto-resized window size: " << width << "x" << height << "." << Logger::endl;
			#endif
		}
		
		screen->width = width;
		screen->height = height;
		
		// Centering
		screen->x = static_cast<int>( overscanLeft + (viewWidth - width) / 2.0 );
		screen->y = static_cast<int>( overscanRight + (viewHeight - height) / 2.0 );

		if( !screen->createWindow( displayWidth, displayHeight ) )
		{
			success = false;
			delete screen;
		}
		else
		{
			#ifdef DEBUG0
			Logger::get() << "[Screen] Window created." << Logger::endl;
			#endif
	
			if( !screen->createContext( majorVersion, minorVersion ) )
			{
				success = false;
				delete screen;
			}
			else
			{
				#ifdef DEBUG0
				Logger::get() << "[Screen] Context created." << Logger::endl;
				#endif
			
				Screen::instance = screen;
				
				OpenGL::initialize();
			
				screen->clear();
				screen->render();
			}
		}
	
		return success;
	}
	
	bool Screen::initializeWindowed( int width, int height, bool maximized, int majorVersion, int minorVersion )
	{
		bool success = true;
		
		Screen::initializeSystem();
		
		int displayWidth = 0;
		int displayHeight = 0;
		Screen::getDisplaySize( &displayWidth, &displayHeight );
		
		if( displayWidth == 0 || displayHeight == 0 )
		{
			displayWidth = width;
			displayHeight = height;
		}
		
		Screen * screen = new Screen();
		
		if( width == 0 || height == 0 )
		{
			width = displayWidth;
			height = displayHeight;
			
			#ifdef DEBUG0
			Logger::get() << "[Screen] Auto-sized window: " << width << "x" << height << "." << Logger::endl;
			#endif
		}
		
		screen->width = width;
		screen->height = height;

		if( !screen->createWindow( displayWidth, displayHeight, true ) )
		{
			success = false;
			delete screen;
		}
		else
		{
			#ifdef DEBUG0
			Logger::get() << "[Screen] Window created." << Logger::endl;
			#endif
	
			if( !screen->createContext( majorVersion, minorVersion ) )
			{
				success = false;
				delete screen;
			}
			else
			{
				#ifdef DEBUG0
				Logger::get() << "[Screen] Context created." << Logger::endl;
				#endif
			
				Screen::instance = screen;
				
				OpenGL::initialize();
			
				screen->clear();
				screen->render();
			}
		}
	
		return success;
	}

	Screen * Screen::get()
	{
		return Screen::instance;
	}

	void Screen::destroy()
	{
		Screen * screen = Screen::get();
		delete screen;
		Screen::instance = NULL;
	
		Overscan::destroy();
		SDL_Quit();
	}

	void Screen::clear( int flags, bool automaticViewport )
	{
		glClearColor( this->clearColor.getRed(), this->clearColor.getGreen(), this->clearColor.getBlue(), this->clearColor.getAlpha() );
		glClear( flags );
		
		if( automaticViewport )
			glViewport( this->x, this->y, this->width, this->height );
	}

	void Screen::render()
	{
		#ifdef __NO_X_WINDOW__
		eglSwapBuffers( this->display, this->surface );
		#else
		SDL_GL_SwapWindow( this->window );
		#endif
	}
	
	void Screen::setTitle( const string& title )
	{
		SDL_SetWindowTitle( this->window, title.c_str() );
	}
	
	void Screen::setClearColor( const Color& color )
	{
		this->clearColor.setColor( color );
	}
	
	int Screen::getWidth()
	{		
		return this->width;
	}
	
	int Screen::getHeight()
	{
		return this->height;
	}
	
	void Screen::resize( int width, int height )
	{
		this->width = width;
		this->height = height;
		
		/*#ifdef DEBUG0
		Logger::get() << "[Screen] Window resized to : " << width << "x" << height << Logger::endl;
		#endif*/
	}
	
	Point2D Screen::getCoordinates( int systemX, int systemY )
	{
		return Point2D( systemX - this->x, systemY - this->y );
	}
	
	#ifdef __NO_X_WINDOW__
	bool Screen::checkEglError()
	{
		bool success = true;
		EGLint error = eglGetError();
		
		if( error != EGL_SUCCESS )
		{
			success = false;
			
			#ifdef DEBUG0
			Logger::get() << "[EGL] Error#" << error << ": ";
			
			switch( error )
			{
				case EGL_NOT_INITIALIZED: Logger::get() << "EGL_NOT_INITIALIZED"; break;
				case EGL_BAD_ACCESS: Logger::get() << "EGL_BAD_ACCESS"; break;
				case EGL_BAD_ALLOC: Logger::get() << "EGL_BAD_ALLOC"; break;
				case EGL_BAD_ATTRIBUTE: Logger::get() << "EGL_BAD_ATTRIBUTE"; break;
				case EGL_BAD_CONTEXT: Logger::get() << "EGL_BAD_CONTEXT"; break;
				case EGL_BAD_CONFIG: Logger::get() << "EGL_BAD_CONFIG"; break;
				case EGL_BAD_CURRENT_SURFACE: Logger::get() << "EGL_BAD_CURRENT_SURFACE"; break;
				case EGL_BAD_DISPLAY: Logger::get() << "EGL_BAD_DISPLAY"; break;
				case EGL_BAD_SURFACE: Logger::get() << "EGL_BAD_SURFACE"; break;
				case EGL_BAD_MATCH: Logger::get() << "EGL_BAD_MATCH"; break;
				case EGL_BAD_PARAMETER: Logger::get() << "EGL_BAD_PARAMETER"; break;
				case EGL_BAD_NATIVE_PIXMAP: Logger::get() << "EGL_BAD_NATIVE_PIXMAP"; break;
				case EGL_BAD_NATIVE_WINDOW: Logger::get() << "EGL_BAD_NATIVE_WINDOW"; break;
				case EGL_CONTEXT_LOST: Logger::get() << "EGL_CONTEXT_LOST"; break;
				
				default: Logger::get() << "Unknown error code."; break;
			}
			
			Logger::get() << "." << Logger::endl;
			#endif
		}
		
		return success;
	}
	#endif
}
