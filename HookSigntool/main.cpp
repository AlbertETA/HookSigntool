#pragma comment(lib, "detours.lib")
#include <Windows.h>
#include <wintrust.h>
#include <stdlib.h>
#include <detours.h>

using fntWinVerifyTrust = decltype(WinVerifyTrust);
using fntGetLocalTime = decltype(GetLocalTime);

fntWinVerifyTrust *pOldWinVerifyTrust = NULL;
fntGetLocalTime *pOldGetLocalTime = NULL;

LONG WINAPI NewWinVerifyTrust(
	HWND hwnd,
	GUID *pgActionID,
	LPVOID pWVTData
)
{
	return 0;
}

void WINAPI NewGetLocalTime(
	LPSYSTEMTIME lpSystemTime
)
{
	pOldGetLocalTime(lpSystemTime);
	lpSystemTime->wYear = 2011;
	lpSystemTime->wMonth = 4;
	lpSystemTime->wDay = 1;
}


BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD fdwReason,
	_In_ LPVOID lpvReserved
)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		pOldGetLocalTime = (fntGetLocalTime *)GetProcAddress(LoadLibraryW(L"kernel32.dll"), "GetLocalTime");
		pOldWinVerifyTrust = (fntWinVerifyTrust *)GetProcAddress(LoadLibraryW(L"crypt32.dll"), "WinVerifyTrust");

		MessageBoxA(NULL, "Attaching", "Hook", NULL);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)pOldWinVerifyTrust, NewWinVerifyTrust);
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