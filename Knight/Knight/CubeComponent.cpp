#include "CubeComponent.h"
#include "SceneActor.h"

CubeComponent::CubeComponent()
	: Size(Vector3 {1, 1, 1})
	, _Size(Vector3 { 1, 1, 1 })
{
	_Mesh = GenMeshCube(_Size.x, _Size.y, _Size.z);
	_Material = LoadMaterialDefault();
	_Material.maps[MATERIAL_MAP_DIFFUSE].color = LIGHTGRAY;
}

CubeComponent::~CubeComponent()
{
}

void CubeComponent::Update(float ElapsedSeconds)
{
	Component::Update(ElapsedSeconds);
	if (_Size.x != Size.x || _Size.y != Size.y || _Size.z != Size.z)
	{
		_Size = Size;
		_Mesh = GenMeshCube(_Size.x, _Size.y, _Size.z);
	}
}


void CubeComponent::Draw(RenderHints* pRH)
{
	if (pRH != nullptr && pRH->pOverrideShader != nullptr) {
		Shader old = _Material.shader;
		_Material.shader = *pRH->pOverrideShader;
		DrawMesh(_Mesh, _Material, *_SceneActor->GetWorldTransformMatrix());
		_Material.shader = old;
	}
	else
		DrawMesh(_Mesh, _Material, *_SceneActor->GetWorldTransformMatrix());
}

void CubeComponent::SetColor(Color Color)
{
	_Material.maps[MATERIAL_MAP_DIFFUSE].color = Color;
}

Color CubeComponent::GetColor()
{ 
	return _Material.maps[MATERIAL_MAP_DIFFUSE].color;
}
