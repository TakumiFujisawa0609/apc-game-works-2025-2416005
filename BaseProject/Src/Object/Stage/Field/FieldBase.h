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

    static constexpr VECTOR SCALES = { 3.0f, 3.0f, 3.0f };
    static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };

    FieldBase(void);
    virtual ~FieldBase(void);

  virtual void Init(float _x, float _y, float _z) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    virtual void Release(void) = 0;

    void Damage(int dmg);
    void StartCapture(FieldState capturer);
    void UpdateCapture(float delta);

    void OnEnterField();

    void OnExitField();

    bool IsCapturedBy(FieldState team) const { return state_ == team; }
    FieldState GetState() const { return state_; }
    VECTOR GetPos() const { return pos_; }
    bool IsCleared() const { return isCleared_; }
    const std::string& GetName() const { return name_; }
    bool IsCapturing() const { return isCapturing_; }

    void SetPlayer(Player* player) { player_ = player; }

protected:

    void StartBattle();
    void UpdateBattle();


    void SpawnEnemies();       
    void UpdateEnemySpawn();   

    void CheckCaptureCondition();

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
    int targetKillCount_ = 100;
    bool isActive_ = false; // バトル中か？
    bool isCaptured_ = false;      // すでに制圧済みか？

    const int maxEnemies_ = 100;
    const float spawnRadius_ = 300.0f;
    const int respawnDelayFrames_ = 180; // 3秒(60fps)
    int respawnTimer_ = 0;
    bool isPlayerInside_ = false;

    VECTOR minLocal_, maxLocal_;
};