#include <DxLib.h>
#include "Weapon.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"

Weapon::Weapon(Player* player)
{
    modelId_ = -1;
    pos_ = VGet(0.0f, 0.0f, 0.0f);
    swordTip_ = VGet(0.0f, 0.0f, 0.0f);
    swordBase_ = VGet(0.0f, 0.0f, 0.0f);
    isAttacking_ = false;
    player_ = player;
    hitEnemies_.clear();

    attackTimer_ = 0;
    attackActiveStart_ = 10;
    attackActiveEnd_ = 25;
    attackDuration_ = 35;
}

Weapon::~Weapon() {}

void Weapon::Init()
{
    modelId_ = MV1LoadModel("Data/Model/Weapon/Sword/sword.mv1");
    if (modelId_ == -1) {
        MessageBoxA(NULL, "武器モデルの読み込みに失敗しました。", "エラー", MB_OK);
    }

    float scalef = 2.0f;
    MV1SetScale(modelId_, VGet(scalef, scalef, scalef));
}

void Weapon::Update()
{
    int playerModelId = player_->GetModelId();

    int rightHandFrame = MV1SearchFrame(playerModelId, "mixamorig:RightHand");
    if (rightHandFrame == -1) rightHandFrame = MV1SearchFrame(playerModelId, "RightHand");
    if (rightHandFrame == -1) rightHandFrame = MV1SearchFrame(playerModelId, "Right_Hand");

    if (rightHandFrame != -1) {
        MATRIX handMatrix = MV1GetFrameLocalWorldMatrix(playerModelId, rightHandFrame);
        MATRIX rotation = MGetRotX(DX_PI_F);
        VECTOR offset = VGet(0.0f, -90.0f, 0.0f);
        MATRIX finalMat = MMult(MGetTranslate(offset), rotation);
        finalMat = MMult(finalMat, handMatrix);
        MV1SetMatrix(modelId_, finalMat);

        swordTip_ = VTransform(VGet(0.0f, 50.0f, 0.0f), finalMat);
        swordBase_ = VTransform(VGet(0.0f, -50.0f, 0.0f), finalMat);
    }

    if (isAttacking_) {
        attackTimer_++;

        if (attackTimer_ >= attackActiveStart_ && attackTimer_ <= attackActiveEnd_) {
            CheckCollision();
        }

        if (attackTimer_ > attackDuration_) {
            EndAttack();
        }
    }
}

void Weapon::Draw()
{
    MV1DrawModel(modelId_);

    // カプセルの半径
    const float capsuleRadius = 30.0f;

    // 当たり判定を可視化
    if (isAttacking_) {
        // 剣の線を赤で表示
        DrawLine3D(swordBase_, swordTip_, GetColor(255, 0, 0));

        // カプセルを表現するために複数の球体を配置
        int segments = 10; // 球体の数
        for (int i = 0; i <= segments; i++) {
            float t = (float)i / (float)segments;
            VECTOR pos = VAdd(swordBase_, VScale(VSub(swordTip_, swordBase_), t));
            unsigned int color = GetColor(255, (int)(100 * (1.0f - t)), 0);
            DrawSphere3D(pos, capsuleRadius, 8, color, color, FALSE);
        }
    }
    else {
        // 攻撃していない時は青で表示
        DrawLine3D(swordBase_, swordTip_, GetColor(0, 0, 255));

        // カプセルを表現
        int segments = 5;
        for (int i = 0; i <= segments; i++) {
            float t = (float)i / (float)segments;
            VECTOR pos = VAdd(swordBase_, VScale(VSub(swordTip_, swordBase_), t));
            DrawSphere3D(pos, capsuleRadius * 0.7f, 8, GetColor(0, 0, 255), GetColor(100, 100, 255), FALSE);
        }
    }

}

void Weapon::Release()
{
    if (modelId_ != -1) {
        MV1DeleteModel(modelId_);
        modelId_ = -1;
    }
}

void Weapon::StartAttack()
{
    isAttacking_ = true;
    attackTimer_ = 0;
    hitEnemies_.clear();

    // 攻撃判定が長すぎる場合はここを調整
    attackActiveStart_ = 0;
    attackActiveEnd_ = 70;
    attackDuration_ = 80;
}

void Weapon::EndAttack()
{
    isAttacking_ = false;
    attackTimer_ = 0;
    hitEnemies_.clear();
}

void Weapon::CheckCollision()
{
    EnemyManager* enemyManager = EnemyManager::GetInstance();
    if (!enemyManager) return;

    auto slimes = enemyManager->GetSlimes();

    for (auto slime : slimes) {
        if (!slime) continue;
        if (!slime->CanBeHit()) continue;
        if (hitEnemies_.find(slime) != hitEnemies_.end()) continue;

        VECTOR enemyPos = slime->GetPos();
        float enemyRadius = slime->GetRadius();

        if (CheckLineToSphereCollision(swordBase_, swordTip_, enemyPos, enemyRadius)) {
            hitEnemies_.insert(slime);
            VECTOR swordCenter = VScale(VAdd(swordBase_, swordTip_), 0.5f);
            slime->OnHit(swordCenter);
            slime->TakeDamage(5);
            if (slime->GetHP() <= 0) slime->Kill();
        }
    }
}

bool Weapon::CheckLineToSphereCollision(const VECTOR& lineStart, const VECTOR& lineEnd,
    const VECTOR& spherePos, float sphereRadius)
{
    const float capsuleRadius = 100.0f;

    VECTOR lineDir = VSub(lineEnd, lineStart);
    float lineLength = VSize(lineDir);
    if (lineLength < 0.0001f) {
        float distance = VSize(VSub(spherePos, lineStart));
        return distance <= (sphereRadius + capsuleRadius);
    }

    lineDir = VNorm(lineDir);
    VECTOR toSphere = VSub(spherePos, lineStart);
    float t = VDot(toSphere, lineDir);
    t = (t < 0.0f) ? 0.0f : ((t > lineLength) ? lineLength : t);

    VECTOR closestPoint = VAdd(lineStart, VScale(lineDir, t));
    float distance = VSize(VSub(spherePos, closestPoint));
    return distance <= (sphereRadius + capsuleRadius);
}
