#include "ModelComponent.h"
#include "SceneActor.h"
#include "KnightUtils.h"
#include "rlgl.h"

#include <map>

ModelComponent::ModelComponent()
	: _Model({})
	, _Animations(nullptr)
	, _AnimationsCount(0)
	, _AnimationIndex(-1)	
	, _FrameDuration (0.0167f)
	, _AnimTranistionMode(eAnimTransitionMode::Immediate)
	, _TransitionDuration(0.1f)
	, _TransiteToAnimationIndex(-1)
	, _TransitionTime(0.0f)
	, _Texture2DMaps()
	, _Color(WHITE)
	, _LoadState(0)
	, _BoundingBox()
{
	Type = Component::eComponentType::Model3D;
}

ModelComponent::~ModelComponent()
{
	if (_LoadState & DiffuseMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_DIFFUSE]);
	}
	if (_LoadState & SpecularMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_SPECULAR]);
	}
	if (_LoadState & NormalMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_NORMAL]);
	}
	if (_LoadState & MetalicMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_METALNESS]);
	}	
	if (_LoadState & RoughnessMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_ROUGHNESS]);
	}	
	if (_LoadState & HeightMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_HEIGHT]);
	}	
	if (_LoadState & CubeMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_CUBEMAP]);
	}	
	if (_LoadState & EmmissionMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_EMISSION]);
	}
	if (_LoadState & OcclusionMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_OCCLUSION]);
	}

	if (_LoadState & Loaded_Animations)
	{
		UnloadModelAnimations(_Animations, _AnimationsCount);
	}
	
	if (_LoadState & Loaded_Model)
	{
		UnloadModel(_Model);
	}
}

void ModelComponent::Update(float ElapsedSeconds)
{
	__super::Update(ElapsedSeconds);

	if (_SceneActor)
	{
		_Model.transform = *(_SceneActor->GetWorldTransformMatrix());
	}

	if ((_LoadState & Loaded_Animations) && 
		_AnimationIndex >= 0 && _AnimationIndex < _AnimationsCount)
	{
		if (_AnimationMode == eAnimMode::Default)	
			//Use the Frame-by-frame (default) mode to play the animation
		{
			//Get next keyframe index
			_CurrentFrame[0] = GetNextFrame();
			//Update skeleton and the meshes based on current keyframe index 
			UpdateModelAnimation(_Model, _Animations[_AnimationIndex], _CurrentFrame[0]);
		}
		else //_AnimationMode == Linear_interpolation || _AnimationMode == Exponential_interpolation
		{
			UpdateModelAnimationWithInterpolation(ElapsedSeconds);

			if (_TransiteToAnimationIndex >= 0)
			{
				_TransitionTime += ElapsedSeconds;
				if (_TransitionTime > _TransitionDuration)
				{
					_CurrentFrame[0] = _CurrentFrame[1];
					_PrevFrame[0] = _PrevFrame[1];
					_InterpolationTime[0] = _InterpolationTime[1];
					_AnimationIndex = _TransiteToAnimationIndex;
					_TransiteToAnimationIndex = -1;
				}
			}
		}
	}
}

void ModelComponent::Draw(RenderHints* pRH)
{
	if (pRH != nullptr && pRH->pOverrideShader != nullptr) {

		Shader* pShaders = new Shader[_Model.materialCount];
		for (int i=0; i < _Model.materialCount; i++) {
			pShaders[i] = _Model.materials[i].shader;
			_Model.materials[i].shader = *pRH->pOverrideShader;
		}
		DrawModel(_Model, Vector3Zero(), 1.0f, _Color);
		for (int i=0; i < _Model.materialCount; i++) {
			_Model.materials[i].shader = pShaders[i];
		}
	}
	else
		DrawModel(_Model, Vector3Zero(), 1.0f, _Color);

	if (DrawBoundingBox)
	{
		Vector3 pos{ (_BoundingBox.max.x + _BoundingBox.min.x) * 0.5f, 
			(_BoundingBox.max.y + _BoundingBox.min.y) * 0.5f, 
			(_BoundingBox.max.z + _BoundingBox.min.z) * 0.5f };
		Vector3 size{ (_BoundingBox.max.x - _BoundingBox.min.x) * _SceneActor->Scale.x,
			(_BoundingBox.max.y - _BoundingBox.min.y) * _SceneActor->Scale.y,
			(_BoundingBox.max.z - _BoundingBox.min.z) * _SceneActor->Scale.z };
		DrawCubeWires(pos, size.x, size.y, size.z, GRAY);
	}
}

