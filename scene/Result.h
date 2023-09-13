#pragma once
#include "WorldTransform.h"
#include <cassert>
#include <input.h>
#include <list>

#include "Model.h"
#include "TextureManager.h"

class Sprite;
class Result {
public:

	Result();
	~Result();
	void Initialize(int num);
	void Update();
	void Draw();

private:

	uint32_t textureNum_;
	uint32_t textureTen_;
	// 2Dレティクル用スプライト
	Sprite* sprite2DResult_ = nullptr;
	Sprite* sprite2DTen_ = nullptr;
	Sprite* sprite2DNum_ = nullptr;

};
