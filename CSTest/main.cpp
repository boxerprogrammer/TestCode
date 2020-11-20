//�R���s���[�g�V�F�[�_�͂��߂̈��
//�ЂƂ܂�UAV�ɃR���s���[�g�V�F�[�_����l���������ނ���
//�������񂾒l��CPU���猩���悤��READ_BACK�̃o�b�t�@�ɃR�s�[����
//Map�ŎQ�Ƃ��܂��B
//���������邽�߂̃e�X�g�p�R�[�h�Ȃ̂ŁA���e�Ɋւ��Ă�
//���@����������
#include<d3d12.h>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include<cassert>
#include<vector>
#include<string>
#include<iostream>

using namespace std;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")
namespace {
	/// <summary>
	/// �V�F�[�_�G���[���N�����Ƃ���ErrorBlob���o�͂���
	/// </summary>
	/// <param name="errBlob">errBlob</param>
	void OutputFromErrorBlob(ID3DBlob* errBlob)
	{
		if (errBlob != nullptr) {
			string errStr = "";
			auto errSize = errBlob->GetBufferSize();
			errStr.resize(errSize);
			copy_n((char*)errBlob->GetBufferPointer(), errSize, errStr.begin());
			OutputDebugStringA(errStr.c_str());
			errBlob->Release();
		}
	}

	//�g�p����D3D12�I�u�W�F�N�g
	ID3D12Device* dev_ = nullptr;//�f�o�C�X�I�u�W�F�N�g
	ID3D12CommandAllocator* cmdAlloc_ = nullptr;
	ID3D12GraphicsCommandList * cmdList_ = nullptr;//�R�}���h���X�g
	ID3D12CommandQueue* cmdQue_ = nullptr;
	ID3D12PipelineState* pipeline_ = nullptr;
	ID3D12RootSignature* rootSignature_ = nullptr;
	ID3D12DescriptorHeap* descriptorHeap_ = nullptr;
	ID3D12Fence* fence_ = nullptr;
	UINT64 fenceValue_ = 0;
}

//CPU���ŊǗ�����UAV�f�[�^
std::vector<float> uavdata(4 * 4 * 4);//�e�X�gUAV�f�[�^

/// <summary>
/// UAV�������݃o�b�t�@���쐬����
/// </summary>
/// <param name="dev">�f�o�C�X�I�u�W�F�N�g</param>
/// <param name="res">�v�Z���\�[�X(�Ԃ�l�p)</param>
/// <returns>result</returns>
HRESULT CreateUAVBuffer(ID3D12Device* dev,ID3D12Resource*& res) {
	HRESULT result= S_OK;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Width = sizeof(uavdata[0]) * uavdata.size();//UAV�̃T�C�Y���v�Z
	resDesc.DepthOrArraySize = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	result = dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
		IID_PPV_ARGS(&res));
	assert(SUCCEEDED(result));
	return result;
}

/// <summary>
/// UAV�̓��e��CPU�����猩�邽�߂̃R�s�[��o�b�t�@���쐬����
/// </summary>
/// <param name="dev">�f�o�C�X�I�u�W�F�N�g</param>
/// <param name="res">�R�s�[�惊�\�[�X(�Ԃ�l�p)</param>
/// <returns>result</returns>
HRESULT CreateCopyBuffer(ID3D12Device* dev, ID3D12Resource*& res) {
	HRESULT result = S_OK;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_READBACK;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Width = sizeof(uavdata[0]) * uavdata.size();//�R�s�[��̃T�C�Y���v�Z(UAV�Ɠ���)
	resDesc.DepthOrArraySize = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	result = dev->CreateCommittedResource(&heapProp, 
		D3D12_HEAP_FLAG_NONE, 
		&resDesc, 
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr,
		IID_PPV_ARGS(&res));
	assert(SUCCEEDED(result));
	return result;
}


/// <summary>
/// �f�o�b�O���C���[�I��
/// </summary>
void EnableDebugLayer()
{
	ID3D12Debug* debug;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
	debug->Release();
	
}

ID3D12RootSignature* CreateRootSignatureForComputeShader()
{
	HRESULT result = S_OK;
	ID3DBlob* errBlob = nullptr;
	D3D12_DESCRIPTOR_RANGE range[1] = {};
	range[0].BaseShaderRegister = 0;
	range[0].NumDescriptors = 1;
	range[0].OffsetInDescriptorsFromTableStart = 0;
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	range[0].RegisterSpace = 0;

	D3D12_ROOT_PARAMETER rp[1] = {};
	rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rp[0].DescriptorTable.NumDescriptorRanges = 1;
	rp[0].DescriptorTable.pDescriptorRanges = range;

	D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
	rootSigDesc.NumParameters = 1;
	rootSigDesc.pParameters = rp;
	rootSigDesc.NumStaticSamplers = 0;
	rootSigDesc.pStaticSamplers = nullptr;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;


	ID3DBlob* rootSigBlob = nullptr;
	D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errBlob);
	ID3D12RootSignature* rootSignature = nullptr;
	result = dev_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	return rootSignature;
}

