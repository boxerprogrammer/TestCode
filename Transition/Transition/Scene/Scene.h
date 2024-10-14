#pragma once

class Input;
class SceneManager;

/// <summary>
/// �V�[���̊��N���X(����1�N���X)
/// </summary>
class Scene
{
protected:
	SceneManager& sceneManager_;
public:
	Scene(SceneManager& manager);
	virtual ~Scene() {};//������͑厖
	virtual void Update(Input& input)=0;
	virtual void Draw() = 0;
};

