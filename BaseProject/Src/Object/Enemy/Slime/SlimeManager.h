#pragma once
#include <vector>
#include "SlimeEnemy.h"

class player;
class SlimeManager {
public:
    static SlimeManager* GetInstance();
    static void SetInstance(SlimeManager* instance);

private:
    std::vector<SlimeEnemy*> slimes;

public:
    SlimeManager(Player* player);
    ~SlimeManager();

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

	Player* player_ = nullptr;
};
