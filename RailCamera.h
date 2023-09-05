#pragma once

#include <ViewProjection.h>
#include <WorldTransform.h>
#include <input.h>
class RailCamera {
public:
	//~RailCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 pos, Vector3 rot);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; };
	const WorldTransform& GetWorldTransform() { return worldTransform_; };

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// キーボード入力
	Input* input_ = nullptr;

	Vector3 move;
};
