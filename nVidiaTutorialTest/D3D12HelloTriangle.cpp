//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
#include<stdexcept>
#include<cmath>
#include<Windows.h>
#include<DirectXTex.h>
#include"stdafx.h"

#include"nv_helpers_dx12/TopLevelASGenerator.h"
#include"nv_helpers_dx12/BottomLevelASGenerator.h"
#include"nv_helpers_dx12/RaytracingPipelineGenerator.h"
#include"nv_helpers_dx12/RootSignatureGenerator.h"
#include"nv_helpers_dx12/ShaderBindingTableGenerator.h"
#include"DXRHelper.h"
#include "D3D12HelloTriangle.h"

namespace {
	XMVECTOR bufferData[] = {//9個のデータ
	//A
	XMVECTOR{1.0f,1.0f,0.0f,1.0f},
	XMVECTOR{1.0f,0.4f,0.0f,1.0f},
	XMVECTOR{1.0f,0.7f,0.0f,1.0f},

	//B
	XMVECTOR{0.0f,1.0f,1.0f,1.0f},
	XMVECTOR{0.0f,1.0f,0.4f,1.0f},
	XMVECTOR{0.0f,1.0f,0.7f,1.0f},

	//C
	XMVECTOR{1.0f,0.0f,1.0f,1.0f},
	XMVECTOR{0.4f,0.0f,1.0f,1.0f},
	XMVECTOR{0.7f,0.0f,1.0f,1.0f},
	};

	///string(マルチバイト文字列)からwstring(ワイド文字列)を得る
///@param str マルチバイト文字列
///@return 変換されたワイド文字列
	std::wstring
		GetWideStringFromString(const std::string& str) {
		//呼び出し1回目(文字列数を得る)
		auto num1 = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
			str.c_str(), -1, nullptr, 0);

		std::wstring wstr;//stringのwchar_t版
		wstr.resize(num1);//得られた文字列数でリサイズ

		//呼び出し2回目(確保済みのwstrに変換文字列をコピー)
		auto num2 = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
			str.c_str(), -1, &wstr[0], num1);

		assert(num1 == num2);//一応チェック
		return wstr;
	}
	///モデルのパスとテクスチャのパスから合成パスを得る
	///@param modelPath アプリケーションから見たpmdモデルのパス
	///@param texPath PMDモデルから見たテクスチャのパス
	///@return アプリケーションから見たテクスチャのパス
	std::string GetTexturePathFromModelAndTexPath(const std::string& modelPath, const std::string& texPath) {
		//ファイルのフォルダ区切りは\と/の二種類が使用される可能性があり
		//ともかく末尾の\か/を得られればいいので、双方のrfindをとり比較する
		//int型に代入しているのは見つからなかった場合はrfindがepos(-1→0xffffffff)を返すため
		int pathIndex1 = modelPath.rfind('/');
		int pathIndex2 = modelPath.rfind('\\');
		auto pathIndex = max(pathIndex1, pathIndex2);
		auto folderPath = modelPath.substr(0, pathIndex + 1);
		return folderPath + texPath;
	}
}

void 
D3D12HelloTriangle::OnButtonDown(UINT32) {

}
void 
D3D12HelloTriangle::OnMouseMove(UINT8, UINT32) {

}



D3D12HelloTriangle::D3D12HelloTriangle(UINT width, UINT height, std::wstring name) :
	DXSample(width, height, name),
	m_frameIndex(0),
	m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
	m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)),
	m_rtvDescriptorSize(0),
	m_raster(false)
{
}


ComPtr<ID3D12RootSignature> 
D3D12HelloTriangle::CreateRayGenSignature() {
	nv_helpers_dx12::RootSignatureGenerator rsc;
	rsc.AddHeapRangesParameter({ 
		{0,1,0,
		D3D12_DESCRIPTOR_RANGE_TYPE_UAV,0},//u0
		{0,1,0,
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1},//t0
		{0,1,0,
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV,2},//b0
		
		});

	return rsc.Generate(m_device.Get(), true);
}

ComPtr<ID3D12RootSignature> 
D3D12HelloTriangle::CreateHitSignature() {
	nv_helpers_dx12::RootSignatureGenerator rsc;
	rsc.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV, 0);//register(t0)に対応(Vertex)
	rsc.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV, 1);//register(t1)に対応(Index)
	rsc.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 0);//register(b0)に対応
	rsc.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV, 3);//register(t3)に対応(MaterialTable)
	rsc.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV, 4);//register(t4)に対応(MaterialTable)
	//rsc.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV, 5,0);//register(t5)に対応(MaterialTable)
	
	rsc.AddHeapRangesParameter({ 
		{ 2,1,0,
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1 }, //t2
		});//
	rsc.AddHeapRangesParameter({ { 5,1,0,D3D12_DESCRIPTOR_RANGE_TYPE_SRV,5 } });
	return rsc.Generate(m_device.Get(), true);
}

ComPtr<ID3D12RootSignature>
D3D12HelloTriangle::CreateMissSignature() {
	nv_helpers_dx12::RootSignatureGenerator rsc;
	
	return rsc.Generate(m_device.Get(), true);
}

