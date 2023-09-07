#include "FollowCamera.h"

/// <summary>
/// 初期化
/// </summary>
void FollowCamera::Init() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

}

/// <summary>
/// 更新
/// </summary>
void FollowCamera::Update() {

	//追従対象があれば
	if (target_) {
		//追従カメラまでのオフセット
		Vector3 offset = { 0.0f,2.0f,-10.0f };

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_.x = target_->translation_.x + offset.x;
		viewProjection_.translation_.y = target_->translation_.y + offset.y;
		viewProjection_.translation_.z = target_->translation_.z + offset.z;
	}
	//ビュー行列の更新
	viewProjection_.UpdateViewMatrix();
	//ビュー行列の転送
	viewProjection_.TransferMatrix();
}
