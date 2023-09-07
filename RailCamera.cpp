#include "RailCamera.h"
#include<Input.h>
#include"function.h"
#include<ImGuiManager.h>

void RailCamera::Initialize(Vector3 pos, Vector3 rot) {	
	
	// 引数でワールド座標を受け取ってワールドトランスフォームに設定
	worldTransform_.translation_.x = pos.x;
	worldTransform_.translation_.y = pos.y;
	worldTransform_.translation_.z = pos.z - 15;
	// 引数で回転角[ラジアンを受け取ってワールドトランスフォームに設定
	worldTransform_.rotation_ = rot;
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 10000.0f;
	viewProjection_.Initialize();
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
};

void RailCamera::Update(WorldTransform playerWorldTransform_){
	// 行列を定数バッファに転送
	//worldTransform_.TransferMatrix();

	
	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat("%f", &move_.z, 0.0f, 0.2f);

	//ImGui::SliderFloat("%f", &.z, 0.0f, 0.2f);
	//ImGui::SliderFloat("%f", &move.z, 0.0f, 0.2f);
	//ImGui::SliderFloat("%f", &move.z, 0.0f, 0.2f);

	const float kBulletSpeed = 0.5f;
	Vector3 velocity(0, 0, 0);
	velocity = Normalize(velocity);
	velocity.x *= kBulletSpeed;
	velocity.y *= kBulletSpeed;
	velocity.z *= kBulletSpeed;

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, playerWorldTransform_.matWorld_);
	ImGui::Text(
	    " translasion %f  %f  %f", worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	ImGui::Text(
	    "rotate %f  %f  %f", worldTransform_.rotation_.x, worldTransform_.rotation_.y,worldTransform_.rotation_.z
	);
	ImGui::Text(
		"rotate %f  %f  %f", viewProjection_.rotation_.x, viewProjection_.rotation_.y,viewProjection_.rotation_.z
	);
	ImGui::Text(
		"velocity %f  %f  %f", velocity.x, velocity.y, velocity.z
	);
	ImGui::End();

	// 座標移動（ベクトルの加算）
	//worldTransform_.translation_ = Transform_Move(worldTransform_.translation_, move_);

	// 回転速さ[ラジアン/frame]
	//const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	/*if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}*/

	

	velocity = Normalize(velocity);
	velocity.x *= kBulletSpeed;
	velocity.y *= kBulletSpeed;
	velocity.z *= kBulletSpeed;

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, playerWorldTransform_.matWorld_);

	//worldTransform_.translation_.x += velocity.x;
	//worldTransform_.translation_.y += velocity.y;
	//worldTransform_.translation_.z += velocity.z;

	//worldTransform_.translation_ = Transform_Move(worldTransform_.translation_, velocity);

	//worldTransform_.translation_.z += move_.z;
	//move_ = TransformNormal(move_, worldTransform_.matWorld_);
	XINPUT_STATE joyState1;

	

	//.UpdateMatrix();
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.translation_);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);



};