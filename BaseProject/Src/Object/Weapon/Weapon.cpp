#include <DxLib.h>
#include "Weapon.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"

Weapon::Weapon()
{
    modelId_ = -1;
    isAttacking_ = false;
}

Weapon::~Weapon()
{
}

void Weapon::Init()
{
    modelId_ = MV1LoadModel("Data/Model/Weapon/Sword/sword.mv1");
    if (modelId_ == -1) {
        MessageBoxA(NULL, "武器モデルの読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
    }

    float scalef = 2.0f;
    VECTOR scale = VGet(scalef, scalef, scalef);
    MV1SetScale(modelId_, scale);
}

void Weapon::Update()
{
    Player* player = Player::GetInstance();
    int playerModelId = player->GetModelId();

    // 右手のフレーム番号を取得
    int rightHandFrame = MV1SearchFrame(playerModelId, "mixamorig:RightHand");

    if (rightHandFrame == -1) {
        rightHandFrame = MV1SearchFrame(playerModelId, "RightHand");
    }
    if (rightHandFrame == -1) {
        rightHandFrame = MV1SearchFrame(playerModelId, "Right_Hand");
    }

    if (rightHandFrame != -1) {
        // 右手のワールド座標変換行列を取得
        MATRIX handMatrix = MV1GetFrameLocalWorldMatrix(playerModelId, rightHandFrame);

        // 剣を180度回転
        MATRIX rotation = MGetRotX(DX_PI_F);

        // 位置オフセット（Y値を負にすると下に移動）
        VECTOR offset = VGet(0.0f, -90.0f, 0.0f);
        MATRIX offsetMat = MGetTranslate(offset);

        // オフセット→回転→手の位置の順に適用
        MATRIX finalMat = MMult(offsetMat, rotation);
        finalMat = MMult(finalMat, handMatrix);

        MV1SetMatrix(modelId_, finalMat);

        // 剣の先端と根元の位置を計算（当たり判定用）- 5倍に拡大
        swordTip_ = VTransform(VGet(0.0f, 50.0f, 0.0f), finalMat);    // 剣先（50→250）
        swordBase_ = VTransform(VGet(0.0f, -50.0f, 0.0f), finalMat);  // 剣の根元（-50→-250）
    }

    // 攻撃中なら当たり判定チェック
    if (isAttacking_) {
        CheckCollision();
    }
}

void Weapon::Draw()
{
    MV1DrawModel(modelId_);

    const float capsuleRadius = 30.0f; // カプセルの半径

    // 当たり判定を可視化（常に表示）
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

        // カプセルを表現（青）
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
}

void Weapon::EndAttack()
{
    isAttacking_ = false;
    hitEnemies_.clear(); // 攻撃終了時に当たった敵のリストをクリア
}

void Weapon::CheckCollision()
{
    EnemyManager* enemyManager = EnemyManager::GetInstance();
    if (!enemyManager) return;

    auto& slimes = enemyManager->GetSlimes();

    for (auto slime : slimes) {
        if (!slime) continue;
        if (!slime->CanBeHit()) continue; // ← 点滅中など無敵状態をスキップ

        if (hitEnemies_.find(slime) != hitEnemies_.end()) continue;

        VECTOR enemyPos = slime->GetPos();
        float enemyRadius = slime->GetRadius();

        if (CheckLineToSphereCollision(swordBase_, swordTip_, enemyPos, enemyRadius)) {
            hitEnemies_.insert(slime);

            VECTOR swordCenter = VScale(VAdd(swordBase_, swordTip_), 0.5f);
            slime->OnHit(swordCenter);
            slime->TakeDamage(30);

            if (slime->GetHP() <= 0) {
                slime->Kill();
            }

            break;
        }
    }
}


bool Weapon::CheckLineToSphereCollision(const VECTOR& lineStart, const VECTOR& lineEnd,
    const VECTOR& spherePos, float sphereRadius)
{
    // カプセルの半径（剣の太さ）
    const float capsuleRadius = 100.0f;

    // 線分の方向ベクトル
    VECTOR lineDir = VSub(lineEnd, lineStart);
    float lineLength = VSize(lineDir);

    if (lineLength < 0.0001f) {
        // 線分が点の場合
        float distance = VSize(VSub(spherePos, lineStart));
        return distance <= (sphereRadius + capsuleRadius);
    }

    lineDir = VNorm(lineDir);

    // 線分の始点から球の中心へのベクトル
    VECTOR toSphere = VSub(spherePos, lineStart);

    // 線分上の最も近い点を求める
    float t = VDot(toSphere, lineDir);
    t = (t < 0.0f) ? 0.0f : ((t > lineLength) ? lineLength : t);

    // 線分上の最も近い点
    VECTOR closestPoint = VAdd(lineStart, VScale(lineDir, t));

    // 最も近い点と球の中心の距離
    float distance = VSize(VSub(spherePos, closestPoint));

    // カプセル半径を考慮した当たり判定
    return distance <= (sphereRadius + capsuleRadius);
}