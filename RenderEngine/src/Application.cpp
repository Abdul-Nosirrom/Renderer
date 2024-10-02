#include "Application.h"
#include "Math/Math.h"
#include <chrono>
#include <random>
#include "Scene/Camera.h"
#include <iostream>

#include "Scene/Scene.h"
//#include "ImGuiContext.h"

RenderEngine::Application::Application()
    : m_Window(800, 800, "Renderer")
{
	Matrix4x4 A;
	A << 1, 0, 1, 0,
		0, 0, 1, 0,
		1, 2, 0, 0,
		0, 1, 1, 1;

	Matrix4x4 B;
	B << 0, 2, 2, 1,
		1, 1, 0, 0,
		0, 1, 2, 3,
		0, 0, 0, 1;

	std::cout << "Trasnlation Matrix = " << std::endl << MatrixConstruction::TranslationMatrix({ 2.f, 5.f, 8.f }) << std::endl;

	std::cout << std::endl << "A = " << std::endl << A << std::endl;
	std::cout << std::endl << "B = " << std::endl << B << std::endl;

	std::cout << std::endl << "A * B = " << std::endl << A * B << std::endl;

	Vector3 Trans(2.f, 3.f, 4.f);
	Vector3 Forward(1.f, 0.f, 0.f);
	Vector3 Up(0.f, 1.f, 0.f);

	//std::cout << MatrixConstruction::LookAtTransform(Trans, -Forward, Up) << std::endl;
}

int RenderEngine::Application::Run()
{
    return 0;
}

void RenderEngine::Application::RunFrame()
{
	const float dt = m_Timer.Mark();
	Camera::Get().Update(dt);


	m_Window.GFX().StartFrame();

	Scene::Get()->Render(m_Window.GFX());

	//ImGuiManager::EndFrame();
	m_Window.GFX().EndFrame();
}
