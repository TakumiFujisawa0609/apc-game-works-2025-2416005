#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "GameClear.h"

GameClear::GameClear(void)
{
}

GameClear::~GameClear(void)
{
}

void GameClear::Init(void)
{
}

void GameClear::Update(void)
{
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
    }
}

void GameClear::Draw(void)
{
    DrawString(100, 100, "Game Clear!", GetColor(255, 255, 255));
	DrawString(100, 150, "Press SPACE to return to Title", GetColor(255, 255, 255));
}

void GameClear::Release(void)
{
}
