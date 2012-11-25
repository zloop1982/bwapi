#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>

#include <BWAPI.h>

#include "DevAIModule.h"

BOOL APIENTRY DllMain( HANDLE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    BWAPI::BWAPI_init();
    break;
  }
  return TRUE;
}

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
  return new DevAIModule();
}
