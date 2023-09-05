#pragma once
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include <cassert>
#include <input.h>
#include <list>
#include"GameScene.h"

class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </ summary>
	void Initialize(Model* model, uint32_t textureHandle,Vector3 pos);

	Vector3 Rotate(Vector3 rot);

	/// <summary>
	/// 初期化
	/// </ summary>
	void Update(ViewProjection viewProjection);

	/// <summary>
	/// 初期化
	/// </ summary>
	void Draw(ViewProjection);
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();


	/*----------------------------------
	        Getter
	-----------------------------------*/
	Vector3 GetWorldPosition();
	Vector3 GetReticleWorldPosition();

	int GetRadius() { return radius_; }

	const WorldTransform* GetParent() { return worldTransform_.parent_; }
	/*----------------------------------
	         Setter
	----------------------------------*/
	/// <summary>
	/// 親となるワールドトランスフォームセット
	/// </summary>
	///	<parma name="parent">親となるワールドトランスフォーム<.param>
	void SetParent(const WorldTransform* parent);

	
	Vector3 operator=(const Vector3& m) { return m; }
	Vector2 operator=(const Vector2& m) { return m; }

	// 衝突を検出したらコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& Getbullet() const { return bullets_; }
	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	int radius_ = 1;

	// キーボード入力
	Input* input_ = nullptr;
	// 弾
	std::list<PlayerBullet*> bullets_;

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

};
