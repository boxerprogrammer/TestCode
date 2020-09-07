#include<DxLib.h>
#include<string>
#include<sstream>
#include<iomanip>
#include<list>
#include<string>
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
	DxLib::SetMainWindowText(L"C++ログ側");
	if (DxLib::DxLib_Init() == -1) {
		return -1;
	}
	SetAlwaysRunFlag(true);
	SetDrawScreen(DX_SCREEN_BACK);
	size_t frame = 0;
	ip = { 127,0,0,1 };
	//netHandle = ConnectNetWork(ip, 9850);

	netHandle =PreparationListenNetWork(9850);

	// 接続してくるかＥＳＣキーが押されるまでループ
	netHandle = -1;
	while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		// 新しい接続があったらそのネットワークハンドルを得る
		netHandle = GetNewAcceptNetWork();
		if (netHandle != -1) break;
		DrawString(50, 50, L"Wait Connecting", 0xffffff);
		ScreenFlip();
	}
	list<wstring> listlog;
	while (DxLib::ProcessMessage() != -1) {
		auto fps = DxLib::GetFPS();
		DrawBox(0, 0, 640, 480, 0xffffff, true);
		fill_n( StrBuf,256,0);
		NetWorkRecv(netHandle, StrBuf, 256);
		wstring wstr;
		wstr.resize(256);
		std::copy(begin(StrBuf), end(StrBuf), wstr.data());
		if (wstr!=L"") {
			listlog.push_back(wstr);
			if (listlog.size() > 25) {
				listlog.pop_front();
			}
		}
		int y = 25;
		for (auto& str : listlog) {
			DrawFormatString(10, y, 0x000000, L"%s", str.c_str());
			y += 16;
		}
		ScreenFlip();
	}
	return 0;
}