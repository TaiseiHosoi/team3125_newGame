#include "FBXObject3d.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"MathFunc.h"

using namespace Microsoft::WRL;
using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* FBXObject3d::device = nullptr;
Camera* FBXObject3d::camera = nullptr;
ComPtr<ID3D12RootSignature> FBXObject3d::rootsignature;
ComPtr<ID3D12PipelineState> FBXObject3d::pipelinestate;


void FBXObject3d::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	assert(device);

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXVS.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXPS.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//�e�����󂯂�{�[���ԍ�
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//�{�[���̃X�L���E�F�C�g(4��)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//CBV(�X�L�j���O�p)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootsignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelinestate.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

}

bool FBXObject3d::Initialize()
{

	HRESULT result;
	CD3DX12_HEAP_PROPERTIES unt6 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC unt7 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&unt6,
		D3D12_HEAP_FLAG_NONE,
		&unt7,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));

	CD3DX12_HEAP_PROPERTIES unt8 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC unt9 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);

	result = device->CreateCommittedResource(
		&unt8,
		D3D12_HEAP_FLAG_NONE,
		&unt9,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin));


	//1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames30);

	return true;

}

void FBXObject3d::Update()
{
	Matrix4 matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = MathFunc::Scale(wtf.scale_);
	matRot = MathFunc::Rotation(wtf.rotation_, 6);
	matTrans = MathFunc::Move(wtf.translation_);

	// ���[���h�s��̍���
	wtf.matWorld_.identity(); // �ό`�����Z�b�g
	wtf.matWorld_ *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	wtf.matWorld_ *= matRot; // ���[���h�s��ɉ�]�𔽉f
	wtf.matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	// �r���[�v���W�F�N�V�����s��
	const Matrix4& matViewProjection = camera->GetViewProjectionMatrix();
	// ���f���̃��b�V���g�����X�t�H�[��
	const XMMATRIX& modelTransform = fbxmodel->GetModelTransform();
	// �J�������W
	const Vector3& cameraPos = camera->GetEye();

	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->viewproj = MathFunc::ConvertMat4toXMMat(matViewProjection);
		constMap->world = modelTransform * MathFunc::ConvertMat4toXMMat(wtf.matWorld_);
		constMap->cameraPos = { cameraPos.x, cameraPos.y,cameraPos.z };
		constBuffTransform->Unmap(0, nullptr);
	}

	std::vector<FBXModel::Bone>& bones = fbxmodel->GetBones();

	//�A�j���[�V����
	if (isAnim == true) {
		if (isPlay) {
			//1�t���[���i�߂�
			currentTime += frameTime;
			//�Ō�܂Ői�߂���擪�ɖ߂�
			if (currentTime > endTime) {
				currentTime = startTime;
				if (animRot == true) {
					currentTime = endTime;
				}
			}
		}
	}



	//�萔�o�b�t�@�ւ̃f�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	if (bones.size() != bonesMat.size()) {
		ResizeBonesMat(bones);
	}
	for (int i = 0; i < bones.size(); i++) {
		//���̎p���s��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
		if (isBonesWorldMatCalc == true) {
			bonesMat[i] = MathFunc::ConvertXMMATtoMat4(constMapSkin->bones[i]) * wtf.matWorld_;
		}


	}
	constBuffSkin->Unmap(0, nullptr);



}

void FBXObject3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// ���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ�
	if (fbxmodel == nullptr) {
		return;
	}

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());
	// ���f���`��
	fbxmodel->Draw(cmdList);
}

std::unique_ptr<FBXObject3d> FBXObject3d::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	std::unique_ptr<FBXObject3d> fbxObject3d = std::make_unique<FBXObject3d>();
	if (fbxObject3d == nullptr) {
		return nullptr;
	}

	// ������
	if (!fbxObject3d->Initialize()) {
		//delete object3d;
		assert(0);
		return nullptr;
	}
	//�X�P�[�����Z�b�g
	float scale_val = 1;
	fbxObject3d->wtf.scale_ = { scale_val,scale_val ,scale_val };

	return fbxObject3d;
}


void FBXObject3d::SetFlame(int flame)
{
	isChangeFlame = true;
	currentTime = frameTime * flame;

}

void FBXObject3d::AnimPlay()
{
	isAnim = true;
}

void FBXObject3d::AnimStop()
{
	isAnim = false;
}

void FBXObject3d::AnimIsRotateChange(bool isRotate)
{
	animRot = isRotate;
}

Camera FBXObject3d::GetCamera()
{
	return *this->camera;
}

FbxTime FBXObject3d::GetCurrentTimer()
{
	return currentTime;
}

FbxTime FBXObject3d::GetEndTime()
{
	return endTime;
}

bool FBXObject3d::GetIsAnimRot()
{
	return animRot;
}

int FBXObject3d::ConvertFbxTimeToInt(FbxTime time)
{
	FbxLongLong frameCount = time.GetFrameCount();
	return static_cast<int>(frameCount);
}

void FBXObject3d::ResetCurrentTime(int animNum)
{
	FbxScene* fbxScene = fbxmodel->GetFbxScene();
	//0�Ԃ̃A�j���[�V�������擾
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(animNum);
	// �擾�����A�j���[�V�����ɕύX
	fbxScene->SetCurrentAnimationStack(animstack);
	//�A�j���[�V�����̖��O�擾
	const char* animstackname = animstack->GetName();
	//�A�j���[�V�����̎��ԏ��
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	//���Z��
	currentTime = takeinfo->mLocalTimeSpan.GetStart();
	
}



WorldTransform FBXObject3d::GetWorldTransform()
{
	return wtf;
}

WorldTransform* FBXObject3d::GetWorldTransformPtr()
{
	return &wtf;
}

void FBXObject3d::ResizeBonesMat(std::vector<FBXModel::Bone> bones_)
{
	bonesMat.resize(bones_.size());
}



std::vector<Matrix4>* FBXObject3d::GetBonesMatPtr()
{
	return &bonesMat;
}

void FBXObject3d::SetIsBonesWorldMatCalc(bool isCalc)
{
	isBonesWorldMatCalc = isCalc;
}

void FBXObject3d::PlayAnimation(int animationNum)
{
	FbxScene* fbxScene = fbxmodel->GetFbxScene();
	//0�Ԃ̃A�j���[�V�������擾
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(animationNum);
	// �擾�����A�j���[�V�����ɕύX
	fbxScene->SetCurrentAnimationStack(animstack);
	//�A�j���[�V�����̖��O�擾
	const char* animstackname = animstack->GetName();
	//�A�j���[�V�����̎��ԏ��
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	//�J�n���Ԏ擾
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ����ɂ���
	isPlay = true;
	

}

//���̃A�j���[�V������currentTime�𖳗����ύX����B
//PlayAnimation���Ăяo���ƃ��Z�b�g�����
void FBXObject3d::AnimFlameInter(FbxTime nowCount, FbxTime maxCount)
{
	FbxTime pDiv = endTime * 1 / maxCount;
	currentTime = pDiv * nowCount;

}


