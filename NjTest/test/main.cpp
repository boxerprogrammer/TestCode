#include<Windows.h>
#include"Application.h"

namespace {
	int test_value = 60;
}
int WINAPI WinMain(HINSTANCE , HINSTANCE,LPSTR,int) {

	test_value++;
	char debug_str[64];
	sprintf_s(debug_str,"\nmain_test_value=%d\n", test_value);
	OutputDebugStringA(debug_str);
	auto& app=Application::Instance();
	if (!app.Initialize()) {
		return -1;
	}
	app.Run();
	app.Terminate();
	return 0;
}
