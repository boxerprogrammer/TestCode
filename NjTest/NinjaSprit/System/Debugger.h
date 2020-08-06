#pragma once

struct Circle;
struct Segment;
struct Capsule;


/// <summary>
/// デバッグ支援に必要な処理を行うための
/// クラス≪シングルトン≫
/// </summary>
class Debugger
{
private:
	Debugger()=default;
	Debugger(const Debugger&) = delete;
	void operator =(const Debugger&) = delete;
public:
	/// <summary>
	/// いつもの
	/// </summary>
	/// <returns>Debuggerインスタンス</returns>
	static Debugger& Instance();

	/// <summary>
	/// 今デバッグモードかどうか
	/// </summary>
	/// <returns>true：デバッグモード</returns>
	bool IsDebugMode()const;

	/// <summary>
	/// 毎フレーム呼ばれ内部変数を更新している
	/// </summary>
	void Update();

	/// <summary>
	/// パフォーマンスカウンタを表示する
	/// </summary>
	void DisplayPerformance();

	void Draw(const Circle& circle,unsigned int color,float xoffset=0.0f, float yoffset = 0.0f,float thickness=1.0f);
	void Draw(const Segment& seg, unsigned int color, float xoffset = 0.0f, float yoffset = 0.0f, float thickness = 1.0f);
	void Draw(const Capsule& capsule, unsigned int color,  float xoffset = 0.0f, float yoffset = 0.0f, float thickness = 1.0f);

};

