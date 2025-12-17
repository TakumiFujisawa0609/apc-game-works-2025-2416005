#include "EffekseerEffect.h"
#include "../Application.h"
#include "../Manager/Input/KeyManager.h"

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
	float kensc = 10.0f;

	if (KEY::GetIns().GetInfo(KEY_TYPE::KARI).down) {
		PlayshalshuEffectHandle = PlayEffekseer3DEffect(shalshutEffectId_);
		SetPosPlayingEffekseer3DEffect(PlayshalshuEffectHandle, 0.0f, 50.0f, 0.0f);
		SetRotationPlayingEffekseer3DEffect(PlayshalshuEffectHandle, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetScalePlayingEffekseer3DEffect(PlayshalshuEffectHandle, kensc, kensc, kensc);
	}

	//PlayshalshuEffectHandle = PlayEffekseer3DEffect(shalshutEffectId_);

	//SetPosPlayingEffekseer3DEffect(PlayshalshuEffectHandle, 0.0f, 50.0f, 0.0f);

	//SetRotationPlayingEffekseer3DEffect(PlayshalshuEffectHandle, 0.0f, DX_PI_F / 2.0f, 0.0f);

	//SetScalePlayingEffekseer3DEffect(PlayshalshuEffectHandle, 2.0f, 2.0f, 2.0f);
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
