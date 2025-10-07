#pragma once
#include <vector>
#include "../Enemy/Slime/SlimeEnemy.h"

class EnemyManager {
public:
    static EnemyManager* GetInstance();
    static void SetInstance(EnemyManager* instance);

private:
    std::vector<SlimeEnemy*> slimes;

public:
    EnemyManager();
    ~EnemyManager();

    void Spawn(float x, float y, float z);
    void Update();
    void Draw();
    void Release();

    const std::vector<SlimeEnemy*>& GetSlimes() const { return slimes; }

private:
    int maxSlimes = 250;
    int spawnedCount = 0;
    int framesSinceLastSpawn = 0;
    int spawnInterval = 1;
    float spawnRadius = 1000.0f;
};
