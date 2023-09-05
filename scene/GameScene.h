#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include"function.h"
#include<list>

#include "Player.h"
#include"Enemy.h"
#include"EnemyBullet.h"
#include"Skydome.h"
#include "Floor.h"
#include"RailCamera.h"

#include<sstream>
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetFlag(bool &flag);

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();
	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name= "enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// 敵リストを取得
	const std::list<Enemy*>& GetEnemy() const { return enemys_; };
		// 弾リストを取得
	const std::list<EnemyBullet*>& Getbullet() const { return enemyBullets_; }
	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	void enemyAppear(Vector3 translation);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// テクスチャハンドル
	uint32_t playerTh_;

	//スプライト
	Sprite* sprite_ = nullptr;

	//3Dモデルデータ
	Model* model_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;
	//自キャラ
	Player* player_ = nullptr;

	/*-----敵のリスト管理--------*/
	////敵キャラ
	Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemys_;

	

	//スポーンタイマー
	int respownTimer_ = 0;

	// スポーンフラグ
	bool isRespown = true;

	//弾
	EnemyBullet* enemybullet_ = nullptr;
	// 弾リスト
	std::list<EnemyBullet*> enemyBullets_;
	//天球
	Skydome* skydome_ = nullptr;

	//床
	Floor* floor_ = nullptr;

	//	レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 3Dモデル
	Model* modelSkydome_ = nullptr;

	// 3Dモデル
	Model* modelFloor_ = nullptr;

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;


	
};
