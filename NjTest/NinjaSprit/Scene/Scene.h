#pragma once

//struct Input {
//	char keyState[256];
//	char lastKeyState[256];
//
//	void Update();
//
//};
class Input;
class SceneController;
/// <summary>
/// シーン基底クラス
/// </summary>
class Scene
{
	friend SceneController;
protected:
	SceneController& controller_;
	Scene() = default;
	Scene(SceneController& );
public:
	/// <summary>
	/// シーン更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void Update(const Input& input) = 0;

	/// <summary>
	/// シーン描画
	/// </summary>
	virtual void Draw() = 0;
	virtual ~Scene() = default;
};

