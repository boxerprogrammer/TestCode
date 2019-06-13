#pragma once

#include<vector>
#include<unordered_map>
#include<map>
#include<string>

///入力情報
struct PeripheralInfo {
	PeripheralInfo() {};
	PeripheralInfo(int pdno, int cd) :padno(pdno), code(cd) {}
	int padno;//生入力機器番号
	int code;//生入力コード
	///unordered_map用ハッシュ関数
	///一意な値を返せばいいだけなんで
	///パッド番号と入力値をor演算してるだけっす
	struct HashFunc {
		size_t operator()(const PeripheralInfo& p)const {
			return p.padno | (p.code << 4);
		}
	};
};
bool operator<(const PeripheralInfo & lval, const PeripheralInfo & rval);
bool operator==(const PeripheralInfo & lval, const PeripheralInfo & rval);
bool operator!=(const PeripheralInfo & lval, const PeripheralInfo & rval);

///入力周りクラス
class Input
{
private:
	std::vector<std::unordered_map<std::string, PeripheralInfo>> _inputTable;//プレイヤー番号と入力対応テーブルのセット
	std::unordered_map < PeripheralInfo, std::pair<unsigned short, std::string>, PeripheralInfo::HashFunc> _inputMap;//入力情報と利用情報のセット
	std::vector<std::unordered_map<std::string, bool>> _currentInputState;//現在の押下情報
public:
	Input();
	~Input();
	///接続中パッド数を返す
	int GetConnectedPadCount()const;

	///プレイヤー数(パッド数とは関係ないよ)を設定
	void SetPlayerCount(int pcount);

	///コマンド設定
	///@param pno プレイヤー番号
	///@param cmd コマンド文字列
	///@param periNo 入力番号
	///@param code 入力コード
	void AddCommand(unsigned short pno, const char* cmd, int periNo, unsigned int code);

	///入力情報更新
	///@remarks 毎フレーム呼び出してください
	void Update();

	///押下チェック
	///@param playerNo プレイヤー番号
	///@param cmd コマンド文字列
	bool IsPressed(unsigned short playerNo,const char* cmd);

	///現在の押下状況を返す
	const std::vector<std::unordered_map<std::string, bool>>& CurrentState()const { return _currentInputState; }
};

