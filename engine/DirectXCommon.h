#pragma once

#include <Windows.h>
#include <cstdlib>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include<chrono>

#include "WinApp.h"


class DirectXCommon
{
public:
	// ������ 
	void Initialize(WinApp* winApp);
	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapchain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();

	// �`��O����
	void PreDraw();
	// �`��㏈��
	void PostDraw();

	ID3D12Device* GetDevice() const { return device_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }
	//�o�b�N�o�b�n�̐���w�擾
	size_t GetBackBufferCount() const { return backBuffers.size(); }
private:
	void InitializeFixFPS();	//������
	void UpdateFixFPS();	//FPS�Œ�X�V
	//�L�^����
	std::chrono::steady_clock::time_point reference_;

private:
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr < IDXGISwapChain4> swapChain_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	Microsoft::WRL::ComPtr < ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> commandList_;
	Microsoft::WRL::ComPtr < ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvHeap_;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	Microsoft::WRL::ComPtr <ID3D12Resource> depthBuff_;
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> dsvHeap_;
	Microsoft::WRL::ComPtr <ID3D12Fence> fence_;
	UINT64 fenceVal = 0;
	D3D12_RESOURCE_BARRIER barrierDesc{};


	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;


	WinApp* winApp_ = nullptr;

public:


};

