#pragma once

#include "Entity.h"

#include <mutex>

class Scene
{
public:
    Scene();
    ~Scene();

    Entity* getRoot() {
       return sceneRoot;
    };

    Entity* sceneRoot = nullptr;
    // lock to guarantee mutual exclusion
    std::mutex mtx;

    // TODO: This function loads and structures the scene graph. This should be made virtual only.
    void SpawnSceneDefinition();
    Entity* GetSceneCopy();
};