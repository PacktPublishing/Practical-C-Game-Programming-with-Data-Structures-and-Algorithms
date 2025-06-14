#include "Knight.h"
#include "Demo4ss.h"

int main(int argc, char* argv[])
{
	Demo4ss* KnightDemo4ss = new Demo4ss();

	KnightDemo4ss->Start();
	KnightDemo4ss->GameLoop();

	delete KnightDemo4ss;
	return 0;
}

Demo4ss::Demo4ss()
{
	background = { 0 };
	midground = { 0 };
	foreground = { 0 };
	scrollingBack = 0;
	scrollingMid = 0;
	scrollingFore = 0;
}

void Demo4ss::OnCreateDefaultResources()
{
	__super::OnCreateDefaultResources();

	UnloadFont(_Font);
	_Font = LoadFontEx("../../resources/fonts/sparky.ttf", 32, 0, 0);
}

void Demo4ss::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	Config.ShowFPS = true;

	background = LoadTexture("../../resources/textures/p10.png");
	midground = LoadTexture("../../resources/textures/p11.png");
	foreground = LoadTexture("../../resources/textures/p12.png");
}

void Demo4ss::EndGame()
{
	UnloadTexture(background);  // Unload background texture
	UnloadTexture(midground);   // Unload midground texture
	UnloadTexture(foreground);  // Unload foreground texture

	__super::EndGame();
}

void Demo4ss::Update(float ElapsedSeconds)
{
	__super::Update(ElapsedSeconds);

	scrollingBack -= 0.1f;
	scrollingMid -= 0.5f;
	scrollingFore -= 1.2f;

	// NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
	if (scrollingBack <= -background.width * 2) scrollingBack = 0;
	if (scrollingMid <= -midground.width * 2) scrollingMid = 0;
	if (scrollingFore <= -foreground.width * 2) scrollingFore = 0;
}

void Demo4ss::DrawFrame()
{
	__super::DrawFrame();
}

void Demo4ss::DrawGUI()
{
	__super::DrawGUI();

	Vector2 v2;

	v2.x = scrollingBack; v2.y = 20;
	DrawTextureEx(background, v2, 0.0f, 2.0f, WHITE);
	v2.x = background.width * 2 + scrollingBack;
	DrawTextureEx(background, v2, 0.0f, 2.0f, WHITE);

	// Draw midground image twice
	v2.x = scrollingMid;
	DrawTextureEx(midground, v2, 0.0f, 2.0f, WHITE);
	v2.x = midground.width * 2 + scrollingMid;
	DrawTextureEx(midground, v2, 0.0f, 2.0f, WHITE);

	// Draw foreground image twice
	v2.x = scrollingFore; v2.y = 70;
	DrawTextureEx(foreground, v2, 0.0f, 2.0f, WHITE);
	v2.x = foreground.width * 2 + scrollingFore; 
	DrawTextureEx(foreground, v2, 0.0f, 2.0f, WHITE);

	DrawText("Parallax scrolling rendering 3 layer maps", 10, 60, 40, DARKBROWN);
}



