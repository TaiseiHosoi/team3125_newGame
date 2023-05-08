#pragma once
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "Mesh.h"
#include "sstream"
#include "memory"
#include "list"
#include "GameCamera.h"
#include "Player.h"
#include "CollisionPrimitive.h"
#include "FBXObject3d.h"
#include "fbx/FBXLoader.h"
#include "FBXModel.h"
#include "ParticleManager.h"

#include "SceneManager.h"

#include "DirectXCommon.h"
#include "Input.h"

// �V�[���Ǘ��̂��߂̊��N���X
// (�������N���X)

class IScene
{
protected:

	SceneManager* _controller;

public:

	IScene();
	virtual ~IScene();

	/// �V�[���̍X�V���s��
	virtual void Initialize(DirectXCommon* dxCommon, GameCamera* camera) = 0;

	/// �V�[���̍X�V���s��
	virtual void Update(Input* input,GameCamera* camera) = 0;

	/// �V�[���̕`����s��
	virtual void Draw(DirectXCommon* dxCommon) = 0;

};

