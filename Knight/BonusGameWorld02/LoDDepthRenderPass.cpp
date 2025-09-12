//New depth render pass that only render shadow within certain distance to the view camera
#include "LoDDepthRenderPass.h"

LoDDepthRenderPass::LoDDepthRenderPass(float cutof, ShadowSceneLight* l) : DepthRenderPass(l)
{
	CutOffDistance = cutof;
}

/// <summary>
/// OnAddToRender - Override the base class method to add distance check to the view camera
/// </summary>
/// <param name="pSC">The Component candidate</param>
/// <param name="pSO">The SceneObject the Component is attached to.</param>
/// <returns>True if we should include this Component.</returns>
bool LoDDepthRenderPass::OnAddToRender(Component* pSC, SceneObject* pSO)
{
	float dist2 = 0;

	//If this Component do not cast shadow to other objects in the scene, no need to redner in depth render pass
	if (pSC->castShadow == Component::eShadowCastingType::NoShadow)
		return false;

	//Check if the object is within certain distance to the view camera
	SceneActor* pActor = dynamic_cast<SceneActor*>(pSO);
	if (pActor != nullptr)
	{
		// Only render shadow for certain distance of the view camera
		SceneCamera* pCam = pScene->GetMainCameraActor();
		//use bounding box distance if available, otherwise fallback to actor position distance
		if (IsBoundingBoxValid(pActor->WorldBoundingBox)) {
			dist2 = PointToBoxDistanceSqr(pCam->GetPosition(), pActor->WorldBoundingBox);
		}
		else {
			//fallback to use actor position if bounding box is not valid
			dist2 = Vector3DistanceSqr(pActor->GetWorldPosition(), pCam->GetPosition());
		}
		if (dist2 > CutOffDistance) 
			return false;
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

//End of LoDDepthRenderPass.cpp