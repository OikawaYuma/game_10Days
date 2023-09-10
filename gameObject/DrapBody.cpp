#include "DrapBody.h"
#include "ImGuiManager.h"
#include<Input.h>
#include"Player.h"
void DrapBody::Initialize(Vector3 translation) {
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("DrapBody.png");
	// 3Dモデルの生成
	model_ = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = translation;
	// 引数で受け取った速度をメンバ変数に代入
}

void DrapBody::Update() {

	if (deadTime <= survivalTimer) {
		isAlive = false;
	}

	worldTransform_.UpdateMatrix();

	
}

void DrapBody::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	/*if (bullet) {
	bullet->Draw(viewProjection_);
	}*/
}

Vector3 DrapBody::GetWorldPosition() {
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// 衝突を検出したらコールバック関数
void DrapBody::OnCollision() {
	isAlive = false; 
	player_->Create();
};



