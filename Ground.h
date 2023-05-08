#pragma once
#include "DirectXCommon.h"

#include "Vector3.h"
#include "input.h"
#include "Object3d.h"
#include"Mesh.h"
class Field
{
	//�R���X�g���N�^
public:
	Field();
	~Field();

public:
	void Initialize();

	void Update();

	void Draw(DirectXCommon* dxcomon_);


	//�����o�ϐ�
private:
	std::unique_ptr<Object3d> gameObject_;	//�n��
	std::unique_ptr<Mesh> fieldM;

	std::unique_ptr<Object3d> skydomeObj_;
	std::unique_ptr<Mesh> skydomeModel_;
	const int fenceLen_ = 4;
	Object3d* fence_[4] = {};	//�t�F���X
	Mesh* fenceM = nullptr;


};
