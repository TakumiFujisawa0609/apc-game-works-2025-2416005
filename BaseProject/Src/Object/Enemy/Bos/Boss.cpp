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

	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK), 30.0f,
		Application::PATH_MODEL + "Enemy/boss action/Idle.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::DAMAGE), 30.0f,
		Application::PATH_MODEL + "Enemy/boss action/Damege.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::DEAD), 30.0f,
		Application::PATH_MODEL + "Enemy/boss action/Idle.mv1");

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

	DrawSphere3D(
		Pos_,           // 座標
		GetRadius(),    // あたり判定半径
		16,
		GetColor(0, 255, 0),     // 外線
		GetColor(0, 128, 0),     // 塗り
		FALSE                    // 塗りつぶし無し
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
	hp_ -= damage;
	ChangeState(STATE::DAMAGE);
	if (hp_ <= 0) {
		Kill();
	}
}

void Boss::OnHit(const VECTOR&)
{
	isAttacking_ = true;
	attackTimer_ = 0;
}

void Boss::ChangeState(STATE newState)
{
	currentState_ = newState;

	switch (newState) {
	case STATE::IDLE:
		animType_ = ANIM_TYPE::IDLE;
		break;
	case STATE::ATTACK:
		animType_ = ANIM_TYPE::ATTACK;
		break;
	case STATE::DAMAGE:
		animType_ = ANIM_TYPE::DAMAGE;
		break;
	case STATE::DEAD:
		animType_ = ANIM_TYPE::DEAD;
		break;
	default:
		animType_ = ANIM_TYPE::IDLE;
		break;
	}
	if (animationController_) {
		animationController_->Play(static_cast<int>(animType_), true);
	}
}

void Boss::UpdateIdle()
{
	// プレイヤーが近ければ攻撃状態へ
	float dist = VSize(VSub(player_->GetPos(), Pos_));
	if (dist < attackRange_) {
		ChangeState(STATE::ATTACK);
	}
	else {
		// プレイヤーに向かって移動
		VECTOR direction = VSub(player_->GetPos(), Pos_);
		direction = VNorm(direction);
		Pos_ = VAdd(Pos_, VScale(direction, moveSpeed_));

		// ボスの向きをプレイヤー方向に180度反転して設定
		float angleY = atan2f(direction.x, direction.z) + DX_PI_F;
		MV1SetRotationXYZ(modelId_, VGet(0.0f, angleY, 0.0f));

		// モデル位置更新
		MV1SetPosition(modelId_, Pos_);

		// 移動中は歩行アニメーション再生
		animType_ = ANIM_TYPE::WALK;
		animationController_->Play(static_cast<int>(animType_), true);
	}
}

void Boss::UpdateAttack()
{
	// 攻撃処理
	attackTimer_++;
	if (attackTimer_ >= attackDuration_) {
		isAttacking_ = false;
		attackTimer_ = 0;
	}
	// 攻撃アニメーション再生
	
	animType_ = ANIM_TYPE::ATTACK;
	animationController_->Play(static_cast<int>(animType_), true);
	
	// 攻撃後は待機状態へ戻る
	ChangeState(STATE::IDLE);
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