#include "Input.h"
#include<cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

/// <summary>
	/// 初期化
	/// </summary>
void Input::Initialize(WinApp* winApp) {
	HRESULT result;
	winApp_ = winApp;

	// DirectInputの初期化
	result = DirectInput8Create(
		winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	// キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	assert(SUCCEEDED(result));
	//　排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	//マウスの生成
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));

	result = mouse->SetDataFormat(&c_dfDIMouse);

	result = mouse->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

/// <summary>
/// 更新
/// </summary>
void Input::Update() {
	HRESULT result;

	// 前回のキー保存
	memcpy(keysPre, keys, sizeof(keys));

	//キーボード情報の取得開始
	result = keyboard->Acquire();
	//全キーの入力状態を取得する
	result = keyboard->GetDeviceState(sizeof(keys), keys);

	mouse->Acquire();

	//全キーの入力状態を取得する
	for (int i = 0; i < sizeof(DIMOUSESTATE::rgbButtons); i++) {
		oldMouseButton.rgbButtons[i] = mouseButton.rgbButtons[i];
	}

	mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseButton);
}

/// <summary>
/// キーの押下をチェック
/// </summary>
bool Input::PushKey(BYTE keyNumber) {

	// 指定キーを押していればtrueを返す
	if (keys[keyNumber]) {
		return true;
	}

	return false;
}

/// <summary>
/// キーのトリガーをチェック
/// </summary>
bool Input::TriggerKey(BYTE keyNumber) {
	// 指定キーを押していればtrueを返す
	if (keysPre[keyNumber] == 0 && keys[keyNumber]) {
		return true;
	}

	return false;
}
bool Input::ReleaseKey(BYTE keyNumber)
{
	// 指定キーを押していればtrueを返す
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