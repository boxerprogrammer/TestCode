#include<DxLib.h>
#include<string>
#include<sstream>
#include<iomanip>
#include<algorithm>

char StrBuf[256];    // �f�[�^�o�b�t�@
IPDATA ip;        // �ڑ��p�h�o�A�h���X�f�[�^
int netHandle;        // �l�b�g���[�N�n���h��
int DataLength;        // ��M�f�[�^�ʕۑ��p�ϐ�


using namespace std;

void InitNetwork() {

}

void UpdateNetwork() {

}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int cmd) {
	DxLib::ChangeWindowMode(true);
	DxLib::SetMainWindowText(L"C++�Q�[����");
	if (DxLib::DxLib_Init() == -1) {
		return -1;
	}
	auto gaugeH=LoadGraph(L"gauge.png");
	SetDrawScreen(DX_SCREEN_BACK);
	size_t frame = 0;
	ip = { 127,0,0,1 };
	netHandle = ConnectNetWork(ip, 9850);
	
	//netHandle =PreparationListenNetWork(9850);

	// �ڑ����Ă��邩�d�r�b�L�[���������܂Ń��[�v
	//netHandle = -1;
	//while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	//{
	//	// �V�����ڑ����������炻�̃l�b�g���[�N�n���h���𓾂�
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