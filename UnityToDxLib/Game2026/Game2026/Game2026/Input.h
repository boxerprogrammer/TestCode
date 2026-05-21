#pragma once
#include<array>
#include<vector>
#include<string>
#include<unordered_map>

enum class PeripheralType {
	keybd,//キーボード
	pad,//ジョイパッド
	mouse//マウス
};

//入力ステート
struct InputState {
	PeripheralType type;//周辺機器種別
	unsigned int id;//実入力の値
};

//プロトタイプ宣言
class KeyconfigScene;
/// <summary>
/// 入力をコントロールするクラス
/// </summary>
class Input
{
	friend KeyconfigScene;//キーコンフィグのみテーブルを変更できる
private:
	using InputTable_t = std::unordered_map<std::string, std::vector<InputState>>;
	InputTable_t inputTable_;

	std::vector<std::string> configurableEventNames_;
	
	//押されたかどうか記録用(実入力じゃなくてイベント単位)
	std::unordered_map<std::string, bool> currentInputInfo_;
	std::unordered_map<std::string, bool> lastInputInfo_;
	/// <summary>
	/// 入力テーブルをデフォルトに戻す
	/// </summary>
	void InitializeInputTable();

	/// <summary>
	/// 入力テーブルを保存する
	/// </summary>
	void SaveInputTable();

	/// <summary>
	/// 入力テーブルをロードする
	/// </summary>
	void LoadInputTable();
public:
	Input();
	void Update();
	bool IsPressed(const std::string& name)const;
	bool IsTriggered(const std::string& name)const;
};

