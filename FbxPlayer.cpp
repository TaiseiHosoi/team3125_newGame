#include "FbxPlayer.h"
#include "Ease.h"
#include "GameCamera.h"
#include <random>
#include "HitStopManager.h"

#include "Boss.h"	//苦肉の策

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
	gameObject_->SetIsBonesWorldMatCalc(true);	//ボーンワールド行列計算あり
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



	//ヒットポイント
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

	//パーティクル
	particle_ = std::make_unique<ParticleManager>();
	particle_->Initialize();
	particle_->LoadTexture("effect.png");
	particle_->Update();
	
	// 現在時刻を取得してシード値とする
	std::srand(std::time(nullptr));

}

void FbxPlayer::Update()
{
	if (isHitStop == false)
	{

		oldIsRun = isRun;	//前フレーム処理
		oldAnimCT = animCT;

		cameraAngle_.y =
			atan2(gameObject_.get()->GetCamera().GetTarget().x - gameObject_.get()->GetCamera().GetEye().x ,
				  gameObject_.get()->GetCamera().GetTarget().z - gameObject_.get()->GetCamera().GetEye().z);

		if (isCounter == false && isAtk == false && isGuard == false)
		{

			Move();

			if (isRun == true && isAtk == false)
			{
				nowCTFlame += 1;
				if (nowCTFlame >= 15)
				{
					nowCTFlame = 0;
				}
				gameObject_.get()->AnimFlameInter(nowCTFlame , 20);
				animCT = 0;

			}
			else
			{
			}

			if (oldIsRun == true && isRun == false)
			{
				isbrake = true;
			}

			if (isbrake == true)
			{
				animCT = 5;
				brakeFlameCount++;
				if (brakeFlameCount > 10)
				{
					isbrake = false;
					brakeFlameCount = 0;
					animCT = 5;
				}
			}
		}



#pragma region 攻撃
		if (input_->TriggerKey(DIK_X))
		{
			isAtk = true;
			//particle_->RandParticle(gameObject_->GetWorldTransform().translation_);
		}

		if (isAtk == true)
		{
			atkCurrent++;
			animCT = 1;

			int atkVelCt = atkCurrent - 15;
			int ct = abs(atkVelCt);
			float atkVel = Ease::InOutQuad(4.0 , 0.0 , 15 , ct);

			Vector3 vel = {0 , 0 , 1.0f};
			vel = MathFunc::bVelocity(vel , gameObject_->wtf.matWorld_);

			gameObject_->wtf.translation_ += vel * (4.0f - atkVel);


			if (atkCurrent > 30)
			{	//30フレ目でアニメーション終わり
				gameObject_.get()->AnimFlameInter(atkCurrent , 80);
				animCT = 5;
				gameObject_.get()->PlayAnimation(animCT);
				isAtk = false;
				atkCurrent = 0;
			}

		}
#pragma endregion 攻撃

#pragma region 防御行動
		if (input_->TriggerKey(DIK_C) && isGuardExcute == false)
		{
			//フラグ管理
			//isCounter = true;
			isGuard = true;
			isCounter = false;
			//カウント初期化
			//nowCTFlame = 0;
			//counterFrameCount = 0;
			guardMovePhase = 0;
			guardCurrent = 0;
			animCT = 6;
			gameObject_->PlayAnimation(animCT);

			gameObject_->AnimIsRotateChange(true);


		}
		else if (input_->ReleaseKey(DIK_C) && isGuardExcute == false)
		{

			isGuard = false;
			nowCTFlame = 0;
			counterFrameCount = 0;
			if (gameObject_->GetIsAnimRot() == true)
			{
				gameObject_->AnimIsRotateChange(false);
			}
		}


		if (isGuard == true)
		{

			counterFrameCount++;	//フラグなどプログラム用のカウンター
			if (guardCurrent < 10)
			{
				guardCurrent++;	//アニメーション専用カウンター
			}

			gameObject_.get()->AnimFlameInter(guardCurrent , 20);

			if (sphere[0]->GetIsHit() == true)
			{
				/*isGuard = false;
				isGuardExcute = true;*/
			}

		}



		if (isGuardExcute == true)
		{
			if (input_->TriggerMouseButton(0))
			{
				isCounter = true;
				isGuardExcute = true;
			}
		}


		if (isCounter == true)
		{

			if (gameObject_.get()->GetIsAnimRot() == true)
			{
				gameObject_.get()->AnimIsRotateChange(false);
			}
			counterFrameCount++;	//カウンター行動が開始してからのフレームカウント

			const int max2AnimCT = 30;	//サマーソルトの最大フレーム
			const int max3AnimCT = max2AnimCT + 25;	//回転して力をためる最大フレーム
			const int max4AnimCT = max3AnimCT + 20;
			const int max5AnimCT = max4AnimCT + 45;
			const int max6AnimCT = max5AnimCT + 35;

			if (counterFrameCount == 1)
			{	//初フレーム
				atkMovePhase = 1;	//行動フェイズ
				animCT = 2;	//アニメーション番号

			}
			else if (counterFrameCount == max2AnimCT)
			{
				atkMovePhase = 2;
				nowCTFlame = 0;
				animCT = 3;
			}
			else if (counterFrameCount == max3AnimCT)
			{
				atkMovePhase = 3;

			}
			else if (counterFrameCount == max4AnimCT)
			{
				atkMovePhase = 4;
				nowCTFlame = 10;
				animCT = 3;
				gameObject_.get()->wtf.rotation_.x = 0;


			}
			else if (counterFrameCount == max5AnimCT)
			{
				atkMovePhase = 5;
				nowCTFlame = 10;
				animCT = 2;

			}
			else if (counterFrameCount == max6AnimCT)
			{
				//終了時処理
				atkMovePhase = 0;
				counterFrameCount = 0;
				isCounter = false;
				animCT = 5;
				kAccumulateRotVel = 0.0f;

				if (gameObject_.get()->GetIsAnimRot() == false)
				{	//もし繰り返しアニメーションが無効だった場合
					gameObject_.get()->AnimIsRotateChange(true);
				}
			}

			//
			if (atkMovePhase == 0)
			{
				gameObject_.get()->wtf.translation_.y = 0;
			}
			else if (atkMovePhase == 1)
			{
				if (nowCTFlame < 30)
				{
					nowCTFlame += 1;
				}
				gameObject_.get()->wtf.translation_.y = Ease::InQuad(7.0 , 0.0 , max2AnimCT , counterFrameCount);
				gameObject_.get()->AnimFlameInter(nowCTFlame , 30);

			}
			else if (atkMovePhase == 2)
			{
				if (nowCTFlame < 15)
				{
					nowCTFlame += 1;
				}
				kAccumulateRotVel += 0.05f;
				//gameObject_.get()->wtf.translation_.y = Ease::InQuad(8.0, 7.0, max2AnimCT, counterFrameCount - max2AnimCT);
				gameObject_.get()->wtf.rotation_.x += kAccumulateRotVel;
				gameObject_.get()->AnimFlameInter(nowCTFlame , 30);
			}
			else if (atkMovePhase == 3)
			{

				kAccumulateRotVel += 0.14f;
				gameObject_.get()->wtf.rotation_.x += kAccumulateRotVel;
				gameObject_.get()->AnimFlameInter(10 , 20);
			}
			else if (atkMovePhase == 4)
			{
				Vector3 kATKSpeedVel = {0 , 0 , 1.0f};
				//突進行列計算
				Matrix4 tackleMat = MathFunc::Rotation(Vector3(0.0f , cameraAngle_.y , 0.0f) , 2);
				Vector3 kTackleVel = MathFunc::bVelocity(kATKSpeedVel , tackleMat);
				gameObject_.get()->wtf.rotation_.x += kAccumulateRotVel;
				gameObject_.get()->wtf.translation_ += kTackleVel;
				gameObject_.get()->AnimFlameInter(10 , 20);

			}
			else if (atkMovePhase == 5)
			{
				if (nowCTFlame < 60)
				{
					nowCTFlame += 1;
				}

				gameObject_.get()->wtf.translation_.y = Ease::InQuad(10.0 , 5.0 , max6AnimCT - max5AnimCT , max6AnimCT - counterFrameCount);
				gameObject_.get()->wtf.translation_.x -= cameraAngle_.x * 0.1f;
				gameObject_.get()->wtf.translation_.z -= cameraAngle_.z * 0.1f;

				gameObject_.get()->AnimFlameInter(nowCTFlame , 60);
			}

		}

#pragma endregion 防御行動

#pragma region コライド
		if (hitDeley > 0)
		{	//毎フレームヒットを防止
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
				Boss::minusHp(1);
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


#pragma endregion コライド
		if (input_->TriggerKey(DIK_Q))
		{
			flame = 0;
		}
		flame++;

		ImGui::Begin("fbxPlayer");

		ImGui::InputInt("flame" , &flame);
		ImGui::InputFloat3("transla" , &gameObject_->wtf.translation_.x);
		ImGui::InputFloat3("MatTrans" , &gameObject_->wtf.matWorld_.m[3][0]);
		ImGui::InputInt("atkMovePhase" , &atkMovePhase);
		ImGui::InputInt("currentFlame" , &counterFrameCount);
		ImGui::InputInt("currentFlame" , &counterFrameCount);
		ImGui::InputInt("currentFlame" , &Boss::hp);


		ImGui::End();
		particle_->Update();
		gameObject_->Update();

#pragma region hp
		hpObject_->SetScale({static_cast<float>(hp) * 0.04f , 0.1f , 0.02f});
		hpObject_->SetPosition({gameObject_.get()->GetWorldTransform().translation_.x ,
							   gameObject_.get()->GetWorldTransform().translation_.y + 4.0f ,
							   gameObject_.get()->GetWorldTransform().translation_.z});
		Matrix4 invViewPro = MathFunc::MakeInverse(&hpObject_.get()->camera_->GetViewMatrix());
		float yaw = atan2f(-invViewPro.m[3][1] , sqrtf(invViewPro.m[3][2] * invViewPro.m[3][2] + invViewPro.m[3][3] * invViewPro.m[3][3]));
		hpObject_->SetRotate({0 , yaw , 0});
		hpObject_->Update();
#pragma endregion hp



		if (oldAnimCT != animCT)
		{
			gameObject_.get()->PlayAnimation(animCT);
		}

	}
	else
	{
		gameObject_->AnimStop();
		gameObject_->Update();
		gameObject_->AnimPlay();
	}
	
}

void FbxPlayer::Draw(ID3D12GraphicsCommandList* cmdList)
{

	gameObject_->Draw(cmdList);

	//Object3d::PreDraw(cmdList);
	hpObject_->Draw(cmdList);
	//Object3d::PostDraw();

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
	//速度を0にする
	velocity_ = {0 , 0 , 0};

	//キー入力があったら
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D))
	{
		isRun = true;

		faceAngle_ -= cameraAngle_;

		//Z軸方向にの速度を入れる
		velocity_ = {0 , 0 , kMoveSpeed_};

		//W,Dを押していたら
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D))
		{

			//45度方向に向くように回転させる
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

		//W,Aを押していたら
		else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A))
		{

			//135度方向に向くように回転させる
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

		//S,Dを押していたら
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D))
		{

			//315度方向に向くように回転させる
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

		//S,Aを押していたら
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A))
		{

			//225度方向に向くように回転させる
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

		//Wを押していたら
		else if (input_->PushKey(DIK_W))
		{

			//0度方向に向くように回転させる
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

		//Sを押していたら
		else if (input_->PushKey(DIK_S))
		{

			//180度方向に向くように回転させる
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

		//Dを押していたら
		else if (input_->PushKey(DIK_D))
		{

			//90度方向に向くように回転させる
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

		//Aを押していたら
		else if (input_->PushKey(DIK_A))
		{
			//270度方向に向くように回転させる
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
