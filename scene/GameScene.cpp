#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>
#include <fstream>
#include <math.h>



GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete player_;
	delete debugCamera_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialize() {
	// farZの変更
	viewProjection_.farZ = 1000.0f;
	viewProjection_.Initialize();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// レティクルのテクスチャ
	TextureManager::Load("Reticle.png");
	playerTh_ = TextureManager::Load("PLAYER.png");
	sprite_ = Sprite::Create(playerTh_, {100, 50});

	// BGM
	BGMth_P = audio_->LoadWave("Low_frequency_porter.mp3");
	BGMth_T = audio_->LoadWave("Phone_Thinker.mp3");
	BGMth_R = audio_->LoadWave("Dance.mp3");
	// 3Dモデルの生成
	model_ = Model::Create();

	// 自キャラの生成
	player_ = new Player();
	
	// pos設定
	Vector3 playerPos = {0, 0, -300};
	// 自キャラの初期化
	player_->Initialize(model_, playerTh_, playerPos);
	// レールカメラの生成
	railCamera_ = new RailCamera;
	// レールカメラの初期化
	railCamera_->Initialize();
	player_->SetViewProjection(&railCamera_->GetViewProjection());
	railCamera_->SetworldTransform_(&player_->GetWorldTransform());
	title_ = new Title;
	title_->Initialize();
	time_ = new Time2;
	time_->Initialize();

	// 自キャラとレールカメラの親子関係を結ぶ
	// player_->SetParent(&railCamera_->GetWorldTransform());
	LoadEnemyPopData();
	// 敵弾の生成
	// EnemyBullet* newBullet = new EnemyBullet;
	// newBullet->Initialize(model_, enemy_->GetTranslation(), newBullet->GetVelo());

	// 天球の生成
	skydome_ = new Skydome;

	// 床の生成
	floor_ = new Floor;

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 3Dモデルの生成
	modelFloor_ = Model::CreateFromOBJ("Floor", true);

	// 天球の初期化
	skydome_->Initialize(modelSkydome_);

	// 床の初期化
	floor_->Initialize(modelFloor_);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	// ゲームパッドを押したときの処理
	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

		skydome_->Update();
	//floor_->Update();
	
	switch (phase_) {
	///-----------TITLE-----------///
	case Phase::TITEL:
		if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) ) {
			phase_ = Phase::SETUMEI;
		}

		flag_R = true;
		playerTime = 0;
		if (flag_T == true) {
			BGMth_Tr = audio_->PlayWave(BGMth_T, true, 0.5f);
			flag_T = false;
		}
		if (input_->PushKey(DIK_SPACE)) {
			phase_ = Phase::PLAY;
		}


		break;
	case Phase::SETUMEI:
		
		if (input_->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::PLAY;
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			audio_->StopWave(BGMth_Tr);
			phase_ = Phase::PLAY;

		}
		break;

	///-----------PLAY-----------///
	case Phase::PLAY:
		flag_T = true;
		playerTime++;
		if (playerTime >= 50) {
			if (flag_P == true) {
				BGMth_Pr = audio_->PlayWave(BGMth_P, true, 0.35f);
				flag_P = false;
			}
		}
		// デスフラグの立った弾を削除
		enemyBullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->GetIsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});

		// デスフラグの立った弾を削除
		enemys_.remove_if([](Enemy* enemy) {
			if (!enemy->GetIsAlive()) {
				delete enemy;
				return true;
			}
			return false;
		});

		// デスフラグが立った落ちてる体を削除
		drapBodys_.remove_if([](DrapBody* dropBody) {
			if (!dropBody->GetIsAlive()) {
				delete dropBody;
				return true;
			}
			return false;
		});
	
		
		gameTimer++;
		time_->Update(gameTimer);
	
		if (gameTimer >= gameTimerRimit) {
			result_ = new Result;
			const std::list<PlayerBullet*>& playerBullets2 = player_->Getbullet();
			result_->Initialize((int)playerBullets2.size());
			
			phase_ = Phase::RESULT;
			
		}

		UpdateEnemyPopCommands();

		
		// player_->SetParent(&railCamera_->GetWorldTransform());
		//  自キャラとレールカメラの親子関係を結ぶ

		/*-------------------------------------------
		           DrapBody
		------------------------------------------*/
		DrapBodyAppearTimer_++;

		if (DrapBodyAppearTimer_ >= 50) {
			DrapBody* newDrapBody = new DrapBody;
			newDrapBody->SetPlayer(player_);
			drapBodys_.push_back(newDrapBody);
			randBodyPosX_ = rand() % 300 - 150;
			randBodyPosY_ = rand() % 300 - 150;
			randBodyPosZ_ = rand() % 300 - 150;
			Vector3 randDBodyPos = {
			    (float)randBodyPosX_, (float)randBodyPosY_, (float)randBodyPosZ_};
			newDrapBody->Initialize(randDBodyPos);

			DrapBodyAppearTimer_ = 0;
		}
		for (DrapBody* drapBody : drapBodys_) {
			ImGui::Begin("DrapBody");
			ImGui::Text("DrapBodyT :%d", drapBody->GetWorldPosition().x);
			ImGui::End();
			drapBody->Update();
		}
		// 自キャラの更新あ
		player_->Update(viewProjection_);

		// 敵キャラの更新
		for (Enemy* enemy : enemys_) {
			enemy->Update();

			ImGui::Begin("Debug5");
			ImGui::Text("bullet :%d", enemy->GetShotTimer());
			ImGui::End();
			// enemy->Fire();
			if (enemy->GetShotTimer() >= enemy->kFireInterval) {
				assert(player_);
				// 弾の速度
				const float kBulletSpeed = 1.0f;

				Vector3 start = enemy->GetWorldPosition();
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

				// 速度ベクトルを自機の向きに合わせて回転させる
				velocity = TransformNormal(velocity, enemy->GetWorldTransform().matWorld_);

				// 弾を生成し、初期化
				EnemyBullet* newBullet = new EnemyBullet();
				newBullet->Initialize(model_, enemy->GetWorldTransform().translation_, velocity);
				newBullet->SetPlayer(player_);
				// 弾を登録する
				enemyBullets_.push_back(newBullet);
				enemy->SetShotInterval(0);
			}
		}

		// 弾更新
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Update();
		}

		CheckAllCollision();

