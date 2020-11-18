#include<d3d12.h>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include<cassert>
#include<vector>
#include<string>

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

	ID3D12Resource* res_;//�v�Z�f�[�^�p���\�[�X
	ID3D12Device* dev_;//�f�o�C�X�I�u�W�F�N�g
	ID3D12GraphicsCommandList * cmdList_;//�R�}���h���X�g
	ID3D12PipelineState* _pipeline;

}
std::vector<float> uavdata(4 * 4 * 4);//�e�X�gUAV�f�[�^

/// <summary>
/// UAV�o�b�t�@���쐬����
/// </summary>
/// <param name="dev">�f�o�C�X�I�u�W�F�N�g</param>
/// <param name="res">�v�Z���\�[�X(�Ԃ�l�p)</param>
/// <returns>����</returns>
HRESULT CreateUAVBuffer(ID3D12Device* dev,ID3D12Resource*& res) {
	HRESULT result= S_OK;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Width = sizeof(uavdata[0]) * uavdata.size();
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

int main() {
	HRESULT result = S_OK;

	ID3D12Debug* debug;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
	debug->Release();
	result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&dev_));
	assert(SUCCEEDED(result));

	ID3DBlob* csBlob = nullptr;
	ID3DBlob* errBlob = nullptr;
	result = D3DCompileFromFile(L"ComputeShader.hlsl", nullptr, nullptr, "main", "cs_5_1", 0, 0, &csBlob, &errBlob);
	if (errBlob != nullptr) {
		OutputFromErrorBlob(errBlob);
	}

	D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
	rootSigDesc.NumParameters = 0;
	rootSigDesc.pParameters = nullptr;
	rootSigDesc.NumStaticSamplers = 0;
	rootSigDesc.pStaticSamplers = nullptr;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;

	D3D12_COMPUTE_PIPELINE_STATE_DESC pldesc = {};
	pldesc.CS.pShaderBytecode = csBlob->GetBufferPointer();
	pldesc.CS.BytecodeLength = csBlob->GetBufferSize();
	pldesc.NodeMask = 0;

	pldesc.pRootSignature;
	dev_->CreateComputePipelineState(&pldesc, IID_PPV_ARGS(&_pipeline));


	ID3D12CommandAllocator* cmdAlloc = nullptr;
	//�R�}���h�A���P�[�^�쐬
	dev_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&cmdAlloc));
	//�R�}���h���X�g�쐬
	dev_->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_COMPUTE, cmdAlloc,nullptr,IID_PPV_ARGS(&cmdList_));

	



	ID3D12Resource* uavBuffer = nullptr;
	CreateUAVBuffer(dev_,uavBuffer);
	cmdList_->Dispatch(1, 1, 1);

	assert(SUCCEEDED(result));

	dev_->Release();
	return 0;
}