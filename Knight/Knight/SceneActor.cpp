#include "SceneActor.h"
#include "Scene.h"
#include "KnightUtils.h"

SceneActor::SceneActor(Scene* Scene, const char* Name)
	: SceneObject(Scene, Name)
	, Position(Vector3{0, 0, 0})
	, Rotation(Vector3{ 0, 0, 0 })
	, Scale(Vector3{ 1, 1, 1 })
	, _MatTranslation(MatrixIdentity())
	, _MatRotation(MatrixIdentity())
	, _MatScale(MatrixIdentity())
	, _MatTransform(MatrixIdentity())
	, _MatWorldTransform(MatrixIdentity())
	, WorldBoundingBox(BoundingBox{ 0 })
	,_Scene(Scene)
{
}

bool SceneActor::AddComponent(Component* Component)
{
	//sanity check
	if (HasComponent(Component->Type))
	{
		//each SceneActor can only have one component of each type
		return false;
	}

	//Assign _SceneActor before calling base class AddComponent, so that component can access SceneActor in its Create() method
	Component->_SceneActor = this;
	if (__super::AddComponent(Component))
	{
		return true;
	}

	//failed to add component, reset _SceneActor pointer
	Component->_SceneActor = nullptr;
	return false;
}

bool SceneActor::Update(float ElapsedSeconds)
{
	if (!__super::Update(ElapsedSeconds))
	{
		return false;
	}
	
	_MatTranslation = MatrixTranslate(Position.x, Position.y, Position.z);
	_MatRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * Rotation.x, DEG2RAD * Rotation.y, DEG2RAD * Rotation.z });
	_MatScale = MatrixScale(Scale.x, Scale.y, Scale.z);
	_MatTransform = MatrixMultiply(MatrixMultiply(_MatScale, _MatRotation), _MatTranslation);
	_MatWorldTransform = _MatTransform;

	SceneObject* parent = Parent;
	while (parent)
	{
		SceneActor *parentActor = dynamic_cast<SceneActor*>(parent);
		if (parentActor != nullptr)
		{
			const Matrix *parentTransform = parentActor->GetWorldTransformMatrix();
			_MatWorldTransform = MatrixMultiply(_MatTransform, *parentTransform);
			break;
		}
		else
		{
			parent = parent->Parent;
		}
	}

	UpdateCachedWorldBoundingBox();

	return true;
}

const Matrix* SceneActor::GetTransformMatrix()
{
	return &_MatTransform;
}

const Matrix* SceneActor::GetRotationMatrix()
{
	return &_MatRotation;
}

const Matrix* SceneActor::GetTranslationMatrix()
{
	return &_MatTranslation;
}

const Matrix* SceneActor::GetScaleMatrix()
{
	return &_MatScale;
}

const Matrix* SceneActor::GetWorldTransformMatrix()
{
	return &_MatWorldTransform;
}

Vector3 SceneActor::GetWorldPosition()
{
	return Vector3 { _MatWorldTransform.m12, _MatWorldTransform.m13, _MatWorldTransform.m14 };
}

Quaternion SceneActor::GetWorldRotation()
{
	return QuaternionFromMatrix(_MatWorldTransform);
}

Vector3 SceneActor::GetWorldScale()
{
	return Vector3{ _MatWorldTransform.m0, _MatWorldTransform.m5, _MatWorldTransform.m10 };
}

