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

	// モデルの初期関係
	MV1SetPosition(modelId_, VGet(1.0f, 1.0f, 1.0f));

	// アニメーションコントローラ生成
	animationController_ = new AnimationController(modelId_);
	animType_ = ANIM_TYPE::IDLE;
	animationController_->Play(static_cast<int>(animType_), true);

	// 初期位置設定
	MV1SetPosition(modelId_, VGet(_x, _y, _z));
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
}

void Boss::UpdateAttack()
{
	// 攻撃処理
	player_->TakeDamage(attackDamage_);
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
