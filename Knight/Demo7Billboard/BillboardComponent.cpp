#include "Knight.h"

#include "BillboardComponent.h"
#include "rlgl.h"

BillboardComponent::BillboardComponent()
{
	renderQueue = Component::eRenderQueueType::Geometry;
	blendingMode = BLEND_ALPHA;
	EnableAlphaTest = true;
}

void BillboardComponent::Draw(RenderHints* pRH)
{
	Vector3 billUp = { 0, 1, 0 };

	SceneCamera* pSC = this->_SceneActor->GetMainCamera();

	if (pSC != NULL) {

		if (AlignType == SCREEN_ALIGNED) {
			Matrix matView = rlGetMatrixModelview();
			billUp = { matView.m1, matView.m5, matView.m9 };
		}

		if (pRH != NULL && pRH->pOverrideShader != NULL) {
			int oldid = rlGetShaderIdDefault();
			rlEnableShader(pRH->pOverrideShader->id);
			DrawBillboardPro(*pSC->GetCamera3D(), texture, source, this->_SceneActor->GetWorldPosition(), billUp, size, origin, 0, tint);
			rlEnableShader(oldid);
		}
		else
			DrawBillboardPro(*pSC->GetCamera3D(), texture, source, this->_SceneActor->GetWorldPosition(), billUp, size, origin, 0, tint);
	}
}

BillboardComponent::~BillboardComponent()
{
}

//end of BillboardComponent.cpp