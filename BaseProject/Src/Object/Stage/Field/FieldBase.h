#pragma once
#include <DxLib.h>
#include <string>
#include <memory>
#include <vector>
#include "../../Enemy/EnemyManager.h"

class Player;

class FieldBase
{
public:

    enum class FieldState
    {
        NEUTRAL,  // 中立
        ENEMY,    // 敵側
        PLAYER    // 味方側
    };

    struct SpawnPoint {
        VECTOR pos;
        bool isActive;
        int wave;      // 今のウェーブ番号
        int spawnCount; // 今の湧き数
        int maxWave;   // 最大ウェーブ数
    };

    struct FallingEnemy {
        VECTOR pos;      // 現在位置
        VECTOR vel;      // 速度
        bool landed;     // 着地済みか
        int timer;       // 演出タイマー
    };

    struct PendingEnemy {
        VECTOR pos;
        int delay; // スポーンまでの待ち時間（フレーム）
    };

    std::vector<PendingEnemy> pendingEnemies_;

    std::vector<SpawnPoint> spawnPoints_;

    std::vector<FallingEnemy> fallingEnemies_;

    static constexpr VECTOR SCALES = { 3.0f, 3.0f, 3.0f };
    static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };

    FieldBase(void);
    virtual ~FieldBase(void);

  virtual void Init(float _x, float _y, float _z, Player* player) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    virtual void Release(void) = 0;

    void Damage(int dmg);
    void StartCapture(FieldState capturer);
    void UpdateCapture(float delta);

    void OnEnterField();

    void OnExitField();

	void UpdateFallingEnemies();

    bool IsCapturedBy(FieldState team) const { return state_ == team; }
    FieldState GetState() const { return state_; }
    VECTOR GetPos() const { return pos_; }
    bool IsCleared() const { return isCleared_; }
    const std::string& GetName() const { return name_; }
    bool IsCapturing() const { return isCapturing_; }

    void SetPlayer(Player* player) { player_ = player; }

    VECTOR GetMinPos() const {
        VECTOR scale = VGet(3.0f, 1.0f, 3.0f);
        VECTOR half = VGet((1350.0f * scale.x) / 2.0f, (20.0f * scale.y) / 2.0f, (1350.0f * scale.z) / 2.0f);
        return VSub(pos_, half);
    }

    VECTOR GetMaxPos() const {
        VECTOR scale = VGet(3.0f, 1.0f, 3.0f);
        VECTOR half = VGet((1350.0f * scale.x) / 2.0f, (20.0f * scale.y) / 2.0f, (1350.0f * scale.z) / 2.0f);
        return VAdd(pos_, half);
    }

    // ★ 指定座標がこのフィールドの範囲内か？
    bool IsInsideField(float x, float z) const {
        VECTOR minPos = GetMinPos();
        VECTOR maxPos = GetMaxPos();
        return (x >= minPos.x && x <= maxPos.x && z >= minPos.z && z <= maxPos.z);
    }

protected:

    void StartBattle();
    void UpdateBattle();


    void SpawnEnemies();       
    void UpdateEnemySpawn();   

    virtual void CheckCaptureCondition();

	void UpdatePendingEnemies();

    VECTOR pos_;          // 位置（VGet(_x, _y, _z)）
    VECTOR scales_;       // モデルスケール
    float radius_;        // 制圧範囲
    int durability_;      // 制圧耐久
    FieldState state_;    // 所属状態
    FieldState capturer_;     // 今制圧を仕掛けている勢力
    bool isCapturing_;    // 制圧中か
    float captureProgress_;  // 制圧ゲージ (0〜100)
    bool isCleared_;      // クリア済みか
    std::string name_;    // 拠点名など
    int modelHandle_;  // フィールドの範囲モデル


	Player* player_; // プレイヤーへの参照
    bool wasInside_;  // プレイヤーがフィールド内にいるか

    std::unique_ptr<EnemyManager> enemyManager_;
    int killCount_ = 0;
    int targetKillCount_ = 300;
    bool isActive_ = false; // バトル中か？
    bool isCaptured_ = false;      // すでに制圧済みか？

    const int maxEnemies_ = 100;
    const float spawnRadius_ = 300.0f;
    const int respawnDelayFrames_ = 180; // 3秒(60fps)
    int respawnTimer_ = 0;
    bool isPlayerInside_ = false;

    VECTOR minLocal_, maxLocal_;
    std::string fieldName_; // ← フィールド名を保持

    const std::string& GetFieldName() const { return fieldName_; }
    bool IsPlayerInside() const { return isPlayerInside_; }
};