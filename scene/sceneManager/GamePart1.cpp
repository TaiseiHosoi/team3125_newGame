#include "SceneIntegrate.h"


GamePart1::GamePart1(SceneManager* controller) {
	_controller = controller;
}

GamePart1::~GamePart1() {

}

void GamePart1::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {

	
}

void GamePart1::Update(Input* input, GameCamera* camera) {

	if (input->TriggerKey(DIK_RETURN)) {
		_controller->ChangeScene(new GamePart2(_controller));
	}
	_controller->fbxPlayer_.get()->Update();

}

void GamePart1::Draw(DirectXCommon* dxCommon) {
	_controller->fbxPlayer_.get()->Draw(dxCommon->GetCommandList());

}