void 
D3D12HelloTriangle::CreateRaytracingPipeline() {
	nv_helpers_dx12::RayTracingPipelineGenerator pipeline(m_device.Get());

	m_rayGenLibrary = nv_helpers_dx12::CompileShaderLibrary(L"shaders/RayGen.hlsl");
	m_missLibrary = nv_helpers_dx12::CompileShaderLibrary(L"shaders/Miss.hlsl");
	m_hitLibrary = nv_helpers_dx12::CompileShaderLibrary(L"shaders/Hit.hlsl");

	m_shadowLibrary = nv_helpers_dx12::CompileShaderLibrary(L"shaders/ShadowRay.hlsl");

	pipeline.AddLibrary(m_shadowLibrary.Get(), { L"ShadowClosestHit",L"ShadowMiss" });
	m_shadowSignature = CreateHitSignature();

	pipeline.AddLibrary(m_rayGenLibrary.Get(), { L"RayGen" });
	pipeline.AddLibrary(m_missLibrary.Get(), { L"Miss" });
	pipeline.AddLibrary(m_hitLibrary.Get(), { L"ClosestHit",L"PlaneClosestHit" });

	m_rayGenSignature = CreateRayGenSignature();
	m_missSignature = CreateMissSignature();
	m_hitSignature = CreateHitSignature();

	pipeline.AddHitGroup(L"HitGroup", L"ClosestHit");
	pipeline.AddHitGroup(L"PlaneHitGroup", L"PlaneClosestHit");
	pipeline.AddHitGroup(L"ShadowHitGroup", L"ShadowClosestHit");

	pipeline.AddRootSignatureAssociation(m_rayGenSignature.Get(), { L"RayGen" });
	pipeline.AddRootSignatureAssociation(m_missSignature.Get(), { L"Miss" });
	pipeline.AddRootSignatureAssociation(m_hitSignature.Get(), { L"HitGroup" });

	pipeline.AddRootSignatureAssociation(m_shadowSignature.Get(), { L"ShadowHitGroup" });
	pipeline.AddRootSignatureAssociation(m_missSignature.Get(), { L"Miss" ,L"ShadowMiss" });
	pipeline.AddRootSignatureAssociation(m_hitSignature.Get(), { L"HitGroup",L"PlaneHitGroup" });

	pipeline.SetMaxPayloadSize(sizeof(XMFLOAT4));//ペイロードサイズ(RGB+距離)
	pipeline.SetMaxAttributeSize(sizeof(XMFLOAT2));//重心UV
	pipeline.SetMaxRecursionDepth(2);

	m_rtStateObject = pipeline.Generate();
	ThrowIfFailed(m_rtStateObject->QueryInterface(IID_PPV_ARGS(&m_rtStateObjectProps)));


}

void
D3D12HelloTriangle::CreateShaderBindingTable() {
	m_sbtHelper.Reset();
	D3D12_GPU_DESCRIPTOR_HANDLE srvUavHeapHandle = m_srvUavHeap->GetGPUDescriptorHandleForHeapStart();

	auto heapPointer = reinterpret_cast<UINT64*>(srvUavHeapHandle.ptr);

	m_sbtHelper.AddRayGenerationProgram(L"RayGen", { heapPointer });
	m_sbtHelper.AddMissProgram(L"Miss", {});
	m_sbtHelper.AddMissProgram(L"ShadowMiss", {});
	for (int i = 0; i < 3; ++i) {
		std::vector<void*> inputdata = {
			(void*)(m_vertexBuffer->GetGPUVirtualAddress()),//t0
			(void*)(m_indexBuffer->GetGPUVirtualAddress()),//t1
			(void*)(m_perInstanceConstantBuffers[i]->GetGPUVirtualAddress()),//t2
			(void*)(m_matIdBuffer->GetGPUVirtualAddress()),//t3
			(void*)(m_materialBuffer->GetGPUVirtualAddress())//t4
		};
		for (auto& texbuff : m_textureBuffers) {
			inputdata.push_back((void*)(texbuff->GetGPUVirtualAddress()));//t5
		}
		m_sbtHelper.AddHitGroup(L"HitGroup",inputdata);
		m_sbtHelper.AddHitGroup(L"ShadowHitGroup", {});
	}
	
	m_sbtHelper.AddHitGroup(L"PlaneHitGroup", {(void*)(m_vertexBuffer->GetGPUVirtualAddress()),heapPointer });
	m_sbtHelper.AddHitGroup(L"ShadowHitGroup", {});
	uint32_t sbtSize = m_sbtHelper.ComputeSBTSize();
	m_sbtStorage = nv_helpers_dx12::CreateBuffer(m_device.Get(), sbtSize, D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATE_GENERIC_READ, nv_helpers_dx12::kUploadHeapProps);

	if (!m_sbtStorage) {
		throw std::logic_error("could not allocate the shader binding table");
	}

	m_sbtHelper.Generate(m_sbtStorage.Get(), m_rtStateObjectProps.Get());


}

