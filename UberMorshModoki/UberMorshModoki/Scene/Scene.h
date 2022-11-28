#pragma once

class SceneManager;
struct InputState;
/// <summary>
/// 各シーンの基底クラス
/// すべてのシーンの抽象をあらわす
/// </summary>
class Scene {
protected:
	SceneManager& manager_;
public:
	Scene(SceneManager& manager) :manager_(manager) {}
	virtual ~Scene() {};
	/// <summary>
	/// シーンの内容の更新
	/// </summary>
	/// <param name="input">入力状態/param>
	/// <remarks>inputは呼び出し側で実体を作って代入</remarks>
	virtual void Update(const InputState& input) = 0;
	/// <summary>
	/// シーンの描画時に呼び出される
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 指定のキーの押された瞬間にtrueを返す	
	/// </summary>
	/// <param name="input">入力状態</param>
	/// <param name="key">入力キー指定値</param>
	/// <returns>true:押された、false:押されてないorおしっぱ</returns>
	/// <example>
	/// if(IsTriggered(input,KEY_INPUT_RETURN))
	/// </example>
	bool IsTriggered(const InputState& input, char key);
};
