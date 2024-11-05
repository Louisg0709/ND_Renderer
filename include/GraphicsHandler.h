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

//for testing purposes
static const char* VSSource = R"(
struct PSInput 
{ 
    float4 Pos   : SV_POSITION; 
    float3 Color : COLOR; 
};
void main(in  uint    VertId : SV_VertexID,
          out PSInput PSIn) 
{
    float4 Pos[3];
    Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
    Pos[1] = float4( 0.0, +0.5, 0.0, 1.0);
    Pos[2] = float4(+0.5, -0.5, 0.0, 1.0);
    float3 Col[3];
    Col[0] = float3(1.0, 0.0, 0.0); // red
    Col[1] = float3(0.0, 1.0, 0.0); // green
    Col[2] = float3(0.0, 0.0, 1.0); // blue
    PSIn.Pos   = Pos[VertId];
    PSIn.Color = Col[VertId];
}
)";

// Pixel shader simply outputs interpolated vertex color
static const char* PSSource = R"(
struct PSInput 
{ 
    float4 Pos   : SV_POSITION; 
    float3 Color : COLOR; 
};
struct PSOutput
{ 
    float4 Color : SV_TARGET; 
};
void main(in  PSInput  PSIn,
          out PSOutput PSOut)
{
    PSOut.Color = float4(PSIn.Color.rgb, 1.0);
}
)";
//