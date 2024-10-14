#pragma once
#include "Scene.h"

#include<list>
#include<functional>


class PauseScene :
    public Scene
{
private:
    using UpdateFunc_t = void (PauseScene::*)(Input& input);
    
    //Update関数で実行される関数を指し示すポインタ
    UpdateFunc_t updateFunc_;

    void BeginUpdate(Input& input);//ポーズ表示までの更新
    void NormalUpdate(Input& input);//通常更新
    void EndUpdate(Input& input);//ポーズ解除の更新

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

