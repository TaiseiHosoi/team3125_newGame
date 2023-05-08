#pragma once
#include <memory>
#include <stack>


#include "FBXObject3d.h"
#include "fbx/FBXLoader.h"
#include "FBXModel.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "Mesh.h"
#include "Object3d.h"
#include "ParticleManager.h"

#include "FbxPlayer.h"
#include "Boss.h"
#include "Ground.h"


class Input;
class IScene;
class DirectXCommon;
class GameCamera;

// �V�[���Ǘ��N���X
class SceneManager
{
private:
	std::shared_ptr<IScene> _scene;
	DirectXCommon* _dxCommon;
	GameCamera* _camera;
public:



	SceneManager(DirectXCommon* dxCommon,GameCamera* camera);
	~SceneManager();	

	//�I�u�W�F�N�g��Initialize���Ăяo��
	void ObjectInitialize();
	// �e�V�[����Initialize���Ăяo��
	void SceneInitialize();
	// �e�V�[����Update���Ăяo��
	void SceneUpdate(Input* input);
	// �e�V�[����Draw���Ăяo��
	void SceneDraw();

	// �V�[����ύX����
	void ChangeScene(IScene*);
	//ID3D12GraphicsCommandList* GetCommandList() { return _dxCommon->GetCommandList(); };
public:
	
	std::unique_ptr <Audio> audio;
	std::unique_ptr <SpriteCommon> spriteCommon_;

	std::unique_ptr<ParticleManager> particleManager_;
	std::unique_ptr<ParticleManager> particleManager2_;
	
	std::unique_ptr <Object3d> object3d;
	std::unique_ptr <Mesh> model;

	//FBX���f��
	std::unique_ptr<FBXModel> hitokunFbxM_;
	std::unique_ptr<FBXObject3d>hitokunFbxO_;
	//FBX���f��
	std::unique_ptr<FBXModel> bossFbxM_;
	std::unique_ptr<FBXObject3d> bossFbxO_;

	std::unique_ptr<FbxPlayer> fbxPlayer_;
	std::unique_ptr<Boss> boss_;

	Field* field_;


};

//	�Q�ƌ�
//	https://qiita.com/tsuchinokoman/items/390a22a20abe2a6daa6d <-�����킩���