#include <cmath>
#include <DxLib.h>
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage/Stage.h"
#include "TitleScene.h"
#include "../Object/Player/Player.h"
#include "../Manager/Input/KeyManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/GameScene.h"

TitleScene::TitleScene(void) : SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
}

void TitleScene::Update(void)
{
	if (CheckHitKey(KEY_INPUT_SPACE) || (KEY::GetIns().GetInfo(KEY_TYPE::GAME_NEXT).down))
	{
		// スペースキーが押下されたら、ゲームシーンへ遷移する
		SceneManager::GetInstance()->ChangeScene(std::make_shared<GameScene>());
	}
}

void TitleScene::Load(void)
{
}

void TitleScene::LoadEnd(void)
{
}

void TitleScene::Draw(void)
{
	DrawString(100, 100, "TitleScene", GetColor(255, 0, 0));
	DrawString(100, 200, "Press A Button", GetColor(255, 0, 0));
	DrawString(100, 300, "Press SPACE", GetColor(255, 0, 0));
}

void TitleScene::Release(void)
{
}