void 
D3D12HelloTriangle::CreateRaytracingOutputBuffer() {
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.DepthOrArraySize = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;


	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resDesc.Width = GetWidth();
	resDesc.Height = GetHeight();
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;

	ThrowIfFailed(m_device->CreateCommittedResource(&nv_helpers_dx12::kDefaultHeapProps,
		D3D12_HEAP_FLAG_NONE, &resDesc,
		D3D12_RESOURCE_STATE_COPY_SOURCE, nullptr,
		IID_PPV_ARGS(&m_outputResource)));


}
void 
D3D12HelloTriangle::CreateShaderResourceHeap() {
	//マテリアルのために3→5へ
	int numHeapSize = 5 + m_textureBuffers.size();
	m_srvUavHeap = nv_helpers_dx12::CreateDescriptorHeap(m_device.Get(), numHeapSize, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, true);

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = m_srvUavHeap->GetCPUDescriptorHandleForHeapStart();

	//0
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	m_device->CreateUnorderedAccessView(m_outputResource.Get(), nullptr, &uavDesc, srvHandle);

	//1
	srvHandle.ptr += m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = m_TLASBuffer.pResult->GetGPUVirtualAddress();
	m_device->CreateShaderResourceView(nullptr, &srvDesc, srvHandle);

	//2
	srvHandle.ptr += m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = m_cameraBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = m_cameraBufferSize;
	m_device->CreateConstantBufferView(&cbvDesc, srvHandle);

	//3
	//マテリアルIDテーブル
	srvHandle.ptr += m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = m_matNtexIDs.size();
	srvDesc.Buffer.StructureByteStride = sizeof(m_matNtexIDs[0]);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_device->CreateShaderResourceView(m_matIdBuffer.Get(), &srvDesc, srvHandle);

	//4
	//マテリアルヒープ
	srvHandle.ptr += m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = m_materials.size();
	srvDesc.Buffer.StructureByteStride = sizeof(m_materials[0]);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_device->CreateShaderResourceView(m_materialBuffer.Get(), &srvDesc, srvHandle);

	//5～
	
	
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = 0;
	srvDesc.Buffer.StructureByteStride = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.PlaneSlice = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0;
	for (auto& texbuff : m_textureBuffers) {
		auto desc=texbuff->GetDesc();
		srvDesc.Format = desc.Format;
		srvHandle.ptr += m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_device->CreateShaderResourceView(texbuff.Get(), &srvDesc, srvHandle);
	}
}



void 
D3D12HelloTriangle::OnInit()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	LoadPipeline();
	LoadAssets();
	
	ID3D12CommandList* cmdLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(1, cmdLists);
	m_fenceValue++;
	m_commandQueue->Signal(m_fence.Get(), m_fenceValue);

	m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent);
	WaitForSingleObject(m_fenceEvent, INFINITE);
	ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));

	CheckRaytracingSupport();
	
	CreateAccelerationStructures();
	ThrowIfFailed(m_commandList->Close());
	
	//CreateTextureBuffer();

	CreateRaytracingPipeline();
	CreateGlobalConstantBuffer();
	CreatePerInstanceConstantBuffers();
	CreateRaytracingOutputBuffer();
	CreateCameraBuffer();
	CreateShaderResourceHeap();
	CreateShaderBindingTable();
}

// Load the rendering pipeline dependencies.
void D3D12HelloTriangle::LoadPipeline()
{
	UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif

	ComPtr<IDXGIFactory4> factory;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

	if (m_useWarpDevice)
	{
		ComPtr<IDXGIAdapter> warpAdapter;
		ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			warpAdapter.Get(),
			D3D_FEATURE_LEVEL_12_1,
			IID_PPV_ARGS(&m_device)
			));
	}
	else
	{
		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(factory.Get(), &hardwareAdapter);

		ThrowIfFailed(D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_12_1,
			IID_PPV_ARGS(&m_device)
			));
	}

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Width = m_width;
	swapChainDesc.Height = m_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(factory->CreateSwapChainForHwnd(
		m_commandQueue.Get(),		// Swap chain needs the queue so that it can force a flush on it.
		Win32Application::GetHwnd(),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
		));

	// This sample does not support fullscreen transitions.
	ThrowIfFailed(factory->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

	ThrowIfFailed(swapChain.As(&m_swapChain));
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	// Create descriptor heaps.
	{
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (UINT n = 0; n < FrameCount; n++)
		{
			ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorSize);
		}
	}

	ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
}

void 
D3D12HelloTriangle::CreateTextureBuffer(std::string path,ComPtr<ID3D12Resource>& texBuff) {
	static size_t no = 0;

	path = GetTexturePathFromModelAndTexPath("model/satori.pmd", path);

	auto wpath = GetWideStringFromString(path);
	
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage scratchImg;
	auto result = DirectX::LoadFromWICFile(wpath.c_str(),DirectX::WIC_FLAGS_FILTER_LINEAR, &metadata, scratchImg);
	ThrowIfFailed(result);

	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_CUSTOM;//テクスチャ用
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapprop.CreationNodeMask = 0;
	heapprop.VisibleNodeMask = 0;
	D3D12_RESOURCE_DESC resdesc = CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, metadata.width, metadata.height, metadata.arraySize, metadata.mipLevels);

	result = m_device->CreateCommittedResource(&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(texBuff.ReleaseAndGetAddressOf()));
	ThrowIfFailed(result);
	//std::vector<uint8_t> col(4 * 256 * 256);
	//int idx = 0;
	//uint8_t c = 0xff;
	//for (int y = 0; y < 256; ++y) {
	//	for (int x = 0; x < 256; ++x) {
	//		if (((x / 32) + (y / 32)) % 2 == 0) {
	//			c = 0;
	//		}
	//		else {
	//			c= 192;
	//		}
	//		auto n = no;
	//		col[idx+0] = c*((n%2)==0);
	//		col[idx+1] = c*((n%3)==0);
	//		col[idx+2] = c*0xff;
	//		col[idx+3] = 0xff;
	//		idx += 4;
	//	}
	//}
	auto img = scratchImg.GetImage(0, 0, 0);//生データ抽出
	result = texBuff->WriteToSubresource(0, nullptr, img->pixels,img->rowPitch, img->slicePitch);
	ThrowIfFailed(result);
	++no;
}

