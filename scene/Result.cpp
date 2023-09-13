#include "Result.h"
#include <GameScene.h>

Result::~Result() { delete sprite2DResult_; }
Result::Result(){};
void Result::Initialize(int num) {

	int ResultScore = num;

	int scoreTen = ResultScore / 10; 
	int scoreNum = ResultScore % 10; 
	// レティクル用テクスチャ取得
	uint32_t textureResult = TextureManager::Load("result.png");
	if (scoreTen == 0) {	textureTen_ = TextureManager::Load("0.png");}
	if (scoreTen == 1) {	textureTen_ = TextureManager::Load("1.png");}
	if (scoreTen == 2) {	textureTen_ = TextureManager::Load("2.png");}
	if (scoreTen == 3) {	textureTen_ = TextureManager::Load("3.png");}
	if (scoreTen == 4) {	textureTen_ = TextureManager::Load("4.png");}
	if (scoreTen == 5) {	textureTen_ = TextureManager::Load("5.png");}
	if (scoreTen == 6) {	textureTen_ = TextureManager::Load("6.png");}
	if (scoreTen == 7) {	textureTen_ = TextureManager::Load("7.png");}
	if (scoreTen == 8) {	textureTen_ = TextureManager::Load("8.png");}
	if (scoreTen == 9) {	textureTen_ = TextureManager::Load("9.png");}

	if (scoreNum == 0) {	textureNum_ = TextureManager::Load("0.png");}
	if (scoreNum == 1) {	textureNum_ = TextureManager::Load("1.png");}
	if (scoreNum == 2) {	textureNum_ = TextureManager::Load("2.png");}
	if (scoreNum == 3) {	textureNum_ = TextureManager::Load("3.png");}
	if (scoreNum == 4) {	textureNum_ = TextureManager::Load("4.png");}
	if (scoreNum == 5) {	textureNum_ = TextureManager::Load("5.png");}
	if (scoreNum == 6) {	textureNum_ = TextureManager::Load("6.png");}
	if (scoreNum == 7) {	textureNum_ = TextureManager::Load("7.png");}
	if (scoreNum == 8) {	textureNum_ = TextureManager::Load("8.png");}
	if (scoreNum == 9) {	textureNum_ = TextureManager::Load("9.png");}
	

	// スプライト生成
	sprite2DResult_ =
	    Sprite::Create(textureResult, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	// スプライト生成
	sprite2DTen_ =
	    Sprite::Create(textureTen_, {590, 340}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});// スプライト生成
	sprite2DNum_ =
	    Sprite::Create(textureNum_, {690, 340}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
};
void Result::Update(){};
void Result::Draw(){
	// スプライト生成
	sprite2DResult_->Draw();
	sprite2DTen_->Draw();
	sprite2DNum_->Draw();
};
