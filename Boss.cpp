#include "Boss.h"
#include "FbxPlayer.h"

 int Boss::hp = 100;	//hp実態

void Boss::Initialize(DirectXCommon* dxcomon)
{
	dxcomon_ = dxcomon;
	input_ = Input::GetInstance();

	FBXObject3d::SetDevice(dxcomon_->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	//ボスのFBX読み込み
	bossFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("Boss"));
	bossFbxO_ = std::make_unique<FBXObject3d>();
	bossFbxO_->Initialize();
	bossFbxO_->SetModel(bossFbxM_.get());

	bossFbxO_->PlayAnimation(0);
	bossFbxO_->SetPosition({ 0,0,30 });
	bossFbxO_->SetScale({ 2,2,2 });
	bossFbxO_->SetIsBonesWorldMatCalc(true);	// ボーンワールド行列計算あり
	bossFbxO_->Update();


	bossModel_ = Mesh::LoadFormOBJ("enemy", true);
	bossObj_ = Object3d::Create();

	bossObj_->SetModel(bossModel_.get());
	bossObj_->worldTransform.translation_ = { 0,0,30 };

	
	//testobj
	bossObj_ = Object3d::Create();
	ico_ = Mesh::LoadFormOBJ("ico", true);
	//testObj_->SetModel(ico_.get());

	bulletO_ = Object3d::Create();
	bulletM_ = Mesh::LoadFormOBJ("cube",true);
	bulletO_->SetModel(bulletM_.get());

	//当たり判定用
	SPHERE_COLISSION_NUM = bossFbxO_->GetBonesMatPtr()->size();
	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);
	bossFbxO_.get()->isBonesWorldMatCalc = true;	// ボーンの行列を取得するか


	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = bossFbxO_.get()->bonesMat[i].GetWorldPos();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(5.0f);
		sphere[i]->Update();
		sphere[i]->SetAttribute(COLLISION_ATTR_ENEMIES);
		//test


	}

	hp = 100;
	hpModel_ = Mesh::LoadFormOBJ("cube", false);
	hpObject_ = Object3d::Create();
	hpObject_->SetModel(hpModel_.get());
	hpObject_->SetScale({ static_cast<float>(hp) * 0.2f,1.3f,0.02f });
	hpObject_->SetPosition({ bossFbxO_->GetWorldTransform().translation_.x,
		 bossFbxO_->GetWorldTransform().translation_.y + 12.0f,
		 bossFbxO_->GetWorldTransform().translation_.z });
	Matrix4 invViewPro = MathFunc::MakeInverse(&hpObject_.get()->camera_->GetViewProjectionMatrix());
	float yaw = atan2f(-invViewPro.m[2][0], sqrtf(invViewPro.m[2][1] * invViewPro.m[2][1] + invViewPro.m[2][2] * invViewPro.m[2][2]));
	hpObject_->SetRotate({ 0,bossFbxO_->GetWorldTransform().rotation_.y,0});
	hpObject_->Update();
}