// Load the sample assets.
void D3D12HelloTriangle::LoadAssets()
{
	LoadPMDFile(L"model/初音ミク.pmd");
	//LoadPMDFile(L"model/miku.pmd");
	// Create an empty root signature.
	{
		CD3DX12_ROOT_PARAMETER constantParameter;
		CD3DX12_DESCRIPTOR_RANGE range;
		range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		constantParameter.InitAsDescriptorTable(1, &range, D3D12_SHADER_VISIBILITY_ALL);
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(1, &constantParameter, 0, nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
		ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
	}

	// Create the pipeline state, which includes compiling and loading shaders.
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif

		ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"shaders.hlsl").c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"shaders.hlsl").c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
	}

	// Create the command list.
	ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));

	// Command lists are created in the recording state, but there is nothing
	// to record yet. The main loop expects it to be closed, so close it now.
	ThrowIfFailed(m_commandList->Close());

	// Create the vertex buffer.
	{
		// Define the geometry for a triangle.
		//Vertex triangleVertices[] =
		//{
		//	{ { 0.0f, 0.25f * m_aspectRatio, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
		//	{ { 0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		//	{ { -0.25f, -0.25f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } }
		//};

		//Vertex triangleVertices[] =
		//{
		//	{ { std::sqrtf(8.0f/9.0f),0.0f, -1.0f/3.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
		//	{ { -std::sqrtf(2.0f / 9.0f),std::sqrtf(2.0f/3.0f), -1.0f / 3.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
		//	{ { -std::sqrtf(2.0f / 9.0f),-std::sqrtf(2.0f / 3.0f), -1.0f / 3.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
		//	{ { 0.0f,0.0f,1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } }
		//};

		const UINT vertexBufferSize = sizeof(Vertex) * m_pmdVert.size();

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		ThrowIfFailed(m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer)));

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
		ThrowIfFailed(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
		memcpy(pVertexDataBegin, m_pmdVert.data(),vertexBufferSize);
		m_vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;


		//std::vector<UINT> indices = { 0,1,2,0,3,1,0,2,3,1,3,2 };
		const UINT indexBufferSize = static_cast<UINT>(m_pmdIndex.size()) * sizeof(m_pmdIndex[0]);
		CD3DX12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC bufferResource = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
		ThrowIfFailed(m_device->CreateCommittedResource(
			&heapProperty, D3D12_HEAP_FLAG_NONE, &bufferResource,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_indexBuffer)
		));

		auto type = m_pmdIndex[0];
		decltype(type)* indexDataBegin;
		ThrowIfFailed(m_indexBuffer->Map(0, &readRange, (void**)&indexDataBegin));
		std::copy(m_pmdIndex.begin(), m_pmdIndex.end(), indexDataBegin);
		m_indexBuffer->Unmap(0, nullptr);

		m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		m_indexBufferView.SizeInBytes = indexBufferSize;

	}

	CreatePlaneVB();


	// Create synchronization objects and wait until assets have been uploaded to the GPU.
	{
		ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
		m_fenceValue = 1;

		// Create an event handle to use for frame synchronization.
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}

		// Wait for the command list to execute; we are reusing the same command 
		// list in our main loop but for now, we just want to wait for setup to 
		// complete before continuing.
		WaitForPreviousFrame();
	}
}

// Update frame-based values.
void D3D12HelloTriangle::OnUpdate()
{
	UpdateCameraBuffer();
	++m_time;
	m_instances[0].second = XMMatrixRotationAxis({ 0.0f,1.0f,0.0f }, static_cast<float>(m_time) / 50.0f) *
		XMMatrixTranslation(0.0f, 0.1f * cosf((float)m_time / 20.0f), 0.0f);

	m_instances[1].second = XMMatrixRotationAxis({ 0.0f,-1.0f,0.0f }, static_cast<float>(m_time) / 50.0f) *
		XMMatrixTranslation(0.75f, 0.1f * cosf((float)m_time / 10.0f), 0.0f);

	m_instances[2].second = XMMatrixRotationAxis({ 0.0f,1.0f,0.0f }, static_cast<float>(m_time*2) / 50.0f) *
		XMMatrixTranslation(-0.75f, 0.1f * cosf((float)m_time / 40.0f), 0.0f);
	UpdateConstantBuffer();
}

