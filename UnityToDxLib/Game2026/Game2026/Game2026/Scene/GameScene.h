#pragma once
#include "Scene.h"
#include"../Geometry.h"
#include<vector>
#include<string>
#include<map>
class GameScene :
    public Scene
{
private:
    struct GameObject {
        std::string name;
        Position3 pos;
        Vector3 axis;//回転軸
        float angle;//回転角度
        Vector3 scale;//拡大縮小
    };
    std::vector<GameObject> gameObjects_;

    std::map<std::string, int> models_;

    Position2 pos_;
    Position2 vel_;
    Position2 bulletPos_;
    Position2 bulletVel_;
    bool deadBullet_;
    bool isJumping_ = false;
    int gameH_;
    int frame_;
    using Update_t = void (GameScene::*)(Input& input);
    Update_t update_;
    using Draw_t = void(GameScene::*)();
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

    void CreateEnvironmentModelList(const std::wstring& folder);

    void LoadModel(const std::string& name);

public:
    GameScene(SceneController& ctrl);
    void Update(Input& input)override;
    void Draw()override;
};

