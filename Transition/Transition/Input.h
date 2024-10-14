#pragma once

#include<string>
#include<unordered_map>
#include<vector>

//入力種別
enum class InputType {
	keyboard,//キーボード
	gamepad,//ゲームパッド
	mouse//マウス
};

//登録される入力情報(一つ当たり)
struct InputInfo {
	InputType type;//入力種別
	//実際に調べる値(PADやマウスならビットを表しキーボードならキーコード)
	//を表す
	int inputID;
};

//入力の状態を保持するための構造体
//struct InputState {
//	std::string key;//対象の入力をあらわす文字
//	bool isPressed=false;//その対象が押されてるかどうか
//};

//ゲーム中の意味と入力されたかどうかをペアにしたもの
//とにかく対象の「意味」が押されたら値がtrueになる型
using InputState_t = std::unordered_map<std::string, bool >;

//ゲーム中の"意味"と実際のキーコードのペア
//キー(first)は命令の意味、
//ゲーム中の意味と実際の入力(ハード入力)の対応表を表す「型」
using InputTable_t = std::unordered_map<std::string, std::vector<InputInfo>>;

class KeyconfigScene;

class Input
{
	friend KeyconfigScene;
private:

	std::vector<std::string> excludeOnConfigList_;//キーコンフィグ除外リスト

	//意味とハード入力の対応表
	InputTable_t inputTable_;
	InputTable_t tempTable_;

	//押した記録
	InputState_t currentState_ = {};
	InputState_t lastState_ = {};

	//値の代入とコピーの禁止
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	//現在そのキーが押されてるか
	bool GetCurrentInput(const std::string& input)const;
	//直前にそのキーが押されてるか
	bool GetLastInput(const std::string& input)const;
public:
	Input();
	/// <summary>
	/// 入力情報を更新する(毎フレーム呼ばれる)
	/// </summary>
	void Update();

	/// <summary>
	/// 今押した瞬間かどうかを返す(押しっぱなしはfalse)
	/// </summary>
	/// <param name="input">入力識別文字列("next"や"pause"など)</param>
	/// <returns>押した瞬間ならtrue</returns>
	bool IsTriggered(const std::string& input)const;

	/// <summary>
	/// 今押しているかどうかを返す
	/// </summary>
	/// <param name="input">入力識別文字列("next"や"pause"など)</param>
	/// <returns>押してるならtrue</returns>
	bool IsPressed(const std::string& input)const;

	/// <summary>
	/// 入力テーブルの書き換え
	/// </summary>
	/// <param name="key">命令文字列</param>
	/// <param name="inputInfo">機器からの入力情報(ハード)</param>
	void RewriteInput(const std::string& key,
						const InputInfo& inputInfo);

	void CommitInputConfig();

	/// <summary>
	/// 入力テーブル自体を返す
	/// </summary>
	const InputTable_t GetInputTable()const;

	void SaveInputTable();
	void LoadInputTable();

	/// <summary>
	/// 現在の入力が除外入力にあたるのかどうかをチェックする
	/// </summary>
	/// <returns>true:除外入力にあたる, false:除外する必要はない</returns>
	bool CheckExclusiveInput();

};

