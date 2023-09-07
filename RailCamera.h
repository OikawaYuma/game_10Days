#pragma once

#include <ViewProjection.h>
#include <WorldTransform.h>
#include <input.h>
#include "function.h"


class RailCamera {
public:
	//~RailCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; };
	//const WorldTransform& GetWorldTransform() { return worldTransform_; };

	void SetworldTransform_(const WorldTransform* target) {  target_= target; }

private:
	// ワールド変換データ
	const WorldTransform* target_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// キーボード入力
	Input* input_ = nullptr;

	Vector3 move = { 0,0,0.0 };
};
