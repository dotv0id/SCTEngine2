/*
 *	SCT GAME ENGINE
 *	[More like a Direct3D 10 Framework at the moment]
 *	by Anastasios "v0id" Giannakopoulos
 */

 // git 
 

#include "SCTApp.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	SCT::SCTApp *app	= new SCT::SCTApp;

	// Temp config structure
	SCT::SCTConfigRender config;
	config.Fullscreen				= false;
	config.Vsync					= false;
	config.Resolution.width			= 800;
	config.Resolution.height		= 600;
	config.Resolution.refreshRate	= 60;
	
	// Configure the appliction
	app->Configure(config, hInstance);

	if(app->Initialize())
	{
		app->Run();
	}
	else
	{
		// Error initializing the engine
	}

	// Shutdown up the engine
	SCT::SCTReturn ret	= app->Shutdown();

	delete app;
	app	= NULL;

	#if defined(DEBUG) || defined(_DEBUG)
		system("PAUSE");
	#endif

	return (int)ret;
}

