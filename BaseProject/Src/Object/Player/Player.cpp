#include "ShotPlayerManager.h"
#include "AtackPlayerManager.h"
#include "Player.h"
#include"../../Manager/Input/KeyManager.h"
#include "../../Utility/AnimationController.h"
#include "../../Utility/AsoUtility.h"
#include "../../Application.h"
#include "../../Utility/MatrixUtility.h"
#include "../../Manager/Camera.h"
#include "../../Object/Stage/Stage.h"
#include "../../Object/Weapon/Weapon.h"
#include "../../Scene/SceneManager.h"


Player::Player(void) :
animationController_(nullptr),
modelId_(-1),
currentState_(STATE::NONE),
cntKnockBack_(0),
pos_(AsoUtility::VECTOR_ZERO),
angles_(AsoUtility::VECTOR_ZERO),
scales_(AsoUtility::VECTOR_ZERO),
moveDir_(AsoUtility::VECTOR_ZERO),
movePow(AsoUtility::VECTOR_ZERO),
localAngles_(AsoUtility::VECTOR_ZERO),
jumpPow_(0.0f),
isJump_(false),
isAtack_(false),
cntAtack_(0),
dashTp(DASH_TP_MAX),
SHandle(-1),
weapon_(nullptr),
isBrinkAction_(false)
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

	modelId_ = -1;
	animationController_ = nullptr;

	//// テーブルに関数のポインタを割り当て
	//stateTable_[STEP_NON] = AttackStepNon;
	//stateTable_[STEP_PANCH] = AttackStepPanch;
	//stateTable_[STEP_PANCH_2] = AttackStepPanch2;
	//stateTable_[STEP_KICK] = AttackStepKick;

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

	//int frameNum = MV1GetFrameNum(modelId_);
	//for (int i = 0; i < frameNum; i++) {
	//	const char* frameName = MV1GetFrameName(modelId_, i);
	//	printfDx("Frame %d: %s\n", i, frameName);
	//}


	animationController_ = new AnimationController(modelId_);

	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 30.0f,
		Application::PATH_MODEL + "Player/Idle.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), 30.0f,
		Application::PATH_MODEL + "Player/Walk.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::CAPO), 50.0f,
		Application::PATH_MODEL + "Player/Capoeira.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::BAKA), 30.0f,
		Application::PATH_MODEL + "Player/Silly Dancing.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK), 30.0f,
		Application::PATH_MODEL + "Player/Atack.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), 30.0f,
		Application::PATH_MODEL + "Player/Jump1.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK1), 30.0f,
		Application::PATH_MODEL + "Player/attack1.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK2), 30.0f,
		Application::PATH_MODEL + "Player/attack2.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK3), 30.0f,
		Application::PATH_MODEL + "Player/attack3.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACKS), 30.0f,
		Application::PATH_MODEL + "Player/Sword And Shield Slash.mv1");

	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

	prevRootPos_ = MV1GetFramePosition(modelId_, 0); // 0はルートボーンのフレーム番号

	// カメラに自分自身を渡す
	SceneManager::GetInstance()->GetCamera()->SetFollow(this);

	AtackPlayerManager::CreateInstance();

	weapon_ = new Weapon(this);
	weapon_->Init();

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

	switch (attackStep_) {
	case 1:
		animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK1), false);
		break;
	case 2:
		animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK2), false);
		break;
	case 3:
		animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK3), false);
		break;
	case 4:
		animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK4), false);
		break;
	case 5:
		animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK5), false);
		break;
	case 6:
		animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK6), false);
		break;
	}

	weapon_->Update();

	animationController_->Update();
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

	weapon_->Draw();

	{
		const int gaugeX = 50;      // 左上X位置
		const int gaugeY = 600;     // 左上Y位置
		const int gaugeWidth = 300; // ゲージ全体の幅
		const int gaugeHeight = 20; // ゲージの高さ

		// TP割合（0.0～1.0）
		float ratio = (float)dashTp / (float)DASH_TP_MAX;
		if (ratio < 0.0f) ratio = 0.0f;
		if (ratio > 1.0f) ratio = 1.0f;

		// 背景（グレー）
		DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(80, 80, 80), TRUE);

		// スタミナ本体（緑～赤）
		int r = (int)(255 * (1.0f - ratio));
		int g = (int)(255 * ratio);
		int color = GetColor(r, g, 0);

		DrawBox(gaugeX, gaugeY, gaugeX + (int)(gaugeWidth * ratio), gaugeY + gaugeHeight, color, TRUE);

		// 枠線
		DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(255, 255, 255), FALSE);

		//// 数値表示（デバッグ用）
		//DrawFormatString(gaugeX, gaugeY - 20, GetColor(255, 255, 255), "Stamina: %d / %d", dashTp, DASH_TP_MAX);
	}

	DrawFormatString(
		0, 10, 0xffffff,
		"プレイヤー角度　 ：(% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);

	DrawFormatString(
		0, 30, 0xffffff,
		"プレイヤー座標　 ：(%.1f, %.1f, %.1f)",
		pos_.x, pos_.y, pos_.z
	);

	DrawFormatString(
		0, 50, 0xffffff,
		"ジャンプ中　　　 ：%s",
		isJump_ ? "Yes" : "No"
	);

	DrawFormatString(
		0, 90, 0xffffff,
		"攻撃中: %s",
		isAtack_ ? "Yes" : "No"
	);

	//DrawFormatString(
	//	0, 150, 0xffffff,
	//	"こちらが　濃厚とんこつ豚無双さんの濃厚無双ラーメン　海苔トッピングですうっひょ～～～～～～！着席時　コップに水垢が付いていたのを見て大きな声を出したら　店主さんからの誠意でチャーシューをサービスしてもらいました俺の動画次第でこの店潰すことだって出来るんだぞってことでいただきま～～～～す！まずはスープからコラ～！これでもかって位ドロドロの濃厚スープの中には虫が入っており　怒りのあまり卓上調味料を全部倒してしまいました～！すっかり店側も立場を弁え　誠意のチャーシュー丼を貰った所で	お次に　圧倒的存在感の極太麺を	啜る～！　殺すぞ～！ワシワシとした触感の麺の中には、髪の毛が入っておりさすがのSUSURUも　厨房に入って行ってしまいました～！ちなみに、店主さんが土下座している様子は　ぜひサブチャンネルを御覧ください"
	//);

	// 攻撃段数の表示
	if (isAtack_) {
		DrawFormatString(0, 170, GetColor(255, 255, 0), "攻撃段数: %d段目", attackStep_);
	}
	else {
		DrawFormatString(0, 170, GetColor(255, 255, 0), "攻撃段数: なし");
	}
}

