#pragma once
/* @TODO: Extrapolate shader functionality, error handling, and
implement pre and post methods.*/

#include <glad/glad.h>

//#include "Renderer.h"
#include "System.h"
// @TODO: Remove TESTING INCLUDE
#include "Buffers/UniformBuffer.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Resources/Shader.h"
#include "Components/Transform.h"

#include "Window/Window.h"
//#include "GLFW/include/GLFW/glfw3.h"

class Renderer : public System
{
public:
    Renderer();
    ~Renderer();

    void Update() override;

    const char* GetName() const override {
        return "Renderer";
    }


    void FixedUpdate() override {
        // Update Loop logic here

    }

    Mesh testMesh;
    Transform* testTransform;
    Texture* texture;
    Shader* testShader;
    Window* windowRef;
private:
    UniformBuffer mUniformBuffer;

};