#ifdef _DEBUG
		if (input_->TriggerKey(DIK_Q) && isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		}
#endif
		railCamera_->Update();

		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

		// カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->SetFarZ(100000.0f);
			// デバックカメラの更新
			debugCamera_->Update();

			// デバッグカメラの更新
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			// viewProjection_.UpdateMatrix();
		}
		/*float a = player_->GetParent()->parent_->matWorld_.m[3][2];
		float b = player_->GetWorldPosition().z;
		ImGui::Begin("Debug2");
		ImGui::Text("%f  ,  %f", a,b);
		ImGui::End();*/
		break;
	case Phase::RESULT:
		audio_->StopWave(BGMth_Pr);
		if (flag_R == true) {
			BGMth_Rr = audio_->PlayWave(BGMth_R, true, 0.4f);
			flag_R = false;
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			
			// プレイ時間の初期化
			gameTimer = 0;
			time_->Initialize();
			player_->Initialize(model_, playerTh_, {0,0,-300});
			railCamera_->Initialize();
			//出ている弾の初期化
			for (EnemyBullet* bullet : enemyBullets_) {
				delete bullet;
			}
			//body delete
			for (DrapBody* drapbody : drapBodys_) {
				drapbody->OnCollision2();
			}
			// デスフラグが立った落ちてる体を削除
			drapBodys_.remove_if([](DrapBody* dropBody) {
				if (!dropBody->GetIsAlive()) {
					delete dropBody;
					return true;
				}
				return false;
			});
			// 自弾リストの取得
			const std::list<PlayerBullet*>& playerBullets3 = player_->Getbullet();
			for (PlayerBullet* bullet : playerBullets3) {
				bullet->OnCollision();
			}
			player_->bulletDelet();
		
			audio_->StopWave(BGMth_Rr);
			flag_P = true;



			phase_ = Phase::TITEL;
		}
		break;




	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	
	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	// sprite_->Draw();
	
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	skydome_->Draw(viewProjection_);

	// floor_->Draw(viewProjection_);
	//  自キャラの描画
	player_->Draw(viewProjection_);

	// 敵キャラの描画
	// 敵弾の描画
	switch (phase_) {
	case Phase::PLAY:
		for (Enemy* enemy : enemys_) {
			enemy->Draw(viewProjection_);
		}

		// 敵弾の描画
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Draw(viewProjection_);
		}

		for (DrapBody* drapBody : drapBodys_) {
			drapBody->Draw(viewProjection_);
		}
		break;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);
	
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	switch (phase_) {
	///-----------TITLE-----------///
	case Phase::TITEL:
		title_->DrawUI();
		break;
	case Phase::SETUMEI:
		title_->DrawSetumei();
		break;
	case Phase::PLAY:
		time_->DrawUI();
		break;
	case Phase::RESULT:

		result_->Draw();
		break;
	}
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	// 判定対象AとBの座標
	Vector3 posA, posB;
	int radiusA, radiusB;

	// 敵リストの取得
	const std::list<Enemy*>& enemys = GetEnemy();

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->Getbullet();

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = Getbullet();

	// 落ちている体の取得
	const std::list<DrapBody*>& drapBody = GetDrapBody();



