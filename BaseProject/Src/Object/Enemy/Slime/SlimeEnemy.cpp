#include <DxLib.h>
#include <vector>
#include "../../../Application.h"
#include "../../Player/Player.h"
#include "../Slime/SlimeEnemy.h"
#include "../Slime/SlimeManager.h"

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
}

void SlimeEnemy::Update()
{
    // 上下運動
    y += moveSpeed;
    if (y > 50.0f || y < 0.0f) {
        moveSpeed *= -1.0f;
    }

    // プレイヤーの位置を取得
    Player& player = *Player::GetInstance();
    VECTOR playerPos = player.GetPos();

    // 他のスライムとの衝突回避
    VECTOR dir = VSub(playerPos, GetPos());

    // 他のスライムとの衝突回避
    SlimeManager* sm = SlimeManager::GetInstance();
    if (sm) {
        const auto& slimes = sm->GetSlimes();
        for (auto other : slimes) {
            if (other == this) continue;
            if (!other->GetAlive()) continue;

            VECTOR diff = VSub(GetPos(), other->GetPos());
            diff.y = 0.0f;
            float dist = VSize(diff);
            float minDist = GetRadius() + other->GetRadius();

            if (dist < minDist && dist > 0.0001f) {
                // 差分ベクトルを正規化
                VECTOR pushDir = VNorm(diff);
                float pushAmount = (minDist - dist) * 0.5f; // 双方に半分ずつ押し返す

                // 自分を押し返す
                x += pushDir.x * pushAmount;
                y += pushDir.y * pushAmount;
                z += pushDir.z * pushAmount;

                // 相手も押し返す（相手も同じ処理するなら不要）
                other->x -= pushDir.x * pushAmount;
                other->y -= pushDir.y * pushAmount;
                other->z -= pushDir.z * pushAmount;
            }
        }


    }

    // 正規化（長さを1に）
    dir = VNorm(dir);

	int enemymoove = 1;

    // 移動
    x += dir.x * enemymoove;
    z += dir.z * enemymoove;

    {
        VECTOR diff = VSub(GetPos(), playerPos);
        diff.y = 0.0f;

        float dist = VSize(diff);
        float minDist = GetRadius() + player.GetRadius(); // プレイヤー半径とスライム半径

        if (dist < minDist && dist > 0.0001f) {
            VECTOR pushDir = VNorm(diff);
            float pushAmount = (minDist - dist);

            // スライムだけ押し返す
            x += pushDir.x * pushAmount;
            z += pushDir.z * pushAmount;
        }
    }
}

void SlimeEnemy::Draw()
{
	// 当たり判定の可視化
	DrawSphere3D(VGet(x, y, z), GetRadius(), 16, color, color, true);
}

void SlimeEnemy::Release() 
{
}