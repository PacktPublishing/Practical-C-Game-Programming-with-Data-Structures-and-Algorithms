#include "SceneObject.h"
#include "Scene.h"

#include <cstring>
#include <algorithm>

SceneObject::SceneObject(Scene* Scene, const char* Name)
	: ID(0)
	, IsActive(true)
	, _Scene(Scene)
	, Parent(nullptr)
{
	if (Name)
	{
		SetName(Name);
	}
	else
	{
		SetName("");
	}

	_Components.clear();
	_Children.clear();
	_Scene->AddSceneObject(this);
}

SceneObject::~SceneObject()
{
	map<Component::eComponentType, Component*>::iterator it = _Components.begin();
	while (it != _Components.end())
	{
		delete it->second;
		++it;
	}
	_Components.clear();

	for(int i = 0; i < _Children.size(); i++)
	{
		if (_Children[i])
		{
			delete _Children[i];
			_Children[i] = nullptr;
		}
	}
}

void SceneObject::SetName(const char* Name)
{
	if (Name != nullptr)
	{
		strncpy_s(_Name, MAX_SCENE_OBJECT_NAME, Name, _TRUNCATE);
	}
}

const char* SceneObject::GetName() const
{
	return _Name;
}

void SceneObject::SetParent(SceneObject* parent)
{
	if (parent == nullptr)
	{
		return;
	}

	//Remove from current parent
	if (Parent != nullptr) 
	{
		Parent->_Children.erase(std::remove(Parent->_Children.begin(), Parent->_Children.end(), this), Parent->_Children.end());
	}

	//add into parent's children
	parent->_Children.push_back(this);
	Parent = parent;
}

Component* SceneObject::GetComponent(Component::eComponentType ComponentType)
{
	map<Component::eComponentType, Component*>::iterator it = _Components.find(ComponentType);
	if (_Components.end() != it)
	{
		return it->second;
	}

	return nullptr;
}

bool SceneObject::HasComponent(Component::eComponentType ComponentType)
{
	return (GetComponent(ComponentType) != nullptr);
}

bool SceneObject::AddComponent(Component* Component)
{
	if (HasComponent(Component->Type))
	{
		return false;
	}
	Component->_SceneObject = this;
	_Components[Component->Type] = Component;
	return true;
}

Component* SceneObject::RemoveComponent(Component::eComponentType ComponentType, bool destroy)
{
	Component* component = GetComponent(ComponentType);

	if (component)
	{
		_Components.erase(ComponentType);
		if (destroy)
		{
			delete component;
			component = nullptr;
		}
	}

	return component;
}

bool SceneObject::Update(float ElapsedSeconds)
{
	if (IsActive)
	{
		map<Component::eComponentType, Component*>::iterator it = _Components.begin();
		while (it != _Components.end())
		{
			it->second->Update(ElapsedSeconds);
			++it;
		}

		for(int i=0; i < _Children.size(); i++)
		{
			if (_Children[i])
			{
				_Children[i]->Update(ElapsedSeconds);
			}
		}
	}

	return IsActive;
}

bool SceneObject::Draw()
{
	if (IsActive)
	{
		map<Component::eComponentType, Component*>::iterator it = _Components.begin();
		while (it != _Components.end())
		{
			it->second->Draw();
			++it;
		}

		for (int i = 0; i < _Children.size(); i++)
		{
			if (_Children[i])
			{
				_Children[i]->Draw();
			}
		}
	}

	return IsActive;
}