void Boss::Update()
{

	srand(time(nullptr));
	//デスフラグが立った球を削除
	bullets_.remove_if([](std::unique_ptr<BossBullet>& bullet) { return bullet->isDead(); });


	//デスフラグが立った球を削除
	lasers_.remove_if([](std::unique_ptr<BossLaser>& laser) { return laser->isDead(); });
	//デスフラグが立った球を削除
	bursts_.remove_if([](std::unique_ptr<BossBurstBullet>& burst) { return burst->isDead(); });


	for (std::unique_ptr<BossBullet>& bullet : bullets_)
	{
		bullet->Update();
	}


	for (std::unique_ptr<BossLaser>& laser :lasers_)
	{
		laser->Update();
	}
	for (std::unique_ptr<BossBurstBullet>& burst : bursts_)
	{
		burst->Update();
	}

	if (mode > 1)
	{
		mode = 0;
	}


	switch (mode)
	{
		//追いかけパート時

	case BattleMode::Run:
		RunAway();
		if (input_->TriggerKey(DIK_RETURN))
		{
			mode++;
			bossObj_->worldTransform.translation_ = { 0,0,30 };
			bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);
			Reset();
		}
		break;


		//地上戦パート時

	case BattleMode::Ground:

		if (isAttack == false)
		{
			attackValue = AttackValue::nasi;
			attackTimer--;

			/*bossObj_->worldTransform.translation_ = { 0,0,30 };*/
			bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

		}
		if (attackTimer <= 0)
		{
			isAttack = true;

			attackValue = rand() % 4 + 1;
			attackValue = AttackValue::TACKLE;
			attackValue = AttackValue::FIRE;
			//attackValue = AttackValue::LASER;
			//attackValue = AttackValue::TRAMPLE;
			//attackValue = AttackValue::CROSSLANGE;

			attackTimer = 240.0f;
		}


		if (attackValue == AttackValue::FIRE)
		{
			if (isFire == false)
			{
				isFire = true;
			}
			Shot();

		}

		else if (attackValue == AttackValue::TACKLE)
		{
			Tackle();
		}

		else if (attackValue == AttackValue::LASER)
		{
			if (isLaser == false)
			{
				isLaser = true;
			}
			Laser();
		}

		else if (attackValue == AttackValue::TRAMPLE)
		{
			Trample();
		}

		else if (attackValue == AttackValue::CROSSLANGE)
		{
			if (isclCharge == false)
			{
				isclCharge = true;
			}
			CrossLange();
		}

		if (input_->TriggerKey(DIK_RETURN))
		{
			mode++;
			Reset();
		}
		if (input_->TriggerKey(DIK_0))
		{
			if (isFire == false)
			{
				isFire = true;
			}
			else
			{
				isFire = false;
			}
		}
		break;

		//平台パート時
	case BattleMode::Flatbed:
		if (isFAttack == false)
		{

		}
		if (isFAttack == true)
		{

		}
		break;
	}
	/*Tackle();

	Move();*/

	//bossObj_->Update();
	bossFbxO_->Update();

	hpObject_->SetScale({ static_cast<float>(hp) * 0.2f,1.3f,0.02f });
	hpObject_->SetPosition({ bossFbxO_->GetWorldTransform().translation_.x,
		 bossFbxO_->GetWorldTransform().translation_.y + 25.0f,
		 bossFbxO_->GetWorldTransform().translation_.z });
	Matrix4 invViewPro = MathFunc::MakeInverse(&hpObject_.get()->camera_->GetViewProjectionMatrix());
	float yaw = atan2f(-invViewPro.m[2][0], sqrtf(invViewPro.m[2][1] * invViewPro.m[2][1] + invViewPro.m[2][2] * invViewPro.m[2][2]));
	hpObject_->SetRotate({ 0,bossFbxO_->GetWorldTransform().rotation_.y,0 });
	hpObject_->Update();

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = bossFbxO_.get()->bonesMat[i].GetWorldPos();
		sphere[i]->Update();
	}

}

void Boss::Draw()
{

	bossFbxO_->Draw(dxcomon_->GetCommandList());

	for (std::unique_ptr<BossBullet>& bullet : bullets_)
	{
		bullet->Draw(dxcomon_->GetCommandList());
	}


	for (std::unique_ptr<BossLaser>& laser : lasers_)
	{
		laser->Draw(dxcomon_->GetCommandList());
	}
	for (std::unique_ptr<BossBurstBullet>& burst : bursts_)
	{
		burst->Draw(dxcomon_->GetCommandList());
	}
}

void Boss::RunAway()
{
	float speed = 1.0f;

	bossObj_->worldTransform.translation_.z += speed;
	bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

}

