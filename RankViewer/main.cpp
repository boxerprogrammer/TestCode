#include<DxLib.h>
#include<list>
#include<array>
#include<string>
#include<sstream>
#include<algorithm>
#include<set>
#include"masterdata.h"


TCHAR clip[4096] = {};

using namespace std;

using RawData_t = list<int>;
using VoteData_t = array<pair<int, int>,1000>;

void ClearNumberOfVotes(VoteData_t& data) {
	for (auto& m : masterdata) {
		data[m.id].first = m.id;
		data[m.id].second = 0;
	}
}


void LoadRawData(wstringstream& ss, RawData_t& rawData,int& invalidVoteCount, int& duplicateVoteCount) {
	rawData.clear();
	wstring str;
	while (std::getline(ss, str)) {
		wstringstream inss;
		auto lst = str.find_last_of(L'\r');
		if (lst != str.npos) {
			str = str.substr(0, lst);
		}
		inss << str;

		set<int> dup;
		while (std::getline(inss, str, L'\t')) {
			if (!std::all_of(str.begin(), str.end(), isdigit))continue;
			if (str == L"")continue;
			auto n = std::stoi(str);
			if (n <= 0 || n >= 1000) {
				++invalidVoteCount;
			}
			else {
				if (dup.count(n) == 0) {
					dup.insert(n);
					rawData.push_back(n);
				}
				else {
					++duplicateVoteCount;
				}
			}
		}
	}
}

void ConvertVotesData3(RawData_t& rawData, VoteData_t& data,int& invalidCount) {
	if (rawData.empty())return;

	auto n=rawData.front();
	rawData.pop_front();
	{
		auto it = find_if(data.begin(), data.end(), [n](const auto& d) {
			return d.first == n;
			});
		if (it == data.end()) {
			++invalidCount;
		}
		else {
			it->second++;
		}
	}

	
	//if (!rawData.empty()) {
	//	n = rawData.front();
	//	rawData.pop_front();
	//	auto it = find_if(data.begin(), data.end(), [n](const auto& d) {
	//		return d.first == n;
	//		});
	//	if (it == data.end()) {
	//		++invalidCount;
	//	}
	//	else {
	//		it->second++;
	//	}
	//}
	//if (!rawData.empty()) {
	//	n = rawData.front();
	//	rawData.pop_front();
	//	auto it = find_if(data.begin(), data.end(), [n](const auto& d) {
	//		return d.first == n;
	//		});
	//	if (it == data.end()) {
	//		++invalidCount;
	//	}
	//	else {
	//		it->second++;
	//	}
	//}
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	VoteData_t data;
	VoteData_t displayData;


	ClearNumberOfVotes(data);

	ChangeWindowMode(true);
	SetWindowText(L"集計くん(ドラマチックモード)");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	char keystate[256] = {};
	char lastkeystate[256] = {};
	
	int invalidVoteCount = 0;
	int validVoteCount = 0;
	int duplicateVoteCount = 0;
	int scroll = 0;
	bool countDownMode = false;
	RawData_t rawdata;
	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);
		if ((keystate[KEY_INPUT_V] && keystate[KEY_INPUT_LCONTROL]) &&
			!(lastkeystate[KEY_INPUT_V] && lastkeystate[KEY_INPUT_LCONTROL])) {
			
			ClearNumberOfVotes(data);

			DxLib::GetClipboardText(clip);
			wstringstream ss;
			ss << clip;

 			LoadRawData(ss, rawdata, invalidVoteCount, duplicateVoteCount);

		}

		ConvertVotesData3(rawdata, data,invalidVoteCount);

		constexpr int view_data_count = 16;

		if (keystate[KEY_INPUT_DOWN] && !lastkeystate[KEY_INPUT_DOWN]) {
			scroll = std::min(scroll + 1, (int)_countof(masterdata) - view_data_count-1);
		}
		if (keystate[KEY_INPUT_UP] && !lastkeystate[KEY_INPUT_UP]) {
			scroll = std::max(scroll - 1, 0);
		}


		std::copy(data.begin(), data.end(), displayData.begin());
		std::sort(displayData.begin(), displayData.end(), [](const auto& lval, const auto& rval) {
			return lval.second > rval.second;
			});

		//if (rawdata.empty()) {
		//	if (keystate[KEY_INPUT_RETURN] && !lastkeystate[KEY_INPUT_RETURN]) {
		//		countDownMode = true;
		//	}
		//}
		int y = 20;
		int datacount = 0;
		int scrollcountdown = scroll;
		int rank = 1;
		for (auto& d : displayData) {
			if (scrollcountdown > 0) {
				--scrollcountdown; 
				rank++;
				continue;
			}
			if (d.first > 0) {
				auto it=find_if(begin(masterdata), end(masterdata), [d](const auto& md) {return md.id == d.first; });

				wstring teamname = L"";
				if (it != end(masterdata)) {
					teamname = it->name;
					DrawFormatString(50, y, 0xffffff, L"%d位 No:%d  得票数:%d  チーム名:「%s」",rank, d.first, d.second, teamname.c_str());
					y += 20;
					++datacount;
					++rank;
				}
			}
			
			if (datacount > view_data_count) {
				break;
			}
			
		}

		y += 20;
		DrawFormatString(50, y, 0xffaaaa, L"無効票:%d票", invalidVoteCount);
		y += 20;
		DrawFormatString(50, y, 0xffaaaa, L"重複票:%d票", duplicateVoteCount);

		ScreenFlip();
		copy(begin(keystate), end(keystate), begin(lastkeystate));
	}
}