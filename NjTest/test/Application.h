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
	Size GetSize()const;
	Rect GetRect()const;
};

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
	/// メインループを終了する
	/// </summary>
	void Exit();

	/// <summary>
	///後処理
	/// </summary>
	void Terminate();

	const Viewport& GetViewport()const;

};

