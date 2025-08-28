#pragma once

#include "Demo2a.h"

//Update() function of base class Entity is called by Scene::Update() function
void Entity::Update(float elapsedTime)
{
	//We do not need to call Actor->Update() here, because the Scene will automatically call it for us
	//If you have common logic shared by all kinds of inherited class of Entity, you can put it here
}

//Initialize a Player entity with its SceneActor
void PlayerEntity::Create(Scene * pScene, Entity* pParent)
{
	//Place player
	Actor = pScene->CreateSceneObject<SceneActor>("Player");
	Actor->Scale = Vector3{ 3.0f, 3.0f, 3.0f };
	Actor->Position = Vector3{ 0, -3, 30.0f };
	Actor->Rotation.y = 180.0f;
	ModelComponent* animPlayerComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetAnimation(6);
	Actor->AddComponent(animPlayerComponent);
}

//Create an Enemy entity with its SceneActor
void EnemyEntity::Create(Scene* pScene, Entity* pParent)
{
	//Place enemy
	Actor = pScene->CreateSceneObject<SceneActor>("Enemy");
	Actor->Scale = Vector3{ 3.0f, 5.0f, 3.0f };
	Actor->Position = Vector3{ 6.0f, -2.0f, 0.0f };
	ModelComponent* animEnemyComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animEnemyComponent->Load3DModel("../../resources/models/gltf/greenman.glb");
	animEnemyComponent->SetAnimation(0);
	Actor->AddComponent(animEnemyComponent);
}

//Create a Prop entity with its SceneActor
void PropEntity::Create(Scene* pScene, Entity *pParent)
{
	//Place a cube as prop on the terrain
	Actor = pScene->CreateSceneObject<SceneActor>("Prop");
	if (pParent != NULL)
		Actor->SetParent(pParent->Actor);
	Actor->Position = Vector3{ -2.0f, 0.0f, 1.0f };
	Actor->Scale = Vector3{ 0.2f, 5.0f, 0.2f };
	ModelComponent* animEnemyComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animEnemyComponent->Load3DModel("../../resources/models/obj/well.obj", "../../resources/models/obj/well_diffuse.png");
	Actor->AddComponent(animEnemyComponent);
}

//Create a Terrain entity with its SceneActor
//In this demo terrain (a bridge) is also a 3D model
void TerrainEntity::Create(Scene* pScene, Entity* pParent)
{
	//Create a plan as terrain
	Actor = pScene->CreateSceneObject<SceneActor>("Terrain");
	Actor->Position = Vector3{ 0, -5, 0 };
	Actor->Scale = Vector3{ 20, 1, 20 };
	ModelComponent* animEnemyComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animEnemyComponent->Load3DModel("../../resources/models/obj/bridge.obj", "../../resources/models/obj/bridge_diffuse.png");
	Actor->AddComponent(animEnemyComponent);
}