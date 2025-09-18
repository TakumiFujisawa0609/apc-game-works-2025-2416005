#include "Stage.h"

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
	modelId_ = MV1LoadModel("Data/Model/Stage.mv1");
	if (modelId_ == -1) {
		MessageBoxA(NULL, "ステージモデルの読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
	}
	return false;

	// ステージモデルの位置設定
	MV1SetPosition(modelId_, pos_);
}

void Stage::Update(void)
{

}

void Stage::Draw(void)
{
	// ステージモデルの描画
	MV1SetPosition(modelId_, pos_);
	MV1DrawModel(modelId_);
}

void Stage::Release(void)
{
	// ステージモデルの解放
	MV1DeleteModel(modelId_);
}