#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	radiusA = player_->GetRadius();
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		float p2b = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		            (posB.z - posA.z) * (posB.z - posA.z);
		radiusB = bullet->GetRadius();
		int r2r = (radiusA + radiusB) * (radiusA + radiusB);

		if (p2b <= r2r) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 自キャラの衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion
#pragma region 自キャラと落ちている体の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	radiusA = player_->GetRadius();
	// 自キャラと敵弾全ての当たり判定
	for (DrapBody* drapBody1 : drapBody) {
		// 敵弾の座標
		posB = drapBody1->GetWorldPosition();
		float p2b = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		            (posB.z - posA.z) * (posB.z - posA.z);
		radiusB = drapBody1->GetRadius();
		int r2r = (radiusA + radiusB) * (radiusA + radiusB);

		if (p2b <= r2r) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 自キャラの衝突時コールバックを呼び出す
			drapBody1->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 自キャラと敵弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		for (Enemy* enemy : enemys) {

			// 敵の座標
			posA = enemy->GetWorldPosition();
			radiusA = enemy->GetRadius();
			// 敵弾の座標

			posB = bullet->GetWorldPosition();
			float e2b = (posB.x - posA.x) * (posB.x - posA.x) +
			            (posB.y - posA.y) * (posB.y - posA.y) +
			            (posB.z - posA.z) * (posB.z - posA.z);
			radiusB = bullet->GetRadius();
			int r2r = (radiusA + radiusB) * (radiusA + radiusB);

			if (e2b <= r2r) {
				// 自キャラの衝突時コールバックを呼び出す
				// enemy_->OnCollision();
				// 自キャラの衝突時コールバックを呼び出す
				bullet->OnCollision();
				enemy->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 自キャラと敵弾全ての当たり判定
	for (PlayerBullet* bulletA : playerBullets) {
		for (EnemyBullet* bulletB : enemyBullets) {
			// 自弾の座標
			posA = bulletA->GetWorldPosition();
			radiusA = bulletA->GetRadius();
			// 敵弾の座標
			posB = bulletB->GetWorldPosition();
			radiusB = bulletB->GetRadius();
			float a2b = (posB.x - posA.x) * (posB.x - posA.x) +
			            (posB.y - posA.y) * (posB.y - posA.y) +
			            (posB.z - posA.z) * (posB.z - posA.z);

			int r2r = (radiusA + radiusB) * (radiusA + radiusB);

			if (a2b <= r2r) {
				// 自弾の衝突時コールバックを呼び出す
				bulletA->OnCollision();
				// 敵キャラの衝突時コールバックを呼び出す
				bulletB->OnCollision();
			}
		}
	}

#pragma endregion 自キャラと自弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	radiusA = player_->GetRadius();
	// 自キャラと敵弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		float p2b = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		            (posB.z - posA.z) * (posB.z - posA.z);
		radiusB = bullet->GetRadius();
		int r2r = (radiusA + radiusB) * (radiusA + radiusB);

		if (p2b <= r2r) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 自キャラの衝突時コールバックを呼び出す
			//bullet->OnCollision();
		}
	}


}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;
	file.open("enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isRespown) {
		respownTimer_--;
		if (respownTimer_ <= 0) {
			// 待機完了
			isRespown = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変婚して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			enemyAppear({x, y, z});

		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());
			// 待機開始
			isRespown = true;
			respownTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

// 敵の出現
void GameScene::enemyAppear(Vector3 translation) {
	// 敵キャラの生成
	Enemy* newEnemy = new Enemy();
	// 敵キャラの初期化
	newEnemy->Initialize(translation);
	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);
	enemys_.push_back(newEnemy);
};
