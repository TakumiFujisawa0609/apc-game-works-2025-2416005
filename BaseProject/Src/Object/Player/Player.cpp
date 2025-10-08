#include "ShotPlayerManager.h"
#include "AtackPlayerManager.h"
#include "Player.h"
#include "../../Manager/InputManager.h"
#include "../../Utility/AnimationController.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/AsoUtility.h"
#include "../../Application.h"
#include "../../Utility/MatrixUtility.h"
#include "../../Manager/Camera.h"
#include "../../Object/Stage/Stage.h"

Player* Player::instance_ = nullptr;

Player::Player(void)
{
	modelId_ = -1;
	animationController_ = nullptr;
	pos_ = AsoUtility::VECTOR_ZERO;
	angles_ = AsoUtility::VECTOR_ZERO;
	localAngles_ = AsoUtility::VECTOR_ZERO;
	// 状態初期化
	currentState_ = STATE::NONE;
	// ジャンプ関連初期化
	isJump_ = false;
	jumpPow_ = 0.0f;
	// ノックバック関連初期化
	cntKnockBack_ = 0;
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
	//float scalef = 0.5f;
	//VECTOR scale = VGet(scalef, scalef, scalef);
	//MV1SetScale(modelId_, scale);


	animationController_ = new AnimationController(modelId_);

	 animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 30.0f,
		 Application::PATH_MODEL + "Player/Idle.mv1");

	 animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), 30.0f,
		 Application::PATH_MODEL + "Player/Walk.mv1");

	 animationController_->Add(static_cast<int>(ANIM_TYPE::CAPO), 50.0f,
		 Application::PATH_MODEL + "Player/Capoeira.mv1");

	 animationController_->Add(static_cast<int>(ANIM_TYPE::BAKA), 30.0f,
		 Application::PATH_MODEL + "Player/Silly Dancing.mv1");

	 animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK), 60.0f,
		 Application::PATH_MODEL + "Player/Atack.mv1");

	 animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), 30.0f,
		 Application::PATH_MODEL + "Player/Jump1.mv1");

	 animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

	 SHandle = LoadSoundMem("Date/Sound/粉砕玉砕大喝采.wav");

	 // カメラに自分自身を渡す
	 SceneManager::GetInstance().GetCamera()->SetFollow(this);

	 AtackPlayerManager::CreateInstance();

	 // 初期状態
	 ChangeState(STATE::STANDBY);
}

void Player::Update(void)
{

	// 現在の状態に応じた更新処理
	switch (currentState_)
	{
	case STATE::NONE:
		break;

	case STATE::STANDBY:
		UpdateStandby();
		break;
	case STATE::KNOCKBACK:
		UpdateKnockback();
		break;
	case STATE::ATTACK:
		UpdateAttack();
		break;
	case STATE::SHOT:
		UpdateShot();
		break;
	case STATE::DEAD:
		UpdateDead();
		break;
	case STATE::END:
		UpdateEnd();
	case STATE::VICTORY:
		UpdateVictory();
		break;
	}

}

