#include "BonusGameWorld01.h"

bool PropEntity::Create(Scene* pScene, Entity* pContainer)
{
	//Castle
	_Actor = pScene->CreateSceneObject<SceneActor>("Castle");
	_Actor->Scale = Vector3{ 0.3f, 0.3f, 0.3f };
	_Actor->Position = Vector3{ 16.f,0,32.0f };

	if (pContainer != nullptr)
	{
		pTerrainEntity = dynamic_cast<TerrainEntity*>(pContainer);
		if (pTerrainEntity == nullptr)
		{
			TraceLog(LOG_INFO, "<PropEntity.Create> pContainer is null, or not a TerrainEntity!");
			return false;
		}
		QuadTreeTerrainComponent* tc = pTerrainEntity->_Actor->GetComponent<QuadTreeTerrainComponent>();
		if (!tc)
		{
			TraceLog(LOG_INFO, "<PropEntity.Create> No QuadTreeTerrainComponent found in the container!");
			return false;
		}
		_Actor->Position.y = tc->GetTerrainY(_Actor->Position.x, _Actor->Position.z); // Set height based on terrain
	}

	ModelComponent* modelComponent = _Actor->CreateAndAddComponent<ModelComponent>();
	modelComponent->Load3DModel("../../resources/models/obj/castle.obj", "../../resources/models/obj/castle_diffuse.png");
	modelComponent->castShadow = Component::eShadowCastingType::Shadow;
	modelComponent->receiveShadow = true;

	return true;
}

void PropEntity::Update(float elapsedTime)
{
}

//End of PropEntity.cpp