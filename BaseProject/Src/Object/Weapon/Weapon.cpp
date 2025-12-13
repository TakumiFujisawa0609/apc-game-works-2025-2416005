#include <DxLib.h>
#include "Weapon.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"
#include "../../Object/Enemy/Bos/Boss.h"


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

    float scalef = 4.0f;
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

    // 判定の起点をプレイヤー側フラグで制御するようにした
    bool playerAttackFlag = (player_ != nullptr) && player_->IsAtack();

    if (playerAttackFlag) {
        // プレイヤーが攻撃中なら毎フレーム当たり判定を行う
        CheckCollision();
    }
    else {
        // 非攻撃時はヒット済み集合やボスフラグをリセットしておく
        hitEnemies_.clear();
        bossHit_ = false;
        // attackTimer_ 等は Start/End によって管理されるが、
        // プレイヤー側フラグ基準で判定するためここでタイマーはリセットしておく
        attackTimer_ = 0;
    }

    // 既存の isAttacking_ / attackTimer_ ベースの終了処理は残す（視覚・演出用）
    if (isAttacking_) {
        attackTimer_++;

        if (attackTimer_ > attackDuration_) {
            EndAttack();
        }
    }
}

void Weapon::Draw()
{
    MV1DrawModel(modelId_);

    //// 当たり判定が出ているときだけ「判定に一致するカプセル」を厳密に可視化する
    //bool playerAttackFlag = (player_ != nullptr) && player_->IsAtack();

    //if (playerAttackFlag) {
    //    // 判定関数内の capsuleRadius と一致させる（判定と同一の半径で表示）
    //    const float capsuleRadius = 100.0f;
    //    // 表示精度（球の数）。多めにすると見た目が滑らかになりますが処理は増えます。
    //    const int segments = 20;

    //    // 中心軸ラインを赤で描画（判定中心）
    //    DrawLine3D(swordBase_, swordTip_, GetColor(255, 40, 40));

    //    // 等間隔に球を並べてカプセルを表現（両端のキャップ + 中央のボディ）
    //    for (int i = 0; i <= segments; ++i) {
    //        float t = (float)i / (float)segments;
    //        VECTOR pos = VAdd(swordBase_, VScale(VSub(swordTip_, swordBase_), t));
    //        // 色を少しグラデにして視認性向上
    //        int r = 255;
    //        int g = 80 + (int)(80.0f * (1.0f - fabsf(t - 0.5f) * 2.0f)); // 中央やや明るく
    //        int b = 40;
    //        unsigned int color = GetColor(r, g, b);
    //        // 球でカプセル本体を表現（透明ではなく塗りで表示すると見やすい）
    //        DrawSphere3D(pos, capsuleRadius, 16, color, color, false);
    //    }

    //    // 両端の球は更に強調（キャップ）
    //    DrawSphere3D(swordBase_, capsuleRadius, 16, GetColor(255, 60, 60), GetColor(255, 60, 60), false);
    //    DrawSphere3D(swordTip_, capsuleRadius, 16, GetColor(255, 60, 60), GetColor(255, 60, 60), false);
    //}
    //else {
    //    // 当たり判定が出ていないときはカプセル描画を行わない（要求通り）
    //    // 必要ならここで小さなライトな可視化に切り替えられますが現状は何も描かない
    //}
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

    if (player_) {
		VECTOR swingPos = VScale(VAdd(swordBase_, swordTip_), 0.5f);
		player_->PlayEffectAt(swingPos);
    }
}

void Weapon::EndAttack()
{
    isAttacking_ = false;
    attackTimer_ = 0;
    hitEnemies_.clear();
    bossHit_ = false;
}

void Weapon::CheckCollision()
{
    EnemyManager* enemyManager = EnemyManager::GetInstance();
    if (!enemyManager) return;

    auto slimes = enemyManager->GetSlimes();

    for (auto slime : slimes) {
        if (!slime) continue;
        if (!slime->CanBeHit()) continue;

        // すでに当たった敵ならスキップ
        if (hitEnemies_.find(slime) != hitEnemies_.end()) continue;

        VECTOR enemyPos = slime->GetPos();
        float enemyRadius = slime->GetRadius();

        // カプセル vs 球体 で判定
        if (CheckLineToSphereCollision(swordBase_, swordTip_, enemyPos, enemyRadius))
        {
            hitEnemies_.insert(slime);

            VECTOR hitPos = VScale(VAdd(swordBase_, swordTip_), 0.5f);

            slime->OnHit(hitPos);
            slime->TakeDamage(5);

            if (slime->GetHP() <= 0)
                slime->Kill();
        }
    }

    Boss* boss = enemyManager->GetBoss();
    if (boss && boss->CanBeHit() && !bossHit_)
    {
        VECTOR bossPos = boss->GetPos();
        float bossRadius = boss->GetRadius();

        if (CheckLineToSphereCollision(swordBase_, swordTip_, bossPos, bossRadius))
        {
            bossHit_ = true;

            VECTOR hitPos = VScale(VAdd(swordBase_, swordTip_), 0.5f);

            boss->OnHit(hitPos);
            boss->TakeDamage(5);

            if (boss->GetHP() <= 0)
                boss->Kill();
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
