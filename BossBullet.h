#pragma once
#include "Object3d.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "SphereCollider.h"

#include "Ease.h"
class Player;

class FbxPlayer;


enum ShotPhase
{
	Fire ,
	Homing ,
};



class BossBullet
{
public:
	~BossBullet();

private:

	std::unique_ptr<Object3d> bulletObj_;
	std::unique_ptr<Mesh> bulletModel_;
	bool isDead_ = false;

	FbxPlayer* player_ = nullptr;

	SphereCollider* collider_ = nullptr;
	Vector3 colliderPos_;

#pragma region ホーミング関連

	const float change = 1.0f;

	float kBulletSpeed = change;


	//速度
	Vector3 velocity_;

	//ホーミング精度
	int homingAccuary_ = 0;

	int kFireTimer = 0;

	int kStartHomingTime = 30;


	int kHomingTimer_ = 0;


	//ホーミングの角度調整量
	const int homingRotateValue_ = 5;


	Vector3 angle = {0 , 0 , 1};

	Vector3 targetPos_;

	int phase = ShotPhase::Fire;

#pragma endregion	

	float livingBullet = 180.0f;

public:
	void Initialize(Mesh* model , const Vector3& position);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	Vector3 GetPosition();


	void SetPlayer(FbxPlayer* player) { player_ = player; }

	void SetCollider(SphereCollider* collider);

#pragma region 射撃関数

	void Fire();

	void Homing();

	//上からのホーミング弾
	void Fire2();
	void Homing2();

	void SetIsDead(bool isDead) { isDead_ = isDead; }

	bool isDead() { return isDead_; }

#pragma endregion

};
