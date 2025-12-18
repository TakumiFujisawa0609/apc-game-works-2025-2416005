#pragma once
#include <DxLib.h>
#include <set>
#include "../../Object/Enemy/Slime/SlimeEnemy.h"

class Player;
class SlimeEnemy;
class Weapon
{
private:
    int modelId_;
    VECTOR pos_;

    // 当たり判定用
    VECTOR swordTip_;     // 剣の先端座標
    VECTOR swordBase_;    // 剣の根元座標
    bool isAttacking_;    // 攻撃中かどうか
    int attackTimer_;
    int attackActiveStart_; // 判定有効開始フレーム
    int attackActiveEnd_;   // 判定有効終了フレーム
    int attackDuration_;    // 攻撃全体時間
    std::set<SlimeEnemy*> hitEnemies_;  // 1回の攻撃で当たった敵のリスト
    bool bossHit_ = false;

    // 線分と球の当たり判定
    bool CheckLineToSphereCollision(const VECTOR& lineStart, const VECTOR& lineEnd,
        const VECTOR& spherePos, float sphereRadius);

    // 敵との当たり判定チェック
    void CheckCollision();

public:
	Weapon(Player* player);
    ~Weapon();

    void Init();
    void Update();
    void Draw();
    void Release();

    // 攻撃開始・終了
    void StartAttack();
    void EndAttack();

    bool IsAttacking() const { return isAttacking_; }

    VECTOR GetPos(void) const{ return swordTip_; }

    VECTOR GetSwordBasePos() const
    {
        return swordBase_;
    }

	Player* player_;  // プレイヤーへのポインタ
};