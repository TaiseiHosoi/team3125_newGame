#include "WinApp.h"
#include"imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wPalam, LPARAM lParam);
WinApp* WinApp::WinApp_  = nullptr;
void WinApp::Initialize() {
#pragma region WindousAPI����������

	//�R���\�[���ւ̕����o��
	//OutputDebugStringA("Hello,DirectX!!\n");


	//�E�B���h�E�N���X�̐ݒ�
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		//�E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = L"DirectXGame";			//�E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);		//�E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	//�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);
	//�E�B���h�E�T�C�Y�o�@X���W�@Y���W�@�����@�c���@�p�v
	RECT wrc = { 0,0,window_width,window_height };
	//�����ŃT�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(w.lpszClassName,	//�N���X��
		L"DirectXGame",			//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	//�W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,			//�\��X���W�iOS�C���j
		CW_USEDEFAULT,			//�\��Y���W�iOS�C���j
		wrc.right - wrc.left,	//�E�B���h�E����
		wrc.bottom - wrc.top,	//�E�B���h�E�c��
		nullptr,				//�e�E�B���h�E�n���h��
		nullptr,				//���j���E�n���h��
		w.hInstance,			//�Ăяo���A�v���P�[�V�����n���h��
		nullptr);				//�I�v�V����

	//�E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd, SW_SHOW);


#pragma endregion WindousAPI����������
}

// �X�V
bool  WinApp::ProcessMessage() {

#pragma region �E�B���h�E���b�Z�[�W����
	//���b�Z�[�W������H
	MSG msg{};	//���b�Z�[�W
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�		
	}

	//X�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
#pragma endregion �E�B���h�E���b�Z�[�W����
}

//�E�B���h�E�N���X��o�^����
void WinApp::Finalize() {

	UnregisterClass(w.lpszClassName, w.hInstance);
}

WinApp* WinApp::GetInstance()
{
	if (WinApp_ == nullptr)
	{
		WinApp_ = new WinApp();
	}

	return WinApp_;
}

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	//ImGui�p�E�B���h�E�v���V�[�W���Ăяo��
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }

	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg) {
		//�E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}