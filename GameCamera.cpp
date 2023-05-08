#include "GameCamera.h"
#include "Ease.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include"WinApp.h"

float GameCamera::magnitude = 0.0f;
float GameCamera::duration = 0.0f;
bool GameCamera::isShake = 0.0f;

GameCamera::GameCamera(int window_width , int window_height , Input* input)
	: Camera(window_width , window_height)
{
	//���̓C���X�^���X�擾
	input_ = Input::GetInstance();

	//�J�����̏�����
	Vector3 eye = {0.0f , 30.0f , -5.0f};
	Vector3 up = {0 , 1 , 0};
	Vector3 target = {0 , 0 , 0};
	this->SetEye(eye);
	this->SetUp(up);
	this->SetTarget(target);
}

void GameCamera::Initialize()
{

}

void GameCamera::Update()
{

	Camera::Update();
	ImGui::Begin("camera");
	ImGui::InputFloat3("eye", &eye.x);
	ImGui::InputFloat3("target", &target.x);
	ImGui::End();
}

void GameCamera::SetTargetPos(WorldTransform* targetPos)
{
	targetPos_ = targetPos;
}

WorldTransform* GameCamera::GetTargetPos()
{
	return targetPos_;
}

void GameCamera::RemoveTargetPos()
{
	targetPos_ = nullptr;
}

void GameCamera::SetEyePos(WorldTransform* eyePos)
{
	eyePos_ = eyePos;
}

void GameCamera::SetFollowerPos(WorldTransform* wtf)
{
	followerPos_ = wtf;
}

WorldTransform* GameCamera::GetEyePos()
{
	return eyePos_;
}

void GameCamera::RemoveEyePos()
{
	eyePos_ = nullptr;
}

