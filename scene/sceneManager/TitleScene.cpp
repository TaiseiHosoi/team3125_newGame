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

	_controller->field_->Initialize();
}

void TitleScene::Update(Input* input, GameCamera* camera) {

	Vector3 a = {0,0,0};

	_controller->particleManager_->SetMatWorld(_controller->fbxPlayer_->GetObject3d()->GetWorldTransform().matWorld_);
	//_controller->particleManager2_->SetMatWorld(_controller->boss_->GetObject3d()->GetWorldTransform().matWorld_);

	/*_controller->particleManager_->SetBillboardMatWorld(_controller->fbxPlayer_->GetObject3d()->bMW);
	_controller->particleManager2_->SetBillboardMatWorld(_controller->bossFbxO_->bMW);*/

	_controller->fbxPlayer_.get()->Update();
	_controller->boss_.get()->Update();
	_controller->particleManager_->Update();
	//_controller->particleManager2_->Update();
	_controller->field_->Update();

	// パーティクル起動
	{
		//if (input->PushKey(DIK_P)) {

		//	//追加
		//	_controller->particleManager_->RandParticle();
		//	_controller->particleManager2_->RandParticle();

		//}
		
	}
	{
		if (input->TriggerKey(DIK_0)) {
			_controller->bossFbxO_->PlayAnimation(0);
			fbxTimer_ = 60;
		}
		if (input->TriggerKey(DIK_1)) {
			_controller->bossFbxO_->PlayAnimation(1);
			fbxTimer_ = 75;
		}
		if (input->TriggerKey(DIK_2)) {
			_controller->bossFbxO_->PlayAnimation(2);
			fbxTimer_ = 75;
		}
		if (input->TriggerKey(DIK_3)) {
			_controller->bossFbxO_->PlayAnimation(3);
			fbxTimer_ = 75;
		}
		if (input->TriggerKey(DIK_4)) {
			_controller->bossFbxO_->PlayAnimation(4);
			fbxTimer_ = 75;
		}
		if (input->TriggerKey(DIK_5)) {
			_controller->bossFbxO_->PlayAnimation(5);
			fbxTimer_ = 75;
		}
		if (input->TriggerKey(DIK_6)) {
			_controller->bossFbxO_->PlayAnimation(6);
			fbxTimer_ = 70;
		}
		if (input->TriggerKey(DIK_7)) {
			_controller->bossFbxO_->PlayAnimation(7);
			fbxTimer_ = 73;
		}

		if (fbxTimer_ > 0) {
			_controller->bossFbxO_->AnimPlay();
		}
		if (fbxTimer_ <= 0) {
			_controller->bossFbxO_->AnimStop();
		}
		fbxTimer_--;
	}

	ImGui::Begin("pat");

	//ImGui::InputFloat3("eye:x", &_controller->fbxPlayer_->GetObject3d()->GetWorldTransform().matWorld_.m[3][0]);
	////ImGui::InputFloat3("eye:x", &_controller->fbxPlayer_->GetObject3d()->GetWorldTransform().translation_.x);
	//ImGui::InputFloat3("eye:x", &_controller->fbxPlayer_->GetObject3d()->GetWorldTransformPtr()->translation_.x);
	
	//ImGui::InputFloat3("0000", &_controller->particleManager_->GetWorldTransform().matWorld_.m[3][0]);

	ImGui::End();

	if (input->TriggerKey(DIK_RETURN)) {
		_controller->ChangeScene(new GamePart1(_controller));
	}

	//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
	const float rnd_pos = 5.0f;
	Vector3 pos{};
	/*pos = wtf_.translation_;*/
	pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
	const float rnd_vel = 0.05f;
	Vector3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	// 重力に見立ててYのみ[-0.001f,0]でランダムに分布
	Vector3 acc{};
	const float rnd_acc = 0.001f;
	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

	Vector4 col{};
	const float rnd_col = 1.0f;
	col.x = (float)rand() / RAND_MAX * rnd_col;
	col.y = (float)rand() / RAND_MAX * rnd_col;
	col.z = (float)rand() / RAND_MAX * rnd_col;

	// 追加
	//_controller->particleManager2_->Add(60, pos, vel, acc, 1.0f, 0.0f);

}

void TitleScene::Draw(DirectXCommon* dxCommon) {

	_controller->spriteCommon_->SpritePreDraw();

	sprite1->Draw();
	sprite2->Draw();

	_controller->spriteCommon_->SpritePostDraw();

	_controller->field_->Draw(dxCommon);

	_controller->fbxPlayer_.get()->Draw(dxCommon->GetCommandList());
	_controller->boss_.get()->Draw();


	_controller->particleManager_->Draw(dxCommon->GetCommandList());
	
	//_controller->particleManager2_->Draw(dxCommon->GetCommandList());


}