void 
D3D12HelloTriangle::CreatePlaneVB() {
	Vertex planeVertices[] = {
		{{-1.5f,-0.1f,1.5f},{0.0f,1.0f,0.0f},{0,1}},//0
		{{-1.5f,-0.1f,-1.5f},{0.0f,1.0f,0.0f},{0,0}},//1
		{{1.5f,-0.1f,1.5f},{0.0f,1.0f,0.0f},{1,1}},//2
		{{1.5f,-0.1f,1.5f},{0.0f,1.0f,0.0f},{1,1}},//2
		{{-1.5f,-0.1f,-1.5f},{0.0f,1.0f,0.0f},{0,0}},//1
		{{1.5f,-0.1f,-1.5f},{0.0f,1.0f,0.0f},{1,0}},//3
	};
	const UINT planeBufferSize = sizeof(Vertex)*6;

	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto bufferResource = CD3DX12_RESOURCE_DESC::Buffer(planeBufferSize);
	ThrowIfFailed(m_device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferResource,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_planeBuffer)));
	Vertex* pVertexDataBegin;
	CD3DX12_RANGE readRange(0, 0);
	ThrowIfFailed(m_planeBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
	std::copy(std::begin(planeVertices), std::end(planeVertices), pVertexDataBegin);
	m_planeBuffer->Unmap(0, nullptr);


	m_planeBufferView.BufferLocation = m_planeBuffer->GetGPUVirtualAddress();
	m_planeBufferView.StrideInBytes = sizeof(Vertex);
	m_planeBufferView.SizeInBytes = planeBufferSize;

}

void 
D3D12HelloTriangle::CreateTLAS(const Instances_t& instances , bool updateOnly) {
	if (!updateOnly) {
		for (size_t i = 0; i < instances.size(); ++i) {
			m_TLAS.AddInstance(
				instances[i].first.Get(),
				instances[i].second,
				static_cast<UINT>(i),
				static_cast<UINT>(2*i));
		}


		UINT64 scratchSize, resultSize, instanceDescSize;
		m_TLAS.ComputeASBufferSizes(m_device.Get(), true, &scratchSize, &resultSize, &instanceDescSize);

		m_TLASBuffer.pScratch = nv_helpers_dx12::CreateBuffer(m_device.Get(),
			scratchSize,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nv_helpers_dx12::kDefaultHeapProps);

		m_TLASBuffer.pResult = nv_helpers_dx12::CreateBuffer(m_device.Get(),
			resultSize,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
			nv_helpers_dx12::kDefaultHeapProps);

		m_TLASBuffer.pInstanceDesc = nv_helpers_dx12::CreateBuffer(m_device.Get(),
			instanceDescSize,
			D3D12_RESOURCE_FLAG_NONE,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nv_helpers_dx12::kUploadHeapProps);
	}
	m_TLAS.Generate(m_commandList.Get(), 
		m_TLASBuffer.pScratch.Get(), 
		m_TLASBuffer.pResult.Get(), 
		m_TLASBuffer.pInstanceDesc.Get(),
		updateOnly,
		m_TLASBuffer.pResult.Get());

}

void 
D3D12HelloTriangle::CreateAccelerationStructures() {
	AccelerationStructureBuffers blasBuffers = CreateBLAS({ {m_vertexBuffer.Get(),m_pmdVert.size()} }, {{ m_indexBuffer.Get(),m_pmdIndex.size() }});
	
	auto planeBLASBuffers = CreateBLAS({ { m_planeBuffer.Get(), 6 } });
	
	m_instances = { {blasBuffers.pResult,XMMatrixIdentity()},
		{blasBuffers.pResult,XMMatrixTranslation(-0.6f,0.0f,0.0f)},
			{blasBuffers.pResult,XMMatrixTranslation(0.6f,0.0f,0.0f)} ,
	
		{planeBLASBuffers.pResult,XMMatrixTranslation(0,0,0)}
	};



	CreateTLAS(m_instances);

	m_commandList->Close();
	ID3D12CommandList* cmdLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(1, cmdLists);
	m_fenceValue++;
	m_commandQueue->Signal(m_fence.Get(), m_fenceValue);

	m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent);
	WaitForSingleObject(m_fenceEvent, INFINITE);

	ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));
	m_BLAS = blasBuffers.pResult;
}

