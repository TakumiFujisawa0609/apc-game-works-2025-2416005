#include "EnemyManager.h"
#include  "../Player/Player.h"
#include <cmath>
#include "../../Scene/SceneManager.h"
#include "../../Scene/GameClear.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

EnemyManager* g_enemy_manager_instance = nullptr;

EnemyManager* EnemyManager::GetInstance() { return g_enemy_manager_instance; }
void EnemyManager::SetInstance(EnemyManager* instance) { g_enemy_manager_instance = instance; }

EnemyManager* EnemyManager::instance_ = nullptr;

EnemyManager::EnemyManager(Player* player)
{
	slimes.clear();
	spawnedCount = 0;
	framesSinceLastSpawn = 0;
	killedCount = 0;
	player_ = player;
}

EnemyManager::~EnemyManager()
{
    Release();
}


void EnemyManager::Init(float x, float y, float z)
{
    SlimeEnemy* slime = new SlimeEnemy(player_);
    slime->Init(x, y, z);
    slimes.push_back(slime);
}

void EnemyManager::Update()
{
    // プレイヤー位置取得
    VECTOR playerPos = player_->GetPos();

    // スライム更新
    for (auto slime : slimes) {
        if (slime) slime->Update();
    }

 
    std::vector<SlimeEnemy*> toDelete;
    for (auto slime : slimes) {
        if (slime && !slime->GetAlive()) {
            toDelete.push_back(slime);
        }
    }

    for (auto s : toDelete) {
        auto it = std::find(slimes.begin(), slimes.end(), s);
        if (it != slimes.end()) {
            delete* it;
            slimes.erase(it);
            AddKilledCount(1);
        }
    }
}


void EnemyManager::Draw()
{
    for (auto slime : slimes) {
        slime->Draw();
    }

    //DrawFormatString(
    //    0, 70, 0xffffff,
    //    "スライム数 : %d / %d", (int)slimes.size(), maxSlimes
    //);
}

void EnemyManager::Release()
{
    for (auto slime : slimes) {
        delete slime;
    }
    slimes.clear();
    spawnedCount = 0;
    framesSinceLastSpawn = 0;

    ResetKilledCount();
}
