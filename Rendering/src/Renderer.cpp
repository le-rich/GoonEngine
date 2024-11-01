//#include <glad/glad.h>
#include <glm.hpp>

#include "Core.h"
#include "Scene.h"
#include "Managers/AssetManager.h"
#include "Renderer.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::Initialize() {
	System::Initialize();
	updateThread = new std::thread([&]()
		{
			while (!glfwWindowShouldClose(windowRef->GetWindow()))
			{
				Update();
			}
		});
}

void Renderer::Update()
{
	Scene* scene = Core::getInstance().GetScene();
	Entity* root;
	{
		std::lock_guard<std::mutex> lock(scene->mtx);
		root = scene->GetSceneCopy();
	}
	printf("%s", root->GetDisplayName());
	delete root;

	/*Get lights
	  For each light
		Get lighting matrix
		Add lighting matrix to list
	 */

	  // TODO: Bug Physics/Core on way to get modelMatrix directly from transform
	glm::vec3 localScale = testTransform->getLocalScale();
	glm::mat4 modelMatrix =
		glm::translate(glm::mat4(1.0f), testTransform->getWorldPosition()) *
		glm::mat4_cast(testTransform->getLocalOrientation()) *
		glm::mat4( // creates a scale matrix 
			glm::vec4(localScale.x, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, localScale.y, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, localScale.z, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
	{
		std::lock_guard<std::mutex> lock(windowRef->mMutex);
		windowRef->SetWindowToCurrentThread();
		// Clear color and depth buffers (can be moved to pre update
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		//bind lighting list to Shader Buffer*
		mEngineUniformBuffer.SetSubData(modelMatrix, 0);
		// TODO: Set camera uniform data

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
		testMesh.Draw();


		/*Perform Post Processing
		  Draw Frame Buffer*/

		  // Swap window buffers. can be moved to post update
		//texture->Unbind();
	}
	// Swap window buffers. can be moved to post update
	windowRef->SwapWindowBuffers();
}





