#pragma once

#include "SceneObject.h"
#include "raylib.h"
#include "raymath.h" 

#include "Scene.h"
#include "SceneCamera.h" 

class SceneActor : public SceneObject
{
public:

	SceneActor(Scene* Scene, const char* Name = nullptr);

	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;

	bool AddComponent(Component* Component) override;
	bool Update(float ElapsedSeconds) override;
	
	const Matrix* GetTransformMatrix();
	const Matrix* GetRotationMatrix();
	const Matrix* GetTranslationMatrix();
	const Matrix* GetScaleMatrix();

	const Matrix *GetWorldTransformMatrix();
	Vector3 GetWorldPosition();
	Quaternion GetWorldRotation();
	Vector3 GetWorldScale();

	//get forward vector in World space
	Vector3 GetWorldForwardVector();


	inline bool IsTopLevelActor()
	{
		return ((Parent == nullptr) || (Parent == _Scene->SceneRoot));
	}

	BoundingBox WorldBoundingBox; //in world space

	void TranslateWS(float wx, float wy, float wz);

	void UpdateCachedWorldBoundingBox();

	inline SceneCamera* GetMainCamera() { return (_Scene != NULL) ? _Scene->GetMainCameraActor() : NULL; };

	float SquareDistanceToCamera = 0.0f; //cached distance to the active camera, used for sorting in render queue

	void DrawBoundingBox(Color = YELLOW);

protected:

	Matrix _MatTranslation;
	Matrix _MatRotation;
	Matrix _MatScale;
	Matrix _MatTransform;
	Matrix _MatWorldTransform;

	Scene* _Scene;
};