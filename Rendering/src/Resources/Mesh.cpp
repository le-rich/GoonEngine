#include "Resources/Mesh.h"

// Draws mesh
void Mesh::Draw()
{
    for (auto primitive : mPrimitives)
    {
        primitive.Draw();
    }
}

void Mesh::DrawWithOwnMaterial()
{
    for (auto primitive : mPrimitives)
    {
        primitive.DrawWithOwnMaterial();
    }
}
