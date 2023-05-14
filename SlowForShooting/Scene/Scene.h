#pragma once

//#includeではなくプロトタイプ宣言を使用する
class SceneManager;//シーンマネージャ
class InputState;//入力ステート
//なので、この時点ではSceneManagerとInputStateは
//ポインタもしくは参照としてしか扱えない。

/// <summary>
/// シーン基底クラス
/// </summary>
class Scene
{
protected:
	SceneManager& manager_;//シーンマネージャへの参照
public:
	Scene(SceneManager& manager) :manager_(manager) {}
	virtual ~Scene() {};
	///シーンを更新する
	virtual void Update(const InputState& input) = 0;
	///シーンを描画する
	virtual void Draw() = 0;
};

