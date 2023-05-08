#pragma once
#include <wrl.h>
#include <Windows.h>
#include <dinput.h>
#include "WinApp.h"

//#define DIRECTINPUT_VERSION	0x0800

class Input
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号（DIK_0 等）</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号（DIK_0 等）</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);
	bool ReleaseKey(BYTE keyNumber);

	/// <summary>
	/// マウス操作
	/// </summary>
	/// <param name="mouseButtons"></param>
	/// <returns></returns>
	bool PushMouseButton(unsigned char mouseButtons);
	bool TriggerMouseButton(unsigned char mouseButtons);
	bool ReleaseMouseButton(unsigned char mouseButtons);

	static Input* GetInstance();

public:


	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	// windowsAPI
	WinApp* winApp_ = nullptr;
	// キーボードデバイスの生成
	ComPtr <IDirectInputDevice8> keyboard = nullptr;
	// DirectInputの初期化
	ComPtr<IDirectInput8> directInput = nullptr;
	// マウス入力初期化
	ComPtr<IDirectInputDevice8> mouse = nullptr;

	DIMOUSESTATE mouseButton;
	DIMOUSESTATE oldMouseButton;

	BYTE keys[256] = {};
	BYTE keysPre[256] = {};
};