void Boss::Shot()
{

	Vector3 playerPos;
	Vector3 bulletPos;
	Vector3 distance;

	

	////敵の向きを自機の正面
	//angle = (atan2(distance.x, distance.z) + 3.141592f / 2);
	//bossFbxO_->SetRotate(Vector3(0, (angle + 3.141592f / 2) * 1, 0));

	bulletTimer--;
	if (bulletTimer <= 0)
	{
		if (isFire == true)
		{
			bulletPos = GetPosition();

			// 弾を生成し初期化
			std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
			newBullet->Initialize(bulletM_.get(),bossObj_->worldTransform.translation_);

			bulletTimer = 60.0f;
			bulletCount++;
			//
			newBullet->SetPlayer(player_);
			newBullet->SetCollider(new SphereCollider);

			bullets_.push_back(std::move(newBullet));

		}

	}

	//弾の数が3個以上だった時

	if (bulletCount >= 3)
	{
		isAttack = false;
		isFire = false;
		bulletCount = 0;
	}
		

	if (isFire == false)
	{
		playerPos = player_->GetObject3d()->GetPosition();
		
		distance - playerPos - bulletPos;
	
	}
}

void Boss::Tackle()
{


	float speed = 0.1f;

	Vector3 bossPos = bossObj_->GetPosition();
	Vector3 bossPosO_ = bossFbxO_->GetPosition();


	tackleDis = playerPos - bossPos;

	if (isTackle == false)
	{

		moveTimer--;
	}
	if (moveTimer < 0)
	{

		if (isTackle == false)
		{
			isTackle = true;
			moveTimer = 60.0f;

		}
	}
	if (isTackle == true)
	{
		stopTimer--;
	}
	if (stopTimer < 0)
	{
		isTackle = false;

		stopTimer = 60.0f;
		tackleCount++;
	}
	if (tackleCount < 3)
	{
		if (isTackle == false)
		{

			if (moveTimer >= 45.0f)
			{
				playerPos = player_->GetObject3d()->GetPosition();
				//向きをキャラのいる方向を常に正面にする
				angle = (atan2(tackleDis.x, tackleDis.z) + 3.141592f / 2);

				bossFbxO_->SetRotate(Vector3(0, (angle + 3.141592f / 2) * 1, 0));
			}
			
		}
		if (isTackle == true)
		{
			tackleDis *= speed;
			

			bossObj_->worldTransform.translation_.x += tackleDis.x;
			bossObj_->worldTransform.translation_.y += tackleDis.y;
			bossObj_->worldTransform.translation_.z += tackleDis.z;

			bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

			tackleDis.x = std::abs(tackleDis.x);
			tackleDis.z = std::abs(tackleDis.z);


		}

	}
	else
	{
		isAttack = false;
		tackleCount = 0;
	}



}

void Boss::Reset()
{

	//bossObj_->worldTransform.rotation_ = { 0,0,0 };
	////bossObj_->worldTransform.translation_ = { 0,0,30 };
	//bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);


	//bossFbxO_->SetRotate(Vector3(0, 0, 0));

	moveTimer = 60.0f;
	stopTimer = 60.0f;


#pragma region  フラグ関係のリセット


	isAttack = false;
	

	isTackle = false;

	//
	isFire = false;

	//
	isLaser = false;

	//
	isJump = false;

	isTrample = false;

	isReturn = false;

#pragma endregion 


#pragma region  タイマー関係のリセット
	//
	attackTimer = 180.0f;

	coolTimer = 90.0f;

	//
	moveTimer = 60.0f;

	stopTimer = 60.0f;

	//
	bulletTimer = 60.0f;

	//
	laserTimer = 180.0f;

	lcoolTimer = 90.0f;

	//
	jumpTimer = 60.0f;

	tramChageTimer = 120.0f;

	trampleTimer = 120.0f;

#pragma endregion
#pragma region  pos関係のリセット
	//
	distance = { 0,0,0 };

	normDis = { 0,0,0 };

	//
	playerPos = { 0,0,0 };

	tackleDis = { 0,0,0 };

	normTdis = { 0,0,0 };

	//
	bulletDistance = { 0,0,0 };

	//
	tramPlayerPos = {0,0,0};

	tramDistance = { 0,0,0 };

	//
	lPlayerPos = { 0,0,0 };

	laserPos = { 0,0,0 };

	lDistance = { 0,0,0 };

#pragma endregion

#pragma region  カウント関係のリセット
	//
	tackleCount = 0;
	//
	bulletCount = 0;
	//
	laserCount = 0;

#pragma endregion


	burstCount = 0;

	bcoolTimer = 3.0f;

	burstTimer = 180.0f;

	clTimer = 60.0f;

	clChargeTimer = 180.0f;

	isclCharge = false;

	isCross = false;

	isBurst = false;

}