void Player::Draw(void)
{
	// モデルの描画
	if (modelId_ != -1) {
		MV1DrawModel(modelId_);
	}

	ShotPlayerManager::GetInstance().Draw();

	switch (currentState_)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::STANDBY:
		DrawStandby();
		break;
	case Player::STATE::KNOCKBACK:
		break;
		DrawKnockback();
	case Player::STATE::ATTACK:
		break;
		DrawAttack();
	case Player::STATE::SHOT:
		break;
		DrawShot();
	case Player::STATE::DEAD:
		break;
		DrawDead();
	case Player::STATE::END:
		break;
		DrawEnd();
	case Player::STATE::VICTORY:
		break;
		DrawVictory();
	default:
		break;
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

	DrawFormatString(
		0, 110, 0xffffff,
		"ジャンプ中　　　 ：%s",
		isJump_ ? "Yes" : "No"
	);

	DrawFormatString(
		0, 130, 0xffffff, 
		"攻撃中: %s", 
		isAtack_ ? "Yes" : "No"
	);


	//DrawFormatString(
	//	0, 130, 0xffffff,
	//	"こちらが　濃厚とんこつ豚無双さんの濃厚無双ラーメン　海苔トッピングですうっひょ～～～～～～！着席時　コップに水垢が付いていたのを見て大きな声を出したら　店主さんからの誠意でチャーシューをサービスしてもらいました俺の動画次第でこの店潰すことだって出来るんだぞってことでいただきま～～～～す！まずはスープからコラ～！これでもかって位ドロドロの濃厚スープの中には虫が入っており　怒りのあまり卓上調味料を全部倒してしまいました～！すっかり店側も立場を弁え　誠意のチャーシュー丼を貰った所で	お次に　圧倒的存在感の極太麺を	啜る～！　殺すぞ～！ワシワシとした触感の麺の中には、髪の毛が入っておりさすがのSUSURUも　厨房に入って行ってしまいました～！ちなみに、店主さんが土下座している様子は　ぜひサブチャンネルを御覧ください"
	//);
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

float Player::GetRadius() const {
	// プレイヤーの当たり判定半径
	return 30.0f; 
}

void Player::ChangeState(STATE newState)
{

	currentState_ = newState;

	switch (currentState_)
	{
	case STATE::NONE:
		break;
	case STATE::STANDBY:
		// 待機状態に変更
		ChangeStandby();
		break;
	case STATE::KNOCKBACK:
		// ノックバック状態に変更
		ChangeKnockback();
		break;
	case STATE::ATTACK:
		// 攻撃状態に変更
		ChangeAttack();
		break;
	case STATE::SHOT:
		ChangeShot();
		break;
	case STATE::DEAD:
		// 死亡状態に変更
		ChangeDead();
		break;
	case STATE::END:
		ChangeEnd();
		break;
	case STATE::VICTORY:
		// 勝利状態に変更
		ChangeVictory();
		break;
	}
}

void Player::ChangeKnockback(void)
{
	// ジャンプ判定にする
	isJump_ = true;

	// ノックバックカウンタリセット
	cntKnockBack_ = 0;
}


void Player::ChangeStandby(void)
{
	// 待機状態に変更
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void Player::ChangeAttack(void)
{
	//// 武器を使用する
	//useWeapon_->Use(pos_, moveDir_);

	// 攻撃アニメーション
	animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);

}

void Player::ChangeShot(void)
{
}

void Player::ChangeDead(void)
{
	//// DEATHアニメーションをループ無しで再生
	//animationController_->Play(static_cast<int>(ANIM_TYPE::DEATH), false);
}

void Player::ChangeEnd(void)
{
}

void Player::ChangeVictory(void)
{
	//// VICTORYアニメーションをループ再生
	//animationController_->Play(static_cast<int>(ANIM_TYPE::WAVE), true);

}

void Player::DrawStandby(void)
{
}

void Player::DrawKnockback(void)
{
}

void Player::DrawAttack(void)
{
}

void Player::DrawShot(void)
{
}

void Player::DrawDead(void)
{
}

void Player::DrawEnd(void)
{
}

void Player::DrawVictory(void)
{
}

void Player::UpdateKnockback(void)
{
	// 着地したら通常状態に戻す
	if (!isJump_)
	{
		ChangeState(STATE::STANDBY);
		return;
	}
	cntKnockBack_++;
	// ジャンプする
	jumpPow_ -= GRAVITY_POW;
	pos_.y += jumpPow_;
	// ノックバック方向に移動させる
	VECTOR movePow = VScale(knockBackDir_, SPEED_KNOCKBACK);
	pos_ = VAdd(pos_, movePow);
	MV1SetPosition(modelId_, pos_);
}

void Player::UpdateStandby(void)
{
	ProcessUp();
	ProcessDown();
	ProcessMove();
	ProcessShot();
	ProcessAtack();
	ProcessBrink();
}

void Player::UpdateAttack(void)
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::STANDBY);
	}
}

void Player::UpdateShot(void)
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::STANDBY);
	}
}

