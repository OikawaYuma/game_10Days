#pragma once
#include "EnemyBullet.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "function.h"
#include <Input.h>
#include <cassert>
#include<list>
class GameScene;
class Player;
class DrapBody {
public:
	/*Enemy();
	~Enemy();*/
	void Initialize(Vector3 translation);
	void Update();
	void Draw(ViewProjection viewProjection);
	void Move();
	// void MoveApproach();
	// void MoveLeave();
	// getter
	Vector3 GetVelo() { return velocity_; }

	Vector3 GetTranslation() { return worldTransform_.translation_; }

	bool GetIsAlive() { return isAlive; };

	

	
	void SetPlayer(Player* player) { player_ = player; }
	WorldTransform GetWorldTransform() { return worldTransform_; }
	Vector3 GetWorldPosition();
	int GetRadius() { return radius_; }
	// 衝突を検出したらコールバック関数
	void OnCollision();
	void OnCollision2();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	// ViewProjection viewProjection_;

	// モデルのポインタ
	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 行動フェーズ
	// enum class Phase {
	//	Approach, // 接近する
	//	Leave     // 離脱する
	//};

	// フェーズ
	/*Phase phase_ = Phase::Approach;*/

	// メンバポインタ関数のテーブル
	/*static void (Enemy::*pMoveTable[])();*/
	int radius_ = 4;

	// キーボード入力
	Input* input_ = nullptr;



	// 弾
	// std::list<EnemyBullet*> bullets_;

	// 生きているか
	bool isAlive = true;

	// 死ぬまでの時間
	int survivalTimer = 0;

	// 死ぬ時間
	const int deadTime = 1000;



	// 自キャラ
	Player* player_ = nullptr;
	// ゲームシーン
	GameScene* gameScene_ = nullptr;

};
