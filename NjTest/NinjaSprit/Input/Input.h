#pragma once
#include<string>
#include<array>
#include<vector>
#include<unordered_map>
constexpr int input_record_size = 2;///<summary>入力バッファ格納数</summary>
/// <summary>
/// 周辺機器種別
/// </summary>
enum class PeripheralType {
	keyboard,///<キーボード
	gamepad,///<ゲームパッド
	xboxpad,///<xboxパッド
};
/// <summary>
/// 周辺機器入力情報
/// </summary>
struct PeripheralInfo {
	PeripheralType type;///<機器種別
	int inputIdx;///<入力を識別するための数値
};
using PeripheralReferenceTable_t = std::unordered_map < std::string, std::vector<PeripheralInfo>>;
class KeyconfigScene;
class Input
{
	friend KeyconfigScene;
private:
	mutable std::vector<char> rawkeystate_;//生キーボードステート
	mutable int rawpadstate_;//生パッドステート(1コンのみ)
	mutable bool isRawMode_ = false;//生モード
	/// <summary>
	/// 生モードをアンロックする
	/// このアンロックによって通常はローカルにのみ存在する
	/// 生キーボード、生パッド情報が保持され参照できるようになる
	/// </summary>
	void UnlockRawMode()const;

	/// <summary>
	/// 生モードをロックする
	/// このモードで生データにアクセスしようとすればアサーションを起こす
	/// </summary>
	void LockRawMode()const;
	/// <summary>
	/// 生キーボード情報を持ってくる
	/// 但しロック中は中身が空
	/// </summary>
	/// <returns>生キーボード情報配列</returns>
	const std::vector<char>& GetRawKeyboardState()const;

	/// <summary>
	/// 生パッド情報を持ってくる
	/// ただし１番のみ
	/// </summary>
	/// <returns>生パッド情報</returns>
	const int GetRawPadState()const;
	
	mutable PeripheralReferenceTable_t peripheralReferenceTable_;///外側からいじれる用
	/// <summary>
	/// 外側から入力テーブルをいじれる用の関数
	/// </summary>
	/// <param name="prt">変更が行われた入力テーブル(一部)</param>
	void SetPeripheralReferenceTable(const PeripheralReferenceTable_t& prt)const;

	int currentInputIndex_ = 0;///<summary>現在の入力バッファを指すインデックス</summary>
	using InputStateTable_t = std::unordered_map<std::string, bool>;
	std::array<InputStateTable_t, input_record_size> inputStateTable_;///<summary>入力格納テーブル</summary>
	/// <summary>
	/// 次の入力バッファインデックスを返す
	/// </summary>
	/// <returns>次の入力バッファインデックス</returns>
	int GetNextInputBufferIndex();
	/// <summary>
	/// 1フレーム前を表す入力バッファインデックスを返す
	/// </summary>
	/// <returns>１フレーム前の入力バッファインデックス</returns>
	size_t	GetLastInputBufferIndex()const;
	/// <summary>
	/// 現在の入力情報への参照を返す
	/// </summary>
	/// <param name="cmd">コマンド文字列</param>
	/// <returns>それが押されたかどうか</returns>
	bool& CurrentInput(const std::string cmd);
	bool GetCurrentInput(const std::string cmd)const;
	/// <summary>
	/// 直前の入力情報への参照を返す
	/// </summary>
	/// <param name="cmd">コマンド文字列</param>
	/// <returns>それが押されたかどうか</returns>
	bool LastInput(const std::string cmd)const;

	/// <summary>
	/// イベントに対応する機器入力を登録する
	/// </summary>
	/// <param name="eventname">入力イベント名</param>
	/// <param name="peri">機器入力情報配列(ベクタ)</param>
	void RegistAcceptPeripheral(const char* eventname, const std::vector<PeripheralInfo>& peri);
	/// <summary>
	/// 現在それが押されてるかチェックする
	/// </summary>
	/// <param name="eventname">入力イベント名</param>
	/// <param name="keystate">キーボード情報</param>
	/// <param name="padstate">パッド情報</param>
	/// <returns>true:押されてる / false:押されてない</returns>
	bool CheckPressed(const char* eventname,const char* keystate,int padstate);
public:
	Input();
	/// <summary>
	/// 入力内容を更新する
	/// </summary>
	void Update();

	void RecordInput(const char* eventname, char  keystate[256], int pad1);

	/// <summary>
	/// 押してるかどうかを返す
	/// </summary>
	/// <param name="cmd">コマンド文字列</param>
	/// <returns>押されてる</returns>
	bool IsPressed(const char* cmd)const;

	/// <summary>
	/// 押された瞬間なのかどうかを返す
	/// </summary>
	/// <param name="cmd">コマンド文字列</param>
	/// <returns>
	/// 今押されてない:false
	/// 今押されて直前も押されてる:false
	/// 今押されて、直前は押されてない:true
	///</returns>
	bool IsTriggered(const char* cmd)const;

	/// <summary>
	/// 離された瞬間なのかどうかを返す
	/// </summary>
	/// <param name="cmd">コマンド文字列</param>
	/// <returns>
	/// 今押されてる:false
	/// 今押されてない直前も押されない:false
	/// 直前押されて、今押されてない:true
	///</returns>
	bool IsReleased(const char* cmd)const;
};
///入力通知を受け取る(インターフェース)
class InputListener {
public:
	virtual ~InputListener() = default;
	///ぶっちゃけた話毎フレーム呼ばれる
	///こいつが入力を受け取る
	virtual void Notify(const Input& input) = 0;
};