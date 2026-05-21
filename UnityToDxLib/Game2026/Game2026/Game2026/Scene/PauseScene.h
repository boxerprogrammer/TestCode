#pragma once
#include "Scene.h"
#include<vector>
#include<string>
#include<map>
#include<functional>

class PauseScene :
    public Scene
{
private:
    int frame_;

    int selectedIndex_ = 0;//選択中のメニューアイテム
    std::vector<std::wstring> menuItems_;//メニューアイテム文字列
    std::map < std::wstring, std::function<void(Input&)>> commandTable_;


    //YES/NOダイアログに使用する
    std::vector<std::wstring> yesNoItems_;
    std::function<void(Input&)> execYesFunc_;
    bool isYes_ = false;
    std::wstring yesNoTitle_ = L"";

    using Update_t = void(PauseScene::*)(Input& input);
    using Draw_t = void(PauseScene::*)();

    Update_t update_;
    Draw_t draw_;

    void AppearUpdate(Input& input);
    void NormalUpdate(Input& input);
    void DisappearUpdate(Input& input);

    void YesNoUpdate(Input& input);
    void YesNoDraw();

    void ExpandDraw();
    void NormalDraw();

    /// <summary>
    /// 広がるメニュー枠を表示
    /// </summary>
    /// <param name="rate">0.0の時非表示、1.0の時フル表示</param>
    void DrawFrame(float rate);

    /// <summary>
    /// メニューを表示する
    /// </summary>
    void DrawMenu();

public:
    PauseScene(SceneController& ctrl);
    void Update(Input& input)override;
    void Draw()override;
};

