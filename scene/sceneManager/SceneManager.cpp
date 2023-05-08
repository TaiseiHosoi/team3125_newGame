#include "SceneManager.h"
#include "Input.h"
#include <cassert>
#include "DirectXCommon.h"

// シーンのインクルード
#include "SceneIntegrate.h"
#include "Ground.h"


SceneManager::SceneManager(DirectXCommon* dxCommon, GameCamera* camera) {
	_dxCommon = dxCommon;
	_scene.reset(new TitleScene(&*this));
	_camera = camera;
	

}
SceneManager::~SceneManager() {
	delete field_;
}
void SceneManager::ObjectInitialize() {

	//スプライト初期化
	spriteCommon_ = std::make_unique<SpriteCommon>();
	spriteCommon_->Initialize(_dxCommon);
	spriteCommon_->LoadTexture(1, "gamen.png");
	spriteCommon_->LoadTexture(2, "title.png");

	audio = std::make_unique<Audio>();
	audio->Initialize();
	// fbx テスト
	{
		//FBX	(テスト)
		// デバイスをセット
		FBXObject3d::SetDevice(_dxCommon->GetDevice());
		// グラフィックスパイプライン生成
		FBXObject3d::CreateGraphicsPipeline();
		hitokunFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("lowpoliInukun"));
		hitokunFbxO_ = std::make_unique<FBXObject3d>();
		hitokunFbxO_->Initialize();
		hitokunFbxO_->SetModel(hitokunFbxM_.get());
		hitokunFbxO_->SetPosition({ 0,0,0 });

		bossFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4"));
		bossFbxO_ = std::make_unique<FBXObject3d>();
		bossFbxO_->Initialize();
		bossFbxO_->SetModel(bossFbxM_.get());
		bossFbxO_->SetPosition({ 0,0,10 });
		bossFbxO_->SetScale({ 0.5,0.5,0.5 });
		bossFbxO_->PlayAnimation(0);
	}
	{
		//Player
		fbxPlayer_ = std::make_unique<FbxPlayer>();
		fbxPlayer_.get()->Initialize(hitokunFbxM_.get());
		//gameCamera_.get()->SetEyePos(fbxPlayer_.get()->GetObject3d()->GetWorldTransformPtr());
		_camera->SetFollowerPos(fbxPlayer_.get()->GetObject3d()->GetWorldTransformPtr());

		//boss
		boss_ = std::make_unique<Boss>();
		boss_.get()->Initialize(_dxCommon);
		boss_.get()->SetPlayer(fbxPlayer_.get());
		_camera->SetTargetPos(boss_.get()->GetObject3d()->GetWorldTransformPtr());
	}
	//パーティクルのセット
	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_.get()->Initialize();
	//パーティクル用素材
	particleManager_->LoadTexture("effect.png");
	particleManager_->Update();
	//field
	field_ = new Field;
	
}

void SceneManager::SceneInitialize() {
	_scene.get()->Initialize(_dxCommon,_camera);
	
}

void SceneManager::SceneUpdate(Input* input) {
	_scene.get()->Update(input,_camera);
	
}

void SceneManager::SceneDraw() {
	_scene.get()->Draw(_dxCommon);

}

void SceneManager::ChangeScene(IScene* scene) {
	_scene.reset(scene);
	SceneInitialize();

}