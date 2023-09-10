#include "Player.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "function.h"
#include <Input.h>
#include <cassert>

#include"AxisIndicator.h"

Player::~Player() {
	delete sprite2DReticle_;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
		
	};
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {

	// NULLポインタチェック
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.UpdateMatrix();

	CameraworldTransform_.Initialize();

	// 3Dレティクルのワールドトラベル初期化
	worldTransform3DReticle_.Initialize();

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("TITLE.png");

	// スプライト生成
	sprite2DReticle_ =
	    Sprite::Create(textureReticle, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	CameraworldTransform_.translation_.x = worldTransform_.translation_.x;
	CameraworldTransform_.translation_.y = worldTransform_.translation_.y + 5;
	CameraworldTransform_.translation_.z = worldTransform_.translation_.z - 10;


}

Vector3 Player::Rotate(Vector3 rot) { return rot; };

void Player::Update(ViewProjection viewProjection) {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	worldTransform3DReticle_.TransferMatrix();
	CameraworldTransform_.TransferMatrix();
	CameraworldTransform_.UpdateMatrix();
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.5f;

	// キャラの移動
	Vector3 kVelocity(0, 0, kCharacterSpeed);

	kVelocity = Normalize(kVelocity);
	kVelocity.x *= kCharacterSpeed;
	kVelocity.y *= kCharacterSpeed;
	kVelocity.z *= kCharacterSpeed;

	kVelocity = TransformNormal(kVelocity, worldTransform_.matWorld_);

	move.x += kVelocity.x;
	move.y += kVelocity.y;
	move.z += kVelocity.z;

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	{
		// 押した方向で移動ベクトルを変更（左右）
		if (input_->PushKey(DIK_LEFT)) {
			move.x -= kCharacterSpeed;
		} else if (input_->PushKey(DIK_RIGHT)) {

			move.x += kCharacterSpeed;
		}
		if (input_->PushKey(DIK_UP)) {
			move.z += kCharacterSpeed;
		} else if (input_->PushKey(DIK_DOWN)) {

			move.z -= kCharacterSpeed;
		}

		// ゲームパッドの状態を得る変数（XINPUT）
		XINPUT_STATE joyState;

		// ゲームパッド状態取得
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
			move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
		}
		Vector3 tmpPlayerPos = worldTransform_.translation_;
		// 座標移動（ベクトルの加算）
		worldTransform_.translation_ = Transform_Move(worldTransform_.translation_, move);

		CameraworldTransform_.translation_ =
		    Transform_Move(CameraworldTransform_.translation_, move);

		// キーボード入力による移動処理

		// 移動限界座標
		// const float kMoveLimitX = 50;
		// const float kMoveLimitY = 30;

		//// 範囲を超えない処理
		// worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
		// worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);

		// worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
		// worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

		// 回転速さ[ラジアン/frame]
		const float kRotSpeed = 0.02f;

		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_A)) {
			worldTransform_.rotation_.y -= kRotSpeed;
		} else if (input_->PushKey(DIK_D)) {
			worldTransform_.rotation_.y += kRotSpeed;
		}
		if (input_->PushKey(DIK_W)) {
			worldTransform_.rotation_.x -= kRotSpeed;
		} else if (input_->PushKey(DIK_S)) {
			worldTransform_.rotation_.x += kRotSpeed;
		}
		worldTransform_.UpdateMatrix();

		// ビューポート
		Matrix4x4 matViewport =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		// スプライトの現在座標を取得
		Vector2 spritePosition = sprite2DReticle_->GetPosition();

		XINPUT_STATE joyState1;

		// ジョイスティック状態取得
		if (Input::GetInstance()->GetJoystickState(0, joyState1)) {
			spritePosition.x += (float)joyState1.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
			spritePosition.y -= (float)joyState1.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
			// スプライトの座標変更を繁栄
			sprite2DReticle_->SetPosition(spritePosition);
		}
		/*--------合成行列の逆行列--------------*/
		// ビュープロジェクションビューポート合成行列
		Matrix4x4 matVPV =
		    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
		// 合成行列の逆行列を計算する

		Matrix4x4 matInverseVPV = Inverse(matVPV);

		/*--------2点のワールド行列--------------*/
		// スクリーン座標
		Vector3 posNear = Vector3(static_cast<float>(spritePosition.x), (float)spritePosition.y, 0);
		Vector3 posFar = Vector3(static_cast<float>(spritePosition.x), float(spritePosition.y), 1);
		// スクリーン座標系からワールド座標系へ
		posNear = Transform(posNear, matInverseVPV);
		posFar = Transform(posFar, matInverseVPV);

		/*---------3Dレティクルの座標系さん-------*/
		// スティックレイの方向
		Vector3 spriteDierection;
		spriteDierection.x = posFar.x - posNear.x;
		spriteDierection.y = posFar.y - posNear.y;
		spriteDierection.z = posFar.z - posNear.z;
		spriteDierection = Normalize(spriteDierection);
		// カメラから照準オブジェクトの距離
		const float kDistanceTextObject = 100.0f;
		worldTransform3DReticle_.translation_.x = spriteDierection.x * kDistanceTextObject;
		worldTransform3DReticle_.translation_.y = spriteDierection.y * kDistanceTextObject;
		worldTransform3DReticle_.translation_.z = spriteDierection.z * kDistanceTextObject;

		worldTransform3DReticle_.UpdateMatrix();

		ImGui::Begin("Player");
		ImGui::Text("2DReticle:(%2.2f,%2.2f)", spritePosition.x, spritePosition.y);
		ImGui::Text("Near:(%2.2f,%2.2f)", posNear.x, posNear.y);
		ImGui::Text("Far:(%2.2f,%2.2f)", posFar.x, posFar.y);
		ImGui::Text(
		    "3DReticle:(%2.2f,%2.2f,%2.2f)", worldTransform3DReticle_.translation_.x,
		    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);

		ImGui::Text(
		    " player : (% 2.2f,%2.2f,%2.2f)", worldTransform_.translation_.x,
		    worldTransform_.translation_.y, worldTransform_.translation_.z);
		ImGui::End();

		ImGui::Begin("Debug1");
		ImGui::Text("%f", worldTransform_.matWorld_.m[3][2]);
		ImGui::End();

		// キャラクター攻撃処理
		Attack();
		int i = 0;
		Vector3 tmpPlayerBodyPos = worldTransform_.translation_;
		//// 弾更新
		for (PlayerBullet* bullet : bullets_) {
			bullet->Update(GetWorldPosition());
			i++;
		}
	}

	//std::list<PlayerBullet*>::iterator *itr = bullets_.begin();
	//for (int i = 0; i <= bullets_.size(); ++i) {
	//	*itr = PlayerBullet::Update2()

	//}


	// if (bullet_) {
	//	bullet_->Update();
	// }
}
void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
	/*if (bullet_) {
	bullet_->Draw(viewProjection_);
	}*/
}
void Player::Create(){
	
	{
		// 弾があれば破棄する
		/*if (bullet_) {
		    delete bullet_;
		    bullet_ = nullptr;
		}*/
		// 弾の速度
		const float kBulletSpeed = 0.5f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		velocity.x = 0;
		velocity.y = 0;
		velocity.z = 0;

		
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();

		// 弾を登録する
		
		newBullet->Initialize(
		    model_, {GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z - 10});
		bullets_.push_back(newBullet);
	}

	// ゲームパッドを押したときの処理
	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 弾があれば破棄する
		/*if (bullet_) {
		    delete bullet_;
		    bullet_ = nullptr;
		}*/
		// 弾の速度
		const float kBulletSpeed = 0.5f;
		Vector3 velocity(0, 0, 0);

		// 自機から照準オブジェクトへのベクトル
		velocity.x = GetReticleWorldPosition().x - GetWorldPosition().x;
		velocity.y = GetReticleWorldPosition().y - GetWorldPosition().y;
		velocity.z = GetReticleWorldPosition().z - GetWorldPosition().z;

		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition());

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
};
void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾があれば破棄する
		/*if (bullet_) {
		    delete bullet_;
		    bullet_ = nullptr;
		}*/
		// 弾の速度
		const float kBulletSpeed = 0.5f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		velocity.x = worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x;
		velocity.y = worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y;
		velocity.z = worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z;

		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
	

		// 弾を登録する
		bullets_.push_back(newBullet);
		newBullet->Initialize(
		    model_, {GetWorldPosition().x, GetWorldPosition().y,
		             GetWorldPosition().z -10});
	}

	// ゲームパッドを押したときの処理
	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 弾があれば破棄する
		/*if (bullet_) {
		    delete bullet_;
		    bullet_ = nullptr;
		}*/
		// 弾の速度
		const float kBulletSpeed = 0.5f;
		Vector3 velocity(0, 0, 0);

		// 自機から照準オブジェクトへのベクトル
		velocity.x = GetReticleWorldPosition().x - GetWorldPosition().x;
		velocity.y = GetReticleWorldPosition().y - GetWorldPosition().y;
		velocity.z = GetReticleWorldPosition().z - GetWorldPosition().z;

		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition());

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() {
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetReticleWorldPosition() {
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision(){

};

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
	worldTransform3DReticle_.parent_ = parent;
}

void Player::DrawUI() {

	// スプライト生成
	sprite2DReticle_->Draw();
}