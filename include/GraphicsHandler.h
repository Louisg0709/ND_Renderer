#include <vector>
#include <memory>
#include <string>
#pragma once

#include <Common/interface/RefCntAutoPtr.hpp>
#include <EngineFactoryVk.h>

namespace NdRenderer
{
    using namespace Diligent;
	class GraphicsHandler
	{
	public:
		GraphicsHandler(int validationLevel, Diligent::NativeWindow* window);

        void RenderGraphics();
        void PresentGraphics();

	protected:
		void InitialiseDiligent();
		void InitialiseGraphicsPipeline();

    private:
        int ValidationLevel;
        RefCntAutoPtr<IEngineFactory> m_pEngineFactory;
        RefCntAutoPtr<ISwapChain> m_pSwapChain;
        RefCntAutoPtr<IRenderDevice> m_pDevice;
        RefCntAutoPtr<IDeviceContext> m_pImmediateContext;
        std::vector<RefCntAutoPtr<IDeviceContext>> m_pDeferredContexts;
        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IShaderResourceBinding> m_pSRB;

        Diligent::NativeWindow* pWindow;
	};


}
