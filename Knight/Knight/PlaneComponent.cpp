#include "PlaneComponent.h"
#include "SceneActor.h"

PlaneComponent::PlaneComponent()
	: Width(1.0f)
	, Height(1.0f)
	, Cols(8)
	, Rows(8)
	, _Width(1.0f)
	, _Height(1.0f)
	, _Cols(8)
	, _Rows(8)
{
	_Mesh = GenMeshPlane(_Width, _Height, _Cols, _Rows);
	_Material = LoadMaterialDefault();
	_Material.maps[MATERIAL_MAP_DIFFUSE].color = LIGHTGRAY;
}

PlaneComponent::~PlaneComponent()
{
}

void PlaneComponent::Update(float ElapsedSeconds)
{
	__super::Update(ElapsedSeconds);
	if (Width != _Width || Height != _Height || Cols != _Cols || Rows != _Rows)
	{
		_Width = Width;
		_Height = Height;
		_Cols = Cols;
		_Rows = Rows;
		_Mesh = GenMeshPlane(_Width, _Height, _Cols, _Rows);
	}
}


void PlaneComponent::Draw(RenderHints* pRH)
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

void PlaneComponent::SetColor(Color Color)
{
	_Material.maps[MATERIAL_MAP_DIFFUSE].color = Color;
}

Color PlaneComponent::GetColor()
{
	return _Material.maps[MATERIAL_MAP_DIFFUSE].color;
}