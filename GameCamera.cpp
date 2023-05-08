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
	//入力インスタンス取得
	input_ = Input::GetInstance();

	//カメラの初期化
	Vector3 eye = {0.0f , 0.0f , -5.0f};
	Vector3 up = {0 , 1 , 0};
	Vector3 target = {0 , 0 , 300.0f};
	this->SetEye(eye);
	this->SetUp(up);
	this->SetTarget(target);
}

void GameCamera::Initialize()
{

}

void GameCamera::Update()
{
	/*if (targetPos_) {
		Vector3  pos = {
			targetPos_->matWorld_.m[3][0] ,
			targetPos_->matWorld_.m[3][1] ,
			targetPos_->matWorld_.m[3][2]
		};
		SetTarget(pos);
	}

	if (eyePos_) {
		Vector3  pos = {
			eyePos_->matWorld_.m[3][0] ,
			eyePos_->matWorld_.m[3][1] ,
			eyePos_->matWorld_.m[3][2]
		};
		SetEye(pos);
	}*/

	if (input_->TriggerKey(DIK_Q))
	{
		if (isFollowPlayer_ == true)
		{
			isFollowPlayer_ = false;
		}
		else
		{
			isFollowPlayer_ = true;
		}
	}

	if (isFollowPlayer_ == true)
	{

		if (input_->TriggerKey(DIK_F))
		{	//カメラのモード切り替え
			if (cameraMode_ == 0)
			{
				cameraMode_ = 1;
			}
			else if (cameraMode_ == 1)
			{
				cameraMode_ = 0;
			}
			else
			{
				cameraMode_ = 0;
			}
		}

		//カメラの位置
		Vector3 eyeVec = followerPos_->translation_ - targetPos_->translation_;

		Vector3 eyePos = eyeVec;

		float mag = 1.0f;
		float eyeLen = eyePos.length();	//ベクトルの長さ

		if (eyeLen > 1.0f)
		{	//もし差分のベクトルが単位ベクトルより大きかったら
			mag = 1.0f / eyeLen; //ベクトルの長さを1にする
		};

		eyePos.x *= mag;	//magをかけると正規化される
		eyePos.y *= mag;
		eyePos.z *= mag;

		if (cameraMode_ == 0)
		{
			if (cameraModeChangeCountTimer < MAX_CHANGE_TIMER)
			{
				cameraModeChangeCountTimer++;
			}
		}
		else if (cameraMode_ == 1)
		{
			if (cameraModeChangeCountTimer > 0)
			{
				cameraModeChangeCountTimer--;
			}
		}

		cameraDistance_ = Ease::InOutQuad(MIN_CAMERA_DISTANCE , MAX_CAMERA_DISTANCE , cameraModeChangeCountTimer , MAX_CHANGE_TIMER);
		cameraHeight_ = Ease::InOutQuad(3 , 6 , cameraModeChangeCountTimer , MAX_CHANGE_TIMER);


		Vector3 primalyCamera =
		{followerPos_->translation_.x + eyePos.x * cameraDistance_ ,//自機から引いた位置にカメラをセット
			cameraHeight_ ,
			followerPos_->translation_.z + eyePos.z * cameraDistance_};

		float eyeVecAngle = atan2f(primalyCamera.x - targetPos_->translation_.x , primalyCamera.z - targetPos_->translation_.z);//カメラをずらす際に使われる

		float shiftLen = -5.0f;	//ずらす量
		Vector3 shiftVec = {primalyCamera.x + sinf(eyeVecAngle + MathFunc::PI / 2) * shiftLen , primalyCamera.y , primalyCamera.z + cosf(eyeVecAngle + MathFunc::PI / 2) * shiftLen};

		ShakePrim();
		SetEye(shiftVec + loolAtPos);

		Vector3 zOffsetTarget = targetPos_->translation_ - eye;	//敵と自機が近すぎてもバグらないようにしたい
		float targetToEyeLen = zOffsetTarget.length();
		zOffsetTarget.nomalize();
		zOffsetTarget *= targetToEyeLen * 2.0f;
		zOffsetTarget.y += 3.0f;

		SetTarget(targetPos_->translation_ + shakeVec_);

	}
	//SetEye({0,5,-20});



	Camera::Update();
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
	//カメラの回転ベクトル
	Vector3 rotat = {0 , 0 , 0};
	//カメラの移動の速さ
	const float cameraSpeed = 0.0005f;

	Vector2 windowWH = Vector2(WinApp::window_width / 2 , WinApp::window_height / 2);
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd , &mousePosition);

	int xPos_absolute , yPos_absolute;

	int xPos = windowWH.x;  //移動させたいｘ座標（ウィンドウ内の相対座標）
	int yPos = windowWH.y; //移動させたいｙ座標（ウィンドウ内の相対座標）

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd , &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute , yPos_absolute);//移動させる

	//マウスの移動量を取得
	mouseMove = Vector2(0 , 0);
	mouseMove = (Vector2(mousePosition.y , mousePosition.x) - Vector2(windowWH.y , windowWH.x));//座標軸で回転している関係でこうなる(XとYが入れ替え)




	//カメラ制限
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




	//ワールド前方ベクトル
	Vector3 forward(0 , 0 , 5.5f);
	//レールカメラの回転を反映
	forward = MathFunc::MatVector(forward , CameraRot);

	forward.nomalize();





	//target = pos;
	//vTargetEye = target + (forward * cameraDistance_);

	//if (input_->PushKey(DIK_LSHIFT)) {

		//if (input_->TriggerKey(DIK_F)) {	//カメラのモード切り替え
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

		////カメラの注視点（仮）
		//target = EnemyPos_;

		////カメラの位置
		//Vector3 eyeVec = playerPos_ - EnemyPos_;

		//Vector3 eyePos = eyeVec;

		//float mag = 1.0f;
		//float eyeLen = std::sqrt(eyePos.x * eyePos.x + eyePos.y * eyePos.y + eyePos.z * eyePos.z);	//ベクトルの長さ

		//if (eyeLen > 1.0f) {	//もし差分のベクトルが単位ベクトルより大きかったら
		//	mag = 1.0f / eyeLen; //ベクトルの長さを1にする
		//};

		//eyePos.x *= mag;	//magをかけると正規化される
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
		//{ playerPos_.x + eyePos.x * cameraDistance_,//自機から引いた位置にカメラをセット
		//cameraHeight_,
		//playerPos_.z + eyePos.z * cameraDistance_ };

		//float eyeVecAngle = atan2f(primalyCamera.x - EnemyPos_.x, primalyCamera.z - EnemyPos_.z);//カメラをずらす際に使われる

		//float shiftLen = 0.0f;	//ずらす量
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
