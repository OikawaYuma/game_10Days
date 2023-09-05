#include "Skydome.h"
#include<cassert>
#include "TextureManager.h"
#include<Input.h>

void Skydome::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	//textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = {0, 0, 0};

	worldTransform_.scale_ = {1000, 1000, 1000};
	worldTransform_.UpdateMatrix();

}

void Skydome::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_ );
}