void 
D3D12HelloTriangle::LoadPMDFile(const wchar_t* path) {
	//PMDヘッダ構造体
	struct PMDHeader {
		float version; //例：00 00 80 3F == 1.00
		char model_name[20];//モデル名
		char comment[256];//モデルコメント
	};
	char signature[3];
	PMDHeader pmdheader = {};

	std::wstring strModelPath = path;
	FILE* fp;
	auto err = _wfopen_s(&fp,strModelPath.c_str(), L"rb");
	if (fp == nullptr) {
		//エラー処理
		assert(0);
		return ;
	}
	fread(signature, sizeof(signature), 1, fp);
	fread(&pmdheader, sizeof(pmdheader), 1, fp);

	unsigned int vertNum;//頂点数
	fread(&vertNum, sizeof(vertNum), 1, fp);
	m_pmdVert.resize(vertNum);
	for (auto& v : m_pmdVert) {
		fread(&v.position, sizeof(v.position), 1, fp);
		v.position.x *= 0.05f;
		v.position.y *= 0.05f;
		v.position.z *= 0.05f;
		fread(&v.normal, sizeof(v.normal), 1, fp);
		fread(&v.uv, sizeof(v.uv), 1, fp);
		fseek(fp, 6, SEEK_CUR);
	}

	unsigned int indicesNum;//インデックス数
	fread(&indicesNum, sizeof(indicesNum), 1, fp);//
	m_pmdIndex.resize(indicesNum);
	for (auto& idx : m_pmdIndex) {
		uint16_t index;
		fread(&index, sizeof(index), 1, fp);
		idx = index;
	}
#pragma pack(1)
	struct PMDMaterial {
		XMFLOAT4 diffuse;//ディフューズ色
		float power;//スペキュラ乗数
		XMFLOAT3 specular;//スペキュラ色
		XMFLOAT3 ambient;//環境光
		uint8_t toon;//トゥーン番号
		uint8_t edge;//エッジフラグ
		uint32_t indexNum;//インデックス数
		char texturePath[20];//テクスチャパス(相対)
	};
#pragma pack()
	unsigned int materialCount;
	fread(&materialCount, sizeof(materialCount), 1, fp);
	std::vector<PMDMaterial> materials(materialCount);
	m_materials.resize(materialCount);
	_materials.resize(materialCount);
	m_matNtexIDs.resize(indicesNum/3);

	fread(materials.data(), sizeof(PMDMaterial), materialCount, fp);
	UINT idx = 0;
	UINT texIdx = 0;
	auto msize = materials.size();
	for (int i = 0; i < msize; ++i) {
		_materials[i].diffuse = materials[i].diffuse;
		_materials[i].power = materials[i].power;
		_materials[i].specular = materials[i].specular;
		_materials[i].ambient = materials[i].ambient;
		std::string strPath = materials[i].texturePath;
		if (strPath != "") {
			auto it = texTable_.find(strPath);
			if (it == texTable_.end()) {
				texTable_[strPath] = texIdx;
				ComPtr<ID3D12Resource> tex;
				CreateTextureBuffer(strPath,tex);
				m_textureBuffers.push_back(tex);
				++texIdx;
			}
		}
		for (size_t j = 0; j < materials[i].indexNum/3; ++j) {
			m_matNtexIDs[idx].matID=i;//プリミティブ単位でマテリアル番号を登録
			if (strPath == "") {
				m_matNtexIDs[idx].texID = 0xff;
			}
			else {
				m_matNtexIDs[idx].texID = texTable_[strPath];
			}
			++idx;

		}
		m_materials[i].x = materials[i].diffuse.x;
		m_materials[i].y = materials[i].diffuse.y;
		m_materials[i].z = materials[i].diffuse.z;

	}
	fclose(fp);
}

D3D12HelloTriangle::AccelerationStructureBuffers
D3D12HelloTriangle::CreateBLAS(VertBuff_t vertBuffs,IndexBuff_t idxBuffs) {
	nv_helpers_dx12::BottomLevelASGenerator blas;
	//for (const auto& vb : vertBuffs) {
	for(size_t i=0;i<vertBuffs.size();++i){
		//1.バッファ本体
		//2.オフセット
		//3.頂点数
		//4.1頂点当たりのサイズ
		//5.座標変換バッファ
		//6.座標変換バッファオフセット
		auto& vb = vertBuffs[i];
		if (i < idxBuffs.size() && idxBuffs[i].second>0) {
			auto& ib = idxBuffs[i];
			blas.AddVertexBuffer(vb.first.Get(), 0, vb.second, sizeof(Vertex), ib.first.Get(), 0,ib.second,nullptr,0,true);
		}
		else {
			blas.AddVertexBuffer(vb.first.Get(), 0, vb.second, sizeof(Vertex), 0, 0);
		}
	}

	UINT64 scratchSizeInBytes = 0;
	UINT64 resultSizeInBytes = 0;
	blas.ComputeASBufferSizes(m_device.Get(), false, &scratchSizeInBytes, &resultSizeInBytes);
	

	AccelerationStructureBuffers buffers;
	buffers.pScratch = nv_helpers_dx12::CreateBuffer(
		m_device.Get(), scratchSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COMMON,
		nv_helpers_dx12::kDefaultHeapProps);

	buffers.pResult = nv_helpers_dx12::CreateBuffer(
		m_device.Get(),resultSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		nv_helpers_dx12::kDefaultHeapProps);

	//BLASを生成(バッファの中に結果を入れる)
	blas.Generate(m_commandList.Get(), //
		buffers.pScratch.Get(),//
		buffers.pResult.Get(),//
		false, //update only =false
		nullptr);//previous result=nullptr

	return buffers;

}

// Render the scene.
void D3D12HelloTriangle::OnRender()
{
	// Record all the commands we need to render the scene into the command list.
	PopulateCommandList();

	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame.
	ThrowIfFailed(m_swapChain->Present(1, 0));

	WaitForPreviousFrame();
}

void D3D12HelloTriangle::OnDestroy()
{
	// Ensure that the GPU is no longer referencing resources that are about to be
	// cleaned up by the destructor.
	WaitForPreviousFrame();

	CloseHandle(m_fenceEvent);
}

