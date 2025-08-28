#pragma once

#include "raylib.h"

class SceneActor;
class SceneObject;

typedef struct _RenderHints {

	// If not null, this shader will be used to render this component
	Shader* pOverrideShader = nullptr;  

	Camera3D* pOverrideCamera = nullptr; // If not null, this camera will be used to render this component

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
	}
	virtual ~Component() {};
	eComponentType Type;

	virtual void Update(float ElapsedSeconds) {}
	virtual void Draw(RenderHints *pRH = nullptr) { }

	//virtual void SetShader(Shader* pNewShader, int idx) {};

protected:
	friend class SceneObject;
	SceneObject* _SceneObject;

	friend class SceneActor;
	SceneActor* _SceneActor;
};