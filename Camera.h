#pragma once

#include <DirectXMath.h>
#include <Vector3.h>
#include<Vector2.h>
#include<Matrix4.h>
#include"MathFunc.h"

/// <summary>
/// �J������{�@�\
/// </summary>
class Camera
{

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	Camera(int window_width, int window_height);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Camera() = default;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	void UpdateProjectionMatrix();

	float GetAxisY();

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	inline const Matrix4& GetViewMatrix() {
		return matView;
	}

	/// <summary>
	/// �ˉe�s��̎擾
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	inline const Matrix4& GetProjectionMatrix() {
		return matProjection;
	}

	/// <summary>
	/// �r���[�ˉe�s��̎擾
	/// </summary>
	/// <returns>�r���[�ˉe�s��</returns>
	inline const Matrix4& GetViewProjectionMatrix() {
		return matViewProjection;
	}

	/// <summary>
	/// �r���{�[�h�s��̎擾
	/// </summary>
	/// <returns>�r���{�[�h�s��</returns>
	inline const Matrix4& GetBillboardMatrix() {
		return matBillboard;
	}
	inline const Matrix4& GetBillboardYMatrix() {
		return matBillboardY;
	}

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	inline const Vector3& GetEye() {
		return eye;
	}

	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="eye">���W</param>
	inline void SetEye(Vector3 eye) {
		this->eye = eye; viewDirty = true;
	}

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	inline const Vector3& GetTarget() {
		return target;
	}

	/// <summary>
	/// �����_���W�̐ݒ�
	/// </summary>
	/// <param name="target">���W</param>
	inline void SetTarget(Vector3 target) {
		this->target = target; viewDirty = true;
	}

	/// <summary>
	/// ������x�N�g���̎擾
	/// </summary>
	/// <returns>������x�N�g��</returns>
	inline const Vector3& GetUp() {
		return up;
	}

	/// <summary>
	/// ������x�N�g���̐ݒ�
	/// </summary>
	/// <param name="up">������x�N�g��</param>
	inline void SetUp(Vector3 up) {
		this->up = up; viewDirty = true;
	}

	// sensor
	inline const float& GetSensor() {
		return sensor;
	}
	inline void SetSensor(float sensor) {
		this->sensor = sensor; viewDirty = true;
	}
	// FL
	inline const float& GetForcalLengs() {
		return focalLengs;
	}
	inline void SetFocalLengs(float focalLengs) {
		this->focalLengs = focalLengs; viewDirty = true;
	}


	/// <summary>
	/// �x�N�g���ɂ�鎋�_�ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveEyeVector(const Vector3& move);
	//void MoveEyeVector(const Vector3& move);

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveVector(const Vector3& move);
	//void MoveVector(const Vector3& move);

	float FieldOfViewY();

	static void MakePerspectiveL(float fovAngleY, float aspect, float near_, float far_, Matrix4& matrix);

	static void MakeLookL(const Vector3& eye, const Vector3& target, const Vector3& up, Matrix4& mat);

protected: // �����o�ϐ�
	
	// �r���[�s��
	Matrix4 matView = MathFunc::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// �r���{�[�h�s��
	static Matrix4 matBillboard ;
	// Y�����r���{�[�h�s��
	static Matrix4 matBillboardY;
	// �ˉe�s��
	Matrix4 matProjection = MathFunc::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// �r���[�ˉe�s��
	Matrix4 matViewProjection = MathFunc::ConvertXMMATtoMat4(DirectX::XMMatrixIdentity());
	// �r���[�s��_�[�e�B�t���O
	bool viewDirty = false;
	// �ˉe�s��_�[�e�B�t���O
	bool projectionDirty = false;
	// ���_���W
	Vector3 eye = { 0, 0, -1 };
	// �����_���W
	Vector3 target = { 0, 0, 0 };
	// ������x�N�g��
	Vector3 up = { 0, 1, 0 };
	// �A�X�y�N�g��
	float aspectRatio = 1.0f;

	float focalLengs = 50;
	float sensor = 35;
};

