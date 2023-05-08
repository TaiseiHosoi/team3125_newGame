#pragma once
#include <Windows.h>

class WinApp
{
public:// �ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	// ������
	void Initialize();

	// �X�V
	bool ProcessMessage();

	//�E�B���h�E�N���X��o�^����
	void Finalize();

	// getter
	HWND GetHwnd()const { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }

	// �V���O���g���C���X�^���X�̎擾
	static WinApp* GetInstance();

private:
	// �E�B���h�E�n���h��
	HWND hwnd = nullptr;
	//�E�B���h�E�N���X
	WNDCLASSEX w{};

	
public:

	//�E�B���h�E�T�C�Y
	static const int window_width = 1280;	//����
	static const int window_height = 720;	//�c��
	//�V���O���g��
	static WinApp* WinApp_;
};

