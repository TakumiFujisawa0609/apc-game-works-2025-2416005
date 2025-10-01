#pragma once
#include <vector>
#include "SlimeEnemy.h"

class SlimeManager {
public:
    static SlimeManager* GetInstance();
    static void SetInstance(SlimeManager* instance);

private:
    std::vector<SlimeEnemy*> slimes;

public:
    SlimeManager();
    ~SlimeManager();

    void Spawn(float x, float y, float z);
    void Update();
    void Draw();
    void Release();

    const std::vector<SlimeEnemy*>& GetSlimes() const { return slimes; }

private:
    int maxSlimes = 100;
    int spawnedCount = 0;
    int framesSinceLastSpawn = 0;
    int spawnInterval = 1;
    float spawnRadius = 1000.0f;
};
