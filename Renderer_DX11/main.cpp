#include "App.h"
#include <Windows.h>

int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
	App app = App();
	app.Run();
	return 0;
}