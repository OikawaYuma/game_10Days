#include "Time2.h"
#include <GameScene.h>

Time2::Time2() {}
Time2::~Time2() {
	delete sprite2DReticle_;
	delete sprite2DSetumei_;
}

void Time2::Initialize() {
	// レティクル用クスチャ取得
	 textureReticle_ = TextureManager::Load("60.png");
	// レティクル用テクスチャ取得
	uint32_t textureSetumei = TextureManager::Load("setumei.png");

	// スプライト生成
	sprite2DReticle_ =
	    Sprite::Create(textureReticle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	// スプライト生成
	sprite2DSetumei_ =
	    Sprite::Create(textureSetumei, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
};

void Time2::Update(int count) {
	if (count >= 60 * 50) {
		textureReticle_ = TextureManager::Load("10.png");
		// スプライト生成
		sprite2DReticle_ =
		    Sprite::Create(textureReticle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	} else if (count >= 60 * 30) {
		textureReticle_ = TextureManager::Load("30.png");
		// スプライト生成
		sprite2DReticle_ =
		    Sprite::Create(textureReticle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	}
	else if (count >= 60*15) {
		textureReticle_ = TextureManager::Load("45.png");
		// スプライト生成
		sprite2DReticle_ =
		    Sprite::Create(textureReticle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	
	}
};





void Time2::DrawUI() {

	// スプライト生成
	sprite2DReticle_->Draw();
}

void Time2::DrawSetumei() {

	// スプライト生成
	sprite2DSetumei_->Draw();
}


