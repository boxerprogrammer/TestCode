#include "Input.h"
#include<cassert>
#include<DxLib.h>

using namespace std;

namespace {
	const int INPUT_KEYBD = 0;
	const int INPUT_PAD_1 = 1;
	const int INPUT_PAD_2 = 2;
	const int INPUT_PAD_3 = 3;
	const int INPUT_PAD_4 = 4;
}

bool operator<(const PeripheralInfo & lval, const PeripheralInfo & rval) {
	if (lval.padno == rval.padno) {
		return (lval.code < rval.code);
	}
	else if (lval.padno < rval.padno) {
		return true;
	}
	return false;
}

Input::Input()
{
	//とりあえずデフォルトで接続パッド数＋１(キーボード)にしとく
	SetPlayerCount(GetConnectedPadCount() + 1);
}


Input::~Input()
{
}

int 
Input::GetConnectedPadCount()const {
	return DxLib::GetJoypadNum();
}
void 
Input::SetPlayerCount(int pcount) {
	_inputTable.resize(pcount);
	_currentInputState.resize(pcount);
}

void 
Input::AddCommand(unsigned short pno, const char* command, int periNo, unsigned int code) {
	assert(pno < _inputTable.size());
	_inputTable[pno][command] = PeripheralInfo(periNo, code);
	_inputMap.emplace(PeripheralInfo(periNo, code), make_pair(pno,command));
	_currentInputState[pno][command] = false;
}

void
Input::Update() {
	auto padcount=GetConnectedPadCount();

	char keystate[256];
	GetHitKeyStateAll(keystate);

	std::vector<int> padinfo(padcount+1);//+1はキーボード分
	for (int i = 1; i < padinfo.size();++i) {
		padinfo[i] = GetJoypadInputState(i);
	}
	for (auto& p : _currentInputState) {
		for (auto& s : p) {
			s.second = false;
		}
	}
	for (auto& inputinfo : _inputMap) {
			_currentInputState[inputinfo.second.first][inputinfo.second.second] = 
				_currentInputState[inputinfo.second.first][inputinfo.second.second] || 
				(
					inputinfo.first.padno == INPUT_KEYBD ? keystate[inputinfo.first.code] :
					padinfo[inputinfo.first.padno] & inputinfo.first.code
				);
	}
		
	
}

bool 
Input::IsPressed(unsigned short playerNo, const char* command){
	return _currentInputState[playerNo][command];
}


bool operator==(const PeripheralInfo & lval, const PeripheralInfo & rval) {
	return (lval.code == rval.code) && (lval.padno == rval.padno);
}
bool operator!=(const PeripheralInfo & lval, const PeripheralInfo & rval) {
	return  (lval.code != rval.code) || (lval.padno != rval.padno);
}