#pragma once

#include "raylib.h"
#include "rlgl.h"

#include "Knight.h"
#include "SceneRenderPass.h"

/// <summary>
/// Create - Initialize the SceneRenderPass with a pointer to the Scene.
/// </summary>
/// <param name="sc"></param>
/// <returns></returns>
bool SceneRenderPass::Create(Scene* sc)
{
	pScene = sc;
	return true;
}

/// <summary>
/// Render - Render all Components in the render queue.
/// </summary>
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
		Vector3 pos = pActor->GetWorldPosition();
		if (pActiveCamera != nullptr) 
		{
			//use bounding box distance if available, otherwise fallback to actor position distance
			if (IsBoundingBoxValid(pActor->WorldBoundingBox)) {
				dist2 = PointToBoxDistanceSqr(pActiveCamera->GetPosition(), pActor->WorldBoundingBox);
			}
			else {
				//fallback to use actor position if bounding box is not valid
				dist2 = Vector3DistanceSqr(pActor->GetWorldPosition(), pActiveCamera->GetPosition());
			}
			pActor->SquareDistanceToCamera = dist2; //cache the distance to camera for sorting
		}
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

/// <summary>
/// BuildRenderQueue - recursively traverse the scene graph starting from pRoot, and add Components to the render queue
/// </summary>
/// <param name="pRoot">The current SceneObject</param>
void SceneRenderPass::BuildRenderQueue(SceneObject* pRoot)
{
	if (pRoot == nullptr)
		return;

	map<Component::eComponentType, Component*>::iterator it = pRoot->_Components.begin();
		
	if (pRoot->IsActive == false)
		return;

	bool bAddComponents = true;

	SceneActor* pActor = dynamic_cast<SceneActor*>(pRoot);
	if (pActiveCamera != nullptr && pActor != nullptr)
	{
		if (!(pActor->WorldBoundingBox.min.z == pActor->WorldBoundingBox.max.z))
		{
			FrustumPlane frustumPlanes[6];
			pActiveCamera->ExtractFrustumPlanes(frustumPlanes);
			if (pActiveCamera->IsBoundingBoxInFrustum(pActor->WorldBoundingBox, frustumPlanes) == false) 
			{
				bAddComponents = false;
				++NumComponentsSkipped;
			}				
		}
	}

	if (bAddComponents == true)
	{
		while (it != pRoot->_Components.end())
		{
			if (it->second == nullptr)
				continue;
			OnAddToRender(it->second, pRoot);
			++it;
		}
	}
					
	for (int i = 0; i < pRoot->_Children.size(); i++)
		BuildRenderQueue(pRoot->_Children[i]);

}

void SceneRenderPass::InitRenderPassUniforms(const Shader& shader)
{
	alphaTestLoc = GetShaderLocation(shader, "alphaTest");

	//If the shader has alphaTest uniform, enable alpha test by default
	if (alphaTestLoc >= 0) {
		int alphaTestDefault = 1;
		SetShaderValue(shader, alphaTestLoc, &alphaTestDefault, SHADER_UNIFORM_INT);
	}
}


//End of SceneRenderPass.cpp