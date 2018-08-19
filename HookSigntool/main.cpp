#pragma comment(lib, "detours.lib")
#include <Windows.h>
#include <wincrypt.h>
#include <stdlib.h>
#include <detours.h>

using fntCertVerifyTimeValidity = decltype(CertVerifyTimeValidity);
using fntGetLocalTime = decltype(GetLocalTime);

fntCertVerifyTimeValidity *pOldCertVerifyTimeValidity = NULL;
fntGetLocalTime *pOldGetLocalTime = NULL;

LONG WINAPI NewCertVerifyTimeValidity(
	LPFILETIME pTimeToVerify,
	PCERT_INFO pCertInfo
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
		pOldCertVerifyTimeValidity = (fntCertVerifyTimeValidity *)GetProcAddress(LoadLibraryW(L"crypt32.dll"), "CertVerifyTimeValidity");
		pOldGetLocalTime = (fntGetLocalTime *)GetProcAddress(LoadLibraryW(L"kernel32.dll"), "GetLocalTime");

		MessageBoxA(NULL, "Attaching", "Hook", NULL);
		DetourTransactionBegin();
		DetourAttach(&(PVOID&)pOldCertVerifyTimeValidity, NewCertVerifyTimeValidity);
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