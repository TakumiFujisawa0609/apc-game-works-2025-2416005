#include "Field.h"

Field* Field::instance_ = nullptr;


Field::Field()
{

}

Field::~Field()
{

}

void Field::Init()
{
	// フィールドモデルの読み込み
	//modelId_ = MV1LoadModel("Data/Model/Field/Wall.mv1");
	//if (modelId_ == -1) {
	//	MessageBoxA(NULL, "ステージモデルの読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
	//}

	modelId2_ = MV1LoadModel("Data/Model/Field/Line.mv1");
	if (modelId2_ == -1) {
		MessageBoxA(NULL, "ステージモデルの読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
	}

	float scalef = 3.0f;
	VECTOR scale = VGet(scalef, 1.0f, scalef);
	MV1SetScale(modelId_, scale);
	MV1SetScale(modelId2_, scale);

	fieldPositions_.emplace_back(VGet(0.0f, 200.0f, 0.0f));
	//fieldPositions_.emplace_back(VGet(1000.0f, 0.0f, 0.0f));
	//fieldPositions_.emplace_back(VGet(200.0f, 0.0f, 800.0f));
}

void Field::Update()
{

}

void Field::Draw()
{
	// for文で回して描画
	for (auto v : fieldPositions_)
	{
		MV1SetPosition(modelId2_, v);
		MV1DrawModel(modelId2_);
	}

	MV1SetPosition(modelId_, VGet(0.0f, 0.0f, 0.0f));
}

void Field::Release()
{
	//if (modelId_ != -1) {
	//	MV1DeleteModel(modelId_);
	//	modelId_ = -1;
	//}
	if (modelId2_ != -1) {
		MV1DeleteModel(modelId2_);
		modelId2_ = -1;
	}
}
