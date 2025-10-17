#include "DX12Device.h"

#include "core/Log.h"

#if defined(_WIN32) && defined(_MSC_VER)
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#endif

namespace Renderer {

	DX12Device::DX12Device()
	{
	}

	DX12Device::~DX12Device()
	{
		Shutdown();
	}

	bool DX12Device::Initialize()
	{
#if defined(_WIN32) && defined(_MSC_VER)
		HRESULT hr;

		// Enable debug layer in debug builds
#ifdef _DEBUG
		{
			ID3D12Debug* debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
				debugController->Release();
				CORE_LOG_INFO("DX12Device: Debug layer enabled");
			}
		}
#endif

		// Create DXGI Factory4 with debug flag in debug mode
		UINT createFactoryFlags = 0;
#ifdef _DEBUG
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&m_factory));
		if (FAILED(hr))
		{
			CORE_LOG_ERROR("DX12Device: CreateDXGIFactory2 failed, falling back to stub");
			return true; // Fallback to stub
		}

		CORE_LOG_INFO("DX12Device: DXGI Factory4 created");

		// Enumerate adapters and select the first hardware adapter
		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != m_factory->EnumAdapters1(adapterIndex, &m_adapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			m_adapter->GetDesc1(&desc);

			// Skip software adapters
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				m_adapter->Release();
				m_adapter = nullptr;
				continue;
			}

			// Try to create D3D12 device
			hr = D3D12CreateDevice(m_adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
			if (SUCCEEDED(hr))
			{
				// Log GPU info
				wchar_t gpuName[128];
				wcsncpy_s(gpuName, desc.Description, 127);
				gpuName[127] = L'\0';

				// Convert to narrow string for logging
				char gpuNameNarrow[128];
				size_t convertedChars = 0;
				wcstombs_s(&convertedChars, gpuNameNarrow, 128, gpuName, 127);

				CORE_LOG_INFO("DX12Device: Selected GPU - " + std::string(gpuNameNarrow));
				CORE_LOG_INFO("DX12Device: D3D12 Device created successfully");

				// Create Command Queue
				D3D12_COMMAND_QUEUE_DESC queueDesc = {};
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
				queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
				hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
				if (FAILED(hr))
				{
					CORE_LOG_ERROR("DX12Device: Failed to create Command Queue");
					m_device->Release();
					m_device = nullptr;
					m_adapter->Release();
					m_adapter = nullptr;
					continue; // Try next adapter
				}

				CORE_LOG_INFO("DX12Device: Command Queue created successfully");

				hasNativeDevice_ = true;
				return true;
			}

			m_adapter->Release();
			m_adapter = nullptr;
		}

		// No suitable adapter found
		if (m_factory)
		{
			m_factory->Release();
			m_factory = nullptr;
		}

		CORE_LOG_WARN("DX12Device: No suitable D3D12 adapter found, falling back to stub");
		hasNativeDevice_ = false;
		return true;
#else
		CORE_LOG_INFO("DX12Device: Initialize (stub - non-Windows platform)");
		hasNativeDevice_ = false;
		return true;
#endif
	}

	void DX12Device::Shutdown()
	{
#if defined(_WIN32) && defined(_MSC_VER)
		if (m_commandQueue)
		{
			m_commandQueue->Release();
			m_commandQueue = nullptr;
			CORE_LOG_INFO("DX12Device: Command Queue released");
		}

		if (m_device)
		{
			m_device->Release();
			m_device = nullptr;
			CORE_LOG_INFO("DX12Device: Device released");
		}

		if (m_adapter)
		{
			m_adapter->Release();
			m_adapter = nullptr;
		}

		if (m_factory)
		{
			m_factory->Release();
			m_factory = nullptr;
			CORE_LOG_INFO("DX12Device: Factory released");
		}

		hasNativeDevice_ = false;
#else
		CORE_LOG_INFO("DX12Device: Shutdown (stub)");
#endif
	}

	bool DX12Device::HasNativeDevice() const
	{
		return hasNativeDevice_;
	}

	void* DX12Device::NativeDevice() const
	{
#if defined(_WIN32) && defined(_MSC_VER)
		return m_device;
#else
		return nullptr;
#endif
	}

	void* DX12Device::NativeFactory() const
	{
#if defined(_WIN32) && defined(_MSC_VER)
		return m_factory;
#else
		return nullptr;
#endif
	}

	void* DX12Device::NativeCommandQueue() const
	{
#if defined(_WIN32) && defined(_MSC_VER)
		return m_commandQueue;
#else
		return nullptr;
#endif
	}

} // namespace Renderer
