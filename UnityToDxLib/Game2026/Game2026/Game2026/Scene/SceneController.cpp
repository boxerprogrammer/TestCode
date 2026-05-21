#include "SceneController.h"
#include"Scene.h"
void SceneController::ChangeScene(std::shared_ptr<Scene> scene)
{
	if (scenes_.empty()) {
		scenes_.push_back(scene);
		return;
	}
	scenes_.back() = scene;
}

void SceneController::PushScene(std::shared_ptr<Scene> scene)
{
	scenes_.push_back(scene);//末尾にシーンを追加する
}

void SceneController::PopScene()
{
	if (scenes_.size()>1) {
		scenes_.pop_back();//末尾のシーンを削除する
	}
}


void SceneController::ResetScene(std::shared_ptr<Scene> scene)
{
	scenes_.clear();
	scenes_.push_back(scene);
}

void
SceneController::Update(Input& input) {
	scenes_.back()->Update(input);//末尾のやつだけUpdate
}
void 
SceneController::Draw() {
	//乗ってるシーンをすべてDraw
	for (auto& scene : scenes_) {
		scene->Draw();
	}
}