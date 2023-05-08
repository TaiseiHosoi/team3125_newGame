#include "FbxPlayer.h"
#include "Ease.h"
#include "GameCamera.h"
#include <random>
#include "HitStopManager.h"

int FbxPlayer::hp = 100;
FbxPlayer::FbxPlayer()
{

}

FbxPlayer::~FbxPlayer()
{

}

void FbxPlayer::Initialize(FBXModel* fbxModel)
{

	input_ = Input::GetInstance();

	gameObject_ = FBXObject3d::Create();
	gameObject_->SetModel(fbxModel);
	gameObject_->SetIsBonesWorldMatCalc(true);	//�{�[�����[���h�s��v�Z����
	gameObject_->Update();


	SPHERE_COLISSION_NUM = 1;
	sphere.resize(SPHERE_COLISSION_NUM);
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		sphere[i]->SetBasisPos(&spherePos);
		sphere[i]->SetRadius(5.0f);
		sphere[i]->Update();
		sphere[i]->SetAttribute(COLLISION_ATTR_ALLIES);

	}



	//�q�b�g�|�C���g
	hp = 100;
	hpModel_ = Mesh::LoadFormOBJ("cube", false);
	hpObject_ = Object3d::Create();
	hpObject_->SetModel(hpModel_.get());
	hpObject_->SetScale({static_cast<float>(hp) * 0.04f,0.1f,0.02f});
	hpObject_->SetPosition({ gameObject_.get()->GetWorldTransform().translation_.x,
		gameObject_.get()->GetWorldTransform().translation_.y + 4.0f,
		gameObject_.get()->GetWorldTransform().translation_.z });
	Matrix4 invViewPro = MathFunc::MakeInverse(&hpObject_.get()->camera_->GetViewProjectionMatrix());
	float yaw = atan2f(-invViewPro.m[2][0], sqrtf(invViewPro.m[2][1] * invViewPro.m[2][1] + invViewPro.m[2][2] * invViewPro.m[2][2]));
	hpObject_->SetRotate({0,yaw*3.14f,0});
	hpObject_->Update();

	//�p�[�e�B�N��
	particle_ = std::make_unique<ParticleManager>();
	particle_->Initialize();
	particle_->LoadTexture("effect.png");
	particle_->Update();
	
	// ���ݎ������擾���ăV�[�h�l�Ƃ���
	std::srand(std::time(nullptr));

}

void FbxPlayer::Update()
{
	

		cameraAngle_.y =
			atan2(gameObject_.get()->GetCamera().GetTarget().x - gameObject_.get()->GetCamera().GetEye().x ,
				  gameObject_.get()->GetCamera().GetTarget().z - gameObject_.get()->GetCamera().GetEye().z);

		

			Move();

			


#pragma region �R���C�h
		if (hitDeley > 0)
		{	//���t���[���q�b�g��h�~
			hitDeley--;
		}

		spherePos = {
			gameObject_->wtf.matWorld_.m[3][0] ,
			gameObject_->wtf.matWorld_.m[3][1] ,
			gameObject_->wtf.matWorld_.m[3][2]
		};

		for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
		{
			if (isAtk == true && hitDeley <= 0 && sphere[i]->GetIsHit())
			{
				
				hitDeley = 15;
				/*for (int i = 0; i < 20; i++) {
					particle_.get()->Add(15, sphere[i]->GetCollisionInfo().inter,
						{ static_cast<float>((rand() % 10 - 5) / 10),
						static_cast<float>((rand() % 10 - 5) / 10) ,
						static_cast<float>((rand() % 10 - 5) / 10) },
						{ 0.03f,0.03f,0.03f }, 0.2f, 0.0f);
				}*/
				particle_->RandParticle(sphere[i]->GetCollisionInfo().inter);

				HitStopManager::GetInstance()->SetHitStop(&isHitStop , 4);

			}
		}

		for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
		{

			sphere[i]->Update();
		}


#pragma endregion �R���C�h
		

		
		particle_->Update();
		gameObject_->Update();





	
}

void FbxPlayer::Draw(ID3D12GraphicsCommandList* cmdList)
{

	gameObject_->Draw(cmdList);


	particle_->Draw(cmdList);

}

void FbxPlayer::minusHp(int damage)
{
	hp - damage;
}

FBXObject3d* FbxPlayer::GetObject3d()
{
	return gameObject_.get();
}

