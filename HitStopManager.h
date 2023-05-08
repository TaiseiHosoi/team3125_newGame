#pragma once
#include "GameCamera.h"
#include "Vector3.h"
#include <random>

class HitStopManager
{
public:

	static HitStopManager* GetInstance();

	void Update();
	
	void SetHitStop(bool* hitStopFlag , int timer);

	void SetGameCamera(GameCamera* gameCamera);
	GameCamera* GetGameCamera();

private:
	void CulShakeAngle();
	
	void Execute();
	
	void CameraShake();
	
	void Finalize();

private:
	HitStopManager() = default;
	HitStopManager(const HitStopManager&) = delete;
	~HitStopManager() = default;
	HitStopManager& operator= (const HitStopManager&) = delete;

private:
	bool* hitStopFlag_ = nullptr;
	int timer_;
	Vector3 shakeAngle_;
	GameCamera* gameCamera_;
};

