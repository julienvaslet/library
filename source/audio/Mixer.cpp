#include <audio/Mixer.h>
#include <climits>

#ifdef DEBUG0
#include <cstring>
#include <errno.h>
extern int errno;

#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif


namespace audio
{
	Mixer * Mixer::instance = NULL;
	
	#ifdef __NO_X_WINDOW__
	
	/*
	 * RaspberryPi BCM implementation
	 */
	
	Mixer::Mixer( unsigned int samplingFrequency, unsigned short int channels, unsigned int samples ) : thread(NULL), audioRender(NULL), userBufferList(NULL), numBuffers(0), bytesPerSample(0), samplingFrequency(samplingFrequency), channels(channels), samples(samples)
	{
		Mixer::destroy();
		
		#ifdef DEBUG0
		Logger::get() << "[Mixer] Initializing (" << static_cast<int>( this->channels ) << " channels, " << this->samplingFrequency << " Hz, " << this->samples << " samples)." << Logger::endl;
		#endif
		
		// Sanity checks
		if( this->samplingFrequency < 8000 || this->samplingFrequency > 192000 ) this->samplingFrequency = 48000;
		
		if( this->channels < 1 || this->channels > 8 )
			this->channels = 2;
		else
			this->channels = this->channels > 4 ? 8 : (this->channels > 2 ? 4 : this->channels);
			
		if( this->samples < 1 ) this->samples = 1024;
		
		unsigned int bitDepth = 32; // Could be 16 or 32
		this->numBuffers = 10;
		this->bytesPerSample = (bitDepth * this->channels) >> 3;
		
		this->list[0] = NULL;
		this->list[1] = NULL;
		
		if( sem_init( &(this->semaphore), 0, 1 ) == 0 )
		{
			this->client = ilclient_init();
			
			if( this->client != NULL )
			{
				OMX_ERRORTYPE error;
				
				// Register callback for errors
				ilclient_set_error_callback( this->client, Mixer::errorCallback, this );
				
				error = OMX_Init();
				
				if( error == OMX_ErrorNone )
				{
					char sAudioRender[] = "audio_render";
					ilclient_create_component( this->client, &(this->audioRender), sAudioRender, static_cast<ILCLIENT_CREATE_FLAGS_T>( ILCLIENT_ENABLE_INPUT_BUFFERS | ILCLIENT_DISABLE_ALL_PORTS ) );

					if( this->audioRender != NULL )
					{
						this->list[0] = this->audioRender;
						
						// Set up number/size of buffers
						OMX_PARAM_PORTDEFINITIONTYPE param;
						memset( &param, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE) );
						param.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
						param.nVersion.nVersion = OMX_VERSION;
						param.nPortIndex = 100;
						
						if( OMX_GetParameter( ILC_GET_HANDLE(this->audioRender), OMX_IndexParamPortDefinition, &param ) == OMX_ErrorNone )
						{
							unsigned int bufferSize = (this->samples * bitDepth * this->channels) >> 3;
							
							// Buffer length must be 16-byte aligned for VCHI
							int size = (bufferSize + 15) & ~15;
							
							param.nBufferSize = size;
							param.nBufferCountActual = this->numBuffers;
							
							if( OMX_SetParameter( ILC_GET_HANDLE(this->audioRender), OMX_IndexParamPortDefinition, &param ) == OMX_ErrorNone )
							{
								// Set up PCM parameters
								OMX_AUDIO_PARAM_PCMMODETYPE pcm;
								memset( &pcm, 0, sizeof(OMX_AUDIO_PARAM_PCMMODETYPE) );
								pcm.nSize = sizeof(OMX_AUDIO_PARAM_PCMMODETYPE);
								pcm.nVersion.nVersion = OMX_VERSION;
								pcm.nPortIndex = 100;
								pcm.nChannels = this->channels;
								pcm.eNumData = OMX_NumericalDataSigned;
								pcm.eEndian = OMX_EndianLittle;
								pcm.nSamplingRate = this->samplingFrequency;
								pcm.bInterleaved = OMX_TRUE;
								pcm.nBitPerSample = bitDepth;
								pcm.ePCMMode = OMX_AUDIO_PCMModeLinear;
								
								switch( this->channels )
								{
									case 1:
										pcm.eChannelMapping[0] = OMX_AUDIO_ChannelCF;
										break;
										
									case 3:
										pcm.eChannelMapping[2] = OMX_AUDIO_ChannelCF;
										pcm.eChannelMapping[1] = OMX_AUDIO_ChannelRF;
										pcm.eChannelMapping[0] = OMX_AUDIO_ChannelLF;
										break;
										
									case 8:
										pcm.eChannelMapping[7] = OMX_AUDIO_ChannelRS;
										
									case 7:
										pcm.eChannelMapping[6] = OMX_AUDIO_ChannelLS;
										
									case 6:
										pcm.eChannelMapping[5] = OMX_AUDIO_ChannelRR;
										
									case 5:
										pcm.eChannelMapping[4] = OMX_AUDIO_ChannelLR;
										
									case 4:
										pcm.eChannelMapping[3] = OMX_AUDIO_ChannelLFE;
										pcm.eChannelMapping[2] = OMX_AUDIO_ChannelCF;
										
									case 2:
										pcm.eChannelMapping[1] = OMX_AUDIO_ChannelRF;
										pcm.eChannelMapping[0] = OMX_AUDIO_ChannelLF;
										break;
								}
								
								if( OMX_SetParameter( ILC_GET_HANDLE(this->audioRender), OMX_IndexParamAudioPcm, &pcm ) == OMX_ErrorNone )
								{
									ilclient_change_component_state( this->audioRender, OMX_StateIdle );
									
									if( ilclient_enable_port_buffers( this->audioRender, 100, NULL, NULL, NULL ) == 0 )
									{
										// Initialization successful, starting MixerThread
										ilclient_change_component_state( this->audioRender, OMX_StateExecuting );			
										Mixer::instance = this;
		
										#ifdef DEBUG0
										Logger::get() << "[Mixer] Initialized (" << static_cast<int>( this->channels ) << " channels, " << this->samplingFrequency << " Hz, " << this->samples << " samples)." << Logger::endl;
										#endif
										
										// Set audio destination to local (jack)
										this->setDestination( 0 );
										
										this->thread = new MixerThread( this );
										this->thread->start();
									}
									else
									{
										ilclient_change_component_state( this->audioRender, OMX_StateLoaded );
										
										#ifdef DEBUG0
										Logger::get() << "[Mixer] Unable to enable port buffers." << Logger::endl;
										#endif
									}
								}
								#ifdef DEBUG0
								else
									Logger::get() << "[Mixer] Unable to set PCM parameters." << Logger::endl;
								#endif
							}
							#ifdef DEBUG0
							else
								Logger::get() << "[Mixer] Unable to set OMX port definition." << Logger::endl;
							#endif
						}
						#ifdef DEBUG0
						else
							Logger::get() << "[Mixer] Unable to get OMX port definition." << Logger::endl;
						#endif
					}
					#ifdef DEBUG0
					else
						Logger::get() << "[Mixer] Unable to create \"" << sAudioRender << "\" component." << Logger::endl;
					#endif
				}
				#ifdef DEBUG0
				else
					Logger::get() << "[Mixer] Unable to initialize OMX." << Logger::endl;
				#endif
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[Mixer] Unable to initialize IL Client." << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "[Mixer] Unable to initialize semaphore: " << strerror(errno) << Logger::endl;
		#endif
	}
	
	Mixer::~Mixer()
	{
		if( this->audioRender != NULL )
		{
			ilclient_change_component_state( this->audioRender, OMX_StateIdle );
		
			if( this->thread != NULL )
			{
				this->thread->stop();
				this->thread->join();
				
				delete this->thread;
			}
			
			OMX_SendCommand( ILC_GET_HANDLE(this->audioRender), OMX_CommandStateSet, OMX_StateLoaded, NULL );
			
			ilclient_disable_port_buffers( this->audioRender, 100, this->userBufferList, NULL, NULL );
			ilclient_change_component_state( this->audioRender, OMX_StateLoaded );
			ilclient_cleanup_components( this->list );
		}
		
		OMX_Deinit();

		if( this->client != NULL )	
			ilclient_destroy( this->client );
	
		sem_destroy( &this->semaphore );

		this->clearSounds();
	
		#ifdef DEBUG0
		Logger::get() << "[Mixer] Destroyed." << Logger::endl;
		#endif
	
	}
	
	string Mixer::errorToString( OMX_ERRORTYPE error )
	{
		string errorStr;
		
		switch( error )
		{
			case OMX_ErrorInsufficientResources: errorStr = "OMX_ErrorInsufficientResources"; break;
			case OMX_ErrorUndefined: errorStr = "OMX_ErrorUndefined"; break;
			case OMX_ErrorInvalidComponentName: errorStr = "OMX_ErrorInvalidComponentName"; break;
			case OMX_ErrorComponentNotFound: errorStr = "OMX_ErrorComponentNotFound"; break;
			case OMX_ErrorInvalidComponent: errorStr = "OMX_ErrorInvalidComponent"; break;
			case OMX_ErrorBadParameter: errorStr = "OMX_ErrorBadParameter"; break;
			case OMX_ErrorNotImplemented: errorStr = "OMX_ErrorNotImplemented"; break;
			case OMX_ErrorUnderflow: errorStr = "OMX_ErrorUnderflow"; break;
			case OMX_ErrorOverflow: errorStr = "OMX_ErrorOverflow"; break;
			case OMX_ErrorHardware: errorStr = "OMX_ErrorHardware"; break;
			case OMX_ErrorInvalidState: errorStr = "OMX_ErrorInvalidState"; break;
			case OMX_ErrorStreamCorrupt: errorStr = "OMX_ErrorStreamCorrupt"; break;
			case OMX_ErrorPortsNotCompatible: errorStr = "OMX_ErrorPortsNotCompatible"; break;
			case OMX_ErrorResourcesLost: errorStr = "OMX_ErrorResourcesLost"; break;
			case OMX_ErrorNoMore: errorStr = "OMX_ErrorNoMore"; break;
			case OMX_ErrorVersionMismatch: errorStr = "OMX_ErrorVersionMismatch"; break;
			case OMX_ErrorNotReady: errorStr = "OMX_ErrorNotReady"; break;
			case OMX_ErrorTimeout: errorStr = "OMX_ErrorTimeout"; break;
			case OMX_ErrorSameState: errorStr = "OMX_ErrorSameState"; break;
			case OMX_ErrorResourcesPreempted: errorStr = "OMX_ErrorResourcesPreempted"; break;
			case OMX_ErrorPortUnresponsiveDuringAllocation: errorStr = "OMX_ErrorPortUnresponsiveDuringAllocation"; break;
			case OMX_ErrorPortUnresponsiveDuringDeallocation: errorStr = "OMX_ErrorPortUnresponsiveDuringDeallocation"; break;
			case OMX_ErrorPortUnresponsiveDuringStop: errorStr = "OMX_ErrorPortUnresponsiveDuringStop"; break;
			case OMX_ErrorIncorrectStateTransition: errorStr = "OMX_ErrorIncorrectStateTransition"; break;
			case OMX_ErrorIncorrectStateOperation: errorStr = "OMX_ErrorIncorrectStateOperation"; break;
			case OMX_ErrorUnsupportedSetting: errorStr = "OMX_ErrorUnsupportedSetting"; break;
			case OMX_ErrorUnsupportedIndex: errorStr = "OMX_ErrorUnsupportedIndex"; break;
			case OMX_ErrorBadPortIndex: errorStr = "OMX_ErrorBadPortIndex"; break;
			case OMX_ErrorPortUnpopulated: errorStr = "OMX_ErrorPortUnpopulated"; break;
			case OMX_ErrorComponentSuspended: errorStr = "OMX_ErrorComponentSuspended"; break;
			case OMX_ErrorDynamicResourcesUnavailable: errorStr = "OMX_ErrorDynamicResourcesUnavailable"; break;
			case OMX_ErrorMbErrorsInFrame: errorStr = "OMX_ErrorMbErrorsInFrame"; break;
			case OMX_ErrorFormatNotDetected: errorStr = "OMX_ErrorFormatNotDetected"; break;
			case OMX_ErrorContentPipeOpenFailed: errorStr = "OMX_ErrorContentPipeOpenFailed"; break;
			case OMX_ErrorContentPipeCreationFailed: errorStr = "OMX_ErrorContentPipeCreationFailed"; break;
			case OMX_ErrorSeperateTablesUsed: errorStr = "OMX_ErrorSeperateTablesUsed"; break;
			case OMX_ErrorTunnelingUnsupported: errorStr = "OMX_ErrorTunnelingUnsupported"; break;
			
			default:
				stringstream ss;
				ss << "Unknown error (" << static_cast<unsigned int>( error ) << ")";
				errorStr = ss.str();
				break;
		}
		
		return errorStr;
	}
	
	void Mixer::errorCallback( void * userdata, COMPONENT_T * comp, OMX_U32 data )
	{
		Logger::get() << "[Mixer][Error] " << Mixer::errorToString( (OMX_ERRORTYPE) data ) << "." << Logger::endl;
	}
	
	bool Mixer::setDestination( unsigned int destination )
	{
		bool success = false;
		const char * destinations[] = { "local", "hdmi" };
		
		if( destination > 1 ) destination = 0;
		
		OMX_CONFIG_BRCMAUDIODESTINATIONTYPE ar_dest;
		memset( &ar_dest, 0, sizeof(ar_dest) );
		ar_dest.nSize = sizeof(OMX_CONFIG_BRCMAUDIODESTINATIONTYPE);
		ar_dest.nVersion.nVersion = OMX_VERSION;
		strcpy( (char *) ar_dest.sName, destinations[destination] );
		
		if( OMX_SetConfig( ILC_GET_HANDLE(this->audioRender), OMX_IndexConfigBrcmAudioDestination, &ar_dest ) == OMX_ErrorNone )
		{
			success = true;
			
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Audio destination set to \"" << destinations[destination] << "\"." << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "[Mixer] Unable to set destination to \"" << destinations[destination] << "\"." << Logger::endl;
		#endif
		
		return success;
	}
	
	unsigned int Mixer::getLatency()
	{
		OMX_PARAM_U32TYPE param;

		memset( &param, 0, sizeof(OMX_PARAM_U32TYPE) );
		param.nSize = sizeof(OMX_PARAM_U32TYPE);
		param.nVersion.nVersion = OMX_VERSION;
		param.nPortIndex = 100;

		if( OMX_GetConfig( ILC_GET_HANDLE(this->audioRender), OMX_IndexConfigAudioRenderingLatency, &param ) == OMX_ErrorNone )
			return param.nU32;
		else
			return 0;
	}
	
	void Mixer::lockAudio()
	{
		sem_wait( &this->semaphore );
	}
	
	void Mixer::unlockAudio()
	{
		sem_post( &this->semaphore );
	}
	
	#else
	
	/*
	 * SDL Mixer implementation
	 */
	
	Mixer::Mixer( unsigned int samplingFrequency, unsigned short int channels, unsigned int samples ) : device(0), format(0), samplingFrequency(samplingFrequency), channels(channels), samples(samples)
	{
		Mixer::destroy();
		
		#ifdef DEBUG0
		Logger::get() << "[Mixer] Initializing (" << static_cast<int>( this->channels ) << " channels, " << this->samplingFrequency << " Hz, " << this->samples << " samples)." << Logger::endl;
		#endif
		
		SDL_AudioSpec desiredFormat;
		SDL_AudioSpec obtainedFormat;
	
		desiredFormat.format = AUDIO_S32SYS;
		desiredFormat.freq = this->samplingFrequency;
		desiredFormat.channels = this->channels;
		desiredFormat.samples = this->samples;
		desiredFormat.callback = Mixer::callback;
		desiredFormat.userdata = this;
		
		this->device = SDL_OpenAudioDevice( NULL, 0, &desiredFormat, &obtainedFormat, 0 );

		if( this->device > 0 )
		{
			this->samplingFrequency = obtainedFormat.freq;
			this->channels = obtainedFormat.channels;
			this->samples = obtainedFormat.samples;
			this->format = obtainedFormat.format;
			
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Initialized (" << static_cast<int>( this->channels ) << " channels, " << this->samplingFrequency << " Hz, " << this->samples << " samples per channel)." << Logger::endl;
			#endif
			
			Mixer::instance = this;
			SDL_PauseAudioDevice( this->device, 0 );
		}
		#ifdef DEBUG0
		else
		{
			const char * error = SDL_GetError();
		    Logger::get() << "[Mixer] Unable to open audio device: " << error << Logger::endl;
		    Logger::get() << "[Mixer] This mixer should be destroyed." << Logger::endl;
		}
	    #endif
	}
	
	Mixer::~Mixer()
	{
		if( this->device > 0 )
			SDL_PauseAudioDevice( this->device, 1 );
		
		this->clearSounds();

		if( this->device > 0 )
		{		
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Closing..." << Logger::endl;
			#endif
		
			SDL_CloseAudioDevice( this->device );
		
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Destroyed." << Logger::endl;
			#endif
		}
	}
	
	void Mixer::callback( void * userdata, Uint8 * stream, int len )
	{
		Mixer * mixer = static_cast<Mixer *>( userdata );
		unsigned int realLength = Mixer::getRealSamples( len, mixer->getAudioFormat() );

		if( mixer != NULL )
		{
			Sound * sound = mixer->fillBuffer( realLength );
			vector<int> waves = sound->getRawData();
			delete sound;
			
			Mixer::convertStream( waves, stream, mixer->getAudioFormat() );
		}
	}
	
	unsigned int Mixer::getRealSamples( unsigned int samples, SDL_AudioFormat format )
	{
		// Base audio format size is 8 for Uint8
		return 8.0f / static_cast<float>( SDL_AUDIO_BITSIZE( format ) ) * samples;
	}
		
	void Mixer::convertStream( const vector<int>& inStream, Uint8 * outStream, SDL_AudioFormat format )
	{
		// Convert format (format is ignored)
		// See https://wiki.libsdl.org/SDL_AudioFormat
		Uint32 * stream = (Uint32 *) outStream;
		
		for( unsigned int i = 0 ; i < inStream.size() ; i++ )
			stream[i] = inStream[i];
	}
	
	SDL_AudioFormat Mixer::getAudioFormat() const
	{
		return this->format;
	}
	
	void Mixer::lockAudio()
	{
		if( this->device > 0 )
			SDL_LockAudioDevice( this->device );
	}
	
	void Mixer::unlockAudio()
	{
		if( this->device > 0 )
			SDL_UnlockAudioDevice( this->device );
	}
	
	#endif
	
	/*
	 * Common functions
	 */
	 
	void Mixer::clearSounds()
	{
		this->lockAudio();
		
		for( map<string, PlayingSound *>::iterator it = this->sounds.begin() ; it != this->sounds.end() ; it++ )
		{
			delete it->second;
			
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Deleted sound \"" << it->first << "\"." << Logger::endl;
			#endif
		}
		
		this->sounds.clear();
		
		this->unlockAudio();
	}

	unsigned int Mixer::getSamplingFrequency() const
	{
		return this->samplingFrequency;
	}
	
	unsigned short int Mixer::getChannels() const
	{
		return this->channels;
	}
	
	Sound * Mixer::fillBuffer( unsigned int bufferLength )
	{
		bool firstSound = true;
		
		Sound * sound = new Sound( this->samplingFrequency, this->channels );
		sound->fill( 0, bufferLength );
	
		// Mixing sounds
		for( map<string, PlayingSound *>::iterator it = this->sounds.begin() ; it != this->sounds.end() ; it++ )
		{
			if( it->second->isPlaying() )
			{
				unsigned int position = it->second->getPosition();
				unsigned int tocopy = ( position > it->second->getSound()->getRawLength() - bufferLength ) ? it->second->getSound()->getRawLength() - position : bufferLength;
		
				if( firstSound )
				{
					sound->rawMix( it->second->getSound(), 0, 2.0f, position, position + tocopy );
					firstSound = false;
				}
				else
					sound->rawMix( it->second->getSound(), 0, 1.0f, position, position + tocopy );
			
				it->second->setPosition( tocopy, true );
			}
		}
		
		return sound;
	}

	void Mixer::add( const string& name, Sound * sound, bool oneTimePlaying )
	{
		this->lockAudio();
	
		map<string, PlayingSound *>::iterator it = this->sounds.find( name );
	
		if( it != this->sounds.end() )
			delete it->second;
			
		PlayingSound * playingSound = new PlayingSound( sound, this->samplingFrequency, this->channels );
		playingSound->setOneTimePlaying( oneTimePlaying );
	
		this->sounds[name] = playingSound;
	
		#ifdef DEBUG0
		Logger::get() << "[Mixer] Added sound \"" << name << "\" (" << (sound->getDuration() / 1000.0f ) << "s)." << Logger::endl;
		#endif
		
		this->unlockAudio();
	}
	
	void Mixer::play( const string& name, unsigned int ticks )
	{
		this->lockAudio();
	
		map<string, PlayingSound *>::iterator it = this->sounds.find( name );
	
		if( it != this->sounds.end() )
		{
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Playing sound \"" << name << "\"." << Logger::endl;
			#endif
		
			it->second->play( ticks );
		}
		#ifdef DEBUG0
		else
		{
			Logger::get() << "[Mixer] Can not find sound \"" << name << "\" ; sound will not be played." << Logger::endl;
		}
		#endif
	
		this->unlockAudio();
	}
	
	void Mixer::stop( const string& name )
	{
		this->lockAudio();
	
		map<string, PlayingSound *>::iterator it = this->sounds.find( name );
	
		if( it != this->sounds.end() )
		{
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Stopping sound \"" << name << "\"." << Logger::endl;
			#endif
		
			it->second->stop();
		}
		#ifdef DEBUG0
		else
		{
			Logger::get() << "[Mixer] Can not find sound \"" << name << "\" ; sound will not be stopped." << Logger::endl;
		}
		#endif
	
		this->unlockAudio();
	}
	
	void Mixer::setRepeat( const string& name, bool repeat, unsigned int times )
	{
		this->lockAudio();
	
		map<string, PlayingSound *>::iterator it = this->sounds.find( name );
	
		if( it != this->sounds.end() )
		{
			it->second->setRepeat( repeat, times );
			
			#ifdef DEBUG0
			if( repeat )
			{
				if( times > 0 )
					Logger::get() << "[Mixer] Enabled repeat for sound \"" << name << "\" for " << times << " times." << Logger::endl;
				else
					Logger::get() << "[Mixer] Enabled unlimited repeat for sound \"" << name << "\"." << Logger::endl;
			}
			else
				Logger::get() << "[Mixer] Repeat disabled for sound \"" << name << "\"." << Logger::endl;
			#endif
		}
		
		this->unlockAudio();
	}
	
	void Mixer::setPitch( const string& name, double pitch )
	{
		this->lockAudio();
	
		map<string, PlayingSound *>::iterator it = this->sounds.find( name );
	
		if( it != this->sounds.end() )
		{
			it->second->setPitch( pitch );
			
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Pitch set to " << pitch << " for sound \"" << name << "\"." << Logger::endl;
			#endif
		}
		
		this->unlockAudio();
	}
	
	bool Mixer::isPlaying()
	{
		bool playing = false;
		
		this->lockAudio();
	
		for( map<string, PlayingSound *>::iterator it = this->sounds.begin() ; it != this->sounds.end() ; it++ )
		{
			if( it->second->isPlaying() )
			{
				playing = true;
				break;
			}
		}
		
		#ifdef __NO_X_WINDOW__
		if( !playing && this->getLatency() > 0 )
			playing = true;
		#endif
	
		this->unlockAudio();

		return playing;
	}
	
	void Mixer::clean()
	{
		this->lockAudio();
	
		vector<string> soundsToErase;
	
		for( map<string, PlayingSound *>::iterator it = this->sounds.begin() ; it != this->sounds.end() ; it++ )
		{
			if( it->second->hasPlayedOneTime() )
				soundsToErase.push_back( it->first );
		}
		
		for( vector<string>::iterator it = soundsToErase.begin() ; it != soundsToErase.end(); it++ )
		{
			delete (this->sounds[*it]);
			this->sounds.erase( *it );
		}
	
		this->unlockAudio();
	}
	
	Mixer * Mixer::get()
	{
		return Mixer::instance;
	}
	
	void Mixer::destroy()
	{
		if( Mixer::instance != NULL )
		{
			delete Mixer::instance;
			Mixer::instance = NULL;
		}
	}
}

