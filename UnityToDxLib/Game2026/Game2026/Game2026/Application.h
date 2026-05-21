#pragma once
#include"Geometry.h"
/// <summary>
/// アプリケーションクラス(シングルトン)
/// </summary>
class Application
{
private:
	bool isShutdownRequested_ = false;
public:
	~Application();
	/// <summary>
	/// Applicationのシングルトンインスタンスを返します
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static Application& GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>初期化成功:true,失敗:false</returns>
	bool Init();

	/// <summary>
	/// アプリケーションを起動する(メインループに入る)
	/// </summary>
	void Run();

	/// <summary>
	/// アプリケーションの終了処理
	/// </summary>
	void Terminate();

	const Size& GetWindowSize()const;

	void Shutdown();

private:
	Application();
	Application(const Application& app)=delete;
	void operator=(const Application& app) = delete;
	Size windowSize_;


};

