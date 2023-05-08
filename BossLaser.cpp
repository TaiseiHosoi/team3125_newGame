#include "BossLaser.h"
#include "FbxPlayer.h"

BossLaser::~BossLaser()
{
	CollisionManager::GetInstance()->RemoveCollider(collider_);
	delete collider_;
}

void BossLaser::Initialize(const Vector3& position)
{


#pragma region Initlialize��

	LaserModel_ = Mesh::LoadFormOBJ("cube", true);
	LaserObj_ = Object3d::Create();

	LaserObj_->SetModel(LaserModel_.get());

	LaserObj_->worldTransform.translation_ = position;
	LaserObj_->worldTransform.scale_ = { 1,1,5 };

#pragma endregion
}

void BossLaser::Update()
{
	livingBullet--;

	if (livingBullet <= 0)
	{
		SetIsDead(true);
		livingBullet = 180.0f;
	}
	Laser();

	LaserObj_->worldTransform.translation_ += velocity_;

	LaserObj_->Update();

	colliderPos_ = LaserObj_->GetPosition();
	collider_->Update();

}

void BossLaser::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (isLaser == true)
	{
		LaserObj_->Draw(cmdList);
	}
	
}

//Vector3 BossLaser::GetPosition()
//{
//	return ;
//}

void BossLaser::Laser()
{
	laserTimer++;
	
	laserPos = LaserObj_->worldTransform.translation_;

	//�������L�����̂����������ɐ��ʂɂ���
	angle = (atan2(distance.x, distance.z) + 3.141592f / 2);

	LaserObj_->worldTransform.rotation_ = (Vector3(0, (angle + 3.141592f / 2) * 1, 0));
	
	

	if (laserTimer > 60.0f)
	{
		if (isLaser == false)
		{
			isLaser = true;
		}
		/*distance.x = -distance.x;
		distance.y = -distance.y;
		distance.z = -distance.z;*/
	}
	if (isLaser == true)
	{
		distance.nomalize();

		distance.x += distance.x;
		distance.y += distance.y;
		distance.z += distance.z;

		 velocity_ = {
	distance.x * kBulletSpeed,
	distance.y * kBulletSpeed,
	distance.z * kBulletSpeed
		};
		 
		 kBulletSpeed = Ease::InQuad(change, 0, 60, laserTimer);
	}
	if (isLaser == false)
	{
		
		playerPos = player_->GetObject3d()->GetPosition();
		distance = playerPos - laserPos;
	}
	
	/*playerPos.x *= 2;
	playerPos.y *= 2;
	playerPos.z *= 2;*/

	
	

}

void BossLaser::SetCollider(SphereCollider* collider)
{
	collider_ = collider;
	CollisionManager::GetInstance()->AddCollider(collider_);
	collider_->SetAttribute(COLLISION_ATTR_ENEMIEBULLETS);
	collider_->SetBasisPos(&colliderPos_);
	collider_->SetRadius(1.0f);
}
