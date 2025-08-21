#include "Knight.h"
#include "ShadowSceneLight.h"

ShadowSceneLight::ShadowSceneLight(Scene* Scene, const char* Name) : SceneActor(Scene, Name)
{
	SetLight(Vector3{ 1,-0.5f,-1 }, WHITE);
}

void ShadowSceneLight::SetLight(Vector3 dir, Color col)
{
	lightDir = Vector3Normalize(dir);
	lightColor = col;

}

bool ShadowSceneLight::Update(float ElapsedTime)
{
	SceneActor::Update(ElapsedTime);

	return true;
}

