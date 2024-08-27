#include "jkGraphicDevice_DX11.h"

#include "jkApplication.h"
#include "jkRenderer.h"
extern jk::Application app;

namespace jk::graphics
{
    GraphicDevice_DX11::GraphicDevice_DX11()
    {
    }

    GraphicDevice_DX11::~GraphicDevice_DX11()
    {
    }
    void GraphicDevice_DX11::Initialize()
    {
        D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        UINT createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG) || defined(DEBUG)
        createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createFlags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            mDevice.GetAddressOf(),
            nullptr,
            mContext.GetAddressOf()
            );

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.OutputWindow = app.GetHwnd();
		swapChainDesc.Windowed = true;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferDesc.Width = app.GetClientWidth();
        swapChainDesc.BufferDesc.Height = app.GetClientHeight();
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        UINT quility = 0;
        mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quility);
        //if (quility > 0)
        //{
        //	swapChainDesc.SampleDesc.Count = 4; // how many multisamples
        //	swapChainDesc.SampleDesc.Quality = quility - 1;
        //}

        swapChainDesc.SampleDesc.Count = 1; // how many multisamples
        swapChainDesc.SampleDesc.Quality = 0;

        Microsoft::WRL::ComPtr<IDXGIDevice>     pDXGIDevice = nullptr;
        Microsoft::WRL::ComPtr<IDXGIAdapter>    pAdapter = nullptr;
        Microsoft::WRL::ComPtr<IDXGIFactory>    pFactory = nullptr;

        if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
        {
            assert(false);
        }
        if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
        {
            assert(false);
        }
        if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
        {
            assert(false);
        }
        if (FAILED(pFactory->CreateSwapChain(mDevice.Get(), &swapChainDesc, mSwapChain.GetAddressOf())))
        {
            assert(false);
        }

        mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)mRenderTarget.GetAddressOf());
        mDevice->CreateRenderTargetView(mRenderTarget.Get(), nullptr, mRenderTargetView.GetAddressOf());

        D3D11_TEXTURE2D_DESC depthStencilDesc = {};
        depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        depthStencilDesc.Width = app.GetClientWidth();
        depthStencilDesc.Height = app.GetClientHeight();
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;

        if (FAILED(mDevice->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencil.GetAddressOf())))
        {
            assert(false);
        }
        if (FAILED(mDevice->CreateDepthStencilView(mDepthStencil.Get(), nullptr, mDepthStencilView.GetAddressOf())))
        {
            assert(false);
        }

        DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
        shaderFlags |= D3DCOMPILE_DEBUG;
        shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

        {
            ID3DBlob* errorBlob = nullptr;
            D3DCompileFromFile(
                L"..\\Shaders_SOURCE\\TriangleVS.hlsl",
                nullptr,
                D3D_COMPILE_STANDARD_FILE_INCLUDE,
                "main",
                "vs_5_0",
                shaderFlags,
                0,
                &renderer::vsBlob,
                &errorBlob
            );

            mDevice->CreateVertexShader(
                renderer::vsBlob->GetBufferPointer(),
                renderer::vsBlob->GetBufferSize(),
                nullptr,
                &renderer::vsShader
            );
        }
        {
            ID3DBlob* errorBlob = nullptr;
            D3DCompileFromFile(
                L"..\\Shaders_SOURCE\\TrianglePS.hlsl",
                nullptr,
                D3D_COMPILE_STANDARD_FILE_INCLUDE,
                "main",
                "ps_5_0",
                shaderFlags,
                0,
                &renderer::psBlob,
                &errorBlob
            );

            mDevice->CreatePixelShader(
                renderer::psBlob->GetBufferPointer(),
                renderer::psBlob->GetBufferSize(),
                nullptr,
                &renderer::psShader
            );

            D3D11_INPUT_ELEMENT_DESC inputLayoutDesces[2] = {};

            inputLayoutDesces[0].AlignedByteOffset = 0;
            inputLayoutDesces[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
            inputLayoutDesces[0].InputSlot = 0;
            inputLayoutDesces[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            inputLayoutDesces[0].SemanticName = "POSITION";
            inputLayoutDesces[0].SemanticIndex = 0;

            inputLayoutDesces[1].AlignedByteOffset = 12;
            inputLayoutDesces[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            inputLayoutDesces[1].InputSlot = 0;
            inputLayoutDesces[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            inputLayoutDesces[1].SemanticName = "COLOR";
            inputLayoutDesces[1].SemanticIndex = 0;

            mDevice->CreateInputLayout(inputLayoutDesces, 2
                , renderer::vsBlob->GetBufferPointer()
                , renderer::vsBlob->GetBufferSize()
                , &renderer::inputLayouts);

            D3D11_BUFFER_DESC bufferDesc = {};
            bufferDesc.ByteWidth = sizeof(renderer::Vertex) * 3;
            bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

            renderer::vertexes[0].pos = Vector3(0.f, .5f, 0.f);
            renderer::vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);
            renderer::vertexes[1].pos = Vector3(.5f, -.5f, 0.f);
            renderer::vertexes[1].color = Vector4(1.f, 0.f, 0.f, 1.f);
            renderer::vertexes[2].pos = Vector3(-.5f, -.5f, 0.f);
            renderer::vertexes[2].color = Vector4(0.f, 0.f, 1.f, 1.f);

            D3D11_SUBRESOURCE_DATA sub = { renderer::vertexes };

            mDevice->CreateBuffer(&bufferDesc, &sub, &renderer::vertexBuffer);

        }
    }
    void GraphicDevice_DX11::Draw()
    {
        FLOAT backgroundColor[4] = { .1f, 0.1f, .1f, 1.0f };
        mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
        mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

        D3D11_VIEWPORT viewPort =
        {
            0, 0, app.GetClientWidth(), app.GetClientHeight(),
            0.0f, 1.0f
        };
        mContext->RSSetViewports(1, &viewPort);
        mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

        mContext->IASetInputLayout(renderer::inputLayouts);
        mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        UINT vertexSize = sizeof(renderer::Vertex);
        UINT offset = 0;
        mContext->IASetVertexBuffers(0, 1, &renderer::vertexBuffer, &vertexSize, &offset);

        mContext->VSSetShader(renderer::vsShader, 0, 0);
        mContext->PSSetShader(renderer::psShader, 0, 0);

        mContext->Draw(3, 0);

        mSwapChain->Present(1, 0);
    }
}    