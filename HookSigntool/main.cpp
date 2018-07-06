#include <Windows.h>

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD fdwReason,
	_In_ LPVOID lpvReserved
)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		MessageBoxA(NULL, "Attaching", "Hook", NULL);
	}
	if (fdwReason == DLL_PROCESS_DETACH)
	{
		MessageBoxA(NULL, "Detaching", "Hook", NULL);
	}
	return 1;
}

extern "C" __declspec(dllexport) int attach()
{
	return 0;
}