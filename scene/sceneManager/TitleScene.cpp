#include "SceneIntegrate.h"



TitleScene::TitleScene(SceneManager* controller) {
	_controller = controller;
}
TitleScene::~TitleScene() {

}

void TitleScene::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {
	sprite1 = std::make_unique<Sprite>();
	sprite1->Initialize(_controller->spriteCommon_.get(), 1);
	sprite2 = std::make_unique<Sprite>();
	sprite2->Initialize(_controller->spriteCommon_.get(), 2);


	sprite1->SetSize({ 1280,720 });
	sprite2->SetSize({ 1280,720 });
	sprite1->SetPozition({ 0,0 });
	sprite2->SetPozition({ 0,0 });


}

void TitleScene::Update(Input* input, GameCamera* camera) {

	Vector3 a = {0,0,0};

	_controller->particleManager_->SetMatWorld(_controller->fbxPlayer_->GetObject3d()->GetWorldTransform().matWorld_);
	//_controller->particleManager2_->SetMatWorld(_controller->boss_->GetObject3d()->GetWorldTransform().matWorld_);

	/*_controller->particleManager_->SetBillboardMatWorld(_controller->fbxPlayer_->GetObject3d()->bMW);
	_controller->particleManager2_->SetBillboardMatWorld(_controller->bossFbxO_->bMW);*/

	_controller->fbxPlayer_.get()->Update();

	_controller->particleManager_->Update();
	//_controller->particleManager2_->Update();
	
	

	

	if (input->TriggerKey(DIK_RETURN)) {
		_controller->ChangeScene(new GamePart1(_controller));
	}

	

}

void TitleScene::Draw(DirectXCommon* dxCommon) {

	_controller->spriteCommon_->SpritePreDraw();

	sprite1->Draw();
	sprite2->Draw();

	_controller->spriteCommon_->SpritePostDraw();



	_controller->fbxPlayer_.get()->Draw(dxCommon->GetCommandList());
	


	_controller->particleManager_->Draw(dxCommon->GetCommandList());
	
	


}
