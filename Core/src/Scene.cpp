#pragma once

#include <vector>

#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "Entities/LightEntity.h"
//#include "GLTFLoader.h"
#include "SceneParser.h"
#include "Managers/EntityManager.h"
#include "Components/MeshComponent.h"
#include "Components/ScriptComponent.h"
#include "GarageScript.h"
#include "OrbitScript.h"
#include "TimerScript.h"
#include "LapManagerScript.h"
#include "LapCheckpointScript.h"
#include "FollowCamScript.h"
#include <CarControllerScript.h>
//#include "Input.h"

Scene::Scene()
{
    sceneRoot = new Entity("root");
    EntityManager::getInstance().addEntityToMap(*sceneRoot);
}

Scene::~Scene()
{

}

void Scene::SpawnSceneDefinition()
{
    SceneParser::LoadScene(SCENE_FILE);
    //createGameManager(); // GameManager currently setup via SceneJSON

    LightEntity* lightEntity = new LightEntity();
    lightEntity->SetDisplayName("Light");
    lightEntity->transform->setLocalPosition(glm::vec3(0.f, 5.f, 0.f));
    lightEntity->transform->setLocalOrientation(glm::quat(-0.5f, 0.5f, 0.5f, 0.f));
    EntityManager::getInstance().Instantiate(lightEntity);
    auto* entityManager = &(EntityManager::getInstance());

    //Entity* cameraEnt = entityManager->findFirstEntityByDisplayName("Main Camera");

    // TODO: JSONparser for Scene entities and scripts/components.
    // TODO: Figure out location and pathing of assets/non code files within solution

    const std::string MAP_SOURCE_FOLDER = "TestMap/";
    const std::string MAP_MODEL_FILE = "track.gltf";

    Entity* raceTrackEnt = new Entity();
    raceTrackEnt->SetDisplayName("Race Track");
    GLTFLoader::LoadModelAsEntity(raceTrackEnt, MAP_SOURCE_FOLDER, MAP_MODEL_FILE);
    EntityManager::getInstance().Instantiate(raceTrackEnt);

    glm::vec4 trackPosition = glm::vec4(100.0f, 0.0f, 100.0f, 0.0f);

    raceTrackEnt->transform->setWorldPosition(glm::vec4(0.0f, -1.3f, 0.0f, 1.0f) + trackPosition);

    Entity* carEnt = entityManager->findFirstEntityByDisplayName("Test Car");

    const std::string CUBE_SOURCE_FOLDER = "TestCube2/";
    const std::string CUBE_MODEL_FILE = "testcube.gltf";

    // TODO: move waypoint visual creation to the lapcheckpointscripts
    Entity* cubeEntity = new Entity();
    GLTFLoader::LoadModelAsEntity(cubeEntity, CUBE_SOURCE_FOLDER, CUBE_MODEL_FILE);
    EntityManager::getInstance().Instantiate(cubeEntity);
    Entity* checkpoint1 = EntityManager::getInstance().findFirstEntityByDisplayName("Checkpoint 1");
    glm::vec3 checkpointPos = trackPosition.xyz + checkpoint1->transform->getWorldPosition();
    checkpoint1->transform->setWorldPosition(glm::vec4(checkpointPos, 1.0f));
    checkpoint1->addChild(cubeEntity);
    cubeEntity->setParent(checkpoint1);
    EntityManager::getInstance().addEntityToMap(*cubeEntity);

    Entity* checkpoint2 = EntityManager::getInstance().findFirstEntityByDisplayName("Checkpoint 2");
    cubeEntity = new Entity();
    GLTFLoader::LoadModelAsEntity(cubeEntity, CUBE_SOURCE_FOLDER, CUBE_MODEL_FILE);
    EntityManager::getInstance().Instantiate(cubeEntity);
    checkpointPos = trackPosition.xyz + checkpoint2->transform->getWorldPosition();
    checkpoint2->transform->setWorldPosition(glm::vec4(checkpointPos, 1.0f));
    checkpoint2->addChild(cubeEntity);
    cubeEntity->setParent(checkpoint2);
    EntityManager::getInstance().addEntityToMap(*cubeEntity);

    // Garage Room
    Entity* garageRoomController = new Entity();
    garageRoomController->SetDisplayName("Garage Controller");
    EntityManager::getInstance().Instantiate(garageRoomController);

    // spin
    Entity* cameraEnt = entityManager->findFirstEntityByDisplayName("Main Camera");
    ScriptComponent* orb = dynamic_cast<ScriptComponent*>(cameraEnt->getComponent(ComponentType::Script));
    OrbitScript* orbScript = orb->GetScript<OrbitScript>();

    auto garageRoomEnt = entityManager->findFirstEntityByDisplayName("Garage Controller");
    ScriptComponent* garageScriptComponent = new ScriptComponent(garageRoomEnt);
    GarageScriptParams garageParams;
    garageParams.orbitScript = orbScript;
    garageParams.cars.push_back(entityManager->findFirstEntityByDisplayName("Test Car"));
    garageParams.cars.push_back(entityManager->findFirstEntityByDisplayName("TestCar2"));
    garageParams.chosenTarget = garageParams.cars[0]->transform;

    garageScriptComponent->CreateAndAddScript<GarageScript>(&garageParams);
    garageRoomEnt->addComponent(*garageScriptComponent);
    auto garageScript = garageScriptComponent->GetScript<GarageScript>();
    garageScript->BindSelectKey();
    garageScript->BindChooseKey();
}

