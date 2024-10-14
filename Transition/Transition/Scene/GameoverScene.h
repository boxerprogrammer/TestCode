#pragma once
#include "Scene.h"
#include"../MimicFile.h"
#include<vector>
#include<string>
#include<map>
#include"../Geometry.h"
class GameoverScene :
    public Scene
{
private:
    std::vector<std::string> filenames_;
    std::shared_ptr<MimicFile> cutDataFile_;
    int handle_;
    std::map<std::string, Rect> rectTable_;
    int frame_;
    using UpdateFunc_t = void (GameoverScene::*)(Input& input);
    using DrawFunc_t = void (GameoverScene::*)();

    UpdateFunc_t updateFunc_;
    DrawFunc_t drawFunc_;

    void LoadingUpdate(Input& input);
    void LoadingDraw();

    void NormalUpdate(Input& input);
    void NormalDraw();

public:
    GameoverScene(SceneManager& manager);
    virtual void Update(Input& input)override;
    virtual void Draw()override;
};

