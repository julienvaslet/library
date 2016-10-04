#ifndef __TOOLS_CAMERA_GPHOTOCAMERA_H
#define __TOOLS_CAMERA_GPHOTOCAMERA_H	1

#include <gphoto2/gphoto2.h>
#include <tools/logger/Logger.h>

/*
http://www.gphoto.org/doc/api/gphoto2-camera_8h.html#a5ef187a092edd693b3d01ad8304728fb
*/

#include <map>
#include <vector>
#include <string>

using namespace std;

namespace tools
{
	namespace camera
	{
		class GphotoCamera
		{
			protected:
				static GPContext * context;
				static CameraAbilitiesList * abilities;
				static GPPortInfoList * drivers;
				static map<string,GphotoCamera *> openedCameras;
				
				string port;
				string model;
				Camera * camera;
				map<string,CameraWidget *> settings;
				
				GphotoCamera( const string& port );
				bool loadSetting( const string& name, bool reload = false );
			
			public:
				~GphotoCamera();
				
				void listSettings( vector<string>& settings );
				string getSetting( const string& name );
				void getSettingValues( const string& name, vector<string>& values );
				bool setSetting( const string& name, const string& value );
				
				bool preview( const string& filename );
				bool capture( const string& filename, bool deleteFile = true );
				
				static void initializeContext();
				static void destroyContext();
				static GPContext * getContext();
				static void errorCallback( GPContext * context, const char * message, void * data );
				static void informationCallback( GPContext * context, const char * message, void * data );
				static bool checkError( int code );
				
				static string getCameraModelName( const string& port );
				static GphotoCamera * getCamera( const string& port );
				static void list( map<string,string>& cameras );
				
		};
	}
}

#endif

