#pragma once

class Input;
class SceneManager;

/// <summary>
/// シーンの基底クラス(抽象1クラス)
/// </summary>
class Scene
{
protected:
	SceneManager& sceneManager_;
public:
	Scene(SceneManager& manager);
	virtual ~Scene() {};//これ実は大事
	virtual void Update(Input& input)=0;
	virtual void Draw() = 0;
};

