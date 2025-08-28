#include "Demo5FPC.h"

int main(int argc, char* argv[])
{
	Demo5FPC* KnightDemo5FPC = new Demo5FPC();

	KnightDemo5FPC->Start();
	KnightDemo5FPC->GameLoop();

	delete KnightDemo5FPC;
	return 0;
}

void Demo5FPC::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	Config.ShowFPS = true;

	//initialize a first person camera
	PerspectiveCamera* FPSCamera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	FPSCamera->SetPosition(Vector3{0.0f, 2.0f, 4.0f});    // Camera position
	FPSCamera->SetLookAtPosition(Vector3{ 0.0f, 2.0f, 0.0f });      // Camera looking at point
	FPSCamera->SetFovY(60.0f);                                // Camera field-of-view Y	
	FPSCamera->CameraMode = CAMERA_FIRST_PERSON;   

	//create floor and walls
	SceneActor* pFloor = _Scene->CreateSceneObject<SceneActor>("Floor");
	Mesh floorMesh = GenMeshCube(32.0f, 0.1f, 32.0f);
	ModelComponent* floorComponent = pFloor->CreateAndAddComponent<ModelComponent>();
	floorComponent->LoadFromMesh(floorMesh, "../../resources/textures/p8.png");
	pFloor->AddComponent(floorComponent);

	SceneActor* pWall1 = _Scene->CreateSceneObject<SceneActor>("Wall01");
	pWall1->Position = Vector3{ 16.0f, 0.0f, 0.0f };
	pWall1->Rotation.y = 90.0f;
	Mesh wall1Mesh = GenMeshCube(32.0f, 8.0f, 1.0f);
	ModelComponent* wall1Component = pWall1->CreateAndAddComponent<ModelComponent>();
	wall1Component->LoadFromMesh(wall1Mesh, "../../resources/textures/stonewall.png");
	pWall1->AddComponent(wall1Component);

	SceneActor* pWall2 = _Scene->CreateSceneObject<SceneActor>("Wall02");
	pWall2->Position = Vector3{ -16.0f, 0.0f, 0.0f };
	pWall2->Rotation.y = 90.0f;
	Mesh wall2Mesh = GenMeshCube(32.0f, 8.0f, 1.0f);
	ModelComponent* wall2Component = pWall2->CreateAndAddComponent<ModelComponent>();
	wall2Component->LoadFromMesh(wall2Mesh, "../../resources/textures/stonewall.png");
	pWall2->AddComponent(wall2Component);

	SceneActor* pWall3 = _Scene->CreateSceneObject<SceneActor>("Wall03");
	pWall3->Position = Vector3{ 0.0f, 0.0f, 16.0f };
	pWall3->Rotation.y = 0.0f;
	Mesh wall3Mesh = GenMeshCube(32.0f, 8.0f, 1.0f);
	ModelComponent* wall3Component = pWall3->CreateAndAddComponent<ModelComponent>();
	wall3Component->LoadFromMesh(wall3Mesh, "../../resources/textures/stonewall.png");
	pWall3->AddComponent(wall3Component);

	SceneActor* pWall4 = _Scene->CreateSceneObject<SceneActor>("Wall04");
	pWall4->Position = Vector3{ 0.0f, 0.0f, -16.0f };
	pWall4->Rotation.y = 180.0f;
	Mesh wall4Mesh = GenMeshCube(32.0f, 8.0f, 1.0f);
	ModelComponent* wall4Component = pWall4->CreateAndAddComponent<ModelComponent>();
	wall4Component->LoadFromMesh(wall4Mesh, "../../resources/textures/stonewall.png");
	pWall4->AddComponent(wall4Component);

	//create some props in the scene
	SceneActor* pProp1 = _Scene->CreateSceneObject<SceneActor>("scene prop1");
	pProp1->Position = Vector3{ -12, 0, 12 };
	ModelComponent* prop1Component = pProp1->CreateAndAddComponent<ModelComponent>();
	prop1Component->Load3DModel("../../resources/models/obj/well.obj", "../../resources/models/obj/well_diffuse.png");
	pProp1->AddComponent(prop1Component);

	SceneActor* pProp2 = _Scene->CreateSceneObject<SceneActor>("scene prop2");
	pProp2->Position = Vector3{ 0, 0, -11 };
	ModelComponent* prop2Component = pProp2->CreateAndAddComponent<ModelComponent>();
	prop2Component->Load3DModel("../../resources/models/obj/market.obj", "../../resources/models/obj/market_diffuse.png");
	pProp2->AddComponent(prop2Component);

	DisableCursor();
}

//Render help text on the screen
void Demo5FPC::DrawGUI()
{
	__super::DrawGUI();
	DrawText("Rotate camera angle: move mouse.", 10, 100, 40, WHITE);
	DrawText("Move camera: Use W/S/A/D.", 10, 150, 40, WHITE);
}

// This function is called when the engine is ready to create default resources like fonts, textures, etc.
void Demo5FPC::OnCreateDefaultResources()
{
	__super::OnCreateDefaultResources();
	_Font = LoadFontEx("../../resources/fonts/sparky.ttf", 32, 0, 0);
}