Vector3 Boss::GetPosition()
{
	return bossObj_->worldTransform.translation_;
}

Boss* Boss::GetInstance()
{
	static Boss instance;
	return &instance;
}


void Boss::Laser()
{

	laserTimer--;
	
	laserPos = bossObj_->GetPosition();


	//敵の向きを自キャラの正面に向くように回転させる
	angle = (atan2(lDistance.x, lDistance.z) + 3.141592f / 2);
	//回転を反映
	bossFbxO_->SetRotate(Vector3(0, (angle + 3.141592f / 2) * 1, 0));
	

	if (laserTimer < 0)
	{
		
		if (isLaser == true)
		{
			//レーザーの攻撃回数が3回未満の時
			if (laserCount < 3)
			{
				

				// 弾を生成し初期化
				std::unique_ptr<BossLaser> newBullet = std::make_unique<BossLaser>();
				newBullet->Initialize(Vector3(bossObj_->worldTransform.translation_.x, bossObj_->worldTransform.translation_.y + 8, bossObj_->worldTransform.translation_.z));

				laserTimer = 60.0f;
				laserCount++;
				//
				newBullet->SetPlayer(player_);
				newBullet->SetCollider(new SphereCollider);

				lasers_.push_back(std::move(newBullet));
			}
			else
			{
				isAttack = false;
				laserCount = 0;
			}
		}
	}
	else
	{
		lPlayerPos = player_->GetObject3d()->GetPosition();

		lDistance = lPlayerPos - laserPos;
	}


}

