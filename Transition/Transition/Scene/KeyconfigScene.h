#pragma once
#include"Scene.h"
#include"../Input.h"
#include<array>
/// <summary>
/// �L�[�R���t�B�O�̂��߂̃V�[��
/// </summary>
class KeyconfigScene : public Scene
{
private:
	InputTable_t inputTable_;

	std::vector<std::pair<std::string, std::wstring> > commandStringList_;
	void CreateCommandStringList(const InputTable_t& table);
	int cursorIndex_ = 0;//���J�[�\��������C���f�b�N�X������
	bool isEditToggle_=false;

	std::array<char,256> keycode_ = {};
	int padInfo_ = 0;
	int mouseInfo_ = 0;

	std::array<char,256> lastKeycode_ = {};
	int lastpadInfo_ = 0;
	int lastmouseInfo_ = 0;

	std::unordered_map<int, std::wstring> keyboardNameMap_;
	std::unordered_map<int, std::wstring> padNameMap_;


	void EditInput(const std::string& key,Input& input);

public:
	KeyconfigScene(SceneManager& manager);
	~KeyconfigScene() {};//������͑厖
	virtual void Update(Input& input)override;
	virtual void Draw()override;
};

