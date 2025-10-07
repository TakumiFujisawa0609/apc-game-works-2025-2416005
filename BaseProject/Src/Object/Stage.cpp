#include "Stage.h"
#include "../Object/Player/Player.h"

Stage* Stage::instance_ = nullptr;

Stage::Stage(void)
{
	modelId_ = -1;
	pos_ = { 0.0f, 80.0f, 0.0f };
}

Stage::~Stage(void)
{

}

bool Stage::Init(void)
{
	// ステージモデルの読み込み
	modelId_ = MV1LoadModel("Data/Model/SkyDome/SkyDome.mv1");
	if (modelId_ == -1) {
		MessageBoxA(NULL, "ステージモデルの読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
	}

	// ステージモデルの位置設定
	MV1SetPosition(modelId_, pos_);

	float scalef= 50.0f;
	VECTOR scale = VGet(scalef, scalef, scalef);
	MV1SetScale(modelId_, scale);

	return true;
}

void Stage::Update(void)
{
	// プレイヤーの移動ベクトルを取得
	VECTOR playerMove = Player::GetInstance()->GetPos();

	const float stageMoveSpeed = 0.1f; 

	pos_.x += (playerMove.x - pos_.x) * stageMoveSpeed;
	pos_.z += (playerMove.z - pos_.z) * stageMoveSpeed;

	MV1SetPosition(modelId_, pos_);
}

void Stage::Draw(void)
{
	MV1DrawModel(modelId_);
}

void Stage::Release(void)
{
	// ステージモデルの解放
	MV1DeleteModel(modelId_);
}

float Stage::GetGroundHeight(float x, float z)
{
	// 今は固定で y=0 を返す
	return 0.0f;
}
