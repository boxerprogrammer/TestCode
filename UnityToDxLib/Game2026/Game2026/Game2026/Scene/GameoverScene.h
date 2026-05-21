#pragma once
#include "Scene.h"
class GameoverScene :
    public Scene
{
private:
    int gameoverStringH_;
    int backH_;
    int frame_;
    using Update_t = void (GameoverScene::*)(Input& input);
    Update_t update_;
    using Draw_t = void(GameoverScene::*)();
    Draw_t draw_;

    //フェードイン
    void FadeInUpdate(Input& input);
    //フェードアウト
    void FadeOutUpdate(Input& input);
    //通常更新
    void NormalUpdate(Input& input);

    //フェード描画
    void FadeDraw();
    //通常描画
    void NormalDraw();

public:
    GameoverScene(SceneController& ctrl);
    void Update(Input& input)override;
    void Draw()override;
};

