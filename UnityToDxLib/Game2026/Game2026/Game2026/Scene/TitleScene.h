#pragma once
#include "Scene.h"
class TitleScene :
    public Scene
{
private:
    int charH_;
    int backH_;

    int frame_;
    //戻り値なしで、引数はInput&をとる関数ポインタの型
    using Update_t = void (TitleScene::*)(Input& input);
    Update_t update_;//更新メンバ関数ポインタ


    using Draw_t = void(TitleScene::*)();
    Draw_t draw_;//描画メンバ関数ポインタ

    /// <summary>
    /// フェードイン中更新
    /// </summary>
    /// <param name="input">入力</param>
    void FadeInUpdate(Input& input);
    /// <summary>
    /// フェードアウト中更新
    /// </summary>
    /// <param name="input">入力</param>
    void FadeOutUpdate(Input& input);

    //通常更新
    void NormalUpdate(Input& input);

    //通常描画
    void NormalDraw();

    //フェード中描画
    void FadeDraw();
public:
    TitleScene(SceneController& ctrl);
    void Update(Input& input)override;
    void Draw()override;
};

