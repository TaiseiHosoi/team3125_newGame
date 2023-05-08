#include "Input.h"
#include<cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

/// <summary>
	/// ������
	/// </summary>
void Input::Initialize(WinApp* winApp) {
	HRESULT result;
	winApp_ = winApp;

	// DirectInput�̏�����
	result = DirectInput8Create(
		winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	// �L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(result));
	//�@�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	//�}�E�X�̐���
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));

	result = mouse->SetDataFormat(&c_dfDIMouse);

	result = mouse->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

/// <summary>
/// �X�V
/// </summary>
void Input::Update() {
	HRESULT result;

	// �O��̃L�[�ۑ�
	memcpy(keysPre, keys, sizeof(keys));

	//�L�[�{�[�h���̎擾�J�n
	result = keyboard->Acquire();
	//�S�L�[�̓��͏�Ԃ��擾����
	result = keyboard->GetDeviceState(sizeof(keys), keys);

	mouse->Acquire();

	//�S�L�[�̓��͏�Ԃ��擾����
	for (int i = 0; i < sizeof(DIMOUSESTATE::rgbButtons); i++) {
		oldMouseButton.rgbButtons[i] = mouseButton.rgbButtons[i];
	}

	mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseButton);
}

/// <summary>
/// �L�[�̉������`�F�b�N
/// </summary>
bool Input::PushKey(BYTE keyNumber) {

	// �w��L�[�������Ă����true��Ԃ�
	if (keys[keyNumber]) {
		return true;
	}

	return false;
}

/// <summary>
/// �L�[�̃g���K�[���`�F�b�N
/// </summary>
bool Input::TriggerKey(BYTE keyNumber) {
	// �w��L�[�������Ă����true��Ԃ�
	if (keysPre[keyNumber] == 0 && keys[keyNumber]) {
		return true;
	}

	return false;
}
bool Input::ReleaseKey(BYTE keyNumber)
{
	// �w��L�[�������Ă����true��Ԃ�
	if (keysPre[keyNumber] && keys[keyNumber] == 0) {
		return true;
	}

	return false;
}
Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

bool Input::PushMouseButton(unsigned char mouseButtons) {
	return mouseButton.rgbButtons[mouseButtons];
}

bool Input::TriggerMouseButton(unsigned char mouseButtons) {
	return mouseButton.rgbButtons[mouseButtons] && !oldMouseButton.rgbButtons[mouseButtons];
}

bool Input::ReleaseMouseButton(unsigned char mouseButtons) {
	return !mouseButton.rgbButtons[mouseButtons] && oldMouseButton.rgbButtons[mouseButtons];;
}