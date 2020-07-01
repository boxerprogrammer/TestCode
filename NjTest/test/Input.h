#pragma once
class Input
{
	
public:
	Input();
	/// <summary>
	/// 入力内容を更新する
	/// </summary>
	void Update();

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

