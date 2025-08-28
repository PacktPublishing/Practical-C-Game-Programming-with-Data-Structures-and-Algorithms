#include "TurretController.h"

extern SceneActor* Player;

#define sign(a) (((a) < 0) ? -1 : ((a) > 0))

TurretController::TurretController(Scene* Scene, SceneActor* Cannon)
	: _Scene(Scene)
	, _Cannon(Cannon)
	, _ANN(nullptr)
	, _VisionRange(_AttackRange * 2.0f)
	, _FiringDir(_CannonDir)
{
	_Fireball = _Scene->CreateSceneObject<SceneActor>("Fireball");
	_Fireball->CreateAndAddComponent<SphereComponent>();
	_Fireball->Position = _Cannon->Position;
	_Fireball->Scale = Vector3{ 0.5f, 0.5f, 0.5f };
	_Fireball->IsActive = false;
	_IsLoaded = _FireballDuration;
}

TurretController::~TurretController()
{
	if (_ANN)
	{
		delete _ANN;
	}
}

void TurretController::InitANN(int Method)
{
	srand((unsigned int)time(NULL));
	if (Method == 1)
	{
		vector<size_t> layerSizes;
		layerSizes.push_back(2);	//Input layers: 2 inputs 
		layerSizes.push_back(1);	//Hidden layer: 1 neural
		layerSizes.push_back(1);	//Output layer: 1 neural
		_ANN = new NeuralNetwork(layerSizes, 0.3f, false);
	}
	else if (Method == 2)
	{
		vector<size_t> layerSizes;
		layerSizes.push_back(2);	//Input layers: 2 inputs 
		layerSizes.push_back(3);	//Hidden layer 1: 3 neurals
		layerSizes.push_back(3);	//Hidden layer 2: 3 neurals
		layerSizes.push_back(2);	//Output layers: 2 outputs
		_ANN = new NeuralNetwork(layerSizes, 0.1f, true);
	}
}

void TurretController::Train(int Method, int SampleCount, int epochs)
{
	vector<float> inputs;
	vector<float> targets;
	vector<float> sampleData;
	vector<vector<float>> dataset;

	InitANN(Method);
	if (Method == 1)
	{
		for (int i = 0; i < SampleCount; ++i)
		{
			inputs.clear();
			targets.clear();
			sampleData.clear();

			//Generate the distance input
			int distance = rand() % (int)_VisionRange;
			float distanceInput = Clamp((float)distance / _VisionRange, 0.0f, 1.0f);
			
			//Generate the angle error input
			int angleError;
			if (i % 3 == 0)
			{
				angleError = rand() % 181;
			}
			else
			{
				int firingArc = (int)_FiringArc;
				angleError = rand() % ((int)firingArc / 2 + 1);
			}
			float angleErrorInput = Clamp((float)angleError / 180.0f, 0.0f, 1.0f);

			inputs.push_back(distanceInput);
			inputs.push_back(angleErrorInput);
			
			//Determine the output value for firing
			if (distance >= 0 && distance <= _AttackRange && angleError <= _FiringArc * 0.5f)
			{
				targets.push_back(1.0);
			}
			else
			{
				targets.push_back(0.0);
			}
			//Push the sample data into the dataset
			sampleData.clear();
			sampleData.push_back(inputs[0]);
			sampleData.push_back(inputs[1]);
			sampleData.push_back(targets[0]);
			dataset.push_back(sampleData);
		}
	}   
	else if (Method == 2)
	{
		for (int i = 0; i < SampleCount; ++i)
		{
			inputs.clear();
			targets.clear();
			sampleData.clear();

			//Generate the distance input
			int distance = rand() % (int)_VisionRange;
			float distanceInput = Clamp((float)distance / _VisionRange, 0.0f, 1.0f);

			//Generate the angle error input
			int angleError;
			if (i % 2 == 0)
			{
				angleError = 180 - rand() % 361;
			}
			else
			{
				int firingArc = (int)_FiringArc;
				angleError = firingArc / 2 - (rand() % ((int)firingArc + 1));
			}
			float angleErrorInput = Clamp((float)angleError / 180.0f, -1.0f, 1.0f);

			inputs.push_back(distanceInput);
			inputs.push_back(angleErrorInput);

			//Determine the first output value for firing
			float halfFiringArc = _FiringArc * 0.5f;
			if (distance >= 0 && distance <= _AttackRange &&
				angleError >= -halfFiringArc && angleError <= halfFiringArc)
			{
				targets.push_back(1.0);
			}
			else
			{
				targets.push_back(0.0);
			}
			
			//Determine the second output value for turning the turret
			targets.push_back((float)sign(angleError));

			//Push the sample data into the dataset
			sampleData.clear();
			sampleData.push_back(inputs[0]);
			sampleData.push_back(inputs[1]);
			sampleData.push_back(targets[0]);
			sampleData.push_back(targets[1]);
			dataset.push_back(sampleData);
		}
	}

	FILE* fp = NULL;
	fp = fopen("C:\\Logs\\TrainingLogs.txt", "w+");
	for (int epoch = 0; epoch < epochs; ++epoch)
	{
		//Train the model
		for (int i = 0; i < dataset.size(); ++i)
		{
			inputs.clear();
			targets.clear();
			inputs.push_back(dataset[i][0]);
			inputs.push_back(dataset[i][1]);
			targets.push_back(dataset[i][2]);
			if (Method == 2)
			{
				targets.push_back(dataset[i][3]);
			}
			_ANN->Train(inputs, targets);
		}

		//Evaluate the cost
		double overallCost = 0.0;
		for (int i = 0; i < dataset.size(); ++i)
		{
			inputs.clear();
			inputs.push_back(dataset[i][0]);
			inputs.push_back(dataset[i][1]);
			targets.clear();
			targets.push_back(dataset[i][2]);
			if (Method == 2)
			{
				targets.push_back(dataset[i][3]);
			}

			vector<float> outputs = _ANN->Predict(inputs);

			if (fp)
			{
				float cost = (targets[0] - outputs[0]) * (targets[0] - outputs[0]) * 0.5f;
				if (Method == 1)
				{
					fprintf(fp, "%04d-%08d: Inputs(%f,%f); Target(%f); Predict(%f); Cost(%f)\n", epoch, i,
						inputs[0], inputs[1], targets[0], outputs[0], cost);
					overallCost += cost;
				}
				else if (Method == 2)
				{
					float cost1 = (targets[1] - outputs[1]) * (targets[1] - outputs[1]) * 0.5f;
					targets.push_back(dataset[i][3]);
					fprintf(fp, "%04d-%08d: Inputs(%f,%f); Target(%f,%f); Predict(%f,%f); Cost(%f)\n", epoch, i,
						inputs[0], inputs[1], targets[0], targets[1], outputs[0], outputs[1],
						cost + cost1);
					overallCost += cost + cost1;
				}
			}
		}
		if (fp)
		{
			fprintf(fp, "%04d-OveralCost: %lf\n", epoch, overallCost);
		}
	}

	if (fp)
	{
		fclose(fp);
	}

	Trained = Method;
}