void Player::Release(void)
{
	// アニメーションコントローラの解放
	if (animationController_) {
		animationController_->Release();
		delete animationController_;
		animationController_ = nullptr;
	}

	if (modelId_ != -1) {
		MV1DeleteModel(modelId_);
		modelId_ = -1;
	}

	if (weapon_) {
		weapon_->Release();
		delete weapon_;
		weapon_ = nullptr;
	}

	// カメラのフォローを解除（カメラが nullptr でないことを確認）
	Camera* cam = SceneManager::GetInstance()->GetCamera();
	if (cam) {
		cam->SetFollow(nullptr);
	}
}

VECTOR Player::GetPos(void)
{
	return pos_;
}

VECTOR* Player::GetPosPtr(void)
{
	return &pos_;
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

	const float riseSpeed = 10.0f;   // 上昇スピード
	const float gravity = GRAVITY_POW; // 落下重力
	const int tpUsePerFrame = 10;     // 上昇時のスタミナ消費量


	if ((KEY::GetIns().GetInfo(KEY_TYPE::UP).now) && dashTp > 0)
	{
		isJump_ = true;          // 空中状態ON
		pos_.y += riseSpeed;     // 上昇
		dashTp -= tpUsePerFrame; // TPを消費

		if (dashTp < 0)
			dashTp = 0;

		MV1SetPosition(modelId_, pos_);
		return; // 落下処理に行かない
	}

	if (isJump_)
	{
		// 重力を適用して落下
		jumpPow_ -= gravity;
		pos_.y += jumpPow_;

		// 地面との当たり判定
		float groundY = Stage::GetInstance()->GetGroundHeight(pos_.x, pos_.z);
		if (pos_.y <= groundY)
		{
			pos_.y = groundY;
			isJump_ = false;
			jumpPow_ = 0.0f;
		}

		MV1SetPosition(modelId_, pos_);
		return;
	}


	static int tpRecoverCounter = 0;
	if (!isJump_ && !isBrinkAction_)  
	{
		tpRecoverCounter++;
		if (tpRecoverCounter >= 5)
		{
			dashTp += DASH_TP_RECOVER;
			if (dashTp > DASH_TP_MAX)
				dashTp = DASH_TP_MAX;
			tpRecoverCounter = 0;
		}
	}
	else
	{
		tpRecoverCounter = 0; // 空中やブースト中はカウントリセット
	}

	MV1SetPosition(modelId_, pos_);
}

void Player::ProcessDown(void)
{

}

