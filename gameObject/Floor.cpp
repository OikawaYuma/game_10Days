#include "Floor.h"

#include<cassert>
#include "TextureManager.h"
#include<Input.h>

void Floor::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	// textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = {0, 0, 0};

	worldTransform_.scale_ = {1000, 1000, 1000};
	worldTransform_.UpdateMatrix();
}

void Floor::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Floor::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}