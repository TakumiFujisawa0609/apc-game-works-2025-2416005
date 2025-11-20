#include "Boss.h"
#include "../../../Application.h"

Boss::Boss(Player* player)
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

	// ƒ‚ƒfƒ‹‚Ì‰ŠúŠÖŒW
	MV1SetPosition(modelId_, VGet(1.0f, 1.0f, 1.0f));
}

void Boss::Update()
{
	if (!isAlive) return;
}

void Boss::Draw()
{
	if (!isAlive) return;

	// ƒ‚ƒfƒ‹‚Ì•`‰æ
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