void FbxPlayer::Move()
{
	//���x��0�ɂ���
	velocity_ = {0 , 0 , 0};

	//�L�[���͂���������
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D))
	{
		isRun = true;

		faceAngle_ -= cameraAngle_;

		//Z�������ɂ̑��x������
		velocity_ = {0 , 0 , kMoveSpeed_};

		//W,D�������Ă�����
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D))
		{

			//45�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(45))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(45) &&
					faceAngle_.y <= MathFunc::Dig2Rad(225))
				{

					faceAngle_.y -= kTurnSpeed_;

					if (faceAngle_.y <= MathFunc::Dig2Rad(45) ||
						faceAngle_.y >= MathFunc::Dig2Rad(225))
					{
						faceAngle_.y = MathFunc::Dig2Rad(45);
					}
				}
				else
				{

					faceAngle_.y += kTurnSpeed_;

					if (faceAngle_.y >= MathFunc::Dig2Rad(45) &&
						faceAngle_.y <= MathFunc::Dig2Rad(225))
					{
						faceAngle_.y = MathFunc::Dig2Rad(45);
					}
				}
			}

		}

		//W,A�������Ă�����
		else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A))
		{

			//135�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(315))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(125) &&
					faceAngle_.y <= MathFunc::Dig2Rad(315))
				{

					faceAngle_.y += kTurnSpeed_;

					if (faceAngle_.y <= MathFunc::Dig2Rad(125) ||
						faceAngle_.y >= MathFunc::Dig2Rad(315))
					{
						faceAngle_.y = MathFunc::Dig2Rad(315);
					}
				}
				else
				{

					faceAngle_.y -= kTurnSpeed_;

					if (faceAngle_.y >= MathFunc::Dig2Rad(125) &&
						faceAngle_.y <= MathFunc::Dig2Rad(315))
					{
						faceAngle_.y = MathFunc::Dig2Rad(315);
					}
				}
			}

		}

		//S,D�������Ă�����
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D))
		{

			//315�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(135))
			{
				if (faceAngle_.y <= MathFunc::Dig2Rad(315) &&
					faceAngle_.y >= MathFunc::Dig2Rad(135))
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(135) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(135);
					}
				}
				else
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(135) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(135);
					}
				}
			}

		}

		//S,A�������Ă�����
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A))
		{

			//225�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(225))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(45) &&
					faceAngle_.y <= MathFunc::Dig2Rad(225))
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(225) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(225);
					}
				}
				else
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(225) >= faceAngle_.y &&
						faceAngle_.y >= MathFunc::Dig2Rad(45))
					{
						faceAngle_.y = MathFunc::Dig2Rad(225);
					}

				}
			}

		}

		//W�������Ă�����
		else if (input_->PushKey(DIK_W))
		{

			//0�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(0))
			{
				if (faceAngle_.y <= MathFunc::Dig2Rad(180))
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(0) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(0);
					}
				}
				else
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(360) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(0);
					}
				}
			}
		}

		//S�������Ă�����
		else if (input_->PushKey(DIK_S))
		{

			//180�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(180))
			{
				if (faceAngle_.y <= MathFunc::Dig2Rad(180))
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(180) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(180);
					}
				}
				else
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(180) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(180);
					}
				}
			}
		}

		//D�������Ă�����
		else if (input_->PushKey(DIK_D))
		{

			//90�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(90))
			{
				if (faceAngle_.y != MathFunc::Dig2Rad(90))
				{
					if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
						faceAngle_.y <= MathFunc::Dig2Rad(270))
					{
						faceAngle_.y -= kTurnSpeed_;

						if (faceAngle_.y <= MathFunc::Dig2Rad(90) ||
							MathFunc::Dig2Rad(270) <= faceAngle_.y)
						{
							faceAngle_.y = MathFunc::Dig2Rad(90);
						}

					}
					else
					{
						faceAngle_.y += kTurnSpeed_;

						if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
							faceAngle_.y <= MathFunc::Dig2Rad(270))
						{
							faceAngle_.y = MathFunc::Dig2Rad(90);
						}
					}
				}
			}
		}

		//A�������Ă�����
		else if (input_->PushKey(DIK_A))
		{
			//270�x�����Ɍ����悤�ɉ�]������
			if (faceAngle_.y != MathFunc::Dig2Rad(270))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
					faceAngle_.y <= MathFunc::Dig2Rad(270))
				{
					faceAngle_.y += kTurnSpeed_;

					if (faceAngle_.y <= MathFunc::Dig2Rad(90) ||
						MathFunc::Dig2Rad(270) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(270);
					}

				}
				else
				{
					faceAngle_.y -= kTurnSpeed_;

					if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
						MathFunc::Dig2Rad(270) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(270);
					}
				}
			}
		}

		if (MathFunc::Dig2Rad(360) < faceAngle_.y)
		{
			faceAngle_.y += -MathFunc::Dig2Rad(360);
		}
		if (faceAngle_.y < 0)
		{
			faceAngle_.y += MathFunc::Dig2Rad(360);
		}

		faceAngle_ += cameraAngle_;
	}
	else
	{
		isRun = false;
	}
	gameObject_->wtf.rotation_ = faceAngle_;

	gameObject_->wtf.UpdateMatWorld();

	velocity_ = MathFunc::MatVector(velocity_ , gameObject_->wtf.matWorld_);

	
	
		gameObject_->wtf.translation_ += velocity_;
	
	
}

Vector3 FbxPlayer::GetNowFaceAngle()
{

	Vector3 nowVelocity_ = MathFunc::MatVector(velocity_, gameObject_->wtf.matWorld_);
	return nowVelocity_;
}
