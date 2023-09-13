#include "Title.h"
#include <GameScene.h>



Title::Title() {

}
Title::~Title() { 
	delete sprite2DReticle_;
	delete sprite2DSetumei_;
}
    
void Title::Initialize(){
	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("Title.png");
	// レティクル用テクスチャ取得
	uint32_t textureSetumei = TextureManager::Load("setumei.png");

	// スプライト生成
	sprite2DReticle_ =
	    Sprite::Create(textureReticle, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	// スプライト生成
	sprite2DSetumei_ =
	    Sprite::Create(textureSetumei, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
};

void Title::Update(){};

void Title::DrawUI() {

	// スプライト生成
	sprite2DReticle_->Draw();
}

void Title::DrawSetumei() {

	// スプライト生成
	sprite2DSetumei_->Draw();
}
