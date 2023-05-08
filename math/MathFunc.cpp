#include "MathFunc.h"
#include <cmath>

const float PI = 3.141592f;

float MathFunc::Dig2Rad(float dig)
{
	float rad;

	rad = dig * (PI / 180);

	return rad;
}

float MathFunc::Rad2Dig(float rad)
{
	float deg;

	deg = rad * (180 / PI);

	return deg;
}

Matrix4 MathFunc::Initialize() {
	Matrix4 matInitialize = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return matInitialize;
}

Matrix4 MathFunc::Scale(Vector3 scale) {

	//�X�P�[�����O�s���錾
	Matrix4 matScale = {
		scale.x, 0.0f, 0.0f,    0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f,    0.0f, scale.z, 0.0f,
		0.0f, 0.0f,    0.0f, 1.0f
	};

	Matrix4 scaling = Initialize();
	scaling *= matScale;

	return scaling;
}

Matrix4 MathFunc::Rotation(Vector3 rotation, int X_1_Y_2_Z_3_XYZ_6) {
	int rotationX = 1;
	int rotationY = 2;
	int rotationZ = 3;
	int rotationXYZ = 6;

	if (X_1_Y_2_Z_3_XYZ_6 == rotationX) {
		Matrix4 matRotX = {
		  1.0f,0.0f,0.0f,0.0f,
		  0.0f,cos(rotation.x),sin(rotation.x),0.0f,
		  0.0f,-sin(rotation.x),cos(rotation.x),0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		Matrix4 rotationX = Initialize();
		rotationX *= matRotX;

		return rotationX;
	}
	else if (X_1_Y_2_Z_3_XYZ_6 == rotationY) {
		Matrix4 matRotY = {
			cos(rotation.y), 0.0f, -sin(rotation.y), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sin(rotation.y), 0.0f, cos(rotation.y),  0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		Matrix4 rotationY = Initialize();
		rotationY *= matRotY;

		return rotationY;
	}
	else if (X_1_Y_2_Z_3_XYZ_6 == rotationZ) {
		Matrix4 matRotZ = {
		  cos(rotation.z),sin(rotation.z),0.0f,0.0f,
		  -sin(rotation.z),cos(rotation.z),0.0f,0.0f,
		  0.0f,0.0f,1.0f,0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		Matrix4 rotationZ = Initialize();
		rotationZ *= matRotZ;

		return rotationZ;
	}
	else {
		Matrix4 matRotXYZ;
		Matrix4 matRot_X, matRot_Y, matRot_Z;
		matRot_X = {
		  1.0f,0.0f,0.0f,0.0f,
		  0.0f,cos(rotation.x),sin(rotation.x),0.0f,
		  0.0f,-sin(rotation.x),cos(rotation.x),0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		matRot_Y = {
			cos(rotation.y), 0.0f, -sin(rotation.y),
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			sin(rotation.y), 0.0f, cos(rotation.y),
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};
		matRot_Z = {
		  cos(rotation.z),sin(rotation.z),0.0f,0.0f,
		  -sin(rotation.z),cos(rotation.z),0.0f,0.0f,
		  0.0f,0.0f,1.0f,0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		//�e���̉�]�s�������
		matRotXYZ = Initialize();

		matRotXYZ *= matRot_X;
		matRotXYZ *= matRot_Y;
		matRotXYZ *= matRot_Z;

		return matRotXYZ;
	}
}

Matrix4 MathFunc::Move(Vector3 Move) {
	Matrix4 matMove = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		Move.x, Move.y, Move.z, 1.0f
	};
	
	return matMove;
}

//void AffinTrans::affin(WorldTransform& affin) {
//	affin.matWorld_ = Initialize();
//	affin.matWorld_ *= Scale(affin.scale_);
//	affin.matWorld_ *= Rotation(affin.rotation_, 6);
//	affin.matWorld_ *= Move(affin.translation_);
//}

Vector3 MathFunc::MatVector(const Vector3 vector3, const Matrix4 matrix4) {

	Vector3 retVec = {};

	retVec.x =
		vector3.x * matrix4.m[0][0] + vector3.y * matrix4.m[1][0] + vector3.z * matrix4.m[2][0];

	retVec.y =
		vector3.x * matrix4.m[0][1] + vector3.y * matrix4.m[1][1] + vector3.z * matrix4.m[2][1];

	retVec.z =
		vector3.x * matrix4.m[0][2] + vector3.y * matrix4.m[1][2] + vector3.z * matrix4.m[2][2];

	return retVec;

}

Vector3 MathFunc:: bVelocity(Vector3& velocity, Matrix4& mat) {

	Vector3 result = { 0, 0, 0 };


	result.x = velocity.x * mat.m[0][0] +
		velocity.y * mat.m[1][0] +
		velocity.z * mat.m[2][0];

	result.y = velocity.x * mat.m[0][1] +
		velocity.y * mat.m[1][1] +
		velocity.z * mat.m[2][1];

	result.z = velocity.x * mat.m[0][2] +
		velocity.y * mat.m[1][2] +
		velocity.z * mat.m[2][2];


	return result;
}

Vector3 MathFunc::GetWorldtransform(const Matrix4 matrix4) {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ�����
	worldPos.x = matrix4.m[3][0];
	worldPos.y = matrix4.m[3][1];
	worldPos.z = matrix4.m[3][2];

	return worldPos;
}

Vector3 MathFunc::AddVector(const Vector3 v1, const Vector3 v2) {

	Vector3 addVec = {};

	addVec.x = v1.x + v2.x;

	addVec.y = v1.y + v2.y;

	addVec.z = v1.z + v2.z;

	return addVec;

}



Vector3 MathFunc::wDivision(const Vector3& vector3, const Matrix4& matrix4) {

	Vector4 divVec = {};

	divVec.x = vector3.x * matrix4.m[0][0] + vector3.y * matrix4.m[1][0] + vector3.z * matrix4.m[2][0] + 1 * matrix4.m[3][0];

	divVec.y = vector3.x * matrix4.m[0][1] + vector3.y * matrix4.m[1][1] + vector3.z * matrix4.m[2][1] + 1 * matrix4.m[3][1];

	divVec.z = vector3.x * matrix4.m[0][2] + vector3.y * matrix4.m[1][2] + vector3.z * matrix4.m[2][2] + 1 * matrix4.m[3][2];

	divVec.w = vector3.x * matrix4.m[0][3] + vector3.y * matrix4.m[1][3] + vector3.z * matrix4.m[2][3] + 1 * matrix4.m[3][3];

	divVec.x = divVec.x / divVec.w;

	divVec.y = divVec.y / divVec.w;

	divVec.z = divVec.z / divVec.w;

	return { divVec.x, divVec.y, divVec.z };
}

float MathFunc::FieldOfViewY(float focalLengs, float sensor) {

	return 2 * atan(sensor / (2 * focalLengs));

}

Matrix4 MathFunc::ConvertXMMATtoMat4(DirectX::XMMATRIX XMMatrix) {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {

		result.m[i][0] = DirectX::XMVectorGetX(XMMatrix.r[i]);
		result.m[i][1] = DirectX::XMVectorGetY(XMMatrix.r[i]);
		result.m[i][2] = DirectX::XMVectorGetZ(XMMatrix.r[i]);
		result.m[i][3] = DirectX::XMVectorGetW(XMMatrix.r[i]);
	}


	return result;
}

DirectX::XMMATRIX MathFunc::ConvertMat4toXMMat(Matrix4 m) {
	DirectX::XMMATRIX result;
	result = DirectX::XMMatrixSet(
		m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
		m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
		m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
		m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);

	return result;
}

Matrix4 MathFunc::MakeInverse(const Matrix4* mat)
{
	assert(mat);

	//�|���o���@���s���s��
	float sweep[4][8]{};
	//�萔�{�p
	float constTimes = 0.0f;
	//���e����덷
	float MAX_ERR = 1e-10f;
	//�߂�l�p
	Matrix4 retMat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//weep�̍����ɋt�s������߂�s����Z�b�g
			sweep[i][j] = mat->m[i][j];

			//sweep�̉E���ɒP�ʍs����Z�b�g
			sweep[i][4 + j] = Matrix4::MakeIdentity().m[i][j];
		}
	}

	//�S�Ă̗�̑Ίp�����ɑ΂���J��Ԃ�
	for (int i = 0; i < 4; i++)
	{
		//�ő�̐�Βl�𒍖ڑΊp�����̐�Βl�Ɖ���
		float max = fabs(sweep[i][i]);
		int maxIndex = i;

		//i��ڂ��ő�̐�Βl�ƂȂ�s��T��
		for (int j = i + 1; j < 4; j++)
		{
			if (fabs(sweep[j][i]) > max)
			{
				max = fabs(sweep[j][i]);
				maxIndex = j;
			}
		}

		if (fabs(sweep[maxIndex][i]) <= MAX_ERR)
		{
			//�t�s��͋��߂��Ȃ�
			return Matrix4::MakeIdentity();
		}

		//����(1):i�s�ڂ�maxIndex�s�ڂ����ւ���
		if (i != maxIndex)
		{
			for (int j = 0; j < 8; j++)
			{
				float tmp = sweep[maxIndex][j];
				sweep[maxIndex][j] = sweep[i][j];
				sweep[i][j] = tmp;
			}
		}

		//sweep[i][i]�Ɋ|�����1�ɂȂ�l�����߂�
		constTimes = 1 / sweep[i][i];

		//����(2):p�s�ڂ�a�{����
		for (int j = 0; j < 8; j++)
		{
			//����ɂ��sweep[i][i]��1�ɂȂ�
			sweep[i][j] *= constTimes;
		}

		//����(3)�ɂ��i�s�ڈȊO�̍s��i��ڂ�0�ɂ���
		for (int j = 0; j < 4; j++)
		{
			if (j == i)
			{
				//i�s�ڂ͂��̂܂�
				continue;
			}

			//i�s�ڂɊ|����l�����߂�
			constTimes = -sweep[j][i];

			for (int k = 0; k < 8; k++)
			{
				//j�s�ڂ�i�s�ڂ�a�{�����s�𑫂�
				//����ɂ��sweep[j][i]��0�ɂȂ�
				sweep[j][k] += sweep[i][k] * constTimes;
			}
		}
	}

	//sweep�̉E������mat�̋t�s��
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			retMat.m[i][j] = sweep[i][4 + j];
		}
	}

	return retMat;
}