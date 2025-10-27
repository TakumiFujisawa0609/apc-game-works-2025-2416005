#include <cmath>
#include <DxLib.h>
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage/Stage.h"
#include "TitleScene.h"
#include "../Object/Player/Player.h"
#include "GameScene.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Weapon/Weapon.h"
#include "../Scene/SceneManager.h"
#include "../Object/Stage/Field/FieldManager.h"

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

}

void GameScene::Update(void)
{
	// ステージ更新
	Stage::GetInstance()->Update();

	// フィールドの更新
	fieldManager_->Update();

	// プレイヤー更新
	player_->Update();

	//enemy_->Update();
}

void GameScene::Load(void)
{
	// プレイヤー初期化
	player_ = new Player();
	player_->Init();

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance()->GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);

	// ステージ初期化
	Stage::CreateInstance();
	Stage::GetInstance()->Init();
	Stage::GetInstance()->SetPlayer(player_);

	// フィールドの初期化
	fieldManager_ = new FieldManager();
	fieldManager_->Init();

	// 敵初期化
	//enemy_ = new EnemyManager(player_);
	//EnemyManager::SetInstance(enemy_);
	//enemy_->Init(0.0f, 0.0f, 50.0f);
}

void GameScene::LoadEnd(void)
{

}

void GameScene::Draw(void)
{

	Camera* camera = SceneManager::GetInstance()->GetCamera();
	camera->SetBeforeDraw();

	// ステージ描画
	Stage::GetInstance()->Draw();

	// フィールド描画
	fieldManager_->Draw();

	// 敵描画
	//enemy_->Draw();

	// プレイヤー描画
	player_->Draw();

	camera->DrawDebug();
}

void GameScene::Release(void)
{
	// ステージ解放
	Stage::GetInstance()->Release();
	Stage::DeleteInstance();

	// フィールド解放
	fieldManager_->Release();
	delete fieldManager_;

	// プレイヤー解放
	player_->Release();

	// 敵の解放
	//enemy_->Release();
	//EnemyManager::SetInstance(nullptr);
}
