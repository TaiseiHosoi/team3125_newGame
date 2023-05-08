#pragma once
#include"imgui.h"
#include"WinApp.h"
#include <d3dx12.h>
#include "DirectXCommon.h"


class ImGuiManager
{
public:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WinApp *winApp,DirectXCommon* dxCommon);

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	/// <summary>
	/// ��t�J�n
	/// </summary>
	void Begin();

	/// <summary>
	/// �I��
	/// </summary>
	void End();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
private:

	DirectXCommon* dxCommon_ = nullptr;

	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
};