void Scene::createGameManager()
{
    Entity* gameManager = new Entity();
    gameManager->SetDisplayName("GameManager");
    EntityManager::getInstance().Instantiate(gameManager);

    auto* entityManager = &(EntityManager::getInstance());
    auto gameManagerEnt = entityManager->findFirstEntityByDisplayName("GameManager");

    ScriptComponent* gameManagerScriptComponent = new ScriptComponent(gameManagerEnt);
    gameManagerEnt->addComponent(*gameManagerScriptComponent);

    auto carEnt = entityManager->findFirstEntityByDisplayName("Test Car");

    ScriptComponent* timerScriptComponent = new ScriptComponent(gameManagerEnt);
    gameManagerEnt->addComponent(*timerScriptComponent);

    TimerScriptParams timerParams;
    timerParams.m_TimerTarget = carEnt->transform;
    timerParams.m_OriginalPosition = carEnt->transform->getWorldPosition();
    gameManagerScriptComponent->CreateAndAddScript<TimerScript>(&timerParams);

    // Set LapManagerScript parameters & add to component
    LapManagerScriptParams lapManagerParams;

    auto checkpoint1 = entityManager->findFirstEntityByDisplayName("Checkpoint 1");
    auto checkpoint2 = entityManager->findFirstEntityByDisplayName("Checkpoint 2");
    LapCheckpointScript* checkpointScript = dynamic_cast<ScriptComponent*>(checkpoint1->getComponent(ComponentType::Script))->GetScript<LapCheckpointScript>();
    LapCheckpointScript* checkpointScript2 = dynamic_cast<ScriptComponent*>(checkpoint2->getComponent(ComponentType::Script))->GetScript<LapCheckpointScript>();

    std::vector<LapCheckpointScript*> checkpoints = { checkpointScript, checkpointScript2 };
    lapManagerParams.m_Checkpoints = checkpoints;
    gameManagerScriptComponent->CreateAndAddScript<LapManagerScript>(&lapManagerParams);

    // Set LapManager vars of checkpoints
    LapManagerScript* lapManagerPointerFromScript = gameManagerScriptComponent->GetScript<LapManagerScript>();
    checkpointScript->SetLapManager(lapManagerPointerFromScript);
    checkpointScript2->SetLapManager(lapManagerPointerFromScript);

}

Entity* Scene::GetSceneCopy()
{
    Entity* cur = new Entity(*sceneRoot);
    return cur;
}
