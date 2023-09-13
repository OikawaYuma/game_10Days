#pragma once
#include "WorldTransform.h"
#include <cassert>
#include <input.h>
#include <list>

#include "Model.h"
#include "TextureManager.h"

class Sprite;
class Time2 {
public:
	Time2();
	~Time2();
	/// </summary>
	void DrawUI();

	void DrawSetumei();

	void Initialize();

	void Update(int count);

private:
	uint32_t textureReticle_;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	Sprite* sprite2DSetumei_ = nullptr;

};