void TurretController::Update(float DeltaTime)
{
	if (Player == nullptr || Trained == 0)
	{
		return;
	}

	Vector3 vecPlayerDir = Vector3Subtract(Player->Position, _Cannon->Position);
	vecPlayerDir.y = 0.0f;
	float distance = Vector3Length(vecPlayerDir);

	float distanceInput = Clamp(distance / _VisionRange, 0.0f, 1.0f);
	float angleError, angleErrorInput;
	if (Trained == 1)
	{
		angleError = Vector3Angle(vecPlayerDir, _CannonDir) * RAD2DEG;
		angleErrorInput = angleError / 180.0f;
	}
	else
	{
		Vector3 crossProduct = Vector3Normalize(Vector3CrossProduct(_CannonDir, vecPlayerDir));
		angleError = Vector3Angle(vecPlayerDir, _CannonDir) * RAD2DEG * crossProduct.y;
		angleErrorInput = Clamp(angleError / 180.0f, -1.0f, 1.0f);
	}
	
	vector<float> inputs;
	inputs.push_back(distanceInput);
	inputs.push_back(angleErrorInput);
	vector<float> outputs = _ANN->Predict(inputs);
	
	if (Trained == 2 && (outputs[1] < -0.1f || outputs[1] > 0.1f))		//Turn to the player
	{
		_Cannon->Rotation.z += _TurnSpeed * sign(outputs[1]) * DeltaTime;
		if (_Cannon->Rotation.z > 180.0f)
		{
			_Cannon->Rotation.z -= 360.0f;
		}
		else if (_Cannon->Rotation.z < -180.0f)
		{
			_Cannon->Rotation.z += 360.0f;
		}
		_CannonDir = Vector3RotateByAxisAngle(
			Vector3{ 0, 0, -1 }, 
			Vector3{ 0, 1, 0 }, 
			DEG2RAD * _Cannon->Rotation.z);
	}

	//Fire at the player
	if (_IsLoaded == _FireballDuration && outputs[0] > 0.6f)
	{
		_Fireball->IsActive = true;
		_FiringDir = _CannonDir;
		_Fireball->Position = Vector3Add(_Cannon->Position, Vector3Scale(_FiringDir, 3.0f));
		_IsLoaded = 0.0f;
	}

	//Move the fireball
	if (_Fireball->IsActive)
	{
		_Fireball->Position = Vector3Add(_Fireball->Position, Vector3Scale(_FiringDir, _FireballSpeed * DeltaTime));
		_IsLoaded += DeltaTime;
		if (_IsLoaded >= _FireballDuration)
		{
			_IsLoaded = _FireballDuration;
			_Fireball->IsActive = false;
		}
	}
}

