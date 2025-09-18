#pragma once

#include "raylib.h"

class SceneCamera;
class SceneActor;
class SceneObject;

typedef struct _RenderHints {

	// RenderPass 

	// If not null, this shader will be used to render this component
	Shader* pOverrideShader = nullptr;  

	// If not null, this camera will be used to render this component
	SceneCamera* pOverrideCamera = nullptr; 

	// Define the LoD level, value of zero if not used.
	unsigned levelOfDetail = 0;

} RenderHints;

class Component
{
public:
	enum eComponentType
	{
		Undefined = 0,
		Model3D,
		Cube,
		Sphere
	};

	/// <summary>
	/// shadowCastingType - each component can specify how it affect shadow rendering
	/// </summary>
	enum eShadowCastingType
	{
		NoShadow = 0,   
		Shadow,
		ShadowWithAlpha
	};

	eShadowCastingType castShadow = NoShadow;

	/// <summary>
	/// receiveShadow - each component can specify if it will receive shadow from nearby object
	/// </summary>
	bool receiveShadow = false;

	/// <summary>
	/// blendingMode - Specify the type of alpha blending when render this component
	/// </summary>
	BlendMode blendingMode = (BlendMode) - 1;

	/// <summary>
	/// LocalBoundingBox - the world space bounding box of this component, used for frustum culling
	/// </summary>
	BoundingBox LocalBoundingBox; //in local space

	enum eRenderQueueType
	{
		Background = 1,
		Geometry,
		AlphaTest,
		AlphaBlend,
		Overlay
	};

	eRenderQueueType renderQueue = Geometry;

	bool EnableAlphaTest = false; // If true, this component will use alpha test when rendering

public:
	Component() 
		: Type(eComponentType::Undefined)
		, _SceneObject(nullptr)
		, _SceneActor(nullptr)
	{ 
		LocalBoundingBox = { 0 };
	}
	virtual ~Component() {};
	eComponentType Type;

	virtual bool Create() { return true; }
	virtual void Update(float ElapsedSeconds, RenderHints* pRH = nullptr) {}
	virtual void Draw(RenderHints *pRH = nullptr) {}

	SceneActor* GetSceneActor() { return _SceneActor; }
	SceneObject* GetSceneObject() { return _SceneObject; }

protected:
	friend class SceneObject;
	SceneObject* _SceneObject;

	friend class SceneActor;
	SceneActor* _SceneActor;
};