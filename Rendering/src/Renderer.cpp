#include "Renderer.h"

#include "Components/MeshComponent.h"
#include "Components/LightComponent.h"
#include "Managers/AssetManager.h"
#include "Managers/EntityManager.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

#include <iostream>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

Renderer::Renderer(GLFWwindow* window)
{
    m_Window = window;
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
}

void Renderer::Update(float deltaTime)
{
    // Clear color and depth buffers (can be moved to pre update

    std::vector<glm::mat4> lightMatricies;
    auto lightComponentUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Light);
    for (auto& uuid : lightComponentUUIDs)
    {
        auto entity = EntityManager::getInstance().getEntity(uuid);
        LightComponent* lightComponent = dynamic_cast<LightComponent*>(entity->getComponent(ComponentType::Light));
        if (lightComponent == nullptr) continue;
        lightMatricies.push_back(lightComponent->GenerateMatrix(lightComponent->entity->transform));
    }

    shaderStorageBufferObject.SendBlocks(lightMatricies.data(), lightMatricies.size() * sizeof(glm::mat4));
    shaderStorageBufferObject.Bind(0);

    // CAMERA =====================
    CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(mainCameraEntity->getComponent(ComponentType::Camera));

    if (cameraComponent != nullptr){
        // Update Aspect Ratio if the window has resized
        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);
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
    shaderStorageBufferObject.Unbind();
    // ===================================

    /*Perform Post Processing
      Draw Frame Buffer*/

      // Swap window buffers. can be moved to post update
}

void Renderer::FixedUpdate() {
    
}





