#include "EffekseerEffect.h"
#include "../Application.h"
#include "../Manager/Input/KeyManager.h"
#include "../Object/Weapon/Weapon.h"

EffekseerEffect* EffekseerEffect::instance_ = nullptr;

EffekseerEffect::EffekseerEffect(void)
{

}

EffekseerEffect::~EffekseerEffect(void)
{

}

void EffekseerEffect::Init(void)
{
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (Effekseer_Init(8000) == -1) {
		DxLib_End();
		return;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(false);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	shalshutEffectId_ = LoadEffekseerEffect(
			(Application::PATH_EFFECT + "slashu.efkefc").c_str());

	if (shalshutEffectId_ == -1) {
		MessageBoxA(NULL, "エフェクトの読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
	}
}

void EffekseerEffect::Update(void)
{
    if (!weapon_) return;
    
    // 攻撃終了で停止
    if (isSlashing_ && !weapon_->IsAttacking())
    {
        StopEffekseer3DEffect(PlayshalshuEffectHandle);
        isSlashing_ = false;
        PlayshalshuEffectHandle = -1;
    }

    // 攻撃開始
    if (KEY::GetIns().GetInfo(KEY_TYPE::ATTACK).down)
    {
        // すでに再生中なら多重再生しない
        if (!isSlashing_)
        {
            PlayshalshuEffectHandle = PlayEffekseer3DEffect(shalshutEffectId_);
            SetSpeedPlayingEffekseer3DEffect(PlayshalshuEffectHandle, 0.5f);
            isSlashing_ = true;
        }
    }

    // 再生中：剣先に追従
    if (isSlashing_)
    {
        if (IsEffekseer3DEffectPlaying(PlayshalshuEffectHandle) != 0) return;

        VECTOR basePos = weapon_->GetSwordBasePos();
        VECTOR tipPos = weapon_->GetPos();

        // 位置追従（剣先）
        SetPosPlayingEffekseer3DEffect(
            PlayshalshuEffectHandle,
            tipPos.x,
            tipPos.y,
            tipPos.z
        );

        VECTOR dir = VNorm(VSub(tipPos, basePos));

        // 回転計算
        float yaw = atan2f(dir.x, dir.z);

        float clampled_dir_y = dir.y;
        if (clampled_dir_y > 1.0f) clampled_dir_y = 1.0f;
        if (clampled_dir_y < -1.0f) clampled_dir_y = -1.0f;

        float pitch = -asinf(clampled_dir_y);

        SetRotationPlayingEffekseer3DEffect(
            PlayshalshuEffectHandle,
            pitch,
            yaw,
            0.0f
        );

        // スケール
        float scale = 10.0f;
        SetScalePlayingEffekseer3DEffect(
            PlayshalshuEffectHandle,
            scale, scale, scale
        );
    }

	//PlayshalshuEffectHandle = PlayEffekseer3DEffect(shalshutEffectId_);

	//SetPosPlayingEffekseer3DEffect(PlayshalshuEffectHandle, 0.0f, 50.0f, 0.0f);

	//SetRotationPlayingEffekseer3DEffect(PlayshalshuEffectHandle, 0.0f, DX_PI_F / 2.0f, 0.0f);

	//SetScalePlayingEffekseer3DEffect(PlayshalshuEffectHandle, 10.0f, 10.0f, 10.0f);
}

void EffekseerEffect::Draw(void)
{
	DrawEffekseer3D();
}

void EffekseerEffect::Release(void)
{
	Effkseer_End();
}

void EffekseerEffect::Delete(void)
{
	DeleteEffekseerEffect(shalshutEffectId_);
}
