#pragma once
#include<memory>
#include<list>
class Scene;
class Input;
class SceneController
{
private:
	//「プッシュダウンオートマトン」を実現させるために複数
	//シーンをスタックできるようにするために、scenes_をlistで宣言
	std::list<std::shared_ptr<Scene>> scenes_;
public:
	void ChangeScene(std::shared_ptr<Scene> scene);
	void PushScene(std::shared_ptr<Scene> scene);
	void PopScene();
	/// <summary>
	/// 2つ以上のシーンが積まれてても1つのシーンにリセットする
	/// </summary>
	/// <param name="scene">リセット後のシーン</param>
	void ResetScene(std::shared_ptr<Scene> scene);
	void Update(Input& input);
	void Draw();
};

