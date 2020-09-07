#include<DxLib.h>
#include<string>
#include<sstream>
#include<iomanip>
#include<algorithm>

char StrBuf[256];    // データバッファ
IPDATA ip;        // 接続用ＩＰアドレスデータ
int netHandle;        // ネットワークハンドル
int DataLength;        // 受信データ量保存用変数


using namespace std;

void InitNetwork() {

}

void UpdateNetwork() {

}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int cmd) {
	DxLib::ChangeWindowMode(true);
	DxLib::SetMainWindowText(L"C++ゲーム側");
	if (DxLib::DxLib_Init() == -1) {
		return -1;
	}
	auto gaugeH=LoadGraph(L"gauge.png");
	SetDrawScreen(DX_SCREEN_BACK);
	size_t frame = 0;
	ip = { 127,0,0,1 };
	netHandle = ConnectNetWork(ip, 9850);
	
	//netHandle =PreparationListenNetWork(9850);

	// 接続してくるかＥＳＣキーが押されるまでループ
	//netHandle = -1;
	//while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	//{
	//	// 新しい接続があったらそのネットワークハンドルを得る
	//	netHandle = GetNewAcceptNetWork();
	//	if (netHandle != -1) break;
	//}

	while (DxLib::ProcessMessage() !=- 1) {
		auto fps=DxLib::GetFPS();
		ClearDrawScreen();
		DxLib::DrawCircleGauge(320, 240,static_cast<float>(frame) , gaugeH, 0.0f);
		DxLib::DrawFormatString(10, 10, 0xffffff, L"fps=%2.2f", fps);
		ScreenFlip();
		frame=(frame+1)%100;
		wstringstream ss;
		ss << L"fps="<< setprecision(4) << fps;
		wstring wstr = ss.str();
		copy(begin(wstr), end(wstr), StrBuf);
		NetWorkSend(netHandle,StrBuf,256);
	}
	return 0;
}