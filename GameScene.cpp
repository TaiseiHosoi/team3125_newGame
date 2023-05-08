#include "GameScene.h"

#include<sstream>
#include<iomanip>
#include"imgui.h"




GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxcomon)
{
	assert(dxcomon);
	dxCommon_ = dxcomon;

	collisionManager_ = CollisionManager::GetInstance();

	//スプライト初期化
	spriteCommon_ = std::make_unique<SpriteCommon>();
	spriteCommon_->Initialize(dxCommon_);
	spriteCommon_->LoadTexture(1, "gamen.png");
	spriteCommon_->LoadTexture(2, "title.png");


	sprite1 = std::make_unique<Sprite>();
	sprite1->Initialize(spriteCommon_.get(), 1);
	sprite2 = std::make_unique<Sprite>();
	sprite2->Initialize(spriteCommon_.get(), 2);


	sprite1->SetSize({ 1280 , 720 });
	sprite2->SetSize({ 1280 , 720 });
	sprite1->SetPozition({ 0 , 0 });
	sprite2->SetPozition({ 0 , 0 });
	audio = std::make_unique<Audio>();
	audio->Initialize();


	collisionManager_ = CollisionManager::GetInstance();
	/*collisionManager_->*/
	
	

	//gamescene

	//モデル
	model = Mesh::LoadFormOBJ("lowpoliInukun", true);
	bulletModel_ = Mesh::LoadFormOBJ("bume", true);
	model_ = Mesh::LoadFormOBJ("cube", true);
	ico_ = Mesh::LoadFormOBJ("ico", true);
	land_ = Mesh::LoadFormOBJ("testLand", true);

	//FBX	(テスト)
	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon_->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();
	hitokunFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("lowpoliInukun"));
	hitokunFbxO_ = std::make_unique<FBXObject3d>();
	hitokunFbxO_->Initialize();
	hitokunFbxO_->SetModel(hitokunFbxM_.get());
	hitokunFbxO_->PlayAnimation(0);
	hitokunFbxO_->SetPosition({ 0,0,0 });

	

	//パーティクルのセット
	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_.get()->Initialize();

	//かめら初期化
	gameCamera_ = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height, input_);
	assert(gameCamera_);

	//カメラのポインタをセット
	//カメラ位置セット
	//gameCamera_->SetTarget(hitokunFbxO_.get()->wtf.translation_);
	gameCamera_->SetEye({ 0 , 5 , -20 });
	gameCamera_->SetTarget({ 0 , 0 , 0 });
	Object3d::SetCamera(gameCamera_.get());
	FBXObject3d::SetCamera(gameCamera_.get());
	ParticleManager::SetCamera(gameCamera_.get());


	//object3d
	object3d = Object3d::Create();
	//object3d->SetRimEmission();
	object3d->worldTransform.translation_.z = 0.0f;
	object3d->worldTransform.scale_ = Vector3(1000, 1000, 1000);
	object3d->SetRimEmission(0.7f);
	object3d->SetModel(model.get());
	object3d->Update();

	//gameCamera_->SetTargetPos(&object3d.get()->worldTransform);


	//パーティクル用素材

	particleManager_->LoadTexture("effect.png");
	particleManager_->Update();

	////Player
	//fbxPlayer_ = std::make_unique<FbxPlayer>();
	//fbxPlayer_.get()->Initialize(hitokunFbxM_.get());
	////gameCamera_.get()->SetEyePos(fbxPlayer_.get()->GetObject3d()->GetWorldTransformPtr());
	//gameCamera_->SetFollowerPos(fbxPlayer_.get()->GetObject3d()->GetWorldTransformPtr());

	////boss
	//boss_ = std::make_unique<Boss>();
	//boss_.get()->Initialize(dxCommon_);
	//boss_.get()->SetPlayer(fbxPlayer_.get());
	//gameCamera_.get()->SetTargetPos(boss_.get()->GetObject3d()->GetWorldTransformPtr());

	//field
	field_ = std::make_unique<Field>();
	field_.get()->Initialize();
	field_.get()->Update();

	sceneManager_ = std::make_unique<SceneManager>(dxCommon_, gameCamera_.get());
	sceneManager_->ObjectInitialize();
	sceneManager_->SceneInitialize();

	hitStopManager_ = HitStopManager::GetInstance();
	hitStopManager_->SetGameCamera(gameCamera_.get());
}

void GameScene::Update()
{
	//particleManager_->RandParticle({ 0,0,0 });
	gameCamera_->Update();

	collisionManager_->CheckAllCollisions();

	sceneManager_->SceneUpdate(input_);

	//particleManager_.get()->Update();

	//object3d.get()->Update();


	//fbxPlayer_.get()->Update();

	//boss_.get()->Update();
	////fbxtest
	//if (input_->TriggerKey(DIK_V)) {
	//	hitokunFbxO_.get()->PlayAnimation();
	//}
	//hitokunFbxO_->Update();

	float kCameraVel = 0.02f;
	Vector3 nowEye = gameCamera_->GetEye();

	gameCamera_->SetTarget(hitokunFbxO_.get()->wtf.translation_);

	if (input_->PushKey(DIK_RIGHT)) {
		nowEye.x += kCameraVel;
	}
	else if (input_->PushKey(DIK_LEFT)) {
		nowEye.x -= kCameraVel;
	}else if (input_->PushKey(DIK_UP)) {
		nowEye.z += kCameraVel;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		nowEye.z -= kCameraVel;
	}

	//フィールドの更新
	field_.get()->Update();


	ImGui::Begin("Camera");

	ImGui::SliderFloat("eye:x", &nowEye.x, -400.0f, 400.0f);
	ImGui::SliderFloat("eye:xz", &nowEye.z, -400.0f, 400.0f);

	ImGui::End();

	/*gameCamera_->SetEye(nowEye);*/

	hitStopManager_->Update();

	//カメラのアップデート
	gameCamera_->Update();

}

void GameScene::Draw()
{

	/*sceneManager_->SceneDraw();*/
	
	spriteCommon_->SpritePreDraw();

	//field_.get()->Draw(dxCommon_);

	sceneManager_->SceneDraw();


	//spriteCommon_->SpritePreDraw();

	//sprite1->Draw();
	//sprite2->Draw();

	//spriteCommon_->SpritePostDraw();



	//object3d->Draw();
	
		//object3d->Draw();
		/*fbxPlayer_.get()->Draw(dxCommon_->GetCommandList());
		boss_.get()->Draw();*/
		


	particleManager_->Draw(dxCommon_->GetCommandList());


}

