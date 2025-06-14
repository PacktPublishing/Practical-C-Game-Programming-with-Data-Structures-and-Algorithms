#include "Knight.h"

#include "rlgl.h"

#include "DepthRenderPass.h"

DepthRenderPass::DepthRenderPass(ShadowSceneLight* l)
{
	pLight = l;
}

bool DepthRenderPass::Create(Scene* sc)
{
	__super::Create(sc);

	depthShader = LoadShader(NULL, "../../resources/shaders/glsl330/shadow_depth.fs");

	Hints.pOverrideShader = &depthShader;

	shadowMap = LoadShadowmapRenderTexture(SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION);

	// For the shadowmapping algorithm, we will be rendering everything from the light's point of view
	lightCam.position = Vector3Scale(pLight->lightDir, -45.0f);
	lightCam.target = Vector3Zero();

	// Use an orthographic projection for directional lights
	lightCam.projection = CAMERA_ORTHOGRAPHIC;
	lightCam.up = Vector3{ 0.0f, 1.0f, 0.0f };
	lightCam.fovy = 20.0f;

	return true;
}

void DepthRenderPass::Release()
{
	UnloadShadowmapRenderTexture(shadowMap);
	UnloadShader(depthShader);
}

void DepthRenderPass::BeginScene(SceneCamera* pOverrideCamera)
{
	pScene->_CurrentRenderPass = this;
	pActiveCamera = pScene->GetMainCameraActor();
	if (pOverrideCamera != nullptr)
		pActiveCamera = pOverrideCamera;
	ClearRenderQueue();
	BuildRenderQueue(pScene->SceneRoot);

	//Override shader 
	rlSetShader(depthShader.id, depthShader.locs);
	rlEnableShader(depthShader.id);
}

void DepthRenderPass::EndScene()
{
	pActiveCamera = nullptr;
	pScene->_CurrentRenderPass = nullptr;
}

bool DepthRenderPass::OnAddToRender(Component* pSC, SceneObject* pSO)
{
	//If this Component do not cast shadow to other objects in the scene, no need to redner in depth render pass
	if (pSC->castShadow == Component::eShadowCastingType::NoShadow)
		return false;
	return __super::OnAddToRender(pSC, pSO);
}

void DepthRenderPass::BeginShadowMap(Scene* sc, SceneCamera* pOverrideCamera)
{
	// First, render all objects into the shadowmap
	// The idea is, we record all the objects' depths (as rendered from the light source's point of view) in a buffer
	// Anything that is "visible" to the light is in light, anything that isn't is in shadow
	// We can later use the depth buffer when rendering everything from the player's point of view
	// to determine whether a given point is "visible" to the light
	BeginTextureMode(shadowMap);
	ClearBackground(WHITE);
	BeginMode3D(lightCam);
	pLight->lightView = rlGetMatrixModelview();
	pLight->lightProj = rlGetMatrixProjection();
}

void DepthRenderPass::EndShadowMap()
{
	EndMode3D();
	EndTextureMode();
}

// Unload shadowmap render texture from GPU memory (VRAM)
void DepthRenderPass::UnloadShadowmapRenderTexture(RenderTexture2D target)
{
	if (target.id > 0)
	{
		// NOTE: Depth texture/renderbuffer is automatically
		// queried and deleted before deleting framebuffer
		rlUnloadFramebuffer(target.id);
	}
}

RenderTexture2D DepthRenderPass::LoadShadowmapRenderTexture(int width, int height)
{
	RenderTexture2D target = { 0 };

	target.id = rlLoadFramebuffer(); // Load an empty framebuffer
	target.texture.width = width;
	target.texture.height = height;

	if (target.id > 0)
	{
		rlEnableFramebuffer(target.id);

		// Create depth texture
		// We don't need a color texture for the shadowmap
		target.depth.id = rlLoadTextureDepth(width, height, false);
		target.depth.width = width;
		target.depth.height = height;
		target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
		target.depth.mipmaps = 1;

		// Attach depth texture to FBO
		rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

		// Check if fbo is complete with attachments (valid)
		if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

		rlDisableFramebuffer();
	}
	else
		return { 0 };

	return target;
}