void Player::UpdateDead(void)
{
}

void Player::UpdateEnd(void)
{
}

void Player::UpdateVictory(void)
{
}

void Player::ProcessUp(void)
{
	InputManager& ins = InputManager::GetInstance();

	// 重力を適用
	jumpPow_ -= GRAVITY_POW;

	// ジャンプ開始
	if (ins.IsTrgDown(KEY_INPUT_LSHIFT) && !isJump_)
	{
		isJump_ = true;
		jumpPow_ = JUMP_POW;
		//animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), false);
	}

	// 移動量を反映
	pos_.y += jumpPow_;

	// ★ ステージとの当たり判定
	float groundY = Stage::GetInstance()->GetGroundHeight(pos_.x, pos_.z);
	if (pos_.y <= groundY)
	{
		pos_.y = groundY;     // 地面に固定
		isJump_ = false;      // 着地
		jumpPow_ = 0.0f;      // 落下速度リセット
	}

	MV1SetPosition(modelId_, pos_);
}

void Player::ProcessDown(void)
{

}

void Player::ProcessMove(void)
{
	if (isAtack_ || isJump_) {
		// 攻撃中でもアニメーション更新は続ける
		animationController_->Update();
		return;
	}

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

		Camera* camera = SceneManager::GetInstance().GetCamera();

		VECTOR angle = camera->GetAngles();

		// 移動方向をカメラの方向に更新
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(angle.y));
		moveDir = VTransform(moveDir, mat);

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

	animationController_->Update();

}

void Player::ProcessShot(void)
{
	InputManager& ins = InputManager::GetInstance();

	static int prevMouse = 0;

	int mouse = GetMouseInput();
	if ((mouse & MOUSE_INPUT_RIGHT) && !(prevMouse & MOUSE_INPUT_RIGHT))
	{
		VECTOR dir = VGet(-sinf(angles_.y), 0.0f, -cosf(angles_.y));
		dir = VNorm(dir);

		VECTOR shotPos = pos_;
		shotPos.y += 70.0f;
		shotPos = VAdd(shotPos, VScale(dir, 30.0f));

		ShotPlayerManager::GetInstance().AddShot(shotPos, dir);
	}

	prevMouse = mouse;

	ShotPlayerManager::GetInstance().Update();
}

void Player::ProcessAtack(void)
{
	static int prevMouse = 0;
	int mouse = GetMouseInput();

	// 左クリックで攻撃開始
	if ((mouse & MOUSE_INPUT_LEFT) && !isAtack_)
	{
		isAtack_ = true;

		// 読みこんだ音をノーマル再生します(『PlaySoundMem』関数使用)
		PlaySoundMem(SHandle, DX_PLAYTYPE_NORMAL);

		// サウンドハンドルの削除
		DeleteSoundMem(SHandle);

		animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false); // 攻撃モーションを一度だけ再生
	}

	// 攻撃中 → アニメーションが終わったら解除
	if (isAtack_ && animationController_->IsEnd())
	{
		isAtack_ = false;
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true); // 待機モーションへ戻す
	}

	prevMouse = mouse;

	AtackPlayerManager::GetInstance()->Update();
}

void Player::ProcessBrink(void)
{
	int moveSpeed = SPEED_MOVE;
	bool isDashing = false;
	static int dashTpRecoverCounter = 0;

	// ダッシュ判定（元のダッシュ処理をここに）
	if (CheckHitKey(KEY_INPUT_SPACE) && dashTp > 0) {
		moveSpeed = DASH_SPEED;
		isDashing = true;
		dashTp -= DASH_TP_USE;
		if (dashTp < 0) dashTp = 0;
	}
	else {
		// ダッシュしていない場合は体力を回復
		dashTpRecoverCounter++;
		if (dashTpRecoverCounter >= 5) {
			dashTp += DASH_TP_RECOVER;
			if (dashTp > DASH_TP_MAX) dashTp = DASH_TP_MAX;
			dashTpRecoverCounter = 0;
		}
	}

}
