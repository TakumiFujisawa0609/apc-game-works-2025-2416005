#include "Boss.h"
#include "../../../Application.h"

Boss::Boss(Player* player)
	: player_(player), currentState_(STATE::IDLE)
{
	player_ = player;
}

Boss::~Boss()
{
}

void Boss::Init(float _x, float _y, float _z)
{
	EnemyBase::Init(_x, _y, _z);
	isAlive = true;

	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Enemy/Bos.mv1").c_str());

	animationController_ = new AnimationController(modelId_);

	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 30.0f,
		Application::PATH_MODEL + "Enemy/boss action/Idle.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), 30.0f,
		Application::PATH_MODEL + "Enemy/boss action/Walking.mv1");

	// 攻撃用のアニメーションを専用ファイルに変更
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK), 30.0f,
		Application::PATH_MODEL + "Enemy/boss action/Standing Melee Attack Downward.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::DAMAGE), 30.0f,
		Application::PATH_MODEL + "Enemy/boss action/Damege.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::DEAD), 30.0f,
		Application::PATH_MODEL + "Enemy/boss action/Dead.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::WIN), 30.0f,
		Application::PATH_MODEL + "Enemy/boss action/Hip Hop Dancing.mv1");


	animType_ = ANIM_TYPE::IDLE;
	animationController_->Play(static_cast<int>(animType_), true);

	// モデル位置
	MV1SetPosition(modelId_, VGet(_x, _y, _z));

	Pos_ = VGet(_x, _y, _z);

	hp_ = 100;
}


void Boss::Update()
{
	if (!isAlive) return;

	isHitInThisFrame_ = false;

	if (animationController_) {
		animationController_->Update();
	}

	// 状態ごとの処理
	switch (currentState_) {
	case STATE::IDLE:
		UpdateIdle();
		break;
	case STATE::ATTACK:
		UpdateAttack();
		break;
	case STATE::DAMAGE:
		UpdateDamage();
		break;
	case STATE::DEAD:
		UpdateDead();
		break;
	default:
		break;
	}
}

void Boss::Draw()
{
	if (!isAlive) return;

	// モデルの描画
	if (modelId_ != -1) {
		MV1DrawModel(modelId_);
	}

	// ?? 変更: ヒットフラグに基づいて色を決定
	unsigned int lineColor = GetColor(0, 255, 0); // 通常: 緑
	unsigned int fillColor = GetColor(0, 128, 0); // 通常: 濃い緑

	if (isHitInThisFrame_) {
		lineColor = GetColor(255, 0, 0); // ヒット時: 赤
		fillColor = GetColor(128, 0, 0); // ヒット時: 濃い赤
	}

	// Y軸方向に上げるオフセット量 (この値を調整して高さを変える)
	const float sphereOffsetY = 50.0f; // 単位はゲームのスケールに合わせて調整してください

	// 球体の描画位置を計算: 現在の Pos_ から Y 軸方向 (上方向) に持ち上げる
	VECTOR sphereDrawPos = Pos_;
	sphereDrawPos.y += sphereOffsetY;


	DrawSphere3D(
		sphereDrawPos,
		GetRadius(),
		16,
		lineColor,
		fillColor,
		FALSE
	);
}

void Boss::Release()
{

}

void Boss::Kill()
{
	isAlive = false;
}

void Boss::TakeDamage(int damage)
{
	if (!isAlive) return;
	hp_ -= damage;

	damageStunTimer_ = damageStunMax_;

	// HPが0以下になったら死亡
	if (hp_ <= 0)
	{
		hp_ = 0;
		Kill();
	}
}

void Boss::OnHit(const VECTOR&)
{
	// ここは被弾時の処理を追加したい場合に拡張できます（ノックバックなど）
	// 現状は被弾で短時間スタンさせる挙動にしておきます
	damageStunTimer_ = damageStunMax_;

	isHitInThisFrame_ = true;
}

