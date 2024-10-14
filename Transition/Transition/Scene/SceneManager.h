#pragma once

#include<memory>
#include<list>
class Scene;
class Input;
class FileManager;
class SceneManager
{
private:
	FileManager& fileManager_;
	std::list<std::shared_ptr<Scene> > scenes_;//現在実行中のシーン
public:
	FileManager& GetFileManager();
	SceneManager(FileManager& fileManager);
	/// <summary>
	/// 先頭の(Updateが呼ばれる)シーンを切り替える
	/// </summary>
	/// <param name="scene">切り替え先のシーン</param>
	void ChangeScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// シーンをプッシュする。スタックの数が増える
	/// 一番上のシーンのUpdateしか呼ばれません
	/// </summary>
	/// <param name="scene">上に積むシーン</param>
	void PushScene(std::shared_ptr<Scene> scene);
	/// <summary>
	/// スタックの頭のシーンを削除する
	/// ただし、スタック上にシーンが1つしかない場合は
	/// 削除しない
	/// </summary>
	void PopScene();
	/// <summary>
	/// 持っているシーンスタックの先頭のUpdateを呼び出す
	/// </summary>
	/// <param name="input">入力IOパラメータ</param>
	void Update(Input& input);

	/// <summary>
	/// 持っているシーンすべてを描画する
	/// ただし描画順は下から描画されます
	/// </summary>
	void Draw();
};

