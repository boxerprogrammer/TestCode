#pragma once
class Input;
class SceneController;
/// <summary>
/// シーンの基底クラス
/// </summary>
class Scene
{
protected:
	SceneController& controller_;
public:
	Scene(SceneController& ctrl);
	virtual void Update(Input& input) = 0;
	virtual void Draw() = 0;
};