void Boss::ChangeState(STATE newState)
{
	currentState_ = newState;
	bool isLoop = true; // デフォルトはループ再生

	switch (newState) {
	case STATE::IDLE:
		animType_ = ANIM_TYPE::IDLE;
		break;
	case STATE::ATTACK:
		animType_ = ANIM_TYPE::ATTACK;
		isLoop = false;
		break;
	case STATE::DAMAGE:
		animType_ = ANIM_TYPE::DAMAGE;
		isLoop = false;
		break;
	case STATE::DEAD:
		animType_ = ANIM_TYPE::DEAD;
		isLoop = false;
		break;
	default:
		animType_ = ANIM_TYPE::IDLE;
		break;
	}
	if (animationController_) {
		animationController_->Play(static_cast<int>(animType_), isLoop);
	}
}

void Boss::UpdateIdle()
{
	if (damageStunTimer_ > 0) {
		damageStunTimer_--;
		return;
	}

	// フレーム単位でのクールダウン管理（Slime と同様）
	if (attackCooldown_ > 0) attackCooldown_--;

	// プレイヤーとの距離
	float dist = VSize(VSub(player_->GetPos(), Pos_));

	{
		VECTOR dir = VSub(player_->GetPos(), Pos_);
		dir.y = 0.0f;
		if (VSize(dir) > 0.0001f) {
			dir = VNorm(dir);
			float angleY = atan2f(dir.x, dir.z); // Slime と同じ式
			angleY += DX_PI_F; // 180度反転を補正
			MV1SetRotationXYZ(modelId_, VGet(0.0f, angleY, 0.0f));
		}
	}

	// プレイヤーが射程内 AND クールダウン完了
	if (dist < attackRange_ && attackCooldown_ <= 0) {
		ChangeState(STATE::ATTACK);
		// 攻撃開始準備
		isAttacking_ = true;
		attackTimer_ = 0;
		return;
	}
	else if (dist < attackRange_ && attackCooldown_ > 0) {
		// 射程内だがクールダウン中の場合、待機を継続
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
		return;
	}
	else {
		// プレイヤーに向かって移動
		VECTOR direction = VSub(player_->GetPos(), Pos_);
		direction = VNorm(direction);
		Pos_ = VAdd(Pos_, VScale(direction, (float)moveSpeed_));

		MV1SetPosition(modelId_, Pos_);

		// 移動中は歩行アニメーション再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::WALK), true);
	}
}


void Boss::UpdateAttack()
{
	if (damageStunTimer_ > 0) {
		// 被ダメージで割り込み
		ChangeState(STATE::IDLE);
		isAttacking_ = false;
		attackTimer_ = 0;
		attackCooldown_ = attackCooldownMax_; 
		return;
	}

	attackTimer_++;

	// ヒットフレーム
	if (attackTimer_ == attackHitFrame_) {
		float minDist = GetRadius() + player_->GetRadius();
		if (VSize(VSub(Pos_, player_->GetPos())) < minDist + 20.0f) {
			player_->TakeDamage(attackDamage_);
		}
	}

	// 攻撃終了
	if (attackTimer_ >= attackDuration_) {
		isAttacking_ = false;
		attackTimer_ = 0;
		attackCooldown_ = attackCooldownMax_; 
		ChangeState(STATE::IDLE);
	}
}

void Boss::UpdateDamage()
{
	// ダメージを受けた時の処理
	if (hp_ <= 0) {
		ChangeState(STATE::DEAD);
	}
	else {
		ChangeState(STATE::IDLE);
	}
}

void Boss::UpdateDead()
{
	isAlive = false;
}

bool Boss::CanBeHit() const
{
	return isAlive;
}

VECTOR Boss::GetHitCenter() const
{
	VECTOR p = Pos_;
	p.y += 50.0f; // 描画と完全一致させる
	return p;
}

VECTOR Boss::GetHitPos() const
{
	return GetHitCenter();
}
float  Boss::GetHitRadius() const
{
	return GetRadius();
}