#include "Player.h"
#include "../Manager/InputManager.h"
#include "../Utility/AnimationController.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"


Player* Player::instance_ = nullptr;

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	// モデルの読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/player.mv1").c_str());
	if (modelId_ == -1) {
		MessageBoxA(NULL, "プレイヤーモデルの読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
	}

	angles_ = VGet(0.0f, DX_PI_F, 0.0f);

	// モデルの回転行列
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));
	mat = MMult(mat, MGetRotZ(angles_.z));

	// モデルのローカル回転行列
	MATRIX localMat = MGetIdent();
	localMat = MMult(localMat, MGetRotX(localAngles_.x));
	localMat = MMult(localMat, MGetRotY(localAngles_.y));
	localMat = MMult(localMat, MGetRotZ(localAngles_.z));

	// 行列の合成(子, 親と指定すると親⇒子の順に適用される)
	mat = MMult(localMat, mat);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

	// モデルの位置設定
	pos_ = AsoUtility::VECTOR_ZERO;
	MV1SetPosition(modelId_, pos_);

	// モデルのスケール設定
	//VECTOR scale = VGet(10.0f, 10.0f, 10.0f); 
	//MV1SetScale(modelId_, scale);


	animationController_ = new AnimationController(modelId_);

	 animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 30.0f,
		 Application::PATH_MODEL + "Player/Idle.mv1");

	 animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), 30.0f,
		 Application::PATH_MODEL + "Player/Walk.mv1");

	 animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void Player::Update(void)
{
	// ローカルY軸角度を加算
	//const float addAngle = DX_PI_F / 180.0f;
	//angles_.y += addAngle;

	angles_.y += AsoUtility::Deg2RadF(0.1f);

	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));
	mat = MMult(mat, MGetRotZ(angles_.z));

	MV1SetRotationMatrix(modelId_, mat);

	// アニメーションの更新  
	animationController_->Update();
}

void Player::Draw(void)
{
	// モデルの描画
	if (modelId_ != -1) {
		MV1DrawModel(modelId_);
	}
	
	DrawFormatString(
		0, 50, 0xffffff,
		"プレイヤー角度　 ：(% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);
}

void Player::Release(void)
{
	// アニメーションコントローラの解放
	if (animationController_) {
		delete animationController_;
		animationController_ = nullptr;
	}

	// モデルの解放
	if (modelId_ != -1) {
		MV1DeleteModel(modelId_);
		modelId_ = -1;
	}
}
