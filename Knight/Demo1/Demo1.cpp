#include "Knight.h"
#include "Demo1.h"

PerspectiveCamera* camera;
SceneActor* modelActor;
SceneActor* cubeActor;
SceneActor* sphereActor;
SceneActor* cylinderActor;
SceneActor* coneActor;
SceneActor* characterActor;
float spinSpeed = 10.0f;

int main(int argc, char* argv[])
{
	Demo1* KnightDemo1 = new Demo1();

	KnightDemo1->Start();
	KnightDemo1->GameLoop();

	delete KnightDemo1;
	return 0;
}

void Demo1::Start()
{
	Knight::Start();

	Config.ShowFPS = true;

	camera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	camera->SetPosition(Vector3{60, 30, 60});
	camera->CameraMode = CameraMode::CAMERA_CUSTOM;
	camera->ShowCursor = false;
	camera->SetLookAtPosition(Vector3 { 0, 10, 0 });

	modelActor = _Scene->CreateSceneObject<SceneActor>("Castle");
	ModelComponent* modelComponent = modelActor->CreateAndAddComponent<ModelComponent>();
	modelComponent->Load3DModel((std::string(RESOURCES_DIR) + "/models/obj/castle.obj").c_str(), (std::string(RESOURCES_DIR) + "/models/obj/castle_diffuse.png").c_str());
	
	cubeActor = new SceneActor(_Scene, "Cube");
	cubeActor->SetParent(modelActor);
	cubeActor->Position = Vector3{ 40, 0, 0 };
	cubeActor->Scale = Vector3{ 1, 1, 1 };
	CubeComponent* cubeComponent = new CubeComponent();
	cubeComponent->SetColor(RED);
	cubeActor->AddComponent(cubeComponent);
	cubeActor->GetComponent<CubeComponent>()->Size = Vector3{ 10, 10, 5 };

	sphereActor = _Scene->CreateSceneObject<SceneActor>("Sphere", modelActor);
	sphereActor->Position = Vector3{ -40, 0, 0 };
	sphereActor->Scale = Vector3{ 1, 1, 1 };
	SphereComponent* sphereComponent = sphereActor->CreateAndAddComponent<SphereComponent>();
	sphereComponent->SetColor(BLUE);
	sphereComponent->Radius = 5.0f;

	cylinderActor = _Scene->CreateSceneObject<SceneActor>("Cylinder", modelActor);
	cylinderActor->Position = Vector3{ 0, 0, 40 };
	cylinderActor->Scale = Vector3{ 1, 1, 1 };
	CylinderComponent* cylinderComponent = cylinderActor->CreateAndAddComponent<CylinderComponent>();
	cylinderComponent->SetColor(GREEN);
	cylinderComponent->Radius = 5.0f;
	cylinderComponent->Height = 5.0f;

	coneActor = _Scene->CreateSceneObject<SceneActor>("Cone", modelActor);
	coneActor->Position = Vector3{ 0, 0, -40 };
	coneActor->Scale = Vector3{ 1, 1, 1 };
	ConeComponent* coneComponent = coneActor->CreateAndAddComponent<ConeComponent>();
	coneComponent->SetColor(BROWN);
	coneComponent->Radius = 5.0f;
	coneComponent->Height = 5.0f;

	SceneActor* plane = _Scene->CreateSceneObject<SceneActor>("Plane");
	plane->Position = Vector3{ 0, -5, 0 };
	plane->Scale = Vector3{ 100, 1, 100 };
	PlaneComponent* planeComponent = plane->CreateAndAddComponent<PlaneComponent>();
	planeComponent->SetColor(DARKGREEN);

	characterActor = _Scene->CreateSceneObject<SceneActor>("Character");
	characterActor->Scale = Vector3{ 3.0f, 3.0f, 3.0f };
	characterActor->Position.z = 30.0f;
	characterActor->Rotation.y = 90.0f;
	ModelComponent* animModelComponent = characterActor->CreateAndAddComponent<ModelComponent>();
	animModelComponent->Load3DModel((std::string(RESOURCES_DIR) + "/models/gltf/robot.glb").c_str());
	animModelComponent->SetAnimation(10);
	characterActor->AddComponent(animModelComponent);
}

void Demo1::EndGame()
{
	Knight::EndGame();
}

void Demo1::Update(float ElapsedSeconds)
{
	ModelComponent *characterModel = characterActor->GetComponent<ModelComponent>();
	int animIndex = characterModel->GetAnimationIndex();
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && animIndex != 6)
	{
		characterModel->SetAnimation(6);
		spinSpeed = 20.0f;
	}
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && animIndex != 10)
	{
		characterModel->SetAnimation(10);
		spinSpeed = 10.0f;
	}

	modelActor->Rotation.y -= ElapsedSeconds * spinSpeed;

	Knight::Update(ElapsedSeconds);
}

void Demo1::DrawFrame()
{
	Knight::DrawFrame();
}