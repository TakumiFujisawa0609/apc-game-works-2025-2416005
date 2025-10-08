#include <cmath>
#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage/Stage.h"
#include "TitleScene.h"
#include "../Object/Player/Player.h"
#include "GameScene.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Weapon/Weapon.h"

GameScene::GameScene(void) : SceneBase()
{
	grid_ = nullptr;
	SetMouseDispFlag(false);
}

GameScene::~GameScene(void)
{

}

void GameScene::Init(void)
{
	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);

	// グリッド初期化
	grid_ = new Grid();
	grid_->Init();

	//// ステージ初期化
	Stage::CreateInstance();
	Stage::GetInstance()->Init();

	// プレイヤー初期化
	Player::CreateInstance();
	Player::GetInstance()->Init();

	weapon_ = new Weapon();
	weapon_->Init();

	enemy_ = new EnemyManager();
	EnemyManager::SetInstance(enemy_);
	enemy_->Spawn(0.0f, 0.0f, 50.0f);
}

void GameScene::Update(void)
{
	// グリッド更新
	grid_->Update();

	// ステージ更新
	Stage::GetInstance()->Update();

	// プレイヤー更新
	Player::GetInstance()->Update();

	weapon_->Update();

	enemy_->Update();
}

void GameScene::Draw(void)
{
	// グリッド描画
	grid_->Draw();

	// ステージ描画
	Stage::GetInstance()->Draw();

	// プレイヤー描画
	Player::GetInstance()->Draw();

	weapon_->Draw();

	enemy_->Draw();
}

void GameScene::Release(void)
{
	// グリッド解放
	grid_->Release();
	delete grid_;

	// ステージ解放
	Stage::GetInstance()->Release();
	Stage::DeleteInstance();

	// プレイヤー解放
	Player::GetInstance()->Release();
	Player::DeleteInstance();

	weapon_->Release();
	delete weapon_;

	enemy_->Release();
	EnemyManager::SetInstance(nullptr);
}
