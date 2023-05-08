#pragma once
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "Mesh.h"
#include "sstream"
#include "memory"
#include "list"
#include "GameCamera.h"
#include"CollisionPrimitive.h"
#include "CollisionManager.h"
#include"GameCamera.h"
#include"FBXObject3d.h"
#include"fbx/FBXLoader.h"
#include"FBXModel.h"
#include"ParticleManager.h"
#include "SceneManager.h"
#include "HitStopManager.h"


#include "FbxPlayer.h"




class GameScene {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxcomon);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	int gameLevel_ = 0;
	int levelMax_ = 8;
	int scene = 1;

private:
	DirectXCommon* dxCommon_ = nullptr;

	std::unique_ptr <GameCamera> gameCamera_;
	std::unique_ptr <Audio> audio;
	std::unique_ptr <SpriteCommon> spriteCommon_;
	std::unique_ptr <Object3d> object3d;
	std::unique_ptr <Mesh> model;
	std::unique_ptr<ParticleManager> particleManager_;
	Input* input_ = Input::GetInstance();

	std::unique_ptr <SceneManager> sceneManager_;
	HitStopManager* hitStopManager_;
	CollisionManager* collisionManager_ = nullptr;

private://リソース
	std::unique_ptr <Sprite> sprite1;
	std::unique_ptr <Sprite> sprite2;
	std::unique_ptr <Sprite> sprite3;

	std::unique_ptr <Mesh> bulletModel_;
private:


	//3Dモデル
	std::unique_ptr <Mesh> model_;
	std::unique_ptr <Mesh> ico_;
	std::unique_ptr <Mesh> land_;

	//FBXモデル
	std::unique_ptr<FBXModel> hitokunFbxM_;
	std::unique_ptr<FBXObject3d>hitokunFbxO_;


	std::unique_ptr<FBXModel> bossFbxM_;
	std::unique_ptr<FBXObject3d> bossFbxO_;


private://メソッド
	
	


	//レーンデバッグ用のテクスチャ
	//uint32_t laneTexture_[3];

	//待機中フラグ
	bool isStand_ = false;
	//待機タイマー
	int standTime_ = 0;

	int gameTimer_ = 0;

};