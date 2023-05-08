#pragma once
#include "IScene.h"

class TitleScene :public IScene
{
protected:
	SceneManager* _controller;

public:
	
	TitleScene(SceneManager* controller);
	~TitleScene() override;

	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;
	void Update(Input* input, GameCamera* camera) override;
	void Draw(DirectXCommon* dxCommon) override;
public:

private:
	
	Input* input_ = Input::GetInstance();

	std::unique_ptr <SceneManager> sceneManager_;

private://���\�[�X
	std::unique_ptr <Sprite> sprite1;
	std::unique_ptr <Sprite> sprite2;
	std::unique_ptr <Sprite> sprite3;

	std::unique_ptr <Mesh> bulletModel_;
private:


	//3D���f��
	std::unique_ptr <Mesh> model_;
	std::unique_ptr <Mesh> ico_;

	////FBX���f��
	//std::unique_ptr<FBXModel> hitokunFbxM_;
	//std::unique_ptr<FBXObject3d>hitokunFbxO_;


	//���[���f�o�b�O�p�̃e�N�X�`��
	//uint32_t laneTexture_[3];

	//�e�X�g3-2
	//�����蔻��@��
	Sphere sphere;
	//�����蔻��@����
	Plane plane;

	int collideCount;



	//�ҋ@���t���O
	bool isStand_ = false;
	//�ҋ@�^�C�}�[
	int standTime_ = 0;

	int gameTimer_ = 0;

	int fbxTimer_ = 0;

};