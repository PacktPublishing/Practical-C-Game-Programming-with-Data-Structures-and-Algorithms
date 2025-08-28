#include "Demo7QuadTreeTerrain.h"

#include "rlgl.h" // Required for immediate mode drawing (rlVertex3f, etc.)
#include <string>   // For filename manipulation
#include <raymath.h>

const std::string HEIGHTMAP_FILENAME = (std::string(RESOURCES_DIR) + "/textures/heightmap.png").c_str(); // heightmap
const std::string TERRAIN_TEXTURE_FILENAME = (std::string(RESOURCES_DIR) + "/textures/terrain_map.png").c_str(); // terrain texture

//Main entry point for the demo
int main(int argc, char* argv[])
{
    Demo7QuadTreeTerrain* KnightDemo7QuadTreeTerrain = new Demo7QuadTreeTerrain();
    KnightDemo7QuadTreeTerrain->Start();
    KnightDemo7QuadTreeTerrain->GameLoop();
    delete KnightDemo7QuadTreeTerrain;
    return 0;
}

//Initialize Knight Engine with a default scene and camera
void Demo7QuadTreeTerrain::Start()
{
    Knight::Start();

    pTerrain = _Scene->CreateSceneObject<SceneActor>("Terrain");
    pTerrain->Position = Vector3{ 0.0f, 0.0f, 0.0f };           // Define model position
    pTerrain->Scale = Vector3{ 1,1,1 };
    pQuadTreeTerrain = pTerrain->CreateAndAddComponent<QuadTreeTerrainComponent>();
    pQuadTreeTerrain->CreateFromFile(Vector3{ 32, 8, 32 }, Vector2{ 8.0f, 8.0f }, HEIGHTMAP_FILENAME.c_str(), TERRAIN_TEXTURE_FILENAME.c_str());

    pMainCamera = _Scene->CreateSceneObject<FlyThroughCamera>("Main Camera");
    pMainCamera->SetUp(pTerrain->Position, 30, 20, 20, 45, CAMERA_PERSPECTIVE);
}

// Draw the GUI elements for the demo
void Demo7QuadTreeTerrain::DrawGUI()
{
    // Draw UI
    DrawText("Press B to toggle Bounding Boxes:", 10, 40, 50, WHITE);
    DrawText("Use Mouse + arrow keys to move camera", 10, 100, 50, WHITE);
    DrawText(TextFormat("Triangle count = %d", pQuadTreeTerrain->NumTriangles), 10, 160, 50, WHITE);
    DrawText(TextFormat("LOD Factor: %.1f", pQuadTreeTerrain->LevelOfDetailDistance), 10, 220, 50, WHITE);
}

//Create default resources for the demo
void Demo7QuadTreeTerrain::OnCreateDefaultResources()
{
    Knight::OnCreateDefaultResources();
    _Font = LoadFontEx((std::string(RESOURCES_DIR) + "/fonts/sparky.ttf").c_str(), 32, 0, 0);
}

void Demo7QuadTreeTerrain::OnConfigKnightApp()
{
	Config.EnableDefaultLight = true; // Enable default light
}

