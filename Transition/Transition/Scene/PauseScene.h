#pragma once
#include "Scene.h"

#include<list>
#include<functional>


class PauseScene :
    public Scene
{
private:
    using UpdateFunc_t = void (PauseScene::*)(Input& input);
    
    //Update�֐��Ŏ��s�����֐����w�������|�C���^
    UpdateFunc_t updateFunc_;

    void BeginUpdate(Input& input);//�|�[�Y�\���܂ł̍X�V
    void NormalUpdate(Input& input);//�ʏ�X�V
    void EndUpdate(Input& input);//�|�[�Y�����̍X�V

    using DrawFunc_t = void(PauseScene::*)();
    DrawFunc_t drawFunc_;

    void BeginDraw();
    void NormalDraw();
    void EndDraw();

    std::list< std::function<void(void)>> commandList_;

    int frame_ = 0;

public:
    PauseScene(SceneManager& manager);
    virtual void Update(Input& input)override;
    virtual void Draw()override;
};

