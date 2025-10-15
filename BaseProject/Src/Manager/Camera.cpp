#include "../Utility/AsoUtility.h"
#include "../Manager/Input/KeyManager.h"
#include "Camera.h"
#include "../Object/Player/Player.h"

Camera::Camera(void)
{
	// DxLib??????????A
	// ?J???????u?? x = 320.0f, y = 240.0f, z = (????T?C?Y??????ƒÖ?)?A
	// ?????_???u?? x = 320.0f, y = 240.0f, z = 1.0f
	// ?J???????????? x = 0.0f, y = 1.0f, z = 0.0f
	// ?E???u????Z????v???X?????????????J????
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// ?J????????????W
	pos_ = DERFAULT_POS;

	// ?J??????????p?x
	angles_ = DERFAULT_ANGLES;
}

void Camera::Update(void)
{
	if (follow_)
	{
		// ?v???C???[????W???ï
		VECTOR target = follow_->GetPos();

		// ??W?J???????W?i??F?v???C???[??^????????A?D???????j
		VECTOR targetPos = { target.x, target.y + 200.0f, target.z - 150.0f };

		// ???`??????????]
		float followSpeed = 0.1f; // 0.0f?`1.0f?i????????????????]?j
		pos_.x = pos_.x + (targetPos.x - pos_.x) * followSpeed;
		pos_.y = pos_.y + (targetPos.y - pos_.y) * followSpeed;
		pos_.z = pos_.z + (targetPos.z - pos_.z) * followSpeed;

		// ?J???????u?E?p?x??DxLib????f
		SetCameraPositionAndTarget_UpVecY(pos_, target);
	}
}

void Camera::SetBeforeDraw(void)
{
	// ?N???b?v??????????(SetDrawScreen????Z?b?g?????)
	SetCameraNearFar(VIEW_NEAR, VIEW_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;

	}

	// ?J????????(??u??p?x??????)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// ?J????????(??u??p?x??????)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);
}

void Camera::SetBeforeDrawFree(void)
{
	//auto& ins = InputManager::GetInstance();

	//// ?}?E?X???W?ï
	//int mouseX, mouseY;
	//GetMousePoint(&mouseX, &mouseY);

	//if (firstMouse_) {
	//	prevMouseX_ = mouseX;
	//	prevMouseY_ = mouseY;
	//	firstMouse_ = false;
	//}

	//// ?}?E?X?????
	//int deltaX = mouseX - prevMouseX_;
	//int deltaY = mouseY - prevMouseY_;

	//prevMouseX_ = mouseX;
	//prevMouseY_ = mouseY;

	//// ?}?E?X??????J??????]????f
	//angles_.y += deltaX * mouseSensitivity_; // ???E??]
	//angles_.x += deltaY * mouseSensitivity_; // ????]

	//// ?s?b?`?p?????i???????????h?~?j
	//if (angles_.x > DX_PI_F / 2.0f) angles_.x = DX_PI_F / 2.0f;
	//if (angles_.x < -DX_PI_F / 2.0f) angles_.x = -DX_PI_F / 2.0f;

	//// WASD ????
	//float movePow = 3.0f;
	//if (ins.IsNew(KEY_INPUT_W)) { pos_.z += movePow; }
	//if (ins.IsNew(KEY_INPUT_A)) { pos_.x -= movePow; }
	//if (ins.IsNew(KEY_INPUT_S)) { pos_.z -= movePow; }
	//if (ins.IsNew(KEY_INPUT_D)) { pos_.x += movePow; }
	//if (ins.IsNew(KEY_INPUT_Q)) { pos_.y += movePow; }
	//if (ins.IsNew(KEY_INPUT_E)) { pos_.y -= movePow; }

	//// XYZ????????i?????????j
	//MoveXYZDirection();
}

