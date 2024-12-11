#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <vector>
#include <memory>

#include "Utils/IDUtils.h"
#include "Buffers/VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Data/Vertex.h"


class Primitive
{
public:
    Primitive(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    Primitive(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const UUIDv4::UUID& pMaterialID);
    ~Primitive();

    void Draw();
    void DrawWithOwnMaterial();
    std::vector<Vertex> GetLoadedVertices();
    inline void SetMaterial(UUIDv4::UUID& pMaterialID) { mMaterialID = pMaterialID; }
    inline UUIDv4::UUID GetMaterial() { return mMaterialID; }

    UUIDv4::UUID mMaterialID;
private:
    std::shared_ptr<VertexBuffer> mVertexBuffer;
    std::shared_ptr<IndexBuffer> mIndexBuffer;
    std::shared_ptr<VertexArray> mVertexArray;
    VertexBufferLayout mVertexBufferLayout;
    std::vector<Vertex> mLoadedVertices;
    std::vector<uint32_t> mLoadedIndices;

    void InitPrimitive();
};
