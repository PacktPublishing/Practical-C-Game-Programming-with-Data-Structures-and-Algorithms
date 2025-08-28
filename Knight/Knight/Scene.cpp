
#include "Scene.h"
#include "SceneObject.h"
#include "SceneActor.h"
#include "SceneRenderPass.h"
#include "SceneCamera.h"

#include "rlgl.h"

#include <cstring>
#include <algorithm>
#include <set>

Scene::Scene()
	: SceneRoot(nullptr)
	, _MainCamera(nullptr)
{
	SceneRoot = new SceneObject(this);
	SceneRoot->SetName(SCENE_ROOT_NAME);
	_CurrentRenderPass = nullptr;
}

Scene::~Scene()
{
	if (SceneRoot)
	{
		delete SceneRoot;
		SceneRoot = nullptr;
	}
}

SceneObject* Scene::FindObjectByName(const char* Name, bool CaseSensitive)
{
	return FindObjectByName(SceneRoot, Name, CaseSensitive);
}

SceneObject* Scene::FindObjectByName(SceneObject* RootObject, const char* Name, bool CaseSensitive)
{
	if (RootObject == nullptr)
		RootObject = SceneRoot;

	int cmp = CaseSensitive ? strcmp(RootObject->GetName(), Name) : _strcmpi(RootObject->GetName(), Name);
	if (cmp == 0)
	{
		return RootObject;
	}

	for (int i = 0; i < RootObject->_Children.size(); i++)
	{
		SceneObject* child = RootObject->_Children[i];
		if (child == nullptr)
			continue;

		auto foundObj = FindObjectByName(child, Name, CaseSensitive);
		// If we found the object in the child, return it
		if (foundObj != nullptr)
		{
			return foundObj;
		}		
	}

	return nullptr;
}

bool Scene::AddSceneObject(SceneObject* Object, SceneObject* ParentObject)
{
	if (Object == nullptr)
	{
		return false;
	}
	if (ParentObject == nullptr)
	{
		Object->SetParent(SceneRoot);
	}
	else
	{
		Object->SetParent(ParentObject);
	}
	return true;
}

/// <summary>
/// DestroySceneObject - recursively destroys a SceneObject and all its children.
/// </summary>
/// <param name="Object">The object to be destroyed</param>
void Scene::DestroySceneObject(SceneObject* Object)
{
	if (Object == nullptr)
	{
		return;
	}

	if (Object->_Children.size() > 0)
	{
		// Recursively destroy all children first
		for (auto& child : Object->_Children)
		{
			if (child != nullptr)
			{
				DestroySceneObject(child);
			}
		}
		//Object->_Children.clear(); // Clear the children list after destroying them
	}

	if (Object->Parent != nullptr)
	{
		// Remove from parent's children list
		auto it = std::find(Object->Parent->_Children.begin(), Object->Parent->_Children.end(), Object);
		if (it != Object->Parent->_Children.end())
		{
			Object->Parent->_Children.erase(it);
		}
	}

	delete Object;
}

void Scene::DestroySceneObjectByName(const char* Name, bool CaseSensitive)
{
	SceneObject* object = FindObjectByName(Name, CaseSensitive);
	if (object)
	{
		DestroySceneObject(object);
	}
}


void Scene::Update(float ElapsedSeconds)
{
	if (SceneRoot)
	{
		SceneRoot->Update(ElapsedSeconds);
	}
}

void Scene::DrawFrame(SceneCamera *pCamOverride)
{
	ClearRenderQueue();

	if (_CurrentRenderPass != NULL)
	{
		_CurrentRenderPass->BeginScene(pCamOverride);
		_CurrentRenderPass->Render();
		_CurrentRenderPass->EndScene();
	}
}

SceneCamera* Scene::GetMainCameraActor()
{
	return _MainCamera;
}

void Scene::ClearRenderQueue()
{
	_RenderQueue.Background.clear();
	_RenderQueue.Geometry.clear();
	_RenderQueue.AlphaBlending.clear();
	_RenderQueue.Overlay.clear();
}

int Scene::EnabledLights()
{
	int count = 0;
	for (auto& light : Lights)
	{
		if (light.enabled)
		{
			count++;
		}
	}
	return count;
}
