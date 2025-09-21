#include "SlimeManager.h"

SlimeManager::SlimeManager() 
{

}

SlimeManager::~SlimeManager() 
{
    Release();
}

void SlimeManager::Spawn(float x, float y, float z)
{
    SlimeEnemy* slime = new SlimeEnemy();
    slime->Init(x, y, z);
    slimes.push_back(slime);
}

void SlimeManager::Update() 
{
    for (auto slime : slimes) {
        slime->Update();
    }
}

void SlimeManager::Draw() 
{
    for (auto slime : slimes) {
        slime->Draw();
    }
}

void SlimeManager::Release() 
{
    for (auto slime : slimes) {
        delete slime;
    }
    slimes.clear();
}