void ModelComponent::LoadMaterialTextures(int idx,
	const char* DiffuseMapPath,
	const char* SpecularMapPath,
	const char* NormalMapPath ,
	const char* MetalicMapPath,
	const char* RoughnessMapPath ,
	const char* HeightMapPath ,
	const char* CubeMapPath ,
	const char* EmissionMapPath ,
	const char* OcclusionMapPath)
{
	if (DiffuseMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_DIFFUSE] = LoadTexture(DiffuseMapPath);
		_LoadState |= DiffuseMap;
		_Model.materials[idx].maps[MATERIAL_MAP_DIFFUSE].texture = _Texture2DMaps[MATERIAL_MAP_DIFFUSE];
	}
	if (SpecularMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_SPECULAR] = LoadTexture(SpecularMapPath);
		_LoadState |= SpecularMap;
		_Model.materials[idx].maps[MATERIAL_MAP_SPECULAR].texture = _Texture2DMaps[MATERIAL_MAP_SPECULAR];
	}
	if (NormalMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_NORMAL] = LoadTexture(NormalMapPath);
		_LoadState |= NormalMap;
		_Model.materials[idx].maps[MATERIAL_MAP_NORMAL].texture = _Texture2DMaps[MATERIAL_MAP_NORMAL];
	}
	if (MetalicMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_METALNESS] = LoadTexture(MetalicMapPath);
		_LoadState |= MetalicMap;
		_Model.materials[idx].maps[MATERIAL_MAP_SPECULAR].texture = _Texture2DMaps[MATERIAL_MAP_METALNESS];
	}
	if (RoughnessMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_ROUGHNESS] = LoadTexture(RoughnessMapPath);
		_LoadState |= RoughnessMap;
		_Model.materials[idx].maps[MATERIAL_MAP_SPECULAR].texture = _Texture2DMaps[MATERIAL_MAP_ROUGHNESS];
	}
	if (HeightMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_HEIGHT] = LoadTexture(HeightMapPath);
		_LoadState |= HeightMap;
		_Model.materials[idx].maps[MATERIAL_MAP_SPECULAR].texture = _Texture2DMaps[MATERIAL_MAP_HEIGHT];
	}
}

void ModelComponent::LoadFromMesh(Mesh mesh,
	const char* DiffuseMapPath,
	const char* SpecularMapPath,
	const char* NormalMapPath,
	const char* MetalicMapPath,
	const char* RoughnessMapPath,
	const char* HeightMapPath,
	const char* CubeMapPath,
	const char* EmissionMapPath,
	const char* OcclusionMapPath,
	Color Color)
{
	_Model = LoadModelFromMesh(mesh);
	_LoadState |= Loaded_Model;
	LoadMaterialTextures(0, 
		DiffuseMapPath, 
		SpecularMapPath, 
		NormalMapPath, 
		MetalicMapPath, 
		RoughnessMapPath, 
		HeightMapPath, 
		CubeMapPath, 
		EmissionMapPath, 
		OcclusionMapPath);

	RecalculateSmoothNormals(_Model);
}

