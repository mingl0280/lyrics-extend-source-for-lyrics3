// foo_lyrics3exsrc.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "component_defines.h"




namespace
{
	DECLARE_COMPONENT_VERSION( COMPONENT_NAME, VERSION_NUMBER, ABOUT )
	VALIDATE_COMPONENT_FILENAME( COMPONENT_DLL_NAME )
}
















BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