void Camera::DrawDebug(void)
{
	DrawFormatString(
		0, 110, 0xffffff,
		"?J???????W?@ ?F(%.1f, %.1f, %.1f)",
		pos_.x, pos_.y, pos_.z
	);
	DrawFormatString(
		0, 130, 0xffffff,
		"?J?????p?x?@ ?F(%.1f, %.1f, %.1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);

	DrawFormatString(
		0, 150, 0xffffff,
		"?????W?@ ?F(%.1f, %.1f, %.1f)",
		follow_->GetPos().x, follow_->GetPos().y, follow_->GetPos().z
	);

}

void Camera::Release(void)
{
}

const VECTOR& Camera::GetPos(void) const
{
	return pos_;
}

const VECTOR& Camera::GetAngles(void) const
{
	return angles_;
}

void Camera::ChangeMode(MODE mode)
{
	// ?J???????[?h???X
	
	mode_ = mode;
	// ??X?????????????
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	}
}

void Camera::MoveXYZDirection(void)
{
	//auto& ins = InputManager::GetInstance();

	//// ???L?[??J??????p?x??????
	//float rotPow = 1.0f * DX_PI_F / 180.0f;
	//if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x += rotPow; }
	//if (ins.IsNew(KEY_INPUT_UP)) { angles_.x -= rotPow; }
	//if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	//if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }

	//// WASD??J???????????????
	//const float movePow = 3.0f;
	//VECTOR dir = AsoUtility::VECTOR_ZERO;

	//if (ins.IsNew(KEY_INPUT_W)) { dir = AsoUtility::DIR_F; }
	//if (ins.IsNew(KEY_INPUT_A)) { dir = { -1.0f, 0.0f, 0.0f }; }
	//if (ins.IsNew(KEY_INPUT_S)) { dir = { 0.0f, 0.0f, -1.0f }; }
	//if (ins.IsNew(KEY_INPUT_D)) { dir = { 1.0f, 0.0f, 0.0f }; }

	//if (!AsoUtility::EqualsVZero(dir))

	//{
	//	// XYZ???]?s??
	//	// XZ??????????????AXZ???]???l??????????????
	//	MATRIX mat = MGetIdent();
	//	//mat = MMult(mat, MGetRotX(angles_.x));
	//	mat = MMult(mat, MGetRotY(angles_.y));
	//	//mat = MMult(mat, MGetRotZ(angles_.z));
	//	// 
	//	// ??]?s????g?p????A?x?N?g??????]??????
	//	VECTOR moveDir = VTransform(dir, mat);

	//	// ?????~?X?s?[?h????????????A???W?????????
	//	pos_ = VAdd(pos_, VScale(moveDir, movePow));
	//}
}

const VECTOR& Camera::GetTargetPos(void) const
{
	return targetPos_;
}

void Camera::SetBeforeDrawFollow(void)
{
	if (GetJoypadNum() == 0)
	{
		MoveXYZDirection();
	}
		// ???T?C?Y?ï
	int screenW, screenH;
	GetScreenState(&screenW, &screenH, nullptr);

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// ??????
	int centerX = screenW / 2;
	int centerY = screenH / 2;

	// ?}?E?X?????i?????j
	int deltaX = mouseX - centerX;
	int deltaY = mouseY - centerY;

	if (deltaX == 0 && deltaY == 0)
	{
		deltaX = KEY::GetIns().GetRightStickVec().x;
		deltaY = KEY::GetIns().GetRightStickVec().y;
		deltaX *= 8; deltaY *= 8;
	}

	// ???_??]
	angles_.y += deltaX * mouseSensitivity_; // ???E
	angles_.x += deltaY * mouseSensitivity_; // ??

	// ?s?b?`????
	if (angles_.x > DX_PI_F / 2.0f) angles_.x = DX_PI_F / 2.0f;
	if (angles_.x < -DX_PI_F / 2.0f) angles_.x = -DX_PI_F / 2.0f;

	// ?}?E?X????????i?J?[?\?????????j
	SetMousePoint(centerX, centerY);

	// ?????_????
	VECTOR followPos = follow_->GetPos();
	targetPos_ = followPos;

	// ?J????????
	// ?J???????]?s?????
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));
	//mat = MMult(mat, MGetRotZ(angles_.z));

	VECTOR targetLocalRotPos = VTransform(FOLLOW_TARGET_LOCAL_POS, mat);
	targetPos_ = VAdd(followPos, targetLocalRotPos);

	// ?J????????
	// ??????W????]??????A??]????????W???ï????
	VECTOR cameraLocalRotPos = VTransform(FOLLOW_CAMERA_LOCAL_POS, mat);

	// ??????W???‰G?[???h???W???????A?J???????W?????
	pos_ = VAdd(followPos, cameraLocalRotPos);

	// ??????W????]??????A??]????????W???ï????
	VECTOR localRotPos = VTransform(FOLLOW_LOCAL_POS, mat);

	// ??????W???‰G?[???h???W???????A?J???????W?????
	pos_ = VAdd(followPos, localRotPos);

	// ?J????????(??u??????_??????)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		{ 0.0f, 1.0f, 0.0f }
	);
}

