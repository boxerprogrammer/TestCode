#pragma once
class Input;
/// <summary>
/// 入力リスナ基底クラス
///基本的にオブザーバパターンのインターフェースを提供するのみ
/// </summary>
class InputListener
{
public:
	/// <summary>
	/// オブザーバに入力イベントを知らせる
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void Notify(const Input& input) = 0;
};

