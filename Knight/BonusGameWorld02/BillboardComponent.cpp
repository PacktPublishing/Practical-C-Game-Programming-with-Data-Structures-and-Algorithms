#include "Knight.h"

#include "BillboardComponent.h"
#include "rlgl.h"

BillboardComponent::BillboardComponent()
{
	renderQueue = Component::eRenderQueueType::Geometry;
	blendingMode = BLEND_ALPHA;
	EnableAlphaTest = true;
}

/// <summary>
/// Update - called once per frame, re-align upward vector the billboard 
///          if the AlignType is SCREEN_ALIGNED
/// </summary>
/// <param name="EllapsedTime">Seconds since last called</param>
/// <param name="pRH">The RenderHints, use to override default rendering settings</param>
/// <remarks>The billboard is always facing the main camera. You can change this behaviour if needed.</remarks>
void BillboardComponent::Update(float ElapsedTime, RenderHints* pRH)
{
	__super::Update(ElapsedTime, pRH);

	SceneCamera* pSC = this->_SceneActor->GetMainCamera();

	if (pSC != nullptr)
	{
		if (AlignType == SCREEN_ALIGNED)
		{
			Matrix matView = rlGetMatrixModelview();
			billUp = { matView.m1, matView.m5, matView.m9 };
		}

		LocalBoundingBox = { -size.x / 2, -size.y / 2, -0.1f, size.x / 2, size.y / 2, 0.1f };
	}
}

/// <summary>
/// Draw - called once per frame, draws the billboard
/// </summary>
/// <param name="pRH">The RenderHints, use to override default rendering settings</param>
void BillboardComponent::Draw(RenderHints* pRH)
{
	SceneCamera* pSC = this->_SceneActor->GetMainCamera();
	Camera3D* pCam = pSC->GetCamera3D();
	if (pRH != nullptr && pRH->pOverrideCamera)
		pCam = pRH->pOverrideCamera->GetCamera3D();

	if (pSC != nullptr) {

		BeginBlendMode(blendingMode);
		if (pRH != NULL && pRH->pOverrideShader != NULL) {
			BeginShaderMode(*pRH->pOverrideShader);
			DrawBillboardPro(*pCam, texture, source, this->_SceneActor->GetWorldPosition(), billUp, size, origin, 0, tint);
			EndShaderMode();
		}
		else 
			DrawBillboardPro(*pCam, texture, source, this->_SceneActor->GetWorldPosition(), billUp, size, origin, 0, tint);
		EndBlendMode();
	}
}

//end of BillboardComponent.cpp