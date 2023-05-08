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
	/// ������
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ��iDIK_0 ���j</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ��iDIK_0 ���j</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(BYTE keyNumber);
	bool ReleaseKey(BYTE keyNumber);

	/// <summary>
	/// �}�E�X����
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
	// �L�[�{�[�h�f�o�C�X�̐���
	ComPtr <IDirectInputDevice8> keyboard = nullptr;
	// DirectInput�̏�����
	ComPtr<IDirectInput8> directInput = nullptr;
	// �}�E�X���͏�����
	ComPtr<IDirectInputDevice8> mouse = nullptr;

	DIMOUSESTATE mouseButton;
	DIMOUSESTATE oldMouseButton;

	BYTE keys[256] = {};
	BYTE keysPre[256] = {};
};

