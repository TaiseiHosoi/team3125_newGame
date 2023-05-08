#pragma once
#include "Vector3.h"
#include "Matrix4.h"

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	// �P�ʃN�H�[�^�j�I������
	Quaternion()
		: x(0), y(0), z(0), w(1) {}

	// �w�肵���l�Ő���
	Quaternion(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) {}

public:
	// Quaternion�̐�
	Quaternion Multiply(const Quaternion& r);

	// �P��Quaternion��Ԃ�
	Quaternion IdentityQuaternion();

	// ����Quaternion��Ԃ�
	static Quaternion Conjugate(const Quaternion& quaternion);

	// Quaternion��norm��Ԃ�
	static float Norm(const Quaternion& quaternion);

	// ���K������Quaternion��Ԃ�
	Quaternion Normalize(const Quaternion& quaternion);

	// �tQuaternion��Ԃ�
	static Quaternion Inverse(const Quaternion& quaternion);

	// Quaternion -> Mat4�ɕϊ��������
	Matrix4 Quaternion2Matrix() const;

	// �C�ӎ���]��\��Quaternion�̐���
	Quaternion MakeAxisAngle(const Vector3& axis, float angle);

	// �x�N�g����Quaternion�ŉ�]���������ʂ̃x�N�g�������߂�
	Vector3 RotateVector(const Vector3& vector);

	// Quaternion�����]�s������߂�
	Matrix4 MakeRotateMatrix(const Quaternion& quaternion);

	// ���ʐ��`���
	Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

	// ����
	float Dot(const Quaternion& q0, const Quaternion& q1);

	// u����v�ւ̉�]�𐶐�
	Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);


public:
	Quaternion operator/(const float& s) const;
	Quaternion operator*(const float& s) const;
	Quaternion& operator/=(const float& s);
	Quaternion& operator*=(const float& s);
};
