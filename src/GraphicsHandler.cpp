#include <GraphicsHandler.h>

namespace NdRenderer
{
	using namespace Diligent;

	GraphicsHandler::GraphicsHandler(int validationLevel, Diligent::NativeWindow* window)
	{
		ValidationLevel = validationLevel;
		pWindow = window;
        InitialiseDiligent();
        InitialiseGraphicsPipeline();
	}

	void GraphicsHandler::InitialiseDiligent()
	{
		SwapChainDesc SCDesc;
		std::vector<IDeviceContext*> ppContexts;

		EngineVkCreateInfo EngVkAttribs;
		if (ValidationLevel >= 1) { EngVkAttribs.EnableValidation = true; }
		else if (ValidationLevel == 0) { EngVkAttribs.EnableValidation = false; }

		ppContexts.resize(1 + EngVkAttribs.NumDeferredContexts);
		auto* pFactoryVk = GetEngineFactoryVk();
		m_pEngineFactory = pFactoryVk;
		pFactoryVk->CreateDeviceAndContextsVk(EngVkAttribs, &m_pDevice, ppContexts.data());
		if(!m_pDevice)
		{
			std::cout << "Vulkan device not initialised" << std::endl;
			throw std::runtime_error("Vulkan device not initialized");
		}

		if(!m_pSwapChain && pWindow != nullptr)
		{
			pFactoryVk->CreateSwapChainVk(m_pDevice, ppContexts[0], SCDesc, *pWindow, &m_pSwapChain);
		}

		m_pImmediateContext.Attach(ppContexts[0]);
		auto NumDeferredCtx = ppContexts.size() - 1;
		m_pDeferredContexts.resize(NumDeferredCtx);
		for (Uint32 ctx = 0; ctx < NumDeferredCtx; ++ctx)
		m_pDeferredContexts[ctx].Attach(ppContexts[1 + ctx]);
	}

	void GraphicsHandler::InitialiseGraphicsPipeline()
	{
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "Simple triangle PSO";
        // This is a graphics pipeline
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        // Set render target format which is the format of the swap chain's color buffer
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = m_pSwapChain->GetDesc().ColorBufferFormat;
        // Use the depth buffer format from the swap chain
        PSOCreateInfo.GraphicsPipeline.DSVFormat = m_pSwapChain->GetDesc().DepthBufferFormat;
        // Primitive topology defines what kind of primitives will be rendered by this pipeline state
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        // No back face culling for this tutorial
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
        // Disable depth testing
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
        // clang-format on

        RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory("Assets/Shaders/", &pShaderSourceFactory);

        ShaderCreateInfo ShaderCI;
        // Tell the system that the shader source code is in HLSL.
        ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        // Create a vertex shader
        RefCntAutoPtr<IShader> pVS;
        {
            ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;
            ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
            ShaderCI.EntryPoint = "main";
            ShaderCI.Desc.Name = "Triangle vertex shader";
            ShaderCI.FilePath = "triangle.vsh";
            m_pDevice->CreateShader(ShaderCI, &pVS);
        }

        // Create a pixel shader
        RefCntAutoPtr<IShader> pPS;
        {
            ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;
            ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
            ShaderCI.EntryPoint = "main";
            ShaderCI.Desc.Name = "Triangle pixel shader";
            ShaderCI.FilePath = "triangle.psh";
            m_pDevice->CreateShader(ShaderCI, &pPS);
        }

        // Finally, create the pipeline state
        PSOCreateInfo.pVS = pVS;
        PSOCreateInfo.pPS = pPS;
        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

        // Create the shader resource binding (SRB)
        m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);
	}

	void GraphicsHandler::RenderGraphics()
	{
        if (m_pImmediateContext)
        {
            auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
            auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
            m_pImmediateContext->SetRenderTargets(
                1, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            // Clear the back buffer
            const float ClearColor[] = { 0.350f, 0.350f, 0.350f, 1.0f };
            // Let the engine perform required state transitions
            m_pImmediateContext->ClearRenderTarget(
                pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            m_pImmediateContext->ClearDepthStencil(
                pDSV, CLEAR_DEPTH_FLAG, 1.f, 0,
                RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            // Set pipeline state in the immediate context
            m_pImmediateContext->SetPipelineState(m_pPSO);
            // We need to commit shader resource. Even though in this example
            // we don't really have any resources, this call also sets the shaders
            m_pImmediateContext->CommitShaderResources(m_pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            DrawAttribs drawAttrs;
            drawAttrs.NumVertices = 3;  // We will render 3 vertices
            m_pImmediateContext->Draw(drawAttrs);
        }
	}

	void GraphicsHandler::PresentGraphics()
	{
        if (m_pImmediateContext)
        {
            if (!m_pSwapChain) {
                return;
            }
            m_pSwapChain->Present();
        }
	}
}