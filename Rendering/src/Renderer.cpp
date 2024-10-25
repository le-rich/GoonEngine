#include <glad/glad.h>
#include <glm.hpp>


//#include <vector>

#include "Renderer.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

// Constructor.
Renderer::Renderer()
{
    texture = new Texture("../Core/Assets/TestAE/textures/fmesh_Material_001_BasssseColor_001df.png");
    testShader = new Shader("../Rendering/Shaders/default.glsl");
}

// Destructor.
Renderer::~Renderer() {}

void Renderer::Update()
{
    windowRef->SetWindowToCurrentThread();
    // Clear color and depth buffers (can be moved to pre update
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
   

    /*Get lights
      For each light
        Get lighting matrix
        Add lighting matrix to list
      bind lighting list to Shader Buffer*/

      //For each Material
      //if texture --> bind texture
      //if shader  --> bind shader

    texture->Bind();
    testShader->Use();
    //Apply Uniforms (lighting, view matrices, etc...)
    // 
    //std::string name = "u_Texture";
    //GLCall(int location = glGetUniformLocation(1, name.c_str()));
    //GLCall(glUniform1i(location, 0));
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
    mUniformBuffer.SetSubData(modelMatrix, 0);

    /*For each Primitive
        Bind Vertex Array
        Bind Index Buffer
        DrawCall*/

        // GARBAGE METHODOLOGY
    testMesh.Draw();

    /*Perform Post Processing
      Draw Frame Buffer*/

    texture->Unbind();
    // Swap window buffers. can be moved to post update
    windowRef->SwapWindowBuffers();
}





