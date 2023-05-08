#pragma once
#include"Camera.h"
#include"Input.h"
#include "Object3d.h"

class GameCamera :
	public Camera
{
public:
	//コンストラクタ
	GameCamera(int window_width , int window_height , Input* input);
	void Initialize();
	void Update();
	void SetTargetPos(WorldTransform* targetPos);
	WorldTransform* GetTargetPos();
	void RemoveTargetPos();
	void SetEyePos(WorldTransform* eyePos);
	void SetFollowerPos(WorldTransform* wtf);
	WorldTransform* GetEyePos();
	void RemoveEyePos();
	void ViewPointMovement();


	//シェイク用
	float randomFloat(float min , float max);	//ランダムを引き出す
	void ShakePrim();
	static void SetShakePrimST(float dura , float mag , bool isShakePrim);

	void SetShakeVec(Vector3 shakeVec);

private:
	Input* input_ = nullptr;
	WorldTransform* targetPos_ = nullptr;
	WorldTransform* eyePos_ = nullptr;
	WorldTransform* followerPos_ = nullptr;
	bool isFollowPlayer_ = true;

	//カメラモード(tekito-)
	int cameraMode_ = 0;
	//カメラ距離関係
	const float  MAX_CAMERA_DISTANCE = 50.0f;
	const float  MIN_CAMERA_DISTANCE = 7.0f;
	float cameraDistance_ = 20.0f;
	const float MAX_CHANGE_TIMER = 30;
	int cameraModeChangeCountTimer = 30;
	float cameraHeight_ = 6;
	//ShakePrim用変数
	static float magnitude;	//シェイクの強さ
	static float duration;//シェイクを起こす時間
	static bool isShake;
	float elapsedTime = 0.0f;
	const float deltaTime = 0.016;	//1.0fを1秒(60FPS)と考えた際の1F
	Vector3 loolAtPos = {};
	Vector3 shakeVec_ = {0 , 0 , 0};

	//カメラが視点移動
	Vector2 mouseMove;
	Vector2 mouseMoved;	//人をイラつかせるコード


};

