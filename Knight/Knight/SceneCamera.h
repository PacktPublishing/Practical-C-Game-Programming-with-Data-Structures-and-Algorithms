#pragma once

#include "SceneObject.h"
#include "raylib.h"
#include "raymath.h"



// Helper structure for a plane (Ax + By + Cz + D = 0)
// Normal points inwards for the frustum
typedef struct _FrustumPlane {
	Vector3 normal;
	float d;
} FrustumPlane;

class SceneCamera : public SceneObject
{
public:

	SceneCamera(Scene* Scene, const char* Name = nullptr, bool IsMainCamera = true);
	virtual ~SceneCamera();

	CameraMode cameraMode;
	bool ShowCursor;

	inline void SetPosition(Vector3 pos) { _Camera.position = pos; }
	inline void SetFovY(float fy) { _Camera.fovy = fy; }
	inline void SetLookAtPosition(Vector3 tp) { _Camera.target = tp; }
	inline void SetProjectionMode(int mode) { _Camera.projection = mode; }
	inline void SetUpward(Vector3 up) { _Camera.up = up; }

	inline const Vector3 GetPosition() { return _Camera.position; }
	inline const Vector3 GetLookAtPosition() { return _Camera.target; }
	inline const Vector3 GetUp() { return _Camera.up; }
	inline const float GetFov() { return _Camera.fovy; }
	inline const int GetProjectionMode() { return _Camera.projection; }

	bool Update(float ElapsedSeconds) override;

	void ExtractFrustumPlanes(FrustumPlane frustumPlanes[6]);
	bool IsBoundingBoxInFrustum(BoundingBox box, const FrustumPlane frustumPlanes[6]);

	Camera3D* GetCamera3D();

	bool _ProcessInput = true; // Process input events

protected:
	friend class Knight;
	Camera3D _Camera;
};

