#include"SceneManager.h"
#include"Scene.h"
void 
SceneManager::ChangeScene(Scene* scene) {
	//�V�[���X�^�b�N����ł͂Ȃ�������...
	if (!scenes_.empty()) {
		delete scenes_.front();//top�͂Ă��؂�̗v�f��Ԃ��Ă�
		scenes_.pop_front();//1���遨�ӂ���0�ɂȂ�
	}
	scenes_.push_front(scene);//1������
}

void SceneManager::PushScene(Scene* scene)
{
	scenes_.push_front(scene);//1������
}

void SceneManager::PopScene()
{
	//�|�b�v�̌��ʁA�V�[�����O�ɂȂ�Ȃ��悤�ɂ���
	if (scenes_.size() > 1) {
		delete scenes_.front();
		scenes_.pop_front();
	}
}

void
SceneManager::Update(const InputState& input) {
	scenes_.front()->Update(input);
}


void
SceneManager::Draw() {
	for (int i = static_cast<int>(scenes_.size() - 1); i >= 0; --i) {
		scenes_[i]->Draw();
	}
}