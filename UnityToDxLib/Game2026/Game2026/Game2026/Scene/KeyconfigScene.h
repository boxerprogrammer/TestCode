#pragma once
#include "Scene.h"
#include<unordered_map>
#include<functional>
#include<string>
#include<array>
#include<vector>
#include"../Input.h"
class KeyconfigScene :
    public Scene
{
private:
    int frame_;
    Input& input_;

    Input::InputTable_t tempInputTable_;//キーコンフィグのための一時入力テーブル

    std::array<char, 256> lastKeyState_;
    uint32_t lastPadState_;
    
    int selectedIndex_ = 0;//選択中の行


    std::vector<std::wstring> keyConfMenus_;
    std::unordered_map<std::wstring, std::function<void(Input&)>> keyConfExecTable_;


    using UpdateFunc_t = void(KeyconfigScene::*)(Input& input);
    using DrawFunc_t = void(KeyconfigScene::*)();

    UpdateFunc_t update_;
    DrawFunc_t draw_;

    std::unordered_map<int, std::wstring> keyboardNameTable_;
    std::unordered_map<int, std::wstring> padInputNameTable_;

    void AppearUpdate(Input&);//キーコンフィグ枠表示時のUpdate
    void NormalUpdate(Input&);//通常Update
    void IntermediateUpdate(Input&);//Normal→Editingの間のワンクッション
    void EditingUpdate(Input&);//キーを固定して編集中Update;
    void DisappearUpdate(Input&);//キーコンフィグ枠を閉じるUpdate

    void ExpandDraw();//広がり表示
    void NormalDraw();
    void DrawRows(bool isEditing);
    void EditingDraw();//編集中表示

    void DrawFrame(float rate);//

public:
    KeyconfigScene(SceneController& controller,Input& input);
    void Update(Input& input);
    void Draw();
};

