#include "Player.h"
#include "../Manager/InputManager.h"
#include "../Utility/AnimationController.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Utility/MatrixUtility.h"
#include "../Manager/Camera.h"


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

	angles_ = VGet(0.0f, DX_PI_F, 0.0f);

	// 親子回転行列を計算
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

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

	 animationController_->Add(static_cast<int>(ANIM_TYPE::CAPO), 30.0f,
		 Application::PATH_MODEL + "Player/Capoeira.mv1");

	 animationController_->Add(static_cast<int>(ANIM_TYPE::BAKA), 30.0f,
		 Application::PATH_MODEL + "Player/Silly Dancing.mv1");

	 animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

	 // カメラに自分自身を渡す
	 SceneManager::GetInstance().GetCamera()->SetFollow(this);
}

void Player::Update(void)
{
	// 入力制御のインスタンスを取得
	InputManager& ins = InputManager::GetInstance();

	// 移動方向を決める
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }

	if (AsoUtility::EqualsVZero(moveDir))
	{
		// 待機アニメーションをループ再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
	}
	else
	{
		// 移動方向を更新
		moveDir_ = moveDir;

		// 移動量を計算する(方向×スピード)
		movePow = VScale(moveDir, SPEED_MOVE);

		// 移動処理(座標＋移動量)
		pos_ = VAdd(pos_, movePow);

		// プレイヤーのモデルの位置を更新
		MV1SetPosition(modelId_, pos_);

		// 方向から角度に変換する
		angles_.y = atan2(moveDir.x, moveDir.z);

		// モデルの方向が正の負の方向を向いているので補正する
		angles_.y += AsoUtility::Deg2RadF(180.0f);

		MV1SetRotationXYZ(modelId_, angles_);

		// 歩くアニメーションをループ再生
		if (animationController_->GetPlayType() != static_cast<int>(ANIM_TYPE::WALK))
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::WALK), true);
		}
	}

	//angles_.y += AsoUtility::Deg2RadF(0.1f);

	//// 親子回転行列を計算
	//MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	//MV1SetRotationMatrix(modelId_, mat);

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

	DrawFormatString(
		0, 70, 0xffffff,
		"プレイヤー座標　 ：(%.1f, %.1f, %.1f)",
		pos_.x, pos_.y, pos_.z
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

VECTOR Player::GetPos(void)
{
	return pos_;
}

VECTOR Player::GetAngles(void)
{
	return angles_;
}