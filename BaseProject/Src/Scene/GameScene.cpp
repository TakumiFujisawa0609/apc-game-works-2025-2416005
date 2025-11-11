#include <cmath>
#include <DxLib.h>
#include "../Manager/Camera.h"
#include "../Object/Stage/Stage.h"
#include "TitleScene.h"
#include "../Object/Player/Player.h"
#include "GameScene.h"
#include "../Object/Weapon/Weapon.h"
#include "../Scene/SceneManager.h"
#include "../Object/Stage/Field/FieldManager.h"

GameScene::GameScene(void) : SceneBase()
{
	SetMouseDispFlag(false);
}

GameScene::~GameScene(void)
{

}

void GameScene::Init(void)
{
	GameSceneSoundHandle = LoadSoundMem("Data/Sound/BGM.mp3");
}

void GameScene::Update(void)
{
	// ステージ更新
	Stage::GetInstance()->Update();

	PlaySoundMem(GameSceneSoundHandle, DX_PLAYTYPE_LOOP);

	// プレイヤー更新
	player_->Update();

	// フィールドの更新
	fieldManager_->Update();


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
	fieldManager_->Init(player_);
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
}