void Player::ProcessMove(void)
{
	if (isAtack_) {
		// 攻撃中でもアニメーション更新は続ける
		animationController_->Update();
		return;
	}


	// 移動方向を決める
	VECTOR moveDir = { KEY::GetIns().GetLeftStickVec().x,0.0f,-KEY::GetIns().GetLeftStickVec().y };

	if (AsoUtility::EqualsVZero(moveDir))
	{
		if (KEY::GetIns().GetInfo(KEY_TYPE::MOVE_UP).now) { moveDir.z++; }
		if (KEY::GetIns().GetInfo(KEY_TYPE::MOVE_DOWN).now) { moveDir.z--; }
		if (KEY::GetIns().GetInfo(KEY_TYPE::MOVE_LEFT).now) { moveDir.x--; }
		if (KEY::GetIns().GetInfo(KEY_TYPE::MOVE_RIGHT).now) { moveDir.x++; }

		moveDir = AsoUtility::VNormalize(moveDir);
	}


	if (AsoUtility::EqualsVZero(moveDir))
	{
		// 待機アニメーションをループ再生
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
	}
	else
	{

		Camera* camera = SceneManager::GetInstance()->GetCamera();

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

	// アニメーションの更新
	//animationController_->Update();

}

void Player::ProcessShot(void)
{
	// スペースキーで弾を発射
	if ((KEY::GetIns().GetInfo(KEY_TYPE::SHOT).down))
	{
		VECTOR dir = VGet(-sinf(angles_.y), 0.0f, -cosf(angles_.y));
		dir = VNorm(dir);

		VECTOR shotPos = pos_;
		shotPos.y += 70.0f;
		shotPos = VAdd(shotPos, VScale(dir, 30.0f));

		ShotPlayerManager::GetInstance().AddShot(shotPos, dir);
	}

	// 弾の更新
	ShotPlayerManager::GetInstance().Update();
}

void Player::ProcessAtack(void)
{
	//// 左クリックで攻撃開始
	//if ((KEY::GetIns().GetInfo(KEY_TYPE::ATTACK).down) && !isAtack_)
	//{
	//	isAtack_ = true;

	//	// 武器の攻撃開始
	//	if (weapon_) {
	//		weapon_->StartAttack();
	//	}

	//	PlaySoundMem(SHandle, DX_PLAYTYPE_NORMAL);
	//	DeleteSoundMem(SHandle);

	//	animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);
	//}

	//// 攻撃中 → アニメーションが終わったら解除
	//if (isAtack_ && animationController_->IsEnd())
	//{
	//	isAtack_ = false;

	//	// 武器の攻撃終了
	//	if (weapon_) {
	//		weapon_->EndAttack();
	//	}

	//	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
	//}

	//// アニメーションの更新
	//AtackPlayerManager::GetInstance()->Update();

		// 攻撃入力受付
		// 1段目開始
		if (KEY::GetIns().GetInfo(KEY_TYPE::ATTACK).down && attackStep_ == 0 && !isAtack_) {
			attackStep_ = 1;
			isAtack_ = true;
			attackInputTimer_ = 60; // 60フレーム以内に次段入力受付
			attackInputBuffer_ = 0;
			animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK1), false);
			if (weapon_) weapon_->StartAttack();
		}
		// 2段目受付
		else if (isAtack_ && attackStep_ == 1 && attackInputTimer_ > 0) {
			if (KEY::GetIns().GetInfo(KEY_TYPE::ATTACK).down) {
				attackInputBuffer_ = 1; // 2段目受付
			}
		}
		else if (isAtack_ && attackStep_ == 2 && attackInputTimer_ > 0) {
			if (KEY::GetIns().GetInfo(KEY_TYPE::ATTACK).down) {
				attackInputBuffer_ = 1; // 3段目受付
			}
		}
		else if (isAtack_ && attackStep_ == 3 && attackInputTimer_ > 0) {
			if (KEY::GetIns().GetInfo(KEY_TYPE::ATTACK).down) {
				attackInputBuffer_ = 1; // 4段目受付
			}
		}
		else if (isAtack_ && attackStep_ == 4 && attackInputTimer_ > 0) {
			if (KEY::GetIns().GetInfo(KEY_TYPE::ATTACK).down) {
				attackInputBuffer_ = 1; // 5段目受付
			}
		}
		else if (isAtack_ && attackStep_ == 5 && attackInputTimer_ > 0) {
			if (KEY::GetIns().GetInfo(KEY_TYPE::ATTACK).down) {
				attackInputBuffer_ = 1; // 6段目受付
			}
		}

		// 攻撃中の処理
		if (isAtack_) {
			if (attackInputTimer_ > 0) attackInputTimer_--;

			if (animationController_->IsEnd()) {
				if (attackInputBuffer_ == 1) {
					attackStep_++;
					attackInputBuffer_ = 0;
					attackInputTimer_ = 20;
					// 段数ごとにアニメを切り替え
					switch (attackStep_) {
					case 2:
						animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK2), false);
						break;
					case 3:
						animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK3), false);
						break;
					case 4:
						animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK4), false);
						break;
					case 5:
						animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK5), false);
						break;
					case 6:
						animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK6), false);
						break;
	
					}
					if (weapon_) weapon_->StartAttack();
				}
				else {
					// 終了
					isAtack_ = false;
					attackStep_ = 0;
					attackInputBuffer_ = 0;
					attackInputTimer_ = 0;
					if (weapon_) weapon_->EndAttack();
					animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
				}
			}

			//// 攻撃中のみルートモーションを反映
			//if (isAtack_) {
			//	// 現在のルートボーン位置を取得
			//	VECTOR rootPos = MV1GetFramePosition(modelId_, 0);

			//	// 差分を計算
			//	VECTOR diff = VSub(rootPos, prevRootPos_);

			//	// プレイヤーの向きに合わせて差分を回転
			//	MATRIX rotMat = MGetRotY(angles_.y);
			//	diff = VTransform(diff, rotMat);

			//	// プレイヤー座標に加算
			//	pos_ = VAdd(pos_, diff);

			//	// モデルのワールド座標を更新
			//	MV1SetPosition(modelId_, pos_);

			//	// 次フレーム用に保存
			//	prevRootPos_ = rootPos;
			//}
		}
}

