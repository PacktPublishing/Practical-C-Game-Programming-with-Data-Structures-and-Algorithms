#include "Knight.h"
#include "ShadowSceneLight.h"

ShadowSceneLight::ShadowSceneLight(Scene* Scene, const char* Name) : OrthogonalCamera(Scene, Name, false)
{
	_ProcessInput = false; // We don't want to move the light with input
	SetLight(Vector3{ 1,-0.5f,-1 }, WHITE);
	lightAmbient.r = lightAmbient.g = lightAmbient.b = 24;
}

void ShadowSceneLight::SetLight(Vector3 dir, Color col)
{
	lightDir = Vector3Normalize(dir);
	lightColor = col;
}

bool ShadowSceneLight::Update(float ElapsedTime)
{
	__super::Update(ElapsedTime);

	return true;
}

