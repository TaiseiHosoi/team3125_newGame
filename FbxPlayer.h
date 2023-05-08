#pragma once
#include "Object3d.h"
#include "FBXObject3d.h"
#include "FBXModel.h"
#include "input.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"ParticleManager.h"

class FbxPlayer
{
public:
	//�R���X�g���N�^
	FbxPlayer();

	//�f�X�g���N�^
	~FbxPlayer();

	//������
	void Initialize(FBXModel* model);

	//�X�V
	void Update();

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//�_���[�W
	static void minusHp(int damage);

	FBXObject3d* GetObject3d();

	//�f�X�t���O��Setter,Getter
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	bool GetIsDead() { return isDead_; }

private:
	//�ړ�
	void Move();

public:
	Vector3 GetNowFaceAngle();
	ID3D12Resource* GetConstBuff() { return gameObject_->GetConstBuff(); };

private:

	//����
	Input* input_ = nullptr;

	//�I�u�W�F�N�g
	std::unique_ptr<FBXObject3d> gameObject_;
	int SPHERE_COLISSION_NUM;	//�R���C�_�[�i�X�t�B�A�j�̐�
	std::vector<Matrix4>* collisionBonesMat;	//�����蔻��p�̃{�[���̃��[���h�s��
	std::vector<SphereCollider*> sphere;
	Vector3 spherePos = {};

	//hp���f��
	std::unique_ptr<Object3d> hpObject_;
	std::unique_ptr<Mesh> hpModel_;
	std::unique_ptr<ParticleManager> particle_;
	


	//�ړ����x
	const float kMoveSpeed_ = 0.5;
	//���񑬓x
	const float kTurnSpeed_ = MathFunc::Dig2Rad(10);
	//�ړ��x�N�g��
	Vector3 velocity_;
	//���@�̌���
	Vector3 faceAngle_ = {0 , 0 , 0};
	//�J�����̌���
	Vector3 cameraAngle_ = {0 , 0 , 0};
	//�����Ă��邩�t���O
	bool isRun = false;
	bool oldIsRun = false;
	//�u���[�L�A�j���[�V�����p�t���O
	bool isbrake = false;
	int brakeFlameCount = 0;
	//�A�j���[�V�������Ԗڂ�
	int animCT = 2;
	int oldAnimCT = 0;
	//�U���t���O
	bool isAtk = false;

	//�U���p�t���\m
	int atkCurrent = 0;

	//�h�䎞�s��
	bool isGuard = false;	//�K�[�h�����邩���Ȃ���
	bool isGuardExcute = false;	//�K�[�h�������p
	int guardCurrent = 0;
	bool guardMovePhase = 0;
	//�����t���O�I���������J�ڃ��[�V�����������������t���O�I�t
	
	bool isCounter = false;	//�����t���O
	int atkMovePhase = 0;	//�U���p�t�F�[�Y
	int counterFrameCount = 0;	//�S�̃t���[��
	int phaseStartCount = 0;	//�s���̃t�F�[�Y���J�n�����t���[������ۑ�����
	int nowCTFlame = 0;	//�A�j���[�V�����p�t���[���J�E���g
	float kAccumulateRotVel = 0;

	int flame = 0;

	//�q�b�g�|�C���g
	int hitDeley = 0;	//���t���[���A���œ����邩
	static int hp;

	bool isHitStop = false;

	bool isDead_ = false;
};