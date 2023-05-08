#include "Ground.h"

Field::Field()
{
}

Field::~Field()
{
	
}

void Field::Initialize()
{
	fieldM = std::make_unique<Mesh>();
	fieldM = Mesh::LoadFormOBJ("fieldkari",false);

	gameObject_ = std::make_unique<Object3d>();
	gameObject_.get()->Initialize(true);
	gameObject_.get()->SetModel(fieldM.get());
	gameObject_->worldTransform.scale_ = { 20,10,20 };
	gameObject_->worldTransform.translation_ = { 0,-2,0 };
	gameObject_->Update();

	skydomeModel_ = std::make_unique<Mesh>();
	skydomeModel_ = Mesh::LoadFormOBJ("skydome", false);

	skydomeObj_ = std::make_unique<Object3d>();
	skydomeObj_.get()->Initialize(false);
	skydomeObj_.get()->SetModel(skydomeModel_.get());
	skydomeObj_->worldTransform.scale_ = { 400,200,400 };
	skydomeObj_->Update();




}

void Field::Update()
{
	gameObject_->Update();
	skydomeObj_->Update();
}

void Field::Draw(DirectXCommon* dxcomon_)
{
	//Object3d::PreDraw(dxcomon_->GetCommandList());
	
	gameObject_->Draw(dxcomon_->GetCommandList());
	//skydomeObj_->Draw();
	
	//Object3d::PostDraw();
}
