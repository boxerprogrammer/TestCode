#pragma once

#include<map>
#include<vector>
#include<string>

//ゲーム中で使用される仮想入力情報
enum class InputType {
	next,///次へ行くボタン
	prev,///前に戻るボタン
	pause,///ポーズボタン
	keyconf,///キーコンフィグ
	change,///変更
	up,///↑
	down,///↓
	left,///←
	right,///→
	shot,///弾発射
	switching,//フォースの切り離し、吸い込みの切り替え
	slow,//ゲーム中の動きをスローモーションにする
	max//最大入力インデックス
};

/// <summary>
/// 入力装置カテゴリ
/// </summary>
enum class InputCategory {
	keybd,//キーボード
	pad,//ゲームパッド
	mouse//マウス
};

/// <summary>
/// 入力情報
/// </summary>
struct InputInfo{
	InputCategory cat;//入力装置カテゴリ
	int id;//入力ID(KEY_INPUT_～,PAD_INPUT_～,MOUSE_INPUT_～)
};

//フレンド用にプロトタイプ宣言
class KeyConfigScene;

/// <summary>
/// 入力状態を管理する
/// </summary>
class InputState {
	//KeyConfigSceneにだけ、すべてを見せる。
	friend KeyConfigScene;
private:
	//仮想入力情報と、実際の入力のテーブルを作る
	//キー(first)=InputType
	//値(second)=std::vector<InputInfo>
	using InputMap_t = std::map< InputType, std::vector<InputInfo> >;
	InputMap_t inputMapTable_;//実際の入力とゲームボタンの対応テーブル
	
	//書き換え用の一時的なinputMapTable_のコピー
	//いきなり書き換えてしまわないように
	InputMap_t tempMapTable_;

	//リセット用キーマップテーブル
	InputMap_t defaultMapTable_;

	//入力タイプとその名前の対応テーブル
	std::map<InputType, std::wstring> inputNameTable_;

	std::vector<bool> currentInput_;//現在の入力情報(押してるか押してないか)
	std::vector<bool> lastInput_;//直前の入力情報(直前押してるか押してないか)

public:
	InputState();

	/// <summary>
	/// 押した瞬間にtrueになる
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns>押した瞬間ならtrue,それ以外ならfalse</returns>
	bool IsTriggered(InputType type)const;

	/// <summary>
	/// 押されてたらtrueになる
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns>押されてたらtrue,押されてないならfalse</returns>
	bool IsPressed(InputType type)const;
	/// <summary>
	/// 入力情報を更新する
	/// (注意)毎フレームUpdateを呼ばないと入力状態は
	/// 更新されません。
	/// </summary>
	void Update();

	/// <summary>
	/// 入力情報を更新する
	/// </summary>
	/// <param name="type">ゲームにおける入力種別</param>
	/// <param name="cat">入力カテゴリ(keybd等)</param>
	/// <param name="id">実際の入力</param>
	void RewriteInputInfo(InputType type , InputCategory cat , int id);

	/// <summary>
	/// 現在編集中のキーコンフィグを確定する
	/// </summary>
	void CommitChangedInputInfo();
	
	/// <summary>
	/// 現在編集中のキーコンフィグの変更をなかったことにする
	/// </summary>
	void RollbackChangedInputInfo();

	/// <summary>
	/// キーマップをデフォルトにリセットする
	/// </summary>
	void ResetInputInfo();

	/// <summary>
	/// キーマップをファイルに保存する
	/// </summary>
	void SaveKeyInfo()const;

	void LoadKeyInfo();

};