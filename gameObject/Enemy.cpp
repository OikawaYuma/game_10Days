#include "Enemy.h"
#include "ImGuiManager.h"
#include<Input.h>
#include"Player.h"
void Enemy::Initialize(Vector3 translation) {
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("ENEMY.png");
	// 3Dモデルの生成
	model_ = Model::Create();
	

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = translation;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_;
	/*(this->*pMoveTable[0])();*/

	state = new EnemyStateApoorch();
}

void Enemy::Update() { state->Update(this); 
survivalTimer++;
	shotIntervalTimer_++;

if (deadTime <= survivalTimer) {
	isAlive = false;
}


worldTransform_.UpdateMatrix();

// デスフラグの立った弾を削除
//bullets_.remove_if([](EnemyBullet* bullet) {
//	if (bullet->GetIsDead()) {
//		delete bullet;
//		return true;
//	}
//	return false;
//});

//Fire();


 /*if (bullets_) {
	if (bullet_->GetIsDead()) {
		delete bullet_;
		bullet_ = nullptr;
		
	} else {

		bullet_->Update();
	}
	 
 }*/


}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	/*if (bullet) {
	bullet->Draw(viewProjection_);
	}*/
	
}
void Enemy::Move() {
	worldTransform_.translation_ = Transform_Move(worldTransform_.translation_, velocity_);
}



//
// void Enemy::MoveApproach() {
//	velocity_.z = -0.2f;
//	if (worldTransform_.translation_.z < 0.0f) {
//		phase_ = Phase::Leave;
//	}
//}
// void Enemy::MoveLeave() {
//	velocity_.z = 0;
//	velocity_.x = -0.2f;
//	velocity_.y = +0.2f;
//
//}

// setter
//  setter
void Enemy::SetVelo(Vector3 velocity) {
	velocity_.x = velocity.x;
	velocity_.y = velocity.y;
	velocity_.z = velocity.z;
}

void Enemy::SetShotInterval(int32_t shotIntervalTimer) { shotIntervalTimer_ = shotIntervalTimer; };

// staticで宣言したメンバ関数ポインタテーブルの実態
// void (Enemy::*Enemy::pMoveTable[])() = {&Enemy::MoveApproach, &Enemy::MoveLeave};

// state変更用の関数

void Enemy::ChangeState(BaseEnemyState* newState) { state = newState; }

void EnemyStateApoorch::Update(Enemy* pEnemy) {

	pEnemy->SetVelo({0, 0, -0.4f});
	pEnemy->Move();
	//pEnemy->SetShotInterval(0);

	if (pEnemy->GetTranslation().z <= 0) {
		pEnemy->ChangeState(new EnemyStateLeave());
		pEnemy->SetShotInterval(0);
	}
	/*
	switch (phase_) {
	case Phase::Approach:
	default:
	    MoveApproach();
	    break;
	case Phase::Leave:
	    MoveLeave();
	    break;
	}*/

	
}

void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelo({-0.2f, 0.2f, -2.2f});
	pEnemy->Move();
	
}



void Enemy::Fire() { 
	if (++shotIntervalTimer_ >= kFireInterval) {
		
		// 弾があれば破棄する
		/*if (bullet_) {

		    delete bullet_;
		    bullet_ = nullptr;
		}*/
		assert(player_);
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		
		Vector3 start = GetWorldPosition();
		Vector3 end = player_->GetWorldPosition();
		
		Vector3 diffVector;
		diffVector.x = end.x - start.x;
		diffVector.y = end.y - start.y;
		diffVector.z = end.z - start.z;
		
		diffVector = Normalize(diffVector);
		diffVector.x *= kBulletSpeed;
		diffVector.y *= kBulletSpeed;
		diffVector.z *= kBulletSpeed;

		Vector3 velocity(diffVector.x, diffVector.y, diffVector.z);
		ImGui::Begin("Debug5");
		ImGui::Text("bullet :%f\n:%f\n:%f\n", diffVector.x, diffVector.y, diffVector.z);
		ImGui::End();
		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->SetPlayer(player_);
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// 弾を登録する
		gameScene_->AddEnemyBullet(newBullet);
		shotIntervalTimer_ = 0;

	}
	ImGui::Begin("Debug4");
	ImGui::Text("bullet :%d\n", shotIntervalTimer_);
	
	ImGui::End();
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// 衝突を検出したらコールバック関数
void Enemy::OnCollision() {  isAlive = false; };