#include "Field.h"

Field::Field()
{
}

Field::~Field()
{
}

void Field::Init()
{
	//modelId_ = MV1LoadModel("Data/Model/術式反転赤/術式反転赤/術式反転赤.mv1");
	//if (modelId_ == -1) {
	//	MessageBoxA(NULL, "ステージモデルの読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
	//}

	//modelId2_ = MV1LoadModel("Data/Model/術式反転赤/術式反転赤/術式反転青.mv1");
	//if (modelId2_ == -1) {
	//	MessageBoxA(NULL, "ステージモデルの読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
	//}

	//MV1SetPosition(modelId_, VGet(0.0f, 0.0f, 0.0f));



	//fieldPositions_.emplace_back(VGet(0.0f, 0.0f, 0.0f));
	//fieldPositions_.emplace_back(VGet(1000.0f, 0.0f, 0.0f));
	//fieldPositions_.emplace_back(VGet(200.0f, 0.0f, 800.0f));

}

void Field::Update()
{

}

void Field::Draw()
{
	
	//for (auto v : fieldPositions_)
	//{
	//	DrawSphere3D(v, 300.0f, 8, GetColor(255, 0, 0), GetColor(255, 100, 100), FALSE);
	//	MV1SetPosition(modelId2_, v);
	//	MV1DrawModel(modelId2_);
	//}
}

void Field::Release()
{
	if (modelId_ != -1) {
		MV1DeleteModel(modelId_);
		modelId_ = -1;
	}
	if (modelId2_ != -1) {
		MV1DeleteModel(modelId2_);
		modelId2_ = -1;
	}
}