//�㏈��
void Terminate()
{
	fence_->Release();
	descriptorHeap_->Release();
	rootSignature_->Release();
	pipeline_->Release();
	cmdQue_->Release();
	cmdList_->Release();
	cmdAlloc_->Release();
	dev_->Release();
}

/// <summary>
/// �R���s���[�g�V�F�[�_�̃��[�h
/// </summary>
/// <returns>�R���s���[�g�V�F�[�_Blob</returns>
ID3DBlob* LoadComputeShader()
{
	ID3DBlob* csBlob = nullptr;
	ID3DBlob* errBlob = nullptr;
	auto result = D3DCompileFromFile(L"ComputeShader.hlsl", nullptr, nullptr, "main", "cs_5_1", 0, 0, &csBlob, &errBlob);
	if (errBlob != nullptr) {
		OutputFromErrorBlob(errBlob);
	}
	assert(SUCCEEDED(result));
	return csBlob;
}

void CreateComputePipeline()
{
	ID3DBlob* csBlob = LoadComputeShader();
	D3D12_COMPUTE_PIPELINE_STATE_DESC pldesc = {};
	pldesc.CS.pShaderBytecode = csBlob->GetBufferPointer();
	pldesc.CS.BytecodeLength = csBlob->GetBufferSize();
	pldesc.NodeMask = 0;
	pldesc.pRootSignature = rootSignature_;
	auto result = dev_->CreateComputePipelineState(&pldesc, IID_PPV_ARGS(&pipeline_));
	assert(SUCCEEDED(result));
}

void CreateDescriptorHeap()
{
	HRESULT result = S_OK;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	result = dev_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptorHeap_));
	assert(SUCCEEDED(result));
}

void
CreateUAV(ID3D12Resource* res) {
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;//�o�b�t�@�Ƃ���
	uavDesc.Buffer.NumElements = 4 * 4 * 4;//�v�f�̑���
	uavDesc.Buffer.StructureByteStride = sizeof(float);//1������̑傫��
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	dev_->CreateUnorderedAccessView(res, nullptr, &uavDesc, descriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}

void ExecuteAndWait()
{
	ID3D12CommandList* cmdLists[] = { cmdList_ };
	cmdQue_->ExecuteCommandLists(1, cmdLists);
	cmdQue_->Signal(fence_, ++fenceValue_);
	//�҂�
	while (fence_->GetCompletedValue() < fenceValue_) {
		;
	}
}

int main() {
	HRESULT result = S_OK;
#ifdef _DEBUG
	EnableDebugLayer();
#endif
	result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&dev_));
	assert(SUCCEEDED(result));

	rootSignature_ = CreateRootSignatureForComputeShader();
	CreateComputePipeline();	
	CreateDescriptorHeap();

	D3D12_COMMAND_QUEUE_DESC queDesc = {};
	queDesc.NodeMask = 0;
	queDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queDesc.Priority = 0;
	queDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	dev_->CreateCommandQueue(&queDesc, IID_PPV_ARGS(&cmdQue_));
	
	//�R�}���h�A���P�[�^�쐬
	result = dev_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&cmdAlloc_));
	assert(SUCCEEDED(result));
	//�R�}���h���X�g�쐬
	result = dev_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, cmdAlloc_, pipeline_, IID_PPV_ARGS(&cmdList_));

	assert(SUCCEEDED(result));
	ID3D12Resource* uavBuffer = nullptr;
	CreateUAVBuffer(dev_, uavBuffer);
	CreateUAV(uavBuffer);

	cmdList_->SetComputeRootSignature(rootSignature_);
	ID3D12DescriptorHeap* descHeaps[] = { descriptorHeap_ };
	cmdList_->SetDescriptorHeaps(1, descHeaps);
	cmdList_->SetComputeRootDescriptorTable(0, descriptorHeap_->GetGPUDescriptorHandleForHeapStart());
	cmdList_->Dispatch(1, 1, 1);
	ID3D12Resource* cpyBuffer = nullptr;
	CreateCopyBuffer(dev_, cpyBuffer);

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = uavBuffer;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
	barrier.Transition.Subresource = 0;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	cmdList_->ResourceBarrier(1, &barrier);

	cmdList_->CopyResource(cpyBuffer, uavBuffer);

	cmdList_->Close();

	dev_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	
	ExecuteAndWait();

	float* mappedRes = nullptr;
	D3D12_RANGE rng = {};
	rng.Begin = 0;
	rng.End = uavdata.size() * sizeof(float);
	cpyBuffer->Map(0, &rng, (void**)(&mappedRes));
	copy_n(mappedRes,  uavdata.size(), uavdata.data());
	cpyBuffer->Unmap(0, nullptr);

	uavBuffer->Release();
	cpyBuffer->Release();
	Terminate();

	for (auto& d : uavdata) {
		cout << d << endl;
	}
	return 0;
}

