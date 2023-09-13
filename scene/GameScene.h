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
#include "DrapBody.h"
#include"Skydome.h"
#include "Floor.h"
#include"RailCamera.h"
#include "Result.h"
#include<sstream>
#include "Title.h"
#include "Time2.h"


enum class Phase {
	TITEL,  // タイトル
	SETUMEI,
	PLAY,   // プレイ
	RESULT, // リザルト
	POSE,   // ポーズ
};


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

	const std::list<DrapBody*>& GetDrapBody() const { return drapBodys_; }
	const std::list<PlayerBullet*>& GetPlayerBullet() const { return playerBullets_; }
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
	// 音声ハンドル
	/// TITLE
	uint32_t BGMth_T = 0;
	uint32_t BGMth_Tr = 0;

	bool flag_T = true;

	/// Play
	uint32_t BGMth_P = 0;
	uint32_t BGMth_Pr = 0;

	bool flag_P = true;

	float playerTime = 0;

	///RESULT
	uint32_t BGMth_R = 0;
	uint32_t BGMth_Rr = 0;

	bool flag_R = true;

	//スプライト
	Sprite* sprite_ = nullptr;

	//3Dモデルデータ
	Model* model_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;
	//自キャラ
	Player* player_ = nullptr;
	const std::list<PlayerBullet*> playerBullets_;
	/*-----敵のリスト管理--------*/
	////敵キャラ
	Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemys_;

	Title* title_ = nullptr;
	Time2* time_ = nullptr;

	//スポーンタイマー
	int respownTimer_ = 0;


	// DrapBodyAppearTimer
	int DrapBodyAppearTimer_ = 60;

	//DrapBody 
	DrapBody* drapBody_ = nullptr;
	std::list<DrapBody*> drapBodys_;
	int randBodyPosX_;
	int randBodyPosY_;
	int randBodyPosZ_;


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

	// Title


	//	レールカメラ
	RailCamera* railCamera_ = nullptr;

	// Result
	Result* result_ = nullptr;

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

	// フェーズ
	Phase phase_ = Phase::TITEL;
	//ゲームタイマー
	int32_t gameTimerRimit = 60 * 10;
	int32_t gameTimer = 0;
	
};
