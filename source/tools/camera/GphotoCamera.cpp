#include <tools/camera/GphotoCamera.h>
#include <sstream>

#include <cstring>

using namespace std;
using namespace tools::logger;

namespace tools
{
	namespace camera
	{
		GPContext * GphotoCamera::context = NULL;
		CameraAbilitiesList * GphotoCamera::abilities = NULL;
		GPPortInfoList * GphotoCamera::drivers = NULL;
		map<string,GphotoCamera *> GphotoCamera::openedCameras;
		
		GphotoCamera::GphotoCamera( const string& port ) : port(port), model(""), camera(NULL)
		{
			#ifdef DEBUG1
			Logger::get() << "[GphotoCamera] Initializing camera on port " << this->port << "..." << Logger::endl;
			#endif
			
			this->model = GphotoCamera::getCameraModelName( this->port );
			
			#ifdef DEBUG1
			Logger::get() << "[GphotoCamera] Camera model name is \"" << this->model << "\"..." << Logger::endl;
			#endif
			
			if( checkError( gp_camera_new( &(this->camera) ) ) )
			{
				// Load all abilities
				if( GphotoCamera::abilities == NULL )
				{
					if( checkError( gp_abilities_list_new( &(GphotoCamera::abilities) ) ) )
					{
						if( checkError( gp_abilities_list_load( GphotoCamera::abilities, GphotoCamera::context ) ) )
						{
							#ifdef DEBUG1
							Logger::get() << "[GphotoCamera] Cameras abilities loaded." << Logger::endl;
							#endif
						}
						else
						{
							gp_abilities_list_free( GphotoCamera::abilities );
							GphotoCamera::abilities = NULL;
							
							#ifdef DEBUG1
							Logger::get() << "[GphotoCamera] Unable to load abilities." << Logger::endl;
							#endif
						}
					}
					#ifdef DEBUG0
					else
						Logger::get() << "[GphotoCamera] Unable to initialize abilities list." << Logger::endl;
					#endif
				}
				
				// Find camera abilities in the list
				int index = gp_abilities_list_lookup_model( GphotoCamera::abilities, this->model.c_str() );
				
				if( index >= GP_OK )
				{
					CameraAbilities cameraAbilities;
					
					if( checkError( gp_abilities_list_get_abilities( GphotoCamera::abilities, index, &cameraAbilities ) ) )
					{
						if( checkError( gp_camera_set_abilities( this->camera, cameraAbilities ) ) )
						{
							#ifdef DEBUG1
							Logger::get() << "[GphotoCamera][" << this->model << "] Camera abilities loaded." << Logger::endl;
							#endif
						}
						#ifdef DEBUG0
						else
							Logger::get() << "[GphotoCamera][" << this->model << "] Unable to load camera abilities." << Logger::endl;
						#endif
					}
					#ifdef DEBUG0
					else
						Logger::get() << "[GphotoCamera][" << this->model << "] Unable to get camera abilities from the list." << Logger::endl;
					#endif
				}
				#ifdef DEBUG0
				else
					Logger::get() << "[GphotoCamera][" << this->model << "] Unable to find camera in abilities list." << Logger::endl;
				#endif
				
				// Load all port drivers
				if( GphotoCamera::drivers == NULL )
				{
					if( checkError( gp_port_info_list_new( &(GphotoCamera::drivers) ) ) )
					{
						if( checkError( gp_port_info_list_load( GphotoCamera::drivers ) ) )
						{
							#ifdef DEBUG1
							int count = gp_port_info_list_count( GphotoCamera::drivers );
							
							if( count >= 0 )
								Logger::get() << "[GphotoCamera] " << count << " port drivers loaded." << Logger::endl;
							else
								checkError( count );
							#endif
						}
						#ifdef DEBUG0
						else
							Logger::get() << "[GphotoCamera] Unable to load port drivers list." << Logger::endl;
						#endif
					}
					#ifdef DEBUG0
					else
						Logger::get() << "[GphotoCamera] Unable to initialize port drivers list." << Logger::endl;
					#endif
				}
	
				// Associate camera with portinfo
				int portIndex = gp_port_info_list_lookup_path( GphotoCamera::drivers, this->port.c_str() );
				
				if( portIndex != GP_ERROR_UNKNOWN_PORT )
				{
					GPPortInfo portInfo;
					
					if( checkError( gp_port_info_list_get_info( GphotoCamera::drivers, portIndex, &portInfo ) ) )
					{
						if( checkError( gp_camera_set_port_info( this->camera, portInfo ) ) )
						{
							#ifdef DEBUG1
							Logger::get() << "[GphotoCamera][" << this->model << "] Driver loaded." << Logger::endl;
							#endif
						}
						#ifdef DEBUG0
						else
							Logger::get() << "[GphotoCamera][" << this->model << "] Unable to set the driver to the camera." << Logger::endl;
						#endif
					}
					#ifdef DEBUG0
					else
						Logger::get() << "[GphotoCamera][" << this->model << "] Unable to get the port driver information from the list." << Logger::endl;
					#endif
				}
				#ifdef DEBUG1
				else
					Logger::get() << "[GphotoCamera][" << this->model << "] Unknown port \"" << this->port << "\"." << Logger::endl;
				#endif
				
				GphotoCamera::openedCameras[port] = this;
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[GphotoCamera][" << this->model << "] Unable to allocate memory for Camera." << Logger::endl;
			#endif
		}
		
		GphotoCamera::~GphotoCamera()
		{
			// Delete settings
			for( map<string,CameraWidget *>::iterator it = this->settings.begin() ; it != this->settings.end() ; it++ )
			{
				if( it->second != NULL )
				{
					checkError( gp_widget_free( it->second ) );
					it->second = NULL;
				}
			}
			
			this->settings.clear();
		
			// Remove camera from openedCameras list
			map<string,GphotoCamera *>::iterator it = GphotoCamera::openedCameras.find( this->port );
			
			if( it != GphotoCamera::openedCameras.end() )
				GphotoCamera::openedCameras.erase( it );
			
			if( this->camera != NULL )
				checkError( gp_camera_unref( this->camera ) );
				
			#ifdef DEBUG1
			Logger::get() << "[GphotoCamera][" << this->model << "] Released camera on port " << this->port << "." << Logger::endl;
			#endif
		}
		
		void GphotoCamera::listSettings( vector<string>& settings )
		{
			CameraList * list;
			
			if( checkError( gp_list_new( &list ) ) )
			{
				gp_camera_list_config( this->camera, list, GphotoCamera::context );
				
				for( int i = 0 ; i < gp_list_count( list ) ; i++ )
				{
					const char * name;
					
					if( checkError( gp_list_get_name( list, i, &name ) ) )
					{
						settings.push_back( string(name) );
					}
				}
			
				checkError( gp_list_free( list ) );
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[GphotoCamera][" << this->model << "] Unable to initialize settings list." << Logger::endl;
			#endif
		}
		
		bool GphotoCamera::loadSetting( const string& name, bool reload )
		{
			bool bReturn = true;
			
			map<string,CameraWidget *>::iterator it = this->settings.find( name );
			
			if( reload && it != this->settings.end() )
			{
				checkError( gp_widget_free( it->second ) );
				it->second = NULL;
			}
			
			if( it == this->settings.end() || it->second == NULL )
			{
				CameraWidget * widget = NULL;
				
				if( checkError( gp_camera_get_single_config( this->camera, name.c_str(), &widget, GphotoCamera::context ) ) )
				{
					this->settings[name] = widget;
				}
				else
				{
					bReturn = false;
					
					#ifdef DEBUG0
					Logger::get() << "[GphotoCamera][" << this->model << "] Unable to get setting \"" << name << "\" from camera." << Logger::endl;
					#endif
				}
			}
			
			return bReturn;
		}
		
		string GphotoCamera::getSetting( const string& name )
		{
			string value;
			
			if( this->loadSetting( name ) )
			{
				char * v = NULL;
				
				CameraWidgetType type;
				
				if( checkError( gp_widget_get_type( this->settings[name], &type ) ) )
				{
					if( type == GP_WIDGET_MENU || type == GP_WIDGET_RADIO || type == GP_WIDGET_TEXT )
					{
						if( checkError( gp_widget_get_value( this->settings[name], &v ) ) )
						{
							if( v != NULL )
								value = string(v);
						}
					}
					#ifdef DEBUG0
					else
						Logger::get() << "[GphotoCamera][" << this->model << "] Setting \"" << name << "\" has an unhandled type." << Logger::endl; 
					#endif
				}
			}
			
			return value;
		}
		
		void GphotoCamera::getSettingValues( const string& name, vector<string>& values )
		{
			if( this->loadSetting( name ) )
			{
				CameraWidgetType type;
				
				if( checkError( gp_widget_get_type( this->settings[name], &type ) ) )
				{
					if( type == GP_WIDGET_MENU || type == GP_WIDGET_RADIO )
					{
						int choices = gp_widget_count_choices( this->settings[name] );
						
						for( int i = 0 ; i < choices ; i++ )
						{
							const char * v = NULL;
							
							if( checkError( gp_widget_get_choice( this->settings[name], i, &v ) ) )
							{
								if( v != NULL )
									values.push_back( string(v) );
							}
						}
					}
					#ifdef DEBUG0
					else
						Logger::get() << "[GphotoCamera][" << this->model << "] Setting \"" << name << "\" has no available choices." << Logger::endl; 
					#endif
				}
			}
		}
		
		bool GphotoCamera::setSetting( const string& name, const string& value )
		{
			bool bReturn = false;
			
			if( this->loadSetting( name ) )
			{
				CameraWidgetType type;
			
				if( checkError( gp_widget_get_type( this->settings[name], &type ) ) )
				{
					if( type == GP_WIDGET_MENU || type == GP_WIDGET_RADIO || type == GP_WIDGET_TEXT )
					{
						if( checkError( gp_widget_set_value( this->settings[name], value.c_str() ) ) )
						{
							if( checkError( gp_camera_set_single_config( this->camera, name.c_str(), this->settings[name], GphotoCamera::context ) ) )
								bReturn = true;
								
							#ifdef DEBUG0
							else
								Logger::get() << "[GphotoCamera][" << this->model << "] Unable to write setting \"" << name << "\" to the camera." << Logger::endl; 
							#endif
						}
						#ifdef DEBUG0
						else
							Logger::get() << "[GphotoCamera][" << this->model << "] Unable to change setting \"" << name << "\" value." << Logger::endl; 
						#endif
					}
					#ifdef DEBUG0
					else
						Logger::get() << "[GphotoCamera][" << this->model << "] Setting \"" << name << "\" has an unhandled type." << Logger::endl; 
					#endif
				}
			}
			
			return bReturn;
		}
		
		GPContext * GphotoCamera::getContext()
		{
			return GphotoCamera::context;
		}
		
		void GphotoCamera::initializeContext()
		{
			if( GphotoCamera::context != NULL )
				GphotoCamera::destroyContext();
				
			GphotoCamera::context = gp_context_new();
			gp_context_set_error_func( GphotoCamera::context, GphotoCamera::errorCallback, NULL );
			gp_context_set_message_func( GphotoCamera::context, GphotoCamera::informationCallback, NULL );
			
			#ifdef DEBUG1
			Logger::get() << "[GphotoCamera] Initialized context." << Logger::endl;
			#endif
		}
		
		void GphotoCamera::destroyContext()
		{
			if( GphotoCamera::abilities != NULL )
			{
				gp_abilities_list_free( GphotoCamera::abilities );
				GphotoCamera::abilities = NULL;
				
				#ifdef DEBUG1
				Logger::get() << "[GphotoCamera] Cameras abilities list released." << Logger::endl;
				#endif
			}
			
			if( GphotoCamera::drivers != NULL )
			{
				gp_port_info_list_free( GphotoCamera::drivers );
				GphotoCamera::drivers = NULL;
				
				#ifdef DEBUG1
				Logger::get() << "[GphotoCamera] Port drivers list released." << Logger::endl;
				#endif
			}
			
			if( GphotoCamera::context != NULL )
			{
				while( GphotoCamera::openedCameras.begin() != GphotoCamera::openedCameras.end() )
					delete GphotoCamera::openedCameras.begin()->second;
			
				gp_context_unref( GphotoCamera::context );
				GphotoCamera::context = NULL;
				
				#ifdef DEBUG1
				Logger::get() << "[GphotoCamera] Deleted context." << Logger::endl;
				#endif
			}
		}
		
		void GphotoCamera::errorCallback( GPContext * context, const char * message, void * data )
		{
			Logger::get() << "[Gphoto2][Error] " << message << Logger::endl;
		}
		
		void GphotoCamera::informationCallback( GPContext * context, const char * message, void * data )
		{
			Logger::get() << "[Gphoto2][Information] " << message << Logger::endl;
		}
		
		bool GphotoCamera::checkError( int code )
		{
			bool bReturn = true;
			
			if( code != GP_OK )
			{
				bReturn = false;
				string text = "";
			
				switch( code )
				{
					case GP_ERROR: text = "GP_ERROR"; break;
					case GP_ERROR_BAD_PARAMETERS: text = "GP_ERROR_BAD_PARAMETERS"; break;
					case GP_ERROR_CAMERA_BUSY: text = "GP_ERROR_CAMERA_BUSY"; break;
					case GP_ERROR_CAMERA_ERROR: text = "GP_ERROR_CAMERA_ERROR"; break;
					case GP_ERROR_CANCEL: text = "GP_ERROR_CANCEL"; break;
					case GP_ERROR_CORRUPTED_DATA: text = "GP_ERROR_CORRUPTED_DATA"; break;
					case GP_ERROR_DIRECTORY_EXISTS: text = "GP_ERROR_DIRECTORY_EXISTS"; break;
					case GP_ERROR_DIRECTORY_NOT_FOUND: text = "GP_ERROR_DIRECTORY_NOT_FOUND"; break;
					case GP_ERROR_FILE_EXISTS: text = "GP_ERROR_FILE_EXISTS"; break;
					case GP_ERROR_FILE_NOT_FOUND: text = "GP_ERROR_FILE_NOT_FOUND"; break;
					case GP_ERROR_FIXED_LIMIT_EXCEEDED: text = "GP_ERROR_FIXED_LIMIT_EXCEEDED"; break;
					case GP_ERROR_HAL: text = "GP_ERROR_HAL"; break;
					case GP_ERROR_IO: text = "GP_ERROR_IO"; break;
					case GP_ERROR_IO_INIT: text = "GP_ERROR_IO_INIT"; break;
					case GP_ERROR_IO_LOCK: text = "GP_ERROR_IO_LOCK"; break;
					case GP_ERROR_IO_READ: text = "GP_ERROR_IO_READ"; break;
					case GP_ERROR_IO_SERIAL_SPEED: text = "GP_ERROR_IO_SERIAL_SPEED"; break;
					case GP_ERROR_IO_SUPPORTED_SERIAL: text = "GP_ERROR_IO_SUPPORTED_SERIAL"; break;
					case GP_ERROR_IO_SUPPORTED_USB: text = "GP_ERROR_IO_SUPPORTED_USB"; break;
					case GP_ERROR_IO_UPDATE: text = "GP_ERROR_IO_UPDATE"; break;
					case GP_ERROR_IO_USB_CLAIM: text = "GP_ERROR_IO_USB_CLAIM"; break;
					case GP_ERROR_IO_USB_CLEAR_HALT: text = "GP_ERROR_IO_USB_CLEAR_HALT"; break;
					case GP_ERROR_IO_USB_FIND: text = "GP_ERROR_IO_USB_FIND"; break;
					case GP_ERROR_IO_WRITE: text = "GP_ERROR_IO_WRITE"; break;
					case GP_ERROR_LIBRARY: text = "GP_ERROR_LIBRARY"; break;
					case GP_ERROR_MODEL_NOT_FOUND: text = "GP_ERROR_MODEL_NOT_FOUND"; break;
					case GP_ERROR_NO_MEMORY: text = "GP_ERROR_NO_MEMORY"; break;
					case GP_ERROR_NO_SPACE: text = "GP_ERROR_NO_SPACE"; break;
					case GP_ERROR_NOT_SUPPORTED: text = "GP_ERROR_NOT_SUPPORTED"; break;
					case GP_ERROR_OS_FAILURE: text = "GP_ERROR_OS_FAILURE"; break;
					case GP_ERROR_PATH_NOT_ABSOLUTE: text = "GP_ERROR_PATH_NOT_ABSOLUTE"; break;
					case GP_ERROR_TIMEOUT: text = "GP_ERROR_TIMEOUT"; break;
					case GP_ERROR_UNKNOWN_PORT: text = "GP_ERROR_UNKNOWN_PORT"; break;
					default: text = "Unknown error"; break;
				}
				
				Logger::get() << "[Gphoto2][Error#" << code << "] " << text << Logger::endl;
			}
			
			return bReturn;
		}
		
		void GphotoCamera::list( map<string,string>& cameras )
		{
			CameraList * list;
			
			if( checkError( gp_list_new( &list ) ) )
			{
				gp_camera_autodetect( list, GphotoCamera::context );
				
				for( int i = 0 ; i < gp_list_count( list ) ; i++ )
				{
					const char * name;
					const char * value;
			
					if( checkError( gp_list_get_name( list, i, &name ) ) &&
						checkError( gp_list_get_value( list, i, &value ) ) )
					{
						cameras[string(value)] = string(name);
					}
				}
			
				checkError( gp_list_free( list ) );
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[GphotoCamera] Unable to initialize list." << Logger::endl;
			#endif
		}
		
		string GphotoCamera::getCameraModelName( const string& port )
		{
			string model;
			CameraList * list;
			
			if( checkError( gp_list_new( &list ) ) )
			{
				gp_camera_autodetect( list, GphotoCamera::context );
				
				for( int i = 0 ; i < gp_list_count( list ) ; i++ )
				{
					const char * name;
					const char * value;
			
					if( checkError( gp_list_get_name( list, i, &name ) ) &&
						checkError( gp_list_get_value( list, i, &value ) ) )
					{
						if( strncmp( value, port.c_str(), strlen( value ) ) == 0 )
						{
							model = string(name);
							break;
						}
					}
				}
			
				checkError( gp_list_free( list ) );
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[GphotoCamera] Unable to initialize list." << Logger::endl;
			#endif
			
			return model;
		}
		
		GphotoCamera * GphotoCamera::getCamera( const string& port )
		{
			GphotoCamera * camera = NULL;
			map<string,GphotoCamera *>::iterator it = openedCameras.find( port );
			
			if( it != openedCameras.end() )
				camera = it->second;
			else
				camera = new GphotoCamera( port );
			
			return camera;
		}
		
		bool GphotoCamera::preview( const string& filename )
		{
			bool bReturn = false;
			CameraFile * file = NULL;
			
			if( checkError( gp_file_new( &file ) ) )
			{
				if( checkError( gp_camera_capture_preview( this->camera, file, GphotoCamera::context ) ) )
				{
					if( checkError( gp_file_save( file, filename.c_str() ) ) )
						bReturn = true;

					#ifdef DEBUG0
					else
						Logger::get() << "[GphotoCamera][" << this->model << "] Unable to save file as \"" << filename << "\"." << Logger::endl;
					#endif
					
				}
				#ifdef DEBUG0
				else
					Logger::get() << "[GphotoCamera][" << this->model << "] Unable to capture a preview." << Logger::endl;
				#endif
				
				gp_file_unref( file );
				file = NULL;
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[GphotoCamera][" << this->model << "] Unable to initialize new file." << Logger::endl;
			#endif
			
			return bReturn;
		}
		
		bool GphotoCamera::capture( const string& filename, bool deleteFile )
		{
			bool bReturn = false;
			CameraFile * file = NULL;
			CameraFilePath path;
			
			if( checkError( gp_camera_capture( this->camera, GP_CAPTURE_IMAGE, &path, GphotoCamera::context ) ) )
			{
				#ifdef DEBUG0
				Logger::get() << "[GphotoCamera][" << this->model << "] Captured file \"" << path.folder << "/" << path.name << "\" on camera." << Logger::endl;
				#endif
				
				if( checkError( gp_file_new( &file ) ) )
				{
					if( checkError( gp_camera_file_get( this->camera, path.folder, path.name, GP_FILE_TYPE_NORMAL, file, GphotoCamera::context ) ) )
					{
						if( checkError( gp_file_save( file, filename.c_str() ) ) )
						{
							if( deleteFile )
							{
								if( checkError( gp_camera_file_delete( this->camera, path.folder, path.name, GphotoCamera::context ) ) )
									bReturn = true;
								#ifdef DEBUG0
								else
									Logger::get() << "[GphotoCamera][" << this->model << "] Unable to delete file on the camera." << Logger::endl;
								#endif
							}
							else
								bReturn = true;
						}
						#ifdef DEBUG0
						else
							Logger::get() << "[GphotoCamera][" << this->model << "] Unable to save file as \"" << filename << "\"." << Logger::endl;
						#endif
					}
					#ifdef DEBUG0
					else
						Logger::get() << "[GphotoCamera][" << this->model << "] Unable to retrieve file from the camera." << Logger::endl;
					#endif
					
					gp_file_unref( file );
					file = NULL;
				}
				#ifdef DEBUG0
				else
					Logger::get() << "[GphotoCamera][" << this->model << "] Unable to initialize new file." << Logger::endl;
				#endif
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[GphotoCamera][" << this->model << "] Unable to capture image." << Logger::endl;
			#endif
			
			return bReturn;
		}
	}
}