void GameCamera::ViewPointMovement()
{
	//�J�����̉�]�x�N�g��
	Vector3 rotat = {0 , 0 , 0};
	//�J�����̈ړ��̑���
	const float cameraSpeed = 0.0005f;

	Vector2 windowWH = Vector2(WinApp::window_width / 2 , WinApp::window_height / 2);
	POINT mousePosition;
	//�}�E�X���W(�X�N���[�����W)���擾����
	GetCursorPos(&mousePosition);

	//�N���C�A���g�G���A���W�ɕϊ�����
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd , &mousePosition);

	int xPos_absolute , yPos_absolute;

	int xPos = windowWH.x;  //�ړ��������������W�i�E�B���h�E���̑��΍��W�j
	int yPos = windowWH.y; //�ړ��������������W�i�E�B���h�E���̑��΍��W�j

	WINDOWINFO windowInfo;
	//�E�B���h�E�̈ʒu���擾
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd , &windowInfo);

	//�}�E�X�̈ړ���̐�΍��W�i���j�^�[���ォ��̍��W�j
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//�Ȃ񂩂���Ă邩�璼��
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //�E�B���h�E�̃^�C�g���o�[�̕��i31px�j���v���X
	SetCursorPos(xPos_absolute , yPos_absolute);//�ړ�������

	//�}�E�X�̈ړ��ʂ��擾
	mouseMove = Vector2(0 , 0);
	mouseMove = (Vector2(mousePosition.y , mousePosition.x) - Vector2(windowWH.y , windowWH.x));//���W���ŉ�]���Ă���֌W�ł����Ȃ�(X��Y������ւ�)




	//�J��������
	if (mouseMoved.x < -0.80f)
	{
		mouseMoved.x = -0.80f;
	}
	else if (mouseMoved.x > 1.30f)
	{
		mouseMoved.x = 1.30f;
	}

	Vector3 rotation = Vector3(-mouseMoved.x , mouseMoved.y , 0);

	Matrix4 cameraRot;

	cameraRot = MathFunc::Rotation(rotation , 6);
	//cameraRot *= viewProjection_->matView;

	Vector3 rot = rotation;
	Matrix4 CameraRot = cameraRot;




	//���[���h�O���x�N�g��
	Vector3 forward(0 , 0 , 5.5f);
	//���[���J�����̉�]�𔽉f
	forward = MathFunc::MatVector(forward , CameraRot);

	forward.nomalize();





	//target = pos;
	//vTargetEye = target + (forward * cameraDistance_);

	//if (input_->PushKey(DIK_LSHIFT)) {

		//if (input_->TriggerKey(DIK_F)) {	//�J�����̃��[�h�؂�ւ�
		//	if (cameraMode_ == 0) {
		//		cameraMode_ = 1;
		//	}
		//	else if (cameraMode_ == 1) {
		//		cameraMode_ = 0;
		//	}
		//	else {
		//		cameraMode_ = 0;
		//	}
		//}

		////�J�����̒����_�i���j
		//target = EnemyPos_;

		////�J�����̈ʒu
		//Vector3 eyeVec = playerPos_ - EnemyPos_;

		//Vector3 eyePos = eyeVec;

		//float mag = 1.0f;
		//float eyeLen = std::sqrt(eyePos.x * eyePos.x + eyePos.y * eyePos.y + eyePos.z * eyePos.z);	//�x�N�g���̒���

		//if (eyeLen > 1.0f) {	//���������̃x�N�g�����P�ʃx�N�g�����傫��������
		//	mag = 1.0f / eyeLen; //�x�N�g���̒�����1�ɂ���
		//};

		//eyePos.x *= mag;	//mag��������Ɛ��K�������
		//eyePos.y *= mag;
		//eyePos.z *= mag;


		//if (cameraMode_ == 0) {
		//	if (cameraModeChangeCountTimer < MAX_CHANGE_TIMER) {
		//		cameraModeChangeCountTimer++;
		//	}
		//}
		//else if (cameraMode_ == 1) {
		//	if (cameraModeChangeCountTimer > 0) {
		//		cameraModeChangeCountTimer--;
		//	}
		//}

		//cameraDistance_ = easing_->InOut(MIN_CAMERA_DISTANCE, MAX_CAMERA_DISTANCE, cameraModeChangeCountTimer, MAX_CHANGE_TIMER);
		//cameraHeight_ = easing_->InOut(3, 6, cameraModeChangeCountTimer, MAX_CHANGE_TIMER);

		//Vector3 primalyCamera =
		//{ playerPos_.x + eyePos.x * cameraDistance_,//���@����������ʒu�ɃJ�������Z�b�g
		//cameraHeight_,
		//playerPos_.z + eyePos.z * cameraDistance_ };

		//float eyeVecAngle = atan2f(primalyCamera.x - EnemyPos_.x, primalyCamera.z - EnemyPos_.z);//�J���������炷�ۂɎg����

		//float shiftLen = 0.0f;	//���炷��
		//Vector3 shiftVec = { primalyCamera.x + sinf(eyeVecAngle + PI / 2) * shiftLen,primalyCamera.y,primalyCamera.z + cosf(eyeVecAngle + PI / 2) * shiftLen };

		//rot = MyMath::MatVector(viewProjection_->matView, rot);

		//vTargetEye = shiftVec;
		//}
}



float GameCamera::randomFloat(float min , float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void GameCamera::ShakePrim()
{
	if (isShake == true)
	{

		if (elapsedTime < duration)
		{
			float offsetX = randomFloat(-1.0f , 1.0f) * magnitude;
			float offsetY = randomFloat(-1.0f , 1.0f) * magnitude;
			float offsetZ = randomFloat(-1.0f , 1.0f) * magnitude;

			loolAtPos = {offsetX , offsetY , offsetZ};

			elapsedTime += deltaTime;
		}
		else
		{
			elapsedTime = 0.0f;

			isShake = false;
		}

	}

}

void GameCamera::SetShakePrimST(float dura , float mag , bool isShakePrim)
{
	duration = dura;
	magnitude = mag;
	isShake = isShakePrim;
}

void GameCamera::SetShakeVec(Vector3 shakeVec)
{
	shakeVec_ = shakeVec;
}