void ModelComponent::Load3DModel(const char* ModelPath,
	const char* DiffuseMapPath,
	const char* SpecularMapPath,
	const char* NormalMapPath,
	const char* MetalicMapPath,
	const char* RoughnessMapPath,
	const char* HeightMapPath,
	const char* CubeMapPath,
	const char* EmissionMapPath,
	const char* OcclusionMapPath,
	Color Color)
{
	if (!FileExists(ModelPath))
	{
		return;
	}

	_Model = LoadModel(ModelPath);
	_LoadState |= Loaded_Model;

	_Animations = LoadModelAnimations(ModelPath, &_AnimationsCount);
	if (_AnimationsCount > 0)
	{
		_LoadState |= Loaded_Animations;
		_AnimationIndex = 0;
		_CurrentFrame[0] = 0;
	}

	LoadMaterialTextures(0,
		DiffuseMapPath,
		SpecularMapPath,
		NormalMapPath,
		MetalicMapPath,
		RoughnessMapPath,
		HeightMapPath,
		CubeMapPath,
		EmissionMapPath,
		OcclusionMapPath);

	_Color = Color;
	if (_Model.meshCount > 0)
	{
		_BoundingBox = GetMeshBoundingBox(_Model.meshes[0]);

		RecalculateSmoothNormals(_Model);
	}
}

bool ModelComponent::SetAnimation(int AnimationIndex)
{
	if (AnimationIndex >= 0 && AnimationIndex < _AnimationsCount)
	{
		_AnimTranistionMode = eAnimTransitionMode::Immediate;
		_AnimationIndex = AnimationIndex;
		if (_AnimationMode == eAnimMode::Default)
		{
			_CurrentFrame[0] = 0;
		}
		else
		{
			_PrevFrame[0] = 0;
			_CurrentFrame[0] = _Animations[_AnimationIndex].frameCount > 1 ? 1 : 0;
		}
		return true;
	}
	return false;
}

bool ModelComponent::TransitionAnimation(int AnimationIndex, float TransitionSeconds)
{
	if (_AnimTranistionMode == Immediate)
	{
		SetAnimation(AnimationIndex);  //Transition immediately
		return true;
	}

	if (AnimationIndex >= 0 && AnimationIndex < _AnimationsCount)
	{
		//Initialize transition variables
		_TransiteToAnimationIndex = AnimationIndex;
		_TransitionDuration = TransitionSeconds;
		_TransitionTime = 0.0f;

		//Start playing the next animation on channel 1
		_PrevFrame[1] = 0;
		_CurrentFrame[1] = _Animations[_TransiteToAnimationIndex].frameCount > 1 ? 1 : 0;
		_InterpolationTime[1] = 0.0f;
		return true;
	}
	return false;
}

void ModelComponent::SetTransitionMode(eAnimTransitionMode TransitionMode)
{
	_AnimTranistionMode = TransitionMode;
}

ModelComponent::eAnimTransitionMode ModelComponent::GetTransitionMode()
{
	return _AnimTranistionMode;
}


void ModelComponent::SetAnimationMode(eAnimMode AnimationMode)
{
	_AnimationMode = AnimationMode;
	if (_AnimationMode != eAnimMode::Default)
	{
		_PrevFrame[0] = _CurrentFrame[0];
		_CurrentFrame[0] = GetNextFrame();
		_InterpolationTime[0] = 0.0f;
	}
}

void ModelComponent::SetFrameDuration(float Seconds)
{
	_FrameDuration = Seconds;
}

ModelComponent::eAnimMode ModelComponent::GetAnimationMode()
{
	return _AnimationMode;
}

int ModelComponent::GetAnimationIndex()
{
	return _AnimationIndex;
}

BoundingBox ModelComponent::GetBoundingBox()
{
	return _BoundingBox;
}

int ModelComponent::GetNextFrame(float InterpolationTime, int Channel)
{
	if (InterpolationTime == 0.0f)
	{
		return (_CurrentFrame[Channel] + 1) % _Animations[_AnimationIndex].frameCount;
	}
	else
	{
		float times = InterpolationTime / _FrameDuration;
		int step = (int)times + 1;
		int animIndex = Channel == 0 ? _AnimationIndex : _TransiteToAnimationIndex;
		return (_CurrentFrame[Channel] + step) % _Animations[animIndex].frameCount;
	}
}

