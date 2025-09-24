#pragma once
#include <vector>
#include "SlimeEnemy.h"

class SlimeManager {

private:
    std::vector<SlimeEnemy*> slimes;

public:
    SlimeManager();
    ~SlimeManager();

    void Spawn(float x, float y, float z); // スライム出現
    void Update();
    void Draw();
    void Release();

    // スライムリストを取得
    const std::vector<SlimeEnemy*>& GetSlimes() const { return slimes; }

private:
    // spawn 制御
    int maxSlimes = 100;
    int spawnedCount = 0;        // これまでスポーンした数（0 から maxSlimes-1）
    int framesSinceLastSpawn = 0;
    int spawnInterval = 1;       // 何フレームごとにスポーンするか (1 = 毎フレーム)
    float spawnRadius = 200.0f;  // プレイヤーを中心に置く半径
};
