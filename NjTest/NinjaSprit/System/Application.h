#pragma once
#include<memory>

//アプリケーションクラス
//全てのおおもとになるクラス
//mainの代わり。実際のmainは
//エントリポイントの役割だけにしておく
class SceneController;
struct Size;
struct Rect;
//表示すべき画面の情報を持つ
class Viewport {
public:
	Size GetSize()const;//ゲーム画面(ビューポート)のサイズを返す
	Rect GetRect()const;//ゲーム画面のビューポート矩形を返す
};

/// <summary>
/// アプリケーション全体をコントロール
/// </summary>
class Application
{
private:
	Viewport viewport_;
	std::unique_ptr<SceneController> sceneController_;

	Application() = default;
	Application(const Application&) = delete;
	void operator = (const Application&) = delete;
public:
	/// <summary>
	/// シングルトンインスタンスを返す
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static Application& Instance();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>初期化成功したかどうか</returns>
	bool Initialize();
	/// <summary>
	/// メインループを実行する
	/// </summary>
	void Run();

	/// <summary>
	///後処理
	/// </summary>
	void Terminate();

	///<summary>
	///アプリケーションを終了する
	///</summary>
	void Exit();

	/// <summary>
	/// ゲーム画面のビューポート情報を返す
	/// </summary>
	/// <returns></returns>
	const Viewport& GetViewport()const;

};