/// <summary>
/// TranslateWS - handy function to move an actor to a specific world space position,
///              taking into account any parent transforms updates.
/// </summary>
/// <param name="x">world space coordinate x</param>
/// <param name="y">world space coordinate y</param>
/// <param name="z">world space coordinate z</param>
/// <remarks>A handy function to place a SceneActor in particular world space coordinate (x,y,z)</remarks>
void SceneActor::TranslateWS(float x, float y, float z)
{
	Vector3 targetWorldPosition = { x, y, z };
	Vector3 newLocalPosition = targetWorldPosition; // Start with the target world position

	SceneObject* currentParent = Parent;
	while (currentParent)
	{
		SceneActor* parentActor = dynamic_cast<SceneActor*>(currentParent);
		if (parentActor != nullptr)
		{
			// Get the inverse of the parent's world transform
			Matrix parentWorldTransformInverse = MatrixInvert(*parentActor->GetWorldTransformMatrix());

			// Transform the target world position by the inverse parent matrix
			// This effectively converts the world position into the parent's local space
			newLocalPosition = Vector3Transform(targetWorldPosition, parentWorldTransformInverse);
			break; // Found an actor parent, so we have the local position
		}
		else
		{
			currentParent = currentParent->Parent; // Continue up the hierarchy
		}
	}

	// Set the actor's local position to the calculated value
	Position = newLocalPosition;
	// The _MatTranslation and _MatTransform will be recalculated in the next Update call.
}

void SceneActor::UpdateCachedWorldBoundingBox()
{
	BoundingBox localBox = { 0 };
	map<Component::eComponentType, Component*>::iterator it = _Components.begin();
	if (_Components.end() == it)
	{
		//this is a SceneActor without any component, so make CachedWorldBoundingBox an empty one
		WorldBoundingBox = { 0 };
		return;
	} else 
	{
		localBox = it->second->LocalBoundingBox;
	}

	it++;

	while(it != _Components.end())
	{
		Component* comp = it->second;
		localBox = GetBoundingBoxUnion(localBox, comp->LocalBoundingBox);
		it++;
	}

	// Define the 8 corners of the local bounding box
	Vector3 corners[8] = {
		{ localBox.min.x, localBox.min.y, localBox.min.z },
		{ localBox.min.x, localBox.min.y, localBox.max.z },
		{ localBox.min.x, localBox.max.y, localBox.min.z },
		{ localBox.min.x, localBox.max.y, localBox.max.z },
		{ localBox.max.x, localBox.min.y, localBox.min.z },
		{ localBox.max.x, localBox.min.y, localBox.max.z },
		{ localBox.max.x, localBox.max.y, localBox.min.z },
		{ localBox.max.x, localBox.max.y, localBox.max.z }
	};

	// Transform all 8 corners into world space
	Vector3 transformed[8];
	for (int i = 0; i < 8; i++)
	{
		transformed[i] = Vector3Transform(corners[i], _MatWorldTransform);
	}

	// Compute new world bounding box
	WorldBoundingBox.min = transformed[0];
	WorldBoundingBox.max = transformed[0];
	for (int i = 1; i < 8; i++)
	{
		WorldBoundingBox.min.x = fminf(WorldBoundingBox.min.x, transformed[i].x);
		WorldBoundingBox.min.y = fminf(WorldBoundingBox.min.y, transformed[i].y);
		WorldBoundingBox.min.z = fminf(WorldBoundingBox.min.z, transformed[i].z);

		WorldBoundingBox.max.x = fmaxf(WorldBoundingBox.max.x, transformed[i].x);
		WorldBoundingBox.max.y = fmaxf(WorldBoundingBox.max.y, transformed[i].y);
		WorldBoundingBox.max.z = fmaxf(WorldBoundingBox.max.z, transformed[i].z);
	}
}

void SceneActor::DrawBoundingBox(Color c)
{
	Vector3 pos{(WorldBoundingBox.max.x + WorldBoundingBox.min.x) * 0.5f,
	(WorldBoundingBox.max.y + WorldBoundingBox.min.y) * 0.5f,
	(WorldBoundingBox.max.z + WorldBoundingBox.min.z) * 0.5f };
	Vector3 size{ (WorldBoundingBox.max.x - WorldBoundingBox.min.x),
	(WorldBoundingBox.max.y - WorldBoundingBox.min.y),
	(WorldBoundingBox.max.z - WorldBoundingBox.min.z)};
	DrawCubeWires(pos, size.x, size.y, size.z, c);
}

Vector3 SceneActor::GetWorldForwardVector()
{
	// Z axis of the world transform
	Vector3 forward = { -_MatWorldTransform.m8, -_MatWorldTransform.m9, -_MatWorldTransform.m10 };
	return Vector3Normalize(forward);
}



