#pragma once
#include "Scene.h"

class InputState;
class SceneManager;
/// <summary>
/// �|�[�Y�V�[��
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

