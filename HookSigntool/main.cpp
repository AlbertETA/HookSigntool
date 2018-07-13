#pragma comment(lib, "detours.lib")
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <detours.h>

void (WINAPI *pOldGetLocalTime)(
	LPSYSTEMTIME lpSystemTime
) = GetLocalTime;
time_t(WINAPIV *pOldtime)(
	time_t *_Time
) = time;

void WINAPI NewGetLocalTime(
	LPSYSTEMTIME lpSystemTime
)
{
	pOldGetLocalTime(lpSystemTime);
	lpSystemTime->wYear = 2011;
	lpSystemTime->wMonth = 4;
	lpSystemTime->wDay = 1;
}
time_t WINAPIV Newtime(
	time_t *_Time
)
{
	*((__int64 *)_Time) = 1301587200L;
	return *_Time;
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
		DetourAttach(&(PVOID&)pOldtime, Newtime);
		DetourTransactionCommit();
		MessageBoxA(NULL, "Attached", "Hook", NULL);
	}
	return 1;
}

extern "C" __declspec(dllexport) int attach()
{
	return 0;
}