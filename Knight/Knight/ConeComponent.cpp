#include "ConeComponent.h"
#include "SceneActor.h"

ConeComponent::ConeComponent()
	: Radius(1.0f)
	, Height(1.0f)
	, Slices(16)
	, _Radius(1.0f)
	, _Height(1.0f)
	, _Slices(16)
{
	_Mesh = GenMeshCone(_Radius, _Height, _Slices);
	_Material = LoadMaterialDefault();
	_Material.maps[MATERIAL_MAP_DIFFUSE].color = LIGHTGRAY;
}

ConeComponent::~ConeComponent()
{
}

void ConeComponent::Update(float ElapsedSeconds)
{
	__super::Update(ElapsedSeconds);
	if (Radius != _Radius || Height != _Height || Slices != _Slices)
	{
		_Radius = Radius;
		_Height = Height;
		_Slices = Slices;
		_Mesh = GenMeshCone(_Radius, _Height, _Slices);
	}
}


void ConeComponent::Draw(RenderHints* pRH)
{
	if (pRH != nullptr && pRH->pOverrideShader != nullptr) {
		Shader old = _Material.shader;
		_Material.shader = *pRH->pOverrideShader;
		DrawMesh(_Mesh, _Material, *_SceneActor->GetWorldTransformMatrix());
		_Material.shader = old;
	} else 
		DrawMesh(_Mesh, _Material, *_SceneActor->GetWorldTransformMatrix());
}

void ConeComponent::SetColor(Color Color)
{
	_Material.maps[MATERIAL_MAP_DIFFUSE].color = Color;
}

Color ConeComponent::GetColor()
{
	return _Material.maps[MATERIAL_MAP_DIFFUSE].color;
}