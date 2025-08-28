#pragma once

#include "Defs.h" 
#include "Scene.h"
#include "SceneObject.h"
#include "SceneActor.h"
#include "SceneCamera.h"
#include "PerspectiveCamera.h"
#include "OrthogonalCamera.h"
#include "FlyThroughCamera.h"
#include "Component.h"
#include "ModelComponent.h"
#include "CubeComponent.h"
#include "SphereComponent.h"
#include "PlaneComponent.h"
#include "CylinderComponent.h"
#include "ConeComponent.h"
#include "SceneRenderPass.h"
#include "ForwardRenderPass.h"
#include "LitDepthRenderPass.h"
#include "LitShadowRenderPass.h"
#include "KnightUtils.h"

struct KnightConfig
{
	bool ShowFPS = false;
	bool ShowDebugInfo = false;
	bool EnableDefaultLight = true;
	bool EnableDefaultRenderPasses = true;
};

struct ComparePriorityDescending
{
	bool operator()(SceneRenderPass* lhs, SceneRenderPass* rhs) const
	{
		return lhs->_Priority > rhs->_Priority;
	}
};

class Knight
{
public:
	Knight();
	~Knight();

public:
	virtual void Start();
	void GameLoop();
	virtual void EndGame();
	void ExitGameLoop();

	void DrawText(const char* text, int x, int y, int size, const Color& color = Color{ 255,255,255,255 });
	void SaveScreenshot(const char* fileName);

	//bool ShowFPS = false;
	KnightConfig Config;

	static Knight* Instance;

protected:
	virtual void Update(float ElapsedSeconds);
	virtual void DrawOffscreen();
	virtual void DrawFrame();
	virtual void DrawGUI();

	void DrawFPS(int x, int y);


protected:
	Scene* _Scene;
	Font _Font;
	bool _shouldExitGameLoop;

	// Render passes registered for offscreen rendering
	multiset<SceneRenderPass*, ComparePriorityDescending> _OffScreenPasses;

	// Render passes registered for frame rendering
	multiset<SceneRenderPass*, ComparePriorityDescending> _RenderPasses;

	// Default resources, such as shaders, textures, etc.
	virtual void OnCreateDefaultResources();
	// Called after the default resources are created, can be used to ensure that all default resources are ready to use.
	virtual void AfterCreateDefaultResources();
	// Called when the default resources are released, can be used to clean up any resources that depend on them.
	virtual void OnReleaseDefaultResources();
	// Called after the default resources are released, can be used to ensure that all resources are cleaned up properly.
	virtual void AfterReleaseDefaultResources();
	// Called to configure the Knight application, such as title, rendering settings.
	virtual void OnConfigKnightApp();
};