void Player::ProcessBrink(void)
{
	static int lastPressFrame = -999;
	static bool canDash = false;
	static bool isDash = false;
	static bool isBoost = false;
	static int dashTimer = 0;
	static int boostCooldown = 0;
	static int tpRecoverCounter = 0;

	int nowFrame = GetNowCount();
	const int doubleTapThreshold = 300;
	const int boostDuration = 10;
	const float boostPower = 60.0f;
	const float dashSpeed = 35.0f;
	const int dashTpCost = 1;

	// --- 状態リセット ---
	isBrinkAction_ = (isDash || isBoost);  // ← これが上昇中回復制御に使える

	if (boostCooldown > 0) boostCooldown--;

	// --- 入力 ---
	if ((KEY::GetIns().GetInfo(KEY_TYPE::DASH).down) && boostCooldown <= 0)
	{
		if (nowFrame - lastPressFrame <= doubleTapThreshold)
			canDash = true;
		else
		{
			isBoost = true;
			dashTimer = boostDuration;
		}

		lastPressFrame = nowFrame;
		boostCooldown = 10;
	}

	bool isHolding = (KEY::GetIns().GetInfo(KEY_TYPE::DASH).prev) != 0;

	// --- ダッシュ開始 ---
	if (canDash && isHolding && dashTp > 0)
	{
		isDash = true;
		isBoost = false;
		canDash = false;
	}

	// --- ブーストダッシュ中 ---
	if (isDash)
	{
		if (dashTp > 0 && isHolding)
		{
			VECTOR dir = VGet(-sinf(angles_.y), 0.0f, -cosf(angles_.y));
			pos_ = VAdd(pos_, VScale(dir, dashSpeed));
			dashTp -= dashTpCost;
			if (dashTp < 0) dashTp = 0;
			MV1SetPosition(modelId_, pos_);
		}
		else
		{
			isDash = false;
		}
		isBrinkAction_ = true;
		return;
	}

	// --- 単発ブースト中 ---
	if (isBoost && dashTimer > 0)
	{
		// --- 調整パラメータ ---
		const float boostPower = 200.0f;   // ← 距離を伸ばしたいなら↑ここを上げる（元60）
		const int boostDuration = 10;      // ← 継続フレーム（短くすればより瞬間的に）
		const int boostTpCost = 30;        // ← 1回の単発ブーストで減るTP量を追加

		// 向いている方向へ瞬間的に移動
		VECTOR dir = VGet(-sinf(angles_.y), 0.0f, -cosf(angles_.y));
		pos_ = VAdd(pos_, VScale(dir, boostPower / boostDuration));

		// TP消費（1回だけ）
		if (dashTimer == boostDuration)  // 最初のフレームのみ消費
		{
			dashTp -= boostTpCost;
			if (dashTp < 0) dashTp = 0;
		}

		dashTimer--;

		if (dashTimer <= 0)
		{
			isBoost = false;
		}

		MV1SetPosition(modelId_, pos_);
		isBrinkAction_ = true;
		return;
	}

	// --- 通常回復 ---
	isBrinkAction_ = false;  // ← 通常状態に戻す
	tpRecoverCounter++;
	if (tpRecoverCounter >= 5)
	{
		dashTp += DASH_TP_RECOVER;
		if (dashTp > DASH_TP_MAX) dashTp = DASH_TP_MAX;
		tpRecoverCounter = 0;
	}
}