#pragma comment(lib, "detours.lib")
#include <Windows.h>

#include <stdlib.h>
#include <detours.h>

//保存函数原型
static int (WINAPI *OldMesssageBoxW)(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCWSTR lpText,
	_In_opt_ LPCWSTR lpCaption,
	_In_ UINT uType) = MessageBoxW;

//改写函数
static int WINAPI NewMessageBoxW(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCWSTR lpText,
	_In_opt_ LPCWSTR lpCaption,
	_In_ UINT uType)
{
	return OldMesssageBoxW(NULL, L"new MessageBox", L"Please", MB_OK);

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
		DetourAttach(&(PVOID&)OldMesssageBoxW, NewMessageBoxW);
		DetourTransactionCommit();
		MessageBoxA(NULL, "Attached", "Hook", NULL);
	}
	return 1;
}

extern "C" __declspec(dllexport) int attach()
{
	return 0;
}