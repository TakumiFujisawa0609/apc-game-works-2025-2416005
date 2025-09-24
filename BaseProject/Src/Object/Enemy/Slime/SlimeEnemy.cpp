#include "../Slime/SlimeEnemy.h"

std::vector<SlimeEnemy*> slimes;
std::vector<ShotPlayer*> shots;

SlimeEnemy::SlimeEnemy()
    : moveSpeed(0.1f), color(GetColor(0, 255, 0)) 
{
}

SlimeEnemy::~SlimeEnemy()
{
    Release();
}

void SlimeEnemy::Init(float _x, float _y, float _z) 
{
    EnemyBase::Init(_x, _y, _z);
    moveSpeed = 5.0f;
    color = GetColor(0, 106, 182);

	isAlive = true; // “G‚ð¶‘¶ó‘Ô‚É‚·‚é
}

void SlimeEnemy::Update()
{
    if (!isAlive) return;

    // ’Pƒ‚Éã‰º‰^“®‚·‚é—á
    y += moveSpeed;
    if (y > 50.0f || y < 0.0f) {
        moveSpeed *= -1.0f;
    }
}

void SlimeEnemy::Draw()
{
    if (!isAlive) return;

    // ‹…‘Ì‚Å•`‰æ
    DrawSphere3D(
        VGet(x, y, z),
        15.0f,
        16,
        color,
        color,
        TRUE
    );
}

void SlimeEnemy::Release() 
{
    // ¡‰ñ‚Í“Á‚ÉƒŠƒ\[ƒX‚Í–³‚¢
}