void ModelComponent::UpdateModelAnimationWithInterpolation(float ElapsedSeconds)
{
	int channelCount = _TransiteToAnimationIndex < 0 ? 1 : 2;

	InterpolateAnimation(channelCount);

	for (int channel = 0; channel < channelCount; ++channel)
	{
		_InterpolationTime[channel] += ElapsedSeconds;
		if (_InterpolationTime[channel] > _FrameDuration)
		{
			_PrevFrame[channel] = _CurrentFrame[channel];
			_CurrentFrame[channel] = GetNextFrame(_InterpolationTime[channel], channel);
			_InterpolationTime[channel] = 0.0f;
		}
	}
}

void ModelComponent::InterpolateAnimation(int ChannelCount)
{
	float t;
	ModelAnimation anim;

	if (ChannelCount == 1)
	{
		anim = _Animations[_AnimationIndex];

		if (anim.frameCount <= 0 || anim.bones == nullptr || anim.framePoses == nullptr)
		{
			return;
		}
	}

	for (int m = 0; m < _Model.meshCount; m++)
	{
		Mesh mesh = _Model.meshes[m];

		if (mesh.boneIds == nullptr || mesh.boneWeights == nullptr)
		{
			continue;		//Mesh m has no connection to bones
		}

		bool updated = false;           // Flag to check when anim vertex information is updated
		Vector3 animVertex = { 0 };
		Vector3 animNormal = { 0 };

		Vector3 inTranslation = { 0 };
		Quaternion inRotation = { 0 };

		Vector3 outTranslation = { 0 };
		Quaternion outRotation = { 0 };
		Vector3 outScale = { 0 };

		int boneId = 0;
		int boneCounter = 0;
		float boneWeight = 0.0;

		const int vValues = mesh.vertexCount * 3;
		for (int vCounter = 0; vCounter < vValues; vCounter += 3)
		{
			mesh.animVertices[vCounter] = 0;
			mesh.animVertices[vCounter + 1] = 0;
			mesh.animVertices[vCounter + 2] = 0;

			if (mesh.animNormals != NULL)
			{
				mesh.animNormals[vCounter] = 0;
				mesh.animNormals[vCounter + 1] = 0;
				mesh.animNormals[vCounter + 2] = 0;
			}

			// Iterates over 4 bones per vertex
			for (int j = 0; j < RAYLIB_BONES_PER_VERTEX; j++, boneCounter++)
			{
				boneWeight = mesh.boneWeights[boneCounter];

				// Early stop when no transformation will be applied
				if (boneWeight == 0.0f) continue;

				boneId = mesh.boneIds[boneCounter];
				inTranslation = _Model.bindPose[boneId].translation;
				inRotation = _Model.bindPose[boneId].rotation;

				Vector3 channelOutTranslation[2];
				Quaternion channelOutRotation[2];
				Vector3 channelOutScale[2];

				//Interpolate animations for the effective channels
				for (int channel = 0; channel < ChannelCount; ++channel)
				{
					anim = channel == 0 ? _Animations[_AnimationIndex] : _Animations[_TransiteToAnimationIndex];

					t = (float)_InterpolationTime[channel] / _FrameDuration;
					if (_AnimationMode == Exponential_interpolation)
					{
						t *= t;		//Square of t for exponential interpolation
					}
					Transform* preFrameTransform = &(anim.framePoses[_PrevFrame[channel]][boneId]);
					Transform* currentFrameTransform = &(anim.framePoses[_CurrentFrame[channel]][boneId]);
					channelOutTranslation[channel] = Vector3Lerp(preFrameTransform->translation, currentFrameTransform->translation, t);
					channelOutRotation[channel] = QuaternionLerp(preFrameTransform->rotation, currentFrameTransform->rotation, t);
					channelOutScale[channel] = Vector3Lerp(preFrameTransform->scale, currentFrameTransform->scale, t);
				}

				if (ChannelCount == 1)	//No animation transition
				{
					outTranslation = channelOutTranslation[0];
					outRotation = channelOutRotation[0];
					outScale = channelOutScale[0];
				}
				else  //ChannelCount == 2. Process the animation transition
				{
					//Calculate the normalized transitioning time
					t = Clamp(_TransitionTime / _TransitionDuration, 0.0f, 1.0f);

					//Blend the two channels' animations with t
					if (_AnimTranistionMode == Linear)	//Linear transition animation blending
					{
						outTranslation = Vector3Lerp(channelOutTranslation[0], channelOutTranslation[1], t);
						outRotation = QuaternionLerp(channelOutRotation[0], channelOutRotation[1], t);
						outScale = Vector3Lerp(channelOutScale[0], channelOutScale[1], t);
					}
					else   //Easy-in/Easy-out tranisiton animation blending
					{
						float n = 2.0f;
						float easeInOut = t < 0.5 ?  (float)(pow(2.0 * t, n) / 2.0) : (float)(1.0 - pow(-2.0 * t + 2.0, n) / 2.0);
						outTranslation = Vector3Add(channelOutTranslation[0], 
							Vector3Scale(Vector3Subtract(channelOutTranslation[1], channelOutTranslation[0]),easeInOut));
						outRotation = QuaternionAdd(channelOutRotation[0], 
							QuaternionScale(QuaternionSubtract(channelOutRotation[1], channelOutRotation[0]), easeInOut));
						outScale = Vector3Add(channelOutScale[0],
							Vector3Scale(Vector3Subtract(channelOutScale[1], channelOutScale[0]), easeInOut));
					}
				}

				// Vertices processing
				// NOTE: We use meshes.vertices (default vertex position) to calculate meshes.animVertices (animated vertex position)
				animVertex.x = mesh.vertices[vCounter];
				animVertex.y = mesh.vertices[vCounter + 1];
				animVertex.z = mesh.vertices[vCounter + 2];
				animVertex = Vector3Subtract(animVertex, inTranslation);
				animVertex = Vector3Multiply(animVertex, outScale);
				animVertex = Vector3RotateByQuaternion(animVertex, QuaternionMultiply(outRotation, QuaternionInvert(inRotation)));
				animVertex = Vector3Add(animVertex, outTranslation);
				//animVertex = Vector3Transform(animVertex, _Model.transform);

				mesh.animVertices[vCounter] += animVertex.x * boneWeight;
				mesh.animVertices[vCounter + 1] += animVertex.y * boneWeight;
				mesh.animVertices[vCounter + 2] += animVertex.z * boneWeight;
				updated = true;

				// Normals processing
				// NOTE: We use meshes.baseNormals (default normal) to calculate meshes.normals (animated normals)
				if (mesh.normals != NULL)
				{
					animNormal.x = mesh.normals[vCounter];
					animNormal.y = mesh.normals[vCounter + 1];
					animNormal.z = mesh.normals[vCounter + 2];
					animNormal = Vector3RotateByQuaternion(animNormal, QuaternionMultiply(outRotation, QuaternionInvert(inRotation)));
					mesh.animNormals[vCounter] += animNormal.x * boneWeight;
					mesh.animNormals[vCounter + 1] += animNormal.y * boneWeight;
					mesh.animNormals[vCounter + 2] += animNormal.z * boneWeight;
				}
			}
		}

		// Upload new vertex data to GPU for model drawing (Only update data when values changed)
		if (updated)
		{
			int size = mesh.vertexCount * 3 * sizeof(float);
			rlUpdateVertexBuffer(mesh.vboId[0], mesh.animVertices, size, 0); // Update vertex position
			rlUpdateVertexBuffer(mesh.vboId[2], mesh.animNormals, size, 0);  // Update vertex normals
		}
	}
}

//End of ModelComponent.cpp