//void Camera::SetBeforeDrawFollow(void)
//
//{
// 	if (lookAt_ == nullptr) { return; }
//
//	auto& key = KEY::GetIns();
//
//	// ??]??????W: lookAt_ (VECTOR?^)
//
//	// ?J??????Y????]?p?x: yAngle_ (float?^)
//
//	VECTOR vec = {};
//
//	Vector2 vector;
//
//	vec = { key.GetRightStickVec().y,key.GetRightStickVec().x,0.0f };
//
//	if (AsoUtility::VZERO(vec)) {
//
//		vec = { key.GetMouceMove().y,key.GetMouceMove().x,0.0f };
//
//	}
//
//	if (AsoUtility::VZERO(vec)) {
//
//		if (key.GetInfo(KEY_TYPE::CAMERA_UP).now) { vec.x++; }
//
//		if (key.GetInfo(KEY_TYPE::CAMERA_DOWN).now) { vec.x--; }
//
//		if (key.GetInfo(KEY_TYPE::CAMERA_RIGHT).now) { vec.y++; }
//
//		if (key.GetInfo(KEY_TYPE::CAMERA_LEFT).now) { vec.y--; }
//
//	}
//
//	float rotPow = 3.5f * DX_PI_F / 180.0f;
//
//	if (!AsoUtility::VZERO(vec)) {
//
//		vec = AsoUtility::Normalize(vector);
//
//		vec = VScale(vec, rotPow);
//
//		angles_ = VAdd(angles_, vec);
//
//		if (angles_.y >= AsoUtility::Deg2RadF(360.0f)) { angles_.y -= AsoUtility::Deg2RadF(360.0f); }
//
//		if (angles_.y <= AsoUtility::Deg2RadF(0.0f)) { angles_.y += AsoUtility::Deg2RadF(360.0f); }
//
//		if (angles_.x < AsoUtility::Deg2RadF(-30.0f)) { angles_.x = AsoUtility::Deg2RadF(-30.0f); }
//
//		if (angles_.x > AsoUtility::Deg2RadF(60.0f)) { angles_.x = AsoUtility::Deg2RadF(60.0f); }
//
//	}
//
//	MATRIX mat = MGetIdent();
//
//	mat = MMult(mat, MGetRotX(angles_.x));
//
//	mat = MMult(mat, MGetRotY(angles_.y));
//
//	pos_ = VAdd(targetPos_, VTransform(LOOKAT_DIFF, mat));
//
//	mat = MGetIdent();
//
//	mat = MMult(mat, MGetRotY(angles_.y));
//
//	lookAtMultPos_ = VAdd(*lookAt_, VTransform(LOOKAT_DIFF, mat));
//
//}


void Camera::SetFollow(Player* follow)
{
	if (this == nullptr) return;
	follow_ = follow;

	lookAt_ = follow_->GetPosPtr();
}