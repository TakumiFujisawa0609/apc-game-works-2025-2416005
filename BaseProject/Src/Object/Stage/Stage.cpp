#include "Stage.h"
#include "../../Object/Player/Player.h"
#include "../../Utility/AsoUtility.h"

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
	modelId_ = MV1LoadModel("Data/Model/SkyDome/Skydome.mv1");
	if (modelId_ == -1) {
		MessageBoxA(NULL, "ステージモデルの読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
	}

	kumoImageId_ = LoadGraph("Data/Image/kumo.png");

	toshiImageId_ = LoadGraph("Data/Image/tosi.png");

	// ステージモデルの位置設定
	MV1SetPosition(modelId_, pos_);

	float scalef= 1.0f;
	VECTOR scale = VGet(scalef, scalef, scalef);
	MV1SetScale(modelId_, scale);


	MV1SetRotationXYZ(modelId_, {AsoUtility::Deg2RadF(-90.0f),0.0f, 0.0f});

	return true;
}

void Stage::Update(void)
{
	pos_ = Player::GetInstance()->GetPos();
	KumoPos_ = pos_;
	KumoPos_.y += 5050.0f;

	ToshiPos_ = pos_;
	ToshiPos_.y += -4000.0f;

	MV1SetPosition(modelId_, pos_);
}

void Stage::Draw(void)
{
	MV1DrawModel(modelId_);

	//// 雲の描画
	//DrawModiBillboard3D(VGet(KumoPos_.x, KumoPos_.y, KumoPos_.z),
	//	-600.0f, 400.0f,
	//	600.0f, 400.0f,
	//	600.0f, -400.0f,
	//	-600.0f, -400.0f, kumoImageId_, true);

	//DrawModiBillboard3D(VGet(ToshiPos_.x, ToshiPos_.y, ToshiPos_.z),
	//-600.0f, 400.0f,
	//600.0f, 400.0f,
	//600.0f, -400.0f,
	//-600.0f, -400.0f, toshiImageId_, true);
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
