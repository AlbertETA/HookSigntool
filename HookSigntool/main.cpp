#pragma comment(lib, "detours.lib")
#include <Windows.h>

#include <stdlib.h>
#include <detours.h>

void (WINAPI *pOldGetLocalTime)(
	LPSYSTEMTIME lpSystemTime
	) = GetLocalTime;

void WINAPI NewGetLocalTime(
	LPSYSTEMTIME lpSystemTime
)
{
	pOldGetLocalTime(lpSystemTime);
	lpSystemTime->wYear = 2011;
}

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD fdwReason,
	_In_ LPVOID lpvReserved
)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		MessageBoxA(NULL, "Attaching", "Hook", NULL);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)pOldGetLocalTime, NewGetLocalTime);
		DetourTransactionCommit();
		MessageBoxA(NULL, "Attached", "Hook", NULL);
	}
	return 1;
}

extern "C" __declspec(dllexport) int attach()
{
	return 0;
}