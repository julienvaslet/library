#include <tools/logger/Stdout.h>
#include <tools/time/Time.h>

#include <opengl/Screen.h>
#include <audio/Song.h>
#include <audio/instrument/Sine.h>
#include <audio/instrument/Silence.h>
#include <audio/instrument/Triangle.h>
#include <audio/instrument/Square.h>
#include <audio/Mixer.h>

using namespace tools::logger;
using namespace tools::time;
using namespace audio;
using namespace audio::instrument;

#include <vector>
using namespace std;

#define SAMPLINGFREQ	48000
#define CHANNELS		1
#define BUFFERSIZE		1024

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	// Not needed when Screen is used.
	#ifdef __NO_X_WINDOW__
	bcm_host_init();
	#else
	SDL_Init( SDL_INIT_AUDIO );
	#endif
	
	new Mixer( SAMPLINGFREQ, CHANNELS, BUFFERSIZE );
	
	Sine si( SAMPLINGFREQ, CHANNELS );
	Silence sil( SAMPLINGFREQ, CHANNELS );
	Song * s0 = new Song( 60, SAMPLINGFREQ, CHANNELS );
	s0->mixNote( si, Note::getFrequency( 'C', false, 3 ), Note::Ronde );
	
	Song * s1 = new Song( 60, SAMPLINGFREQ, CHANNELS );
	s1->mixNote( sil, Note::getFrequency( 'C', false, 3 ), Note::Noire );
	s1->mixNote( si, Note::getFrequency( 'C', false, 3 ) * 1.618, Note::Blanche, true );
	
	Song * s2 = new Song( 60, SAMPLINGFREQ, CHANNELS );
	s2->mixNote( sil, Note::getFrequency( 'C', false, 3 ), Note::Blanche );
	s2->mixNote( si, Note::getFrequency( 'C', false, 3 ) * 1.618 * 1.618, Note::Blanche );
	
	Mixer::get()->add( "song0", s0 );
	delete s0;
	Mixer::get()->add( "song1", s1 );
	delete s1;
	Mixer::get()->add( "song2", s2 );
	delete s2;
	
	Mixer::get()->play( "song0" );
	Mixer::get()->play( "song1" );
	Mixer::get()->play( "song2" );
	
	while( Mixer::get()->isPlaying() );
		
	Mixer::destroy();
	Logger::destroy();
	
	return 0;
}

