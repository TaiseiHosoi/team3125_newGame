#pragma once
#include <DirectXMath.h>
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"


//#include "Object3d.h"
//#include <WorldTransform.h>


namespace MathFunc {

	//�~����
	const float PI = 3.141592654f;
	// �x���@->���W�A���ϊ�
	float Dig2Rad(float value);
	// ���W�A��->�x���@�ϊ� 
	float Rad2Dig(float value);

	Matrix4 Initialize();
	Matrix4 Scale(Vector3 scale);
	Matrix4 Rotation(Vector3 rotation, int X_1_Y_2_Z_3_XYZ_6);
	Matrix4 Move(Vector3 move);
	//void  affin(WorldTransform& affin);

	Vector3 MatVector(const Vector3 vector3, const Matrix4 matrix4);

	Vector3 GetWorldtransform(const Matrix4 matrix4);

	Vector3 AddVector(const Vector3 v1, const Vector3 v2);

	Vector3 bVelocity(Vector3& velocity, Matrix4& mat);


	//���W�ϊ�
	Vector3 wDivision(const Vector3& v, const Matrix4& m);

	float FieldOfViewY(float focalLengs, float sensor);

	Matrix4 ConvertXMMATtoMat4(DirectX::XMMATRIX XMMatrix);
	DirectX::XMMATRIX ConvertMat4toXMMat(Matrix4 m);

	Matrix4 MakeInverse(const Matrix4* mat);
};

