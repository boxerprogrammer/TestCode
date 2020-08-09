#pragma once
#include<memory>
#include<deque>
#include<functional>
class Scene;
class Input;
class Application;

/// <summary>
/// シーン切り替えクラス
/// </summary>
class SceneController
{
	friend Application;
private:
	SceneController();
	std::deque<std::shared_ptr<Scene>> scene_;
	std::function<void(void)> postDrawExecuter_;
public:

	void ClearScene();

	///シーンをシーンスタックの頭に入れる
	void PushScene(Scene*);
	///シーンスタックの先頭を削除する
	void PopScene();

	///シーンスタックを一度クリアしてから
	///指定のシーンへ遷移する
	///<param name="scene">切り替え先のシーンオブジェクト</param>
	///<attention>前のシーンは呼出し後に削除
	///されるため、この関数の呼び出し元が
	///削除対象だった場合には、関数呼び出し後に
	///処理を行ってはならない</attension>
	void CleanChangeScene(Scene* scene);
	
	///シーンを切り替える
	///<param name="scene">切り替え先のシーンオブジェクト</param>
	///<attention>前のシーンは呼出し後に削除
	///されるため、この関数の呼び出し元が
	///削除対象だった場合には、関数呼び出し後に
	///処理を行ってはならない</attension>	
	void ChangeScene(Scene*);

	///<summary>持っている先頭シーンのUpdateを呼ぶ</summary>
	///<param name="input">入力情報</param>
	void Update(const Input& input);
	///持っているシーンすべてを描画する
	void Draw();
};

