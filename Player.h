#pragma once
#include "Object3d.h"
#include "input.h"
#include "BaseCollider.h"

class Player
{
public:
	//コンストラクタ
	Player();

	//デストラクタ
	~Player();

	//3Dオブジェクト生成
	static std::unique_ptr<Player> Create(Mesh* model);

	//初期化
	void Initialize(Mesh* model);

	//更新
	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void OnCollision(const CollisionInfo& info);

	Object3d* GetObject3d();

	//デスフラグのSetter,Getter
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	bool GetIsDead() {return isDead_; }

private:
	//移動
	void Move();

private:

	//入力
	Input* input_ = Input::GetInstance();

	std::unique_ptr<Object3d>gameObject_;

	//移動速度
	const float kMoveSpeed_ = 0.5;
	//旋回速度
	const float kTurnSpeed_ = MathFunc::Dig2Rad(45);
	//移動ベクトル
	Vector3 velocity_;
	//自機の向き
	Vector3 faceAngle_ = {0 , 0 , 0};
	//カメラの向き
	Vector3 cameraAngle_ = {0 , 0 , 0};


	bool isDead_ = false;

	BaseCollider* collider_ = nullptr;

	bool onGround = true;

	Vector3 fallVec;


};