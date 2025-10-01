#pragma once
#include <DxLib.h>
#include <vector>

class EnemyBase;
class SlimeManager;
class ShotPlayer
{
public:

	const float SHOT_RADIUS = 10.0f;  // 弾の半径

	ShotPlayer(const VECTOR& pos, const VECTOR& dir);
	~ShotPlayer();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	bool IsAlive(void) const { return alive_; }
	
private:

	VECTOR pos_;     // 位置
	VECTOR dir_;     // 移動方向
	VECTOR startPos_;  // 発射位置（基準）
	float speed_;    // 移動速度
	bool alive_;     // 生存フラグ
	VECTOR prevPos_;
};

