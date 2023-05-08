#pragma once
#include <memory>

#include "Object3d.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "SphereCollider.h"

#include "Ease.h"

class Player;

class FbxPlayer;

class BossLaser
{
private:

	std::unique_ptr<Object3d> LaserObj_;
	std::unique_ptr<Mesh> LaserModel_;

	bool isDead_ = false;

	FbxPlayer* player_ = nullptr;

	float kBulletSpeed = 2.0f;

	Vector3 velocity_;
	Vector3 playerPos;
	Vector3 laserPos;
	Vector3 distance;
	float angle = 0.0f;

	float livingBullet = 180.0f;

	float laserTimer = 0.0f;

	bool isLaser = false;

	const float change = 5.0f;

	SphereCollider* collider_ = nullptr;
	Vector3 colliderPos_;

public:
	~BossLaser();

	void Initialize(const Vector3& position);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	/*Vector3 GetPosition();*/

	void Laser();

	void SetCollider(SphereCollider* collider);

	void SetPlayer(FbxPlayer* player) { player_ = player; }

	void SetIsDead(bool isDead) { isDead_ = isDead; }

	bool isDead() { return isDead_; }

};

