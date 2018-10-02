#pragma comment(lib, "detours.lib")

#include <Windows.h>
#include <wincrypt.h>
#include <stdlib.h>
#include <detours.h>
#include <direct.h>

using fntCertVerifyTimeValidity = decltype(CertVerifyTimeValidity);
using fntGetLocalTime = decltype(GetLocalTime);

int year = -1;
int month = -1;
int day = -1;

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
	if (year >= 0)
		lpSystemTime->wYear = year;
	if (month >= 0)
		lpSystemTime->wMonth = month;
	if (day >= 0)
		lpSystemTime->wDay = day;
}


BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD fdwReason,
	_In_ LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		//Get config file
		char buf[260];
		char* p = strstr(GetCommandLineA(), "-config ");
		if (p) {
			strcat_s(buf, p);
		}
		else {
			_getcwd(buf, sizeof(buf));
			strcat_s(buf, "\\hook.ini");
		}
		year = GetPrivateProfileInt("Time", "Year", -1, buf);
		month = GetPrivateProfileInt("Time", "Month", -1, buf);
		day = GetPrivateProfileInt("Time", "Day", -1, buf);

		pOldCertVerifyTimeValidity = (fntCertVerifyTimeValidity *)GetProcAddress(LoadLibraryW(L"crypt32.dll"), "CertVerifyTimeValidity");
		pOldGetLocalTime = (fntGetLocalTime *)GetProcAddress(LoadLibraryW(L"kernel32.dll"), "GetLocalTime");

		DetourTransactionBegin();
		DetourAttach(&(PVOID&)pOldCertVerifyTimeValidity, NewCertVerifyTimeValidity);
		DetourAttach(&(PVOID&)pOldGetLocalTime, NewGetLocalTime);
		DetourTransactionCommit();
	}
	return 1;
}

extern "C" __declspec(dllexport) int attach()
{
	return 0;
}