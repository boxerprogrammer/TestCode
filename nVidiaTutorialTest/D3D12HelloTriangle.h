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

#pragma once

#include "DXSample.h"
#include<dxcapi.h>
#include<vector>

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class D3D12HelloTriangle : public DXSample
{
public:
	D3D12HelloTriangle(UINT width, UINT height, std::wstring name);

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();
	virtual void OnButtonDown(UINT32);
	virtual void OnMouseMove(UINT8, UINT32);
private:

	struct Material {
		DirectX::XMFLOAT4 diffuse;//ディフューズ色
		float power;//スペキュラ強さ
		DirectX::XMFLOAT3 specular;//スペキュラ色
		DirectX::XMFLOAT3 ambient;//環境色
		uint32_t indicesNum;//インデックス数
	};
	std::vector<Material> _materials;// マテリアルデータ
	static const UINT FrameCount = 2;

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	struct AccelerationStructureBuffers {
		ComPtr<ID3D12Resource> pScratch;//スクラッチメモリ(なんや計算に使用する用メモリ領域か？)
		ComPtr<ID3D12Resource> pResult;//ASの場所を指し示す
		ComPtr<ID3D12Resource> pInstanceDesc;//インスタンスに与える行列領域
	};
	using VertBuff_t = std::vector < std::pair<ComPtr<ID3D12Resource>, uint32_t>>;
	using IndexBuff_t = std::vector < std::pair<ComPtr<ID3D12Resource>, uint32_t>>;
	using Instances_t= std::vector < std::pair<ComPtr<ID3D12Resource>, DirectX::XMMATRIX>>;
	ComPtr<ID3D12Resource> m_BLAS;
	nv_helpers_dx12::TopLevelASGenerator m_TLAS;
	AccelerationStructureBuffers m_TLASBuffer;
	std::vector<std::pair<ComPtr<ID3D12Resource>, DirectX::XMMATRIX>> m_instances;

	AccelerationStructureBuffers CreateBLAS(VertBuff_t vertBuff, IndexBuff_t idxBuff = {});
	void CreateTLAS(const Instances_t& instances,bool updateOnly=false);
	void CreateAccelerationStructures();

	uint32_t m_time = 0;

	// Pipeline objects.
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Device5> m_device;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12GraphicsCommandList4> m_commandList;
	UINT m_rtvDescriptorSize;

	// App resources.
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValue;

	bool m_raster = true;
	virtual void OnKeyUp(UINT8 key);

	void LoadPipeline();
	void LoadAssets();
	void LoadPMDFile(const wchar_t* path);
	void PopulateCommandList();
	void WaitForPreviousFrame();


	//レイトレーシングパイプライン
	ComPtr<ID3D12RootSignature> CreateRayGenSignature();
	ComPtr<ID3D12RootSignature> CreateMissSignature();
	ComPtr<ID3D12RootSignature> CreateHitSignature();

	void CreateRaytracingPipeline();
	ComPtr<IDxcBlob> m_rayGenLibrary;
	ComPtr<IDxcBlob> m_hitLibrary;
	ComPtr<IDxcBlob> m_missLibrary;
	ComPtr<IDxcBlob> m_shadowLibrary;

	ComPtr<ID3D12RootSignature> m_rayGenSignature;
	ComPtr<ID3D12RootSignature> m_hitSignature;
	ComPtr<ID3D12RootSignature> m_missSignature;
	ComPtr<ID3D12RootSignature> m_shadowSignature;

	ComPtr<ID3D12StateObject> m_rtStateObject;
	ComPtr<ID3D12StateObjectProperties> m_rtStateObjectProps;

	void CreateRaytracingOutputBuffer();
	void CreateShaderResourceHeap();

	ComPtr<ID3D12Resource> m_outputResource;
	ComPtr<ID3D12DescriptorHeap> m_srvUavHeap;

	void CreateShaderBindingTable();
	nv_helpers_dx12::ShaderBindingTableGenerator m_sbtHelper;
	ComPtr<ID3D12Resource> m_sbtStorage;

	ComPtr<ID3D12Resource> m_planeBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_planeBufferView;
	void CreatePlaneVB();

	void CreateCameraBuffer();
	void UpdateCameraBuffer();
	ComPtr<ID3D12Resource> m_cameraBuffer;
	ComPtr<ID3D12DescriptorHeap> m_constHeap;
	uint32_t m_cameraBufferSize = 0;

	void CreateGlobalConstantBuffer();
	ComPtr<ID3D12Resource> m_globalConstantBuffer;

	void CreatePerInstanceConstantBuffers();
	void UpdateConstantBuffer(bool first=false);
	std::vector<ComPtr<ID3D12Resource>> m_perInstanceConstantBuffers;

	ComPtr<ID3D12Resource> m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

	std::vector<Vertex> m_pmdVert;
	//struct IndexData {
	//	UINT idx;
	//	float mtl;
	//};
	std::vector<UINT> m_pmdIndex;
	std::vector<float> m_materialIDs;//プリミティブごとのマテリアルID
<<<<<<< HEAD

	ComPtr<ID3D12Resource> m_matIdBuffer;

=======
>>>>>>> 4099280cc3055f3afe6e7f9542b7c3134268c309

public:
	void CheckRaytracingSupport();
};
