#include <windows.h>

int main() {
	MessageBoxW(NULL, L"First MessageBox", L"Test", NULL);

	GetProcAddress(LoadLibraryW(L"HookSigntool.dll"), "attach");

	MessageBoxW(NULL, L"Test", L"Test", NULL);
}