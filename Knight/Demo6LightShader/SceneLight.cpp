#include "Knight.h"

#include "SceneLight.h"

SceneLight::SceneLight(Scene* Scene, const char* Name)
	: SceneActor(Scene, Name)
{

}

SceneLight::~SceneLight()
{

}

void SceneLight::Create()
{
	shader = LoadShader((std::string(RESOURCES_DIR) + "/shaders/glsl330/light_point.vs").c_str(), (std::string(RESOURCES_DIR) + "/shaders/glsl330/light_point.fs").c_str());

	_Scene->_CurrentRenderPass->Hints.pOverrideShader = &shader;

	// Set shader uniform locations
	locLightDirection = GetShaderLocation(shader, "lightDirection");
	locLightPosition = GetShaderLocation(shader, "lightPosition");
	locLightColor = GetShaderLocation(shader, "lightColor");
	locAmbientColor = GetShaderLocation(shader, "ambientColor");

	// Set shader uniform values
	SetShaderValue(shader, locLightDirection, &lightDirection, SHADER_UNIFORM_VEC3);
	SetShaderValue(shader, locLightPosition, &Position, SHADER_UNIFORM_VEC3);
	SetShaderValue(shader, locLightColor, &lightColor, SHADER_UNIFORM_VEC3);
	SetShaderValue(shader, locAmbientColor, &ambientColor, SHADER_UNIFORM_VEC3);
}

bool SceneLight::Update(float ElapsedTime)
{
	SceneActor::Update(ElapsedTime);

	SetShaderValue(shader, locLightPosition, &Position, SHADER_UNIFORM_VEC3);	
	SetShaderValue(shader, locLightDirection, &lightDirection, SHADER_UNIFORM_VEC3);
	return true;
}


