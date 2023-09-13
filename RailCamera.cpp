#include "RailCamera.h"
#include <Input.h>
#include "function.h"
#include <ImGuiManager.h>

void RailCamera::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 10000;
	viewProjection_.Initialize();
	//worldTransform_.translation_ = target_->translation_;
	//worldTransform_.translation_ = target_->rotation_;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	Vector3 offset = { 0.0f,6.0f,-30.0f };

};

void RailCamera::Update() {

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat("%f", &move.z, 0.0f, 0.2f);
	ImGui::Text("rotation:(%f,%f,%f)", viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z);
	ImGui::Text("viewProjection_.rotation_(%f,%f,%f)", viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z);
	ImGui::Text("target_->translation_(%f,%f,%f)", target_->translation_.x, target_->translation_.y, target_->translation_.z);


	//worldTransform_.translation_ = target_->translation_;
	//worldTransform_.translation_ = target_->rotation_;


	

	Vector3 offset = { 0.0f,6.0f,-30.0f };

	//追従対象があれば
	if (target_) {
		//追従カメラまでのオフセット
		 //カメラの角度から回転行列を計算
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);
		// オフセットをカメラの回転に合わせて回転
		offset = TransformNormal(offset, rotateMatrix);
	//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_.x = target_->translation_.x + offset.x;
		viewProjection_.translation_.y = target_->translation_.y + offset.y;
		viewProjection_.translation_.z = target_->translation_.z + offset.z;
	}

	ImGui::Text("offset(%f,%f,%f)", offset.x, offset.y, offset.z);

	ImGui::End();


	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		viewProjection_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		viewProjection_.rotation_.y += kRotSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		viewProjection_.rotation_.x -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_S)) {
		viewProjection_.rotation_.x += kRotSpeed;
	}

	//-1.57で反転
	if (viewProjection_.rotation_.x <= -1.57) {
		viewProjection_.rotation_.x = -1.57f;
	}
	if(viewProjection_.rotation_.x >= 1.57) {
		viewProjection_.rotation_.x = 1.57f;
	}
	
	XINPUT_STATE joyState1;
	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState1)) {
		viewProjection_.rotation_.y += (float)joyState1.Gamepad.sThumbLX / SHRT_MAX * kRotSpeed;
		viewProjection_.rotation_.x -= (float)joyState1.Gamepad.sThumbLY / SHRT_MAX * kRotSpeed;
	}

//	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
    //カメラオブジェクトのワールド行列からビュー行列を計算する
//	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//ビュー行列の更新
	viewProjection_.UpdateViewMatrix();
	//ビュー行列の転送
	viewProjection_.TransferMatrix();

};
