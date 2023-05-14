#pragma once
#include "Scene.h"

class InputState;
class SceneManager;
/// <summary>
/// ポーズシーン
/// </summary>
class PauseScene :
    public Scene
{
private:

public:
    PauseScene(SceneManager& manager);
    ~PauseScene();
    virtual void Update(const InputState& input)override;
    void Draw();
};

