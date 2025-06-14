#include "Knight.h"

#define RAYGUI_IMPLEMENTATION

#include "Demo4texops.h"

int main(int argc, char* argv[])
{
	Demo4TexOps* KnightDemo4TexOps = new Demo4TexOps();

	KnightDemo4TexOps->Start();
	KnightDemo4TexOps->GameLoop();

	delete KnightDemo4TexOps;
	return 0;
}

void Demo4TexOps::OnCreateDefaultResources()
{
	__super::OnCreateDefaultResources();

	UnloadFont(_Font);
	_Font = LoadFontEx("../../resources/fonts/sparky.ttf", 32, 0, 0);
	GuiSetFont(_Font);
}

void Demo4TexOps::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

	InitEntities();
}

void Demo4TexOps::Update(float ElapsedSeconds)
{
	__super::Update(ElapsedSeconds);

	if (IsKeyPressed(KEY_ENTER) == true || IsMouseButtonPressed(0)== true) {
		(*it)->Release();
		++it;
		return;
	}

	if ((*it)->isReady == true)
		(*it)->Update(ElapsedSeconds);
}

void Demo4TexOps::DrawGUI()
{
	Rectangle r = {200,800,1520,250};
	Rectangle r1 = { 200,830,1520,250};
	Rectangle r2 = { 200, 1000, 1520, 50};

	__super::DrawGUI();

	if (it == demos.end()) {
		ExitGameLoop();
		return;
	}

	if ((*it)->isReady == true) {
		(*it)->Draw2D();
	} else {
		// we need to create resources for this demo first
		DrawText("Loading demo ...", 600, 500, 50, WHITE);
		(*it)->Create();
	}

	GuiWindowBox(r, (*it)->title.c_str());
	GuiDrawText((*it)->description.c_str(), r1, TEXT_ALIGN_CENTER, BLUE);
	GuiDrawText("Press 'Enter' or click mouse left button to continue...", r2, TEXT_ALIGN_CENTER, RED);
}

void Demo4TexOps::InitEntities()
{	
	demos.push_back(new SimpleDrawTextureDemo());
	demos.push_back(new CompressTextureDemo());
	demos.push_back(new LRUTextureCacheDemo(3));
	demos.push_back(new DrawPartialRotateDemo());
	demos.push_back(new ColorBlendingDemo());
	demos.push_back(new SmoothColorBlendingDemo()); 
	demos.push_back(new SceneTransitionDemo());
	demos.push_back(new GlowDemo());
	demos.push_back(new AnimatedTexDemo());
	demos.push_back(new NPatchDemo());

	it = demos.begin();
}