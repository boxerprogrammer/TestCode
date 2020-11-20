//コンピュートシェーダはじめの一歩
//ひとまずUAVにコンピュートシェーダから値を書き込むだけ
//書き込んだ値をCPUから見れるようにREAD_BACKのバッファにコピーして
//Mapで参照します。
//理解をするためのテスト用コードなので、内容に関しては
//お察しください
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
	/// シェーダエラーが起きたときのErrorBlobを出力する
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

	//使用するD3D12オブジェクト
	ID3D12Device* dev_ = nullptr;//デバイスオブジェクト
	ID3D12CommandAllocator* cmdAlloc_ = nullptr;
	ID3D12GraphicsCommandList * cmdList_ = nullptr;//コマンドリスト
	ID3D12CommandQueue* cmdQue_ = nullptr;
	ID3D12PipelineState* pipeline_ = nullptr;
	ID3D12RootSignature* rootSignature_ = nullptr;
	ID3D12DescriptorHeap* descriptorHeap_ = nullptr;
	ID3D12Fence* fence_ = nullptr;
	UINT64 fenceValue_ = 0;
}

//CPU側で管理するUAVデータ
std::vector<float> uavdata(4 * 4 * 4);//テストUAVデータ

/// <summary>
/// UAV書き込みバッファを作成する
/// </summary>
/// <param name="dev">デバイスオブジェクト</param>
/// <param name="res">計算リソース(返り値用)</param>
/// <returns>result</returns>
HRESULT CreateUAVBuffer(ID3D12Device* dev,ID3D12Resource*& res) {
	HRESULT result= S_OK;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Width = sizeof(uavdata[0]) * uavdata.size();//UAVのサイズを計算
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
/// UAVの内容をCPU側から見るためのコピー先バッファを作成する
/// </summary>
/// <param name="dev">デバイスオブジェクト</param>
/// <param name="res">コピー先リソース(返り値用)</param>
/// <returns>result</returns>
HRESULT CreateCopyBuffer(ID3D12Device* dev, ID3D12Resource*& res) {
	HRESULT result = S_OK;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_READBACK;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Width = sizeof(uavdata[0]) * uavdata.size();//コピー先のサイズを計算(UAVと同じ)
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
/// デバッグレイヤーオン
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

//後処理
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
/// コンピュートシェーダのロード
/// </summary>
/// <returns>コンピュートシェーダBlob</returns>
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
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;//バッファとして
	uavDesc.Buffer.NumElements = 4 * 4 * 4;//要素の総数
	uavDesc.Buffer.StructureByteStride = sizeof(float);//1個当たりの大きさ
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	dev_->CreateUnorderedAccessView(res, nullptr, &uavDesc, descriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}

void ExecuteAndWait()
{
	ID3D12CommandList* cmdLists[] = { cmdList_ };
	cmdQue_->ExecuteCommandLists(1, cmdLists);
	cmdQue_->Signal(fence_, ++fenceValue_);
	//待ち
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
	
	//コマンドアロケータ作成
	result = dev_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&cmdAlloc_));
	assert(SUCCEEDED(result));
	//コマンドリスト作成
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

