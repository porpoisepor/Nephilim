#define ANDROID_PACKAGE_NAME com_nephilim_samplekinesis
#define ANDROID_ACTIVITY_NAME Kinectic
#include <Nephilim/GenericMain.h>
#include "SampleKinesis.h"

Kinectic sample;

void init()
{    
	_engine->init();
	_engine->execute(&sample);
}

void update()
{
	_engine->update();
}

void shutdown()
{
	_engine->shutdown();
}

