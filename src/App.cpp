#include <App.h>
#include <iostream>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

namespace NdRenderer
{
	App::App(int dx, int dy)
	{
		InitialiseWindow(dx, dy);

		NativeWindow = Diligent::NativeWindow{ static_cast<void*>(glfwGetWin32Window(mWindow)) };
		mGraphicsHandler = new GraphicsHandler(1, &NativeWindow);
	}

	App::~App()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();

		delete mGraphicsHandler;
	}

	void App::InitialiseWindow(int dx, int dy)
	{
		if (!glfwInit())
		{
			std::cout << "GLFW not initialised" << std::endl;
			throw std::runtime_error("GLFW not initialized");
		}

		glfwSetErrorCallback(error_callback); //Set error callback function

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Disable OpenGL context creation
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);    // Allow window resizing

		mWindow = glfwCreateWindow(dx, dy, "NdRenderer", NULL, NULL);
	}

	void App::Step(float DeltaTime){}
	void App::Run()
	{
		while (!glfwWindowShouldClose(mWindow))
		{
			mGraphicsHandler->RenderGraphics();
			mGraphicsHandler->PresentGraphics();
			glfwPollEvents();
		}
	}
}