void Boss::Trample()
{
	/*Vector3 returnPos = {0,0,30};

	Vector3 returnDis = bossFbxO_->GetPosition() - returnPos;*/


	Vector3 bossPosF_ = bossObj_->GetPosition();

	//自機とボスの位置の差分ベクトル
	Vector3 rotate = tramPlayerPos - bossPosF_;

	if (isTrample == false)
	{
		tramPlayerPos = player_->GetObject3d()->GetPosition();
	}


	Vector3 bossToJumpDis;
	//ジャンプ位置とボスの位置の差分ベクトル
	bossToJumpDis = JumpPos - bossPosF_;

	//自機とボスのジャンプ位置の差分ベクトル
	tramDistance = tramPlayerPos - JumpPos;

#pragma region ジャンプ前
	if (isJump == false)
	{
		if (tramChageTimer > 0)
		{
			tramChageTimer--;
		}
		if (isTrample == false)
		{
			//敵の向きを自機の正面
			angle = (atan2(rotate.x, rotate.z) + 3.141592f / 2);

			bossFbxO_->SetRotate(Vector3(0, (angle + 3.141592f / 2) * 1, 0));
		}

	}
	if (tramChageTimer <= 0)
	{
		if (isJump == false)
		{
			
			JumpPos.x = player_->GetObject3d()->wtf.translation_.x;
			JumpPos.y = player_->GetObject3d()->wtf.translation_.y + 10;
			JumpPos.z = player_->GetObject3d()->wtf.translation_.z;
			isJump = true;
		}
	}
#pragma endregion 

#pragma region ジャンプ中
	if (isJump == true)
	{
		if (jumpTimer > 0)
		{
			jumpTimer--;


			//trampleSpeed = Ease::OutQuad(0,0,0, trampleSpeed);

			bossToJumpDis *= (trampleSpeed / 5);

			bossObj_->worldTransform.translation_.x += bossToJumpDis.x;
			bossObj_->worldTransform.translation_.y += bossToJumpDis.y;
			bossObj_->worldTransform.translation_.z += bossToJumpDis.z;

			bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

			/*bossFbxO_->SetPosition(JumpPos);*/

		}
		else
		{
			isJump = false;
			if (isTrample == false)
			{
				isTrample = true;
			}
		}
	}
#pragma endregion 

#pragma region 
	if (isTrample == true)
	{
		trampleTimer--;
		tramDistance *= trampleSpeed;
		bossObj_->worldTransform.translation_.y += tramDistance.y;

		bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

		tackleDis.x = std::abs(tackleDis.x);
		tackleDis.z = std::abs(tackleDis.z);
	}
	if (trampleTimer <= 0)
	{
		/*if (isReturn == false)
		{
			returnDis*= (trampleSpeed / 5);

			bossObj_->worldTransform.translation_.x += returnDis.x;
			bossObj_->worldTransform.translation_.y += returnDis.y;
			bossObj_->worldTransform.translation_.z += returnDis.z;

			bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

			returnDis.x = std::abs(returnDis.x);
			returnDis.z = std::abs(returnDis.z);

			if (returnDis.x || returnDis.z <= 0)
			{
				isReturn == true;
			}
		}*/

		if (isAttack == true)
		{
			isAttack = false;
			isJump = false;
			isTrample = false;
			trampleTimer = 120.0f;
			tramChageTimer = 120.0f;
			jumpTimer = 60.0f;
		}
		bossFbxO_->SetRotate(Vector3(0, 0, 0));
	}
#pragma endregion 
	/*if (trampleCount < 3)
	{

	}*/

}
void Boss::CrossLange()
{

	float speed = 0.1f;

	//突進のチャージをしているとき
	if (isclCharge == true)
	{
		clChargeTimer--;
	}
	//射撃しているとき
	if (isBurst == true)
	{
		burstTimer--;
		bcoolTimer--;
	}

	crossLangePos = bossFbxO_->GetPosition();
	
	//射撃フラグがfalseの時
	if (isBurst == false)
	{
		clDistance = clPlayerPos - crossLangePos;

	}

	//敵の向きの調整
	if (clChargeTimer > 90.0f || isBurst == true)
	{
		clPlayerPos = player_->GetObject3d()->GetPosition();
		clRotate = clPlayerPos - crossLangePos;

		//敵の向きを自機の正面
		angle = (atan2(clRotate.x, clRotate.z) + 3.141592f / 2);

		bossFbxO_->SetRotate(Vector3(0, (angle + 3.141592f / 2) * 1, 0));
	}
	//チャージタイマーが0以下の時
	if(clChargeTimer<=0)
	{
		isclCharge = false;
		isCross = true;
	}

	//突進フラグがtrueの時
	if (isCross == true)
	{
		clTimer--;

		clDistance *= speed;

		bossObj_->worldTransform.translation_.x += clDistance.x;
		bossObj_->worldTransform.translation_.y += clDistance.y;
		bossObj_->worldTransform.translation_.z += clDistance.z;

		bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);


		
	}

	//突進中タイマーが0以下の時
	if (clTimer <= 0)
	{
		isCross = false;
		isBurst = true;
	}

	//射撃間隔が0以下になったとき
	if (bcoolTimer <= 0)
	{
		//射撃フラグがオン
		if (isBurst == true)
		{
			//弾の数が30未満
			if (burstCount < 30)
			{
				// 弾を生成し初期化
				std::unique_ptr<BossBurstBullet> newBullet = std::make_unique<BossBurstBullet>();
				newBullet->Initialize(bulletM_.get(), Vector3(bossObj_->worldTransform.translation_.x, bossObj_->worldTransform.translation_.y + 8, bossObj_->worldTransform.translation_.z));

				burstCount++;
				bcoolTimer = 3.0f;
				//
				newBullet->SetPlayer(player_);
				newBullet->SetCollider(new SphereCollider);

				bursts_.push_back(std::move(newBullet));
			}


		}
	}
	//全体の射撃時間が0以下になったとき
	if (burstTimer <= 0)
	{
		Reset();
		burstCount = 0;
		isBurst = false;
		isAttack = false;
		
	}
}
FBXObject3d* Boss::GetObject3d()
{
	return bossFbxO_.get();

}

void Boss::minusHp(int damage)
{
	hp -= damage;
}
