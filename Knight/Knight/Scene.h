#pragma once

#include <vector>
#include <list>

#include "SceneObject.h"
#include "RenderQueues.h"

#define NUM_MAX_LIGHTS  4

class SceneCamera;
class SceneRenderPass;

typedef struct {
	bool enabled;
	int type;
	Vector3 position;
	Vector3 target;
	Color color;
	float attn_const;
	float attn_linear;
	float attn_quad;
	bool dirty;
} LightData;

class Scene
{
public:

	Scene();
	~Scene();

	SceneObject* SceneRoot;

	template<class T>
	enable_if_t<is_base_of_v<SceneObject, T>, T*> SpawnObject(const char* Name)
	{
		T* obj = new T(this);
		obj->SetName(Name);
		obj->SetParent(SceneRoot);
		return obj;
	}

	SceneObject* FindObjectByName(const char* Name, bool CaseSensitive = false);
	SceneObject* FindObjectByName(SceneObject* RootObject, const char* Name, bool CaseSensitive = false);

	bool AddSceneObject(SceneObject* Object, SceneObject* ParentObject = nullptr);
	void DestroySceneObject(SceneObject* Object);
	void DestroySceneObjectByName(const char* Name, bool CaseSensitive = false);

	void Update(float ElapsedSeconds);
	void DrawFrame(SceneCamera* pCam = nullptr);

	SceneCamera* GetMainCameraActor();

	template<class T>
	T* CreateSceneObject(const char* ObjectName, SceneObject* Parent = nullptr)
	{
		T *sceneObject = new T(this, ObjectName);
		if (!AddSceneObject(sceneObject, Parent))
		{
			delete sceneObject;
			sceneObject = nullptr;
		}
		return sceneObject;
	}

	inline void AssignMainCamera(SceneCamera* cam) { _MainCamera = cam; }

	RenderQueues _RenderQueue;

	void ClearRenderQueue();

	SceneRenderPass* _CurrentRenderPass;

	LightData Lights[NUM_MAX_LIGHTS] = { 0 };
	float AmbientColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; //default ambient light
	float SpecularColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //default specular color
	float DefaultShineness = 16.0f;

	int EnabledLights(); // Number of enabled lights in the scene

protected:
	friend class SceneCamera;
	friend class SceneObject;
	SceneCamera* _MainCamera;	

};
