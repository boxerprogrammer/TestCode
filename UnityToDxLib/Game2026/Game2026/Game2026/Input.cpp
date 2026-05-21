#include "Input.h"

#include<DxLib.h>

namespace {
    struct KeyConfigHeader {
        char signature[4];//"kcnf"
        float version;//1.0
        uint32_t dataNum;//イベントがいくつあるか
    };
}

void Input::InitializeInputTable()
{
    inputTable_.clear();

    inputTable_["up"] = { {PeripheralType::keybd,KEY_INPUT_UP},
                        {PeripheralType::keybd,KEY_INPUT_W},
                        {PeripheralType::pad,PAD_INPUT_UP} };
    inputTable_["down"] = { {PeripheralType::keybd,KEY_INPUT_DOWN},
                            {PeripheralType::keybd,KEY_INPUT_S},
                            {PeripheralType::pad,PAD_INPUT_DOWN} };
    inputTable_["left"] = { {PeripheralType::keybd,KEY_INPUT_LEFT},
                            {PeripheralType::keybd,KEY_INPUT_A},
                            {PeripheralType::pad,PAD_INPUT_LEFT} };
    inputTable_["right"] = { {PeripheralType::keybd,KEY_INPUT_RIGHT},
                            {PeripheralType::keybd,KEY_INPUT_D},
                            {PeripheralType::pad,PAD_INPUT_RIGHT} };

    //DXライブラリはPADのボタン番号が6ボタンベースなのでずれています
    inputTable_["ok"] = { {PeripheralType::keybd,KEY_INPUT_RETURN},
                            {PeripheralType::pad,PAD_INPUT_L},//SELECTボタン
                            {PeripheralType::mouse,MOUSE_INPUT_LEFT }, };
    inputTable_["cancel"] = {
        {PeripheralType::keybd,KEY_INPUT_ESCAPE}
    };

    inputTable_["pause"] = { {PeripheralType::keybd,KEY_INPUT_P},
                            {PeripheralType::pad,PAD_INPUT_R} };//STARTボタン

    inputTable_["jump"] = { {PeripheralType::keybd,KEY_INPUT_Z},
                            {PeripheralType::pad,PAD_INPUT_C} };//Xボタン

    inputTable_["attack"] = { {PeripheralType::keybd,KEY_INPUT_X},
                            {PeripheralType::pad,PAD_INPUT_A} };//Aボタン


 
}

void Input::SaveInputTable()
{
    //C言語のファイル管理
    FILE* fp = nullptr;
    auto err = fopen_s(&fp, "keyconf.dat", "wb");
    if (fp == nullptr) {
        return;
    }
    //ヘッダ部
    KeyConfigHeader header;
    memcpy(header.signature, "kcnf", 4);
    header.version = 1.0f;
    header.dataNum = inputTable_.size();
    fwrite(&header, sizeof(header), 1, fp);

    //データ部
    for (const auto& row : inputTable_) {
        //最初の１バイトはイベント名の文字列サイズ
        byte nameSize = row.first.size();
        fwrite(&nameSize, sizeof(nameSize), 1, fp);
        fwrite(row.first.data(), nameSize, 1, fp);
        //実入力データを書き込む
        byte dataSize = row.second.size();
        fwrite(&dataSize, sizeof(dataSize), 1, fp);
        fwrite(row.second.data(), sizeof(InputState),
            dataSize, fp);
    }

    fclose(fp);
}

void Input::LoadInputTable()
{
    auto handle = FileRead_open(L"keyconf.dat");
    if (handle == 0) {
        return;
    }
    KeyConfigHeader header;
    FileRead_read(&header, sizeof(header), handle);
    for (int i = 0; i < header.dataNum; ++i) {
        byte nameSize;
        FileRead_read(&nameSize, sizeof(nameSize), handle);
        std::string eventName;
        eventName.resize(nameSize);
        FileRead_read(eventName.data(), nameSize, handle);
        byte dataSize;
        FileRead_read(&dataSize, sizeof(dataSize), handle);
        inputTable_[eventName].resize(dataSize);
        FileRead_read(inputTable_[eventName].data(), sizeof(InputState) *
            dataSize, handle);
    }
    FileRead_close(handle);
}

Input::Input()
{
    InitializeInputTable();

    LoadInputTable();

    configurableEventNames_ = { "ok","cancel","pause","jump","attack" };

    for (auto& inputInfo : inputTable_) {
        currentInputInfo_[inputInfo.first] = false;
        lastInputInfo_[inputInfo.first] = false;
    }
}

void Input::Update()
{
    lastInputInfo_ = currentInputInfo_;
    //生データの取得
    std::array<char, 256> keystate;
    GetHitKeyStateAll(keystate.data());
    int mouseState = GetMouseInput();
    int padState = GetJoypadInputState(DX_INPUT_PAD1);//とりあえず１コンだけ
    //生データをcurrentInputInfo_に反映させる
    for (const auto& inputInfo : inputTable_) {
        const auto& eventName = inputInfo.first;
        for (const auto& inputState : inputInfo.second) {
            switch (inputState.type)
            {
            case PeripheralType::keybd:
                currentInputInfo_[eventName] = keystate[inputState.id];
                break;
            case PeripheralType::mouse:
                currentInputInfo_[eventName] = mouseState&inputState.id;
                break;
            case PeripheralType::pad:
                currentInputInfo_[eventName] = (padState & inputState.id);
                break;
            }
            if (currentInputInfo_[eventName]) {
                break;
            }
        }
    }
}

bool Input::IsPressed(const std::string& name) const
{
    if (!currentInputInfo_.contains(name)) {
        return false;
    }
    return currentInputInfo_.at(name);
}

bool Input::IsTriggered(const std::string& name) const
{
    if (!currentInputInfo_.contains(name)) {
        return false;
    }
    return currentInputInfo_.at(name) && !(lastInputInfo_.at(name));
}
