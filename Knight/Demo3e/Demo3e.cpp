#include "Knight.h"
#include "Demo3e.h"
#include "MazeGenerator.h"

#define MAZE_ROWS 9
#define MAZE_COLS 9
#define MAZE_CELL_SIZE 10

int main(int argc, char* argv[])
{
	Demo3e* demo3e = new Demo3e();
	demo3e->Start();
	demo3e->GameLoop();
	delete demo3e;
	return 0;
}

void Demo3e::Start()
{
	__super::Start();

	Config.ShowFPS = true;

	_camera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	_camera->SetPosition(Vector3{0, 120, 30});
	_camera->CameraMode = CameraMode::CAMERA_CUSTOM;
	_camera->ShowCursor = false;
	_camera->SetLookAtPosition(Vector3{ 0, 0, 0 });

	_mazeGenerator = new MazeGenerator(MAZE_ROWS, MAZE_COLS);
	_mazeGenerator->GenerateMaze();
}

void Demo3e::EndGame()
{
	if (_mazeGenerator != nullptr)
	{
		delete 	_mazeGenerator;
		_mazeGenerator = nullptr;
	}
	__super::EndGame();
}

void Demo3e::Update(float ElapsedSeconds)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		_mazeGenerator->GenerateMaze();
	}
	__super::Update(ElapsedSeconds);
}

void Demo3e::DrawFrame()
{
	int** maze = _mazeGenerator->GetMaze();

	//Draw the maze
	Vector3 startPos{ 
		- MAZE_CELL_SIZE * MAZE_COLS / 2, 
		0.0f, 
		- MAZE_CELL_SIZE * MAZE_ROWS / 2
	};
	for (int row = 0; row < MAZE_ROWS; ++row)
	{
		for (int col = 0; col < MAZE_COLS; ++col)
		{
			//Draw blocked cells	
			if (maze[row][col] == MazeGenerator::MazeCellFlag::FlagWall)
			{
				Vector3 pos{
					startPos.x + col * MAZE_CELL_SIZE,
					0.0f,
					startPos.z + row * MAZE_CELL_SIZE
				};
				DrawCube(pos, MAZE_CELL_SIZE, MAZE_CELL_SIZE, MAZE_CELL_SIZE, RED);
			}
			DrawMazeBoundaryWalls(startPos, row, col);
		}
	}
}

void Demo3e::DrawGUI()
{
	DrawText("Left Click: Generate a new maze.", 100, 50, 30, GREEN);
}

void Demo3e::DrawMazeBoundaryWalls(Vector3 startPos, int row, int col)
{
	//Draw left side wall
	if (col == 0)
	{
		Vector3 pos{
			startPos.x - MAZE_CELL_SIZE,
			0.0f,
			startPos.z + row * MAZE_CELL_SIZE
		};
		DrawCube(pos, MAZE_CELL_SIZE, MAZE_CELL_SIZE, MAZE_CELL_SIZE, BLUE);
		if (row == 0)
		{
			pos.z -= MAZE_CELL_SIZE;
			DrawCube(pos, MAZE_CELL_SIZE, MAZE_CELL_SIZE, MAZE_CELL_SIZE, BLUE);
		}
		else if (row == MAZE_ROWS - 1)
		{
			pos.z += MAZE_CELL_SIZE;
			DrawCube(pos, MAZE_CELL_SIZE, MAZE_CELL_SIZE, MAZE_CELL_SIZE, BLUE);
		}
	}
	//Draw right side wall
	if (col == MAZE_COLS - 1)
	{
		Vector3 pos{
			startPos.x + (col + 1) * MAZE_CELL_SIZE,
			0.0f,
			startPos.z + row * MAZE_CELL_SIZE
		};
		DrawCube(pos, MAZE_CELL_SIZE, MAZE_CELL_SIZE, MAZE_CELL_SIZE, BLUE);
		if (row == 0)
		{
			pos.z -= MAZE_CELL_SIZE;
			DrawCube(pos, MAZE_CELL_SIZE, MAZE_CELL_SIZE, MAZE_CELL_SIZE, BLUE);
		}
		else if (row == MAZE_ROWS - 1)
		{
			pos.z += MAZE_CELL_SIZE;
			DrawCube(pos, MAZE_CELL_SIZE, MAZE_CELL_SIZE, MAZE_CELL_SIZE, BLUE);
		}
	}

	//Draw top side wall
	if (row == 0)
	{
		Vector3 pos{
			startPos.x + col * MAZE_CELL_SIZE,
			0.0f,
			startPos.z - MAZE_CELL_SIZE
		};
		DrawCube(pos, MAZE_CELL_SIZE, MAZE_CELL_SIZE, MAZE_CELL_SIZE, BLUE);

	}
	//Draw bottom side wall
	if (row == MAZE_ROWS - 1)
	{
		Vector3 pos{
			startPos.x + col * MAZE_CELL_SIZE,
			0.0f,
			startPos.z + (row + 1) * MAZE_CELL_SIZE
		};
		DrawCube(pos, MAZE_CELL_SIZE, MAZE_CELL_SIZE, MAZE_CELL_SIZE, BLUE);
	}

}