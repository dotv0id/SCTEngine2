#include <SCTCore.h>

namespace SCT
{

namespace Core
{
		// Initilize the core utilities
		SCTReturn InitializeCore()
		{
			SCTTime			*timeMngr		= new SCTTime;
			SCTStatistics	*statMngr		= new SCTStatistics;
			SCTLogManager	*logMngr		= new SCTLogManager;
			SCTFileManager	*fileMngr		= new SCTFileManager;
			SCTStringHelper	*stringHelper	= new SCTStringHelper;
			SCTImageLoader	*imageLoader	= new SCTImageLoader;

			SCTWindows		*windows;
			#ifdef _WIN32
				windows						= new SCTWindows;
			#endif

			// *** Initialize the core utilities***
			// 1. Strng helper functions
			if(stringHelper)
				stringHelper->Initialize();

			// 2. Log Manager
			if(logMngr)
			{
				if(logMngr->Initialize())
				{
					logMngr->showDebugConsole();
					logMngr->PrintDebug("Log manager has been initialized");
				}
			}

			// 3. Time Manager
			if(timeMngr)
			{
				if(timeMngr->Initialize())
					logMngr->PrintDebug("Time manager has been initialized");
				timeMngr->Reset();
			}

			// 4. Statistics
			if(statMngr)
			{
				if(statMngr->Initialize())
					logMngr->PrintDebug("Statistics are now active and counting");
			}

			// 5. File Manager
			if(fileMngr)
			{
				if(fileMngr->Initialize())
					logMngr->PrintDebug("File manager has been initialized");
			}

			// 6. Image Loader
			if(imageLoader)
			{
				if(imageLoader->Initialize())
					logMngr->PrintDebug("Image Loader has been initialized");
			}
	
			// 7. Windows Manager
			if(windows)
			{
				if(windows->Initialize())
					logMngr->PrintDebug("Windows manager has been initialized");
			}

			return OK;
		}

		// Shutdown the core Utilities
		SCTReturn ShutdownCore()
		{
			SCTTime			*timeMngr		= SCTTime::getSingletonPtr();
			SCTStatistics	*statMngr		= SCTStatistics::getSingletonPtr();
			SCTLogManager	*logMngr		= SCTLogManager::getSingletonPtr();
			SCTFileManager	*fileMngr		= SCTFileManager::getSingletonPtr();
			SCTStringHelper	*stringHelper	= SCTStringHelper::getSingletonPtr();
			SCTWindows		*windows		= SCTWindows::getSingletonPtr();
			SCTImageLoader	*imageLoader	= SCTImageLoader::getSingletonPtr();


			// 1. Shutdown Windows Manager
			if(windows)
			{
				windows->Shutdown();
				delete windows;
				windows			= NULL;

				logMngr->PrintDebug("Windows Manager has been successfully shutdown");
			}

			// 2. Shutdown Image Loader
			if(imageLoader)
			{
				imageLoader->Shutdown();
				delete imageLoader;
				imageLoader	= NULL;

				logMngr->PrintDebug("Image Loader has been successfully shutdown");
			}

			// 3. Shutdown File manager
			if(fileMngr)
			{
				fileMngr->Shutdown();
				delete fileMngr;
				fileMngr		= NULL;

				logMngr->PrintDebug("File Manager has been successfully shutdown");
			}

			// 4. Shutdown Statistics
			if(statMngr)
			{
				statMngr->Shutdown();
				delete statMngr;
				statMngr		= NULL;

				logMngr->PrintDebug("Statistics have been successfully shutdown");
			}

			// 5. Shutdown Time manager
			if(timeMngr)
			{
				timeMngr->Shutdown();
				delete timeMngr;
				timeMngr		= NULL;
				
				logMngr->PrintDebug("Time Manager has been successfully shutdown");
			}

			// 6. Shutdowm the Log manager
			if(logMngr)
			{
				logMngr->PrintDebug("Log Manager is shutting down...");

				logMngr->Shutdown();
				delete logMngr;
				logMngr			= NULL;
			}

			// 7. Shutdown String helper
			if(stringHelper)
			{
				stringHelper->Shutdown();
				delete stringHelper;
				stringHelper	= NULL;
			}

			return OK;
		}
}

void SCTPrintD(const char* format, ...)
{
	#if defined(DEBUG) || defined(_DEBUG)
	std::cout<< "$> ";
	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);

	std::cout<< "\n";
	#endif
}

}