#include <DxLib.h>
#include "Weapon.h"
#include "../Player/Player.h"

Weapon::Weapon()
{
	modelId_ = -1;
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

	pos_ = Player::GetInstance()->GetPos();
	MV1SetPosition(modelId_, pos_);
	float scalef = 2.0f;
	VECTOR scale = VGet(scalef, scalef, scalef);
	MV1SetScale(modelId_, scale);
}

void Weapon::Update()
{
	pos_ = Player::GetInstance()->GetPos();
}

void Weapon::Draw()
{
	MV1DrawModel(modelId_);
}

void Weapon::Release()
{
	if (modelId_ != -1) {
		MV1DeleteModel(modelId_);
		modelId_ = -1;
	}
}