void
D3D12HelloTriangle::CreateCameraBuffer() {
	uint32_t nbMatrix = 4;
	m_cameraBufferSize = nbMatrix * sizeof(XMMATRIX);
	m_cameraBuffer = nv_helpers_dx12::CreateBuffer(m_device.Get(), m_cameraBufferSize,
		D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nv_helpers_dx12::kUploadHeapProps);

	m_constHeap = nv_helpers_dx12::CreateDescriptorHeap(m_device.Get(), 1, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, true);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = m_cameraBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = m_cameraBufferSize;

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = m_constHeap->GetCPUDescriptorHandleForHeapStart();
	m_device->CreateConstantBufferView(&cbvDesc, srvHandle);
}

void
D3D12HelloTriangle::UpdateCameraBuffer() {
	std::vector<XMMATRIX> matrices(4);
	static float angle = 0.0f;
	constexpr float r = 1.5f * 1.4142f;

	XMVECTOR eye = XMVectorSet(r*cos(angle),1.0f , r * sin(angle), 0.0f);
	//XMVECTOR eye = XMVectorSet(1.5f, 0.5f, 1.5f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	matrices[0] = XMMatrixLookAtRH(eye, at, up);

	angle += 0.01f;

	float fovAngleY = XM_PIDIV4;
	matrices[1] = XMMatrixPerspectiveFovRH(fovAngleY, m_aspectRatio, 0.1f, 1000.0f);

	XMVECTOR det;
	matrices[2] = XMMatrixInverse(&det, matrices[0]);
	matrices[3] = XMMatrixInverse(&det, matrices[1]);

	uint8_t* pdata;
	ThrowIfFailed(m_cameraBuffer->Map(0, nullptr, (void**)&pdata));
	memcpy(pdata, matrices.data(), m_cameraBufferSize);
	m_cameraBuffer->Unmap(0, nullptr);

}

void D3D12HelloTriangle::PopulateCommandList()
{
	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	ThrowIfFailed(m_commandAllocator->Reset());

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.
	ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));

	CreateTLAS(m_instances,true);


	// Set necessary state.
	m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
	m_commandList->RSSetViewports(1, &m_viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);

	// Indicate that the back buffer will be used as a render target.
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	if (m_raster) {

		std::vector<ID3D12DescriptorHeap*> heaps = { m_constHeap.Get() };
		m_commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()),heaps.data());

		m_commandList->SetGraphicsRootDescriptorTable(0, m_constHeap->GetGPUDescriptorHandleForHeapStart());

		// Record commands.
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
		//m_commandList->DrawInstanced(3, 1, 0, 0);
		//m_commandList->IASetIndexBuffer(&m_indexBufferView);
		m_commandList->DrawIndexedInstanced(12, 1, 0, 0, 0);

		m_commandList->IASetVertexBuffers(0, 1, &m_planeBufferView);


		m_commandList->DrawInstanced(6, 1, 0, 0);
		
	}
	else {
		const float clearColor[] = { 0.6f, 0.8f, 0.4f, 1.0f };
		//m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		std::vector<ID3D12DescriptorHeap*> heaps = { m_srvUavHeap.Get() };
		m_commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
		CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(
			m_outputResource.Get(),
			D3D12_RESOURCE_STATE_COPY_SOURCE,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		m_commandList->ResourceBarrier(1, &transition);

		D3D12_DISPATCH_RAYS_DESC desc = {};
		UINT32 rayGenerationSectionSizeInBytes = m_sbtHelper.GetRayGenSectionSize();
		desc.RayGenerationShaderRecord.StartAddress = m_sbtStorage->GetGPUVirtualAddress();
		desc.RayGenerationShaderRecord.SizeInBytes = rayGenerationSectionSizeInBytes;

		uint32_t missSectionSizeInBytes = m_sbtHelper.GetMissSectionSize();
		desc.MissShaderTable.StartAddress = m_sbtStorage->GetGPUVirtualAddress() + rayGenerationSectionSizeInBytes;
		desc.MissShaderTable.SizeInBytes = missSectionSizeInBytes;
		desc.MissShaderTable.StrideInBytes = m_sbtHelper.GetMissEntrySize();
		auto alignedAdrs= m_sbtStorage->GetGPUVirtualAddress() + rayGenerationSectionSizeInBytes + missSectionSizeInBytes;
		//alignedAdrs += (64 - alignedAdrs % 64) % 64;
		uint32_t hitGroupSectionSize = m_sbtHelper.GetHitGroupSectionSize();
		desc.HitGroupTable.StartAddress = alignedAdrs;// m_sbtStorage->GetGPUVirtualAddress() + rayGenerationSectionSizeInBytes + missSectionSizeInBytes;
		desc.HitGroupTable.SizeInBytes = hitGroupSectionSize;
		desc.HitGroupTable.StrideInBytes = m_sbtHelper.GetHitGroupEntrySize();

		desc.Width = GetWidth();
		desc.Height = GetHeight();
		desc.Depth = 1;

		m_commandList->SetPipelineState1(m_rtStateObject.Get());
		m_commandList->DispatchRays(&desc);

		transition = CD3DX12_RESOURCE_BARRIER::Transition(m_outputResource.Get(), 
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_COPY_SOURCE);
		m_commandList->ResourceBarrier(1, &transition);

		transition = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, 
			D3D12_RESOURCE_STATE_COPY_DEST);
		m_commandList->ResourceBarrier(1, &transition);
		
		m_commandList->CopyResource(m_renderTargets[m_frameIndex].Get(), m_outputResource.Get());
		
		transition = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), 
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_commandList->ResourceBarrier(1, &transition);
		

	}
	// Indicate that the back buffer will now be used to present.
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	ThrowIfFailed(m_commandList->Close());
}

