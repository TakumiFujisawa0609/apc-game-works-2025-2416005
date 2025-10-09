#include <DxLib.h>
#include "Manager/InputManager.h"
#include "Manager/Input/KeyManager.h"
#include "Manager/SceneManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_MODEL = "Data/Model/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("魔法少女無双");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// 乱数のシード値を設定する
	DATEDATA date;

	// 現在時刻を取得する
	GetDateTime(&date);

	// 乱数の初期値を設定する
	// 設定する数値によって、ランダムの出方が変わる
	SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	KEY::CreateIns();

	// シーン管理初期化
	SceneManager::CreateInstance();

}

void Application::Run(void)
{

	InputManager& inputManager = InputManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();

	int StartTime, EndTime, TookTime;

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		StartTime = GetNowCount();

		inputManager.Update();
		KEY::GetIns().Update();
		sceneManager.Update();

		sceneManager.Draw();

		ScreenFlip();

		// フレーム終了時刻を取得
		EndTime = GetNowCount();
		TookTime = EndTime - StartTime;

		// 規定時間より処理が早ければ待機
		if (TookTime < OneFrameTime)
		{
			Sleep(OneFrameTime - TookTime);
		}
	}
}

void Application::Destroy(void)
{

	// シーン管理解放
	SceneManager::GetInstance().Destroy();

	// 入力制御解放
	InputManager::GetInstance().Destroy();
	KEY::DeleteIns();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	// インスタンスのメモリ解放
	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}
