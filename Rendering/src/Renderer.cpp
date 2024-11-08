#include "Renderer.h"

#include "Components/MeshComponent.h"
#include "Core.h"
#include "Scene.h"
#include "Managers/AssetManager.h"
#include "Managers/EntityManager.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

#include <iostream>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

Renderer::Renderer(Window* window)
{
	windowRef = window;
}

Renderer::~Renderer() {}

void Renderer::Initialize() {
	System::Initialize();

	// Get Camera if it exists
	auto mainCameraUUID = EntityManager::getInstance().findFirstEntityByDisplayName("Main Camera");
	auto mainCamera = EntityManager::getInstance().getEntity(mainCameraUUID);
	if (mainCamera != nullptr) {
		this->mainCameraEntity = mainCamera;
	}
	else
	{
		std::cerr << "Error: Main Camera not found. The program will now exit." << std::endl;
		std::cin.get();
		std::exit(EXIT_FAILURE);  // Terminate program with failure status
	}
	updateThread = new std::thread([&]()
		{
			auto previousTime = std::chrono::high_resolution_clock::now();
			while (!glfwWindowShouldClose(windowRef->GetWindow()))
			{
				auto currentTime = std::chrono::high_resolution_clock::now();
				std::chrono::duration<float> deltaTime = currentTime - previousTime;
				auto deltaTimeFloatSeconds = deltaTime.count();
				previousTime = currentTime;
				Update(deltaTimeFloatSeconds);
			}
		});
}

void Renderer::Update(float deltaTime)
{

	Scene* scene = Core::getInstance().GetScene();
	Entity* root;
	{
		std::lock_guard<std::mutex> lock(scene->mtx);
		root = scene->GetSceneCopy();
	}
	delete root; // Memory Leak

	/*Get lights
	  For each light
		Get lighting matrix
		Add lighting matrix to list
	 */


	{
		std::lock_guard<std::mutex> lock(windowRef->mMutex);
		windowRef->SetWindowToCurrentThread();
		// Clear color and depth buffers for set Framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		// CAMERA =====================
		CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(mainCameraEntity->getComponent(ComponentType::Camera));

		if (cameraComponent != nullptr) {
			// Update Aspect Ratio if the window has resized
			int width, height;
			glfwGetWindowSize(windowRef->GetWindow(), &width, &height);
			cameraComponent->updateAspectRatio(width, height);

			cameraComponent->calculateViewMatrix(cameraComponent->entity->transform);
			cameraComponent->calculateProjectionMatrix();
		}
		// ============================

			/*
			Material Based:
			For each Material
				Bind Material
				Apply Material specific Uniforms
				For each Primitive in Material
					Bind Vertex Array
					Bind Index Buffer
					DrawCall
				Unbind Material
			 */

			 // Kinda okay methodology
			 //TODO: Replace with Scene based or Material based Draw


			// MESHES ============================
		auto meshComponentUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Mesh);

		for (auto& uuid : meshComponentUUIDs)
		{
			auto entity = EntityManager::getInstance().getEntity(uuid);
			MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(entity->getComponent(ComponentType::Mesh));

			if (meshComponent != nullptr)
			{
				glm::vec3 localScale = entity->transform->getLocalScale();

				// TODO: Bug Physics/Core on way to get modelMatrix directly from transform
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), entity->transform->getWorldPosition()) *
					glm::mat4_cast(entity->transform->getLocalOrientation()) *
					glm::scale(glm::mat4(1.0f), localScale);

				mEngineUniformBuffer.updateMatrices(
					modelMatrix,
					cameraComponent->getViewMatrix(),
					cameraComponent->getProjectionMatrix(),
					mainCameraEntity->transform->getWorldPosition()
				);

				meshComponent->getMesh()->Draw();
			}
		}

		// ===================================

			/*Perform Post Processing
			  Draw Frame Buffer*/
	}
	// Swap window buffers. can be moved to post update
	windowRef->SwapWindowBuffers();
}

void Renderer::FixedUpdate() {

}





