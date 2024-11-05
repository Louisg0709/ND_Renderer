#pragma once

#include <GLFW/glfw3.h>
#include <EngineFactoryVk.h>
#include <GraphicsHandler.h>

namespace NdRenderer
{
	class App
	{
	public:
		App(int dx, int dy);
		~App();

		virtual void Run();
		virtual void Step(float DeltaTime);

	protected:
		GLFWwindow* mWindow;
		void InitialiseWindow(int dx, int dy);

		GraphicsHandler* mGraphicsHandler;

	private:
		Diligent::NativeWindow NativeWindow;
	};
}