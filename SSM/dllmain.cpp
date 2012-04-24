// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "SSM.h"



BOOL WINAPI DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	printf("SSM:hModule.%p lpReserved.%p \n", hModule, lpReserved);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		printf("SSM:DLL_PROCESS_ATTACH\n");
		break;
	case DLL_THREAD_ATTACH:
		printf("SSM:DLL_THREAD_ATTACH\n");
		break;
	case DLL_THREAD_DETACH:
		printf("SSM:DLL_THREAD_DETACH\n");
		break;
	case DLL_PROCESS_DETACH:
		for(std::vector<CPair*>::size_type i=0; i<ssm_ppairvector.size();i++)
		{
			delete ssm_ppairvector[i];
		}

		for(std::vector<CContract*>::size_type i=0; i<ssm_pcontractvector.size();i++)
		{
			delete ssm_pcontractvector[i];
		}

		for(std::vector<CCommodity*>::size_type i=0; i<ssm_pcommodityvector.size();i++)
		{
			delete ssm_pcommodityvector[i];
		}
		delete ssm_puser;
		printf("SSM:DLL_PROCESS_DETACH\n");
		break;
	}
	return TRUE;
}

