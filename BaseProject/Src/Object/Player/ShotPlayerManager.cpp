#include "ShotPlayerManager.h"

void ShotPlayerManager::AddShot(const VECTOR& pos, const VECTOR& dir)
{
    shots_.push_back(new ShotPlayer(pos, dir));
}

void ShotPlayerManager::Update()
{
    for (auto it = shots_.begin(); it != shots_.end(); )
    {
        (*it)->Update();
        if (!(*it)->IsAlive())
        {
            delete* it;
            it = shots_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void ShotPlayerManager::Draw()
{
    for (auto& shot : shots_)
    {
        shot->Draw();
    }
}

void ShotPlayerManager::Clear()
{
    for (auto& shot : shots_)
    {
        delete shot;
    }
    shots_.clear();
}