#include"Application.h"
#include<DxLib.h>
#include<vector>
#include<algorithm>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	auto& app = Application::GetInstance();

	if (app.Init()) {
		app.Run();
		app.Terminate();
		return 0;
	}
	else {
		return -1;
	}
	
}