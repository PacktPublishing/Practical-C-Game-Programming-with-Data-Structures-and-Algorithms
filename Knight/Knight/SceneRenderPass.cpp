#include "raylib.h"
#include "rlgl.h"

#include "Knight.h"
#include "SceneRenderPass.h"

bool SceneRenderPass::Create(Scene* sc)
{
	pScene = sc;
	return true;
}

void SceneRenderPass::Render()
{
	//render background first
	vector<RenderContext>::iterator bk = pScene->_RenderQueue.Background.begin();
	while (bk != pScene->_RenderQueue.Background.end())
	{
		bk->pComponent->Draw(&Hints);
		++bk;
	}

	//render opauqe geometry from nearest to farest
	multiset<RenderContext, CompareDistanceAscending>::iterator opaque = pScene->_RenderQueue.Geometry.begin();
	while (opaque != pScene->_RenderQueue.Geometry.end())
	{
		opaque->pComponent->Draw(&Hints);
		++opaque;
	}

	rlDisableDepthMask(); //disable depth mask to avoid writing depth for alpha blend image
	//render alpha blend from back to front
	multiset<RenderContext, CompareDistanceDescending>::iterator alpha = pScene->_RenderQueue.AlphaBlending.begin();
	while (alpha != pScene->_RenderQueue.AlphaBlending.end())
	{
		BeginBlendMode(alpha->pComponent->blendingMode);
		alpha->pComponent->Draw(&Hints);
		EndBlendMode();
		++alpha;
	}

	rlDisableDepthTest();

	//render overlay at last
	vector<RenderContext>::iterator overlay = pScene->_RenderQueue.Overlay.begin();
	while (overlay != pScene->_RenderQueue.Overlay.end())
	{
		BeginBlendMode(alpha->pComponent->blendingMode);
		overlay->pComponent->Draw(&Hints);
		EndBlendMode();
		++overlay;
	}
	rlEnableDepthTest();
	rlEnableDepthMask();
}

/// <summary>
/// OnAddToRender - this method is called by the SceneRenderPass to add a Component to the render queue.
/// </summary>
/// <param name="pSC">Pointer to Component object</param>
/// <param name="pSO">Pointer to SceneObject</param>
/// <returns>Return true if the Component pSC is added into renderQueue, false if it gets skipped</returns>
bool SceneRenderPass::OnAddToRender(Component* pSC, SceneObject* pSO)
{
	float dist2 = 0;

	SceneActor* pActor = dynamic_cast<SceneActor*>(pSO);

	if (pActor != nullptr) 
	{
		Vector3 pos = pActor->Position;
		if (pActiveCamera != nullptr)
			dist2 = Vector3DistanceSqr(pos, pActiveCamera->GetPosition());
	}

	switch (pSC->renderQueue)
	{
		case Component::eRenderQueueType::Background:
			pScene->_RenderQueue.Background.push_back(RenderContext{ pSC, dist2 });
			break;
		case Component::eRenderQueueType::Geometry:
			pScene->_RenderQueue.Geometry.insert(RenderContext{ pSC, dist2 });
			break;
		case Component::eRenderQueueType::AlphaBlend:
			pScene->_RenderQueue.AlphaBlending.insert(RenderContext{ pSC, dist2 });
			break;
		case Component::eRenderQueueType::Overlay:
			pScene->_RenderQueue.Overlay.push_back(RenderContext{ pSC, dist2 });
			break;
	}

	return true;
}

void SceneRenderPass::BuildRenderQueue(SceneObject* pRoot)
{
	if (pRoot == nullptr)
		return;

	map<Component::eComponentType, Component*>::iterator it = pRoot->_Components.begin();
		
	if (pRoot->IsActive == false)
		return;
					
	while (it != pRoot->_Components.end())
	{
		if (it->second == nullptr)
			continue;
		OnAddToRender(it->second, pRoot);
		++it;
	}

	for (int i = 0; i < pRoot->_Children.size(); i++)
		BuildRenderQueue(pRoot->_Children[i]);

}

void SceneRenderPass::InitLightUniforms(Shader& shader)
{
	for (int i = 0; i < NUM_MAX_LIGHTS; i++)
	{
		_SceneLightData[i].attenuationLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", i));
		_SceneLightData[i].colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", i));
		_SceneLightData[i].enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", i));
		_SceneLightData[i].positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", i));
		_SceneLightData[i].targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", i));
		_SceneLightData[i].typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", i));
	}
	ambientLoc = GetShaderLocation(shader, "ambient");
	shinenessLoc = GetShaderLocation(shader, "materialShininess");
}

void SceneRenderPass::UpdateLightData(const Shader& shader)
{
	for (int i = 0; i < NUM_MAX_LIGHTS; i++)
	{
		LightData* pLightData = &pScene->Lights[i];

		if (pLightData->dirty != false)
		{
			SetShaderValue(shader, _SceneLightData[i].enabledLoc, &pLightData->enabled, SHADER_UNIFORM_INT);
			if (!pLightData->enabled)
			{
				//if the light get disabled, there is no need to update the rest of the light data
				continue;
			}

			//Update light data
			SetShaderValue(shader, _SceneLightData[i].typeLoc, &pLightData->type, SHADER_UNIFORM_INT);
			float position[3] = { pLightData->position.x, pLightData->position.y, pLightData->position.z };
			SetShaderValue(shader, _SceneLightData[i].positionLoc, position, SHADER_UNIFORM_VEC3);
			float target[3] = { pLightData->target.x, pLightData->target.y, pLightData->target.z };
			SetShaderValue(shader, _SceneLightData[i].targetLoc, target, SHADER_UNIFORM_VEC3);
			float color[4] = { (float)pLightData->color.r / (float)255, (float)pLightData->color.g / (float)255,
							   (float)pLightData->color.b / (float)255, (float)pLightData->color.a / (float)255 };
			SetShaderValue(shader, _SceneLightData[i].colorLoc, color, SHADER_UNIFORM_VEC4);

			//remove dirty flag
			pLightData->dirty = false;
		}
	}
	// Update ambient light value
	SetShaderValue(shader, ambientLoc, pScene->AmbientColor, SHADER_UNIFORM_VEC4);
}
