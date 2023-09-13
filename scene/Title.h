#pragma once
#include "WorldTransform.h"
#include <cassert>
#include <input.h>
#include <list>

#include "Model.h"
#include "TextureManager.h"

class Sprite;
class Title {
public:
	Title();
	~Title();
	/// </summary>
	void DrawUI();

	void DrawSetumei();

	void Initialize();

	void Update();


private:
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	Sprite* sprite2DSetumei_ = nullptr;
};