void D3D12HelloTriangle::WaitForPreviousFrame()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
	// sample illustrates how to use fences for efficient resource usage and to
	// maximize GPU utilization.

	// Signal and increment the fence value.
	const UINT64 fence = m_fenceValue;
	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
	m_fenceValue++;

	// Wait until the previous frame is finished.
	if (m_fence->GetCompletedValue() < fence)
	{
		ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}


void D3D12HelloTriangle::CheckRaytracingSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options5 = {};
	ThrowIfFailed(m_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options5, sizeof(options5)));

	if (options5.RaytracingTier < D3D12_RAYTRACING_TIER_1_0) {
		//throw std::runtime_error("レイトレはサポートされてねえよ");
	}

}
void D3D12HelloTriangle::OnKeyUp(UINT8 key)
{
	// Alternate between rasterization and raytracing using the spacebar
	if (key == VK_SPACE)
	{
		m_raster = !m_raster;
	}
}

void
D3D12HelloTriangle::CreateGlobalConstantBuffer() {
	XMVECTOR bufferData[] = {
		//A
		XMVECTOR{1.0f,1.0f,0.0f,1.0f},
		XMVECTOR{0.7f,0.4f,0.0f,1.0f},
		XMVECTOR{0.4f,0.7f,0.0f,1.0f},

		//B
		XMVECTOR{0.0f,1.0f,1.0f,1.0f},
		XMVECTOR{0.0f,0.7f,0.4f,1.0f},
		XMVECTOR{0.0f,0.4f,0.7f,1.0f},

		//C
		XMVECTOR{1.0f,0.0f,1.0f,1.0f},
		XMVECTOR{0.4f,0.0f,0.7f,1.0f},
		XMVECTOR{0.7f,0.0f,0.4f,1.0f},
	};
	auto sz = sizeof(bufferData) ;
	sz = ROUND_UP(sz, 256);
	m_globalConstantBuffer = nv_helpers_dx12::CreateBuffer(
		m_device.Get(),
		sz,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nv_helpers_dx12::kUploadHeapProps);

	uint8_t* pData;
	ThrowIfFailed(m_globalConstantBuffer->Map(0, nullptr, (void**)&pData));
	memcpy(pData, bufferData, sizeof(bufferData));
	m_globalConstantBuffer->Unmap(0, nullptr);

}


void 
D3D12HelloTriangle::CreatePerInstanceConstantBuffers() {

	m_perInstanceConstantBuffers.resize(3);
	UpdateConstantBuffer(true);
}

void D3D12HelloTriangle::UpdateConstantBuffer(bool first)
{
	static float angle = 0.0f;
	int i(0);
	float s = sinf(angle);
	for (auto& cb : m_perInstanceConstantBuffers) {
		const uint32_t bufferSize = sizeof(XMVECTOR) * 3;
		//bufferData[i * 3 + 0] = { (s+1.0f)/2.0f, 1.0f, 1.0f, 1.0f };
		bufferData[i * 3 + 1] = XMQuaternionRotationMatrix(m_instances[i].second);
		if (first) {
			cb = nv_helpers_dx12::CreateBuffer(m_device.Get(), bufferSize, D3D12_RESOURCE_FLAG_NONE,
				D3D12_RESOURCE_STATE_GENERIC_READ, nv_helpers_dx12::kUploadHeapProps);
		}
		uint8_t* data = nullptr;
		ThrowIfFailed(cb->Map(0, nullptr, (void**)&data));
		memcpy(data, &bufferData[i * 3], bufferSize);
		cb->Unmap(0, nullptr);
		++i;
	}
	if (first) {

		m_matIdBuffer = nv_helpers_dx12::CreateBuffer(m_device.Get(), sizeof(m_matNtexIDs[0]) * m_matNtexIDs.size(), D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nv_helpers_dx12::kUploadHeapProps);
		auto t = m_matNtexIDs[0];
		decltype(t)* mat;
		m_matIdBuffer->Map(0, nullptr, (void**)&mat);
		std::copy(m_matNtexIDs.begin(), m_matNtexIDs.end(), mat);
		m_matIdBuffer->Unmap(0, nullptr);

		m_materialBuffer= nv_helpers_dx12::CreateBuffer(m_device.Get(), sizeof(m_materials[0]) * m_materials.size(), D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nv_helpers_dx12::kUploadHeapProps);
		auto m = m_materials.back();
		decltype(m)* mtl;
		m_materialBuffer->Map(0, nullptr, (void**)&mtl);
		std::copy(m_materials.begin(), m_materials.end(), mtl);
		m_materialBuffer->Unmap(0, nullptr);
	}
	angle += 0.01f;
}
