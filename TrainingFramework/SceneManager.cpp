#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <string>
#include <cmath>
#include "AudioManager.h"

SceneManager* SceneManager::ms_pInstance = nullptr;

SceneManager::SceneManager(int ilevelNumber) {
	ResourceManager::CreateInstance();

	//Read map file
	std::string link = "../Resources/Level/map" + std::to_string(ilevelNumber) + ".txt";
	FILE* filePointer = fopen(link.c_str(), "r");

	//Initialize camera
	float fovY, nearPlane, farPlane, speed;
	fscanf(filePointer, "#CAMERA\n");
	fscanf(filePointer, "NEAR %f\n", &nearPlane);
	fscanf(filePointer, "FAR %f\n", &farPlane);
	fscanf(filePointer, "FOV %f\n", &fovY);
	fscanf(filePointer, "SPEED %f\n", &speed);
	pCamera = new Camera(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), fovY, nearPlane, farPlane, speed);

	//Width height of the map
	fscanf(filePointer, "Width: %d\n", &iWidth);
	fscanf(filePointer, "Height: %d\n", &iHeight);
	fscanf(filePointer, "Horizontal wall: %d\n", &iNumHorizontalWall);
	fscanf(filePointer, "Player: %d\n", &iNumPlayer);
	fscanf(filePointer, "Target: %d\n", &iNumTarget);
	fscanf(filePointer, "Spawn: %d\n", &iNumSpawn);
	fscanf(filePointer, "Obstacle: %d\n", &iNumObstacle);

	//O - can move, 1 - cannot move, 2 - left wall, 3 - right wall
	p_imapType = new int* [iWidth];
	for (int i = 0; i < iWidth; i++) {
		p_imapType[i] = new int[iHeight];
	}

	//Initialize objects, walls and players
	pObstacles = new Animation * [iNumObstacle];
	pObjects = new Object * [iWidth * iHeight];
	pHorizontalWall = new Object * [iNumHorizontalWall];
	pPlayer = new Player * [iNumPlayer + iNumSpawn];
	pTargetPosition = new Vector3[iNumTarget];
	pSpawnPosition = new Vector3[iNumSpawn];

	//Index counter
	int iHorizontalWallCounter = 0, iPlayerCounter = 0, iTargetCounter = 0, iSpawnCounter = 0, iObstacleCounter = 0;

	//Rotation and scale of objects and players
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 scale = Vector3(SQUARE_SIZE, SQUARE_SIZE, 1.0f);

	/*
		1. wall
		2. blank
		6, 7, 8, 9. corner
		10. left wall
		11. right wall
		12. sleep player
		13. obsticle
		20. left wall with obsticle
		21. right wall with obsticle
		22. target
		23. spawn
		24. player with spawn
		44. wake player
		85. torch
	*/

	for (int i = 0; i < iWidth * iHeight; i++) {
		int imapType;
		fscanf(filePointer, "%d ", &imapType);

		//Calculate position of the current object
		Vector3 position = Vector3(SQUARE_SIZE/2, SQUARE_SIZE/2, 0.0f);
		position.x += SQUARE_SIZE * (i % iWidth);
		position.y += SQUARE_SIZE * (i / iWidth);

		//Transalate 1D[i] array into 2D array[width][height]: width = i % iWidth, height = i / iWidth
		if (imapType <= 1  )
		{
			p_imapType[i % iWidth][i / iWidth] = imapType;

			//Path
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(imapType), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);
		}

		if ((imapType >= 6 && imapType <= 9) || imapType == 13)
		{
			p_imapType[i % iWidth][i / iWidth] = 1;

			//Corner and horizontal wall
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(imapType), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);
		}

		if (imapType == 2) {
			p_imapType[i % iWidth][i / iWidth] = 1;

			//Out of bound texture
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(imapType), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);
		}

		if (imapType == 44)
		{
			p_imapType[i % iWidth][i / iWidth] = 0;

			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(0), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			//Player
			pPlayer[iPlayerCounter] = new Player(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(imapType), pCamera,
				ResourceManager::GetInstance()->GetShaderById(1), position, rotation, scale, 6, 1, 0, 0.1, true);

			iMainPlayer = iPlayerCounter;

			iPlayerCounter++;
		}

		if (imapType == 85)
		{
			p_imapType[i % iWidth][i / iWidth] = 1;

			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(0), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			pObstacles[iObstacleCounter] = new Animation(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(imapType), pCamera,
				ResourceManager::GetInstance()->GetShaderById(1), position, rotation, scale, 4, 1, 0, 0.1);

			iObstacleCounter++;
		}

		if (imapType == 10)
		{
			p_imapType[i % iWidth][i / iWidth] = 2;

			//Left wall
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(0), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			pHorizontalWall[iHorizontalWallCounter] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(imapType), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			iHorizontalWallCounter++;
		}

		if (imapType == 11)
		{
			p_imapType[i % iWidth][i / iWidth] = 3;

			//Right wall
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(0), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			pHorizontalWall[iHorizontalWallCounter] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(imapType), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			iHorizontalWallCounter++;
		}

		if (imapType == 12) // sleep player
		{
			p_imapType[i % iWidth][i / iWidth] = 0;

			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(0), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			//"Sleep" player
			pPlayer[iPlayerCounter] = new Player(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(87), pCamera,
				ResourceManager::GetInstance()->GetShaderById(1), position, rotation, scale, 6, 1, 0, 0.1, false); // Sleep texture here

			iPlayerCounter++;
		}

		if (imapType == 14)
		{
			p_imapType[i % iWidth][i / iWidth] = 3;
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(0), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			pHorizontalWall[iHorizontalWallCounter] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(88), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			iHorizontalWallCounter++;
		}

		if (imapType == 15)
		{
			p_imapType[i % iWidth][i / iWidth] = 2;
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(0), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			pHorizontalWall[iHorizontalWallCounter] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(89), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			iHorizontalWallCounter++;
		}

		if (imapType == 16)
		{
			p_imapType[i % iWidth][i / iWidth] = 3;
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(0), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			pHorizontalWall[iHorizontalWallCounter] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(90), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			iHorizontalWallCounter++;
		}

		if (imapType == 17)
		{
			p_imapType[i % iWidth][i / iWidth] = 2;
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(0), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			pHorizontalWall[iHorizontalWallCounter] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(91), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			iHorizontalWallCounter++;
		}

		if (imapType == 20)
		{
			p_imapType[i % iWidth][i / iWidth] = 1;

			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(13), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			pHorizontalWall[iHorizontalWallCounter] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(10), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			iHorizontalWallCounter++;

		}

		if (imapType == 21)
		{
			p_imapType[i % iWidth][i / iWidth] = 1;

			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(13), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			pHorizontalWall[iHorizontalWallCounter] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(11), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			iHorizontalWallCounter++;

		}

		if (imapType == 22)
		{
			p_imapType[i % iWidth][i / iWidth] = 0;

			pTargetPosition[iTargetCounter] = position;
			iTargetCounter++;

			//Target
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(imapType), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);
		}

		if (imapType == 23)
		{
			p_imapType[i % iWidth][i / iWidth] = 0;

			pSpawnPosition[iSpawnCounter] = position;
			iSpawnCounter++;

			//Spawn
			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(imapType), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);
		}

		if (imapType == 24)
		{
			p_imapType[i % iWidth][i / iWidth] = 0;
			pTargetPosition[iTargetCounter] = position;
			iTargetCounter++;

			pObjects[i] = new Object(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(22), pCamera,
				ResourceManager::GetInstance()->GetShaderById(0), position, rotation, scale);

			pPlayer[iPlayerCounter] = new Player(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(44), pCamera,
				ResourceManager::GetInstance()->GetShaderById(1), position, rotation, scale, 6, 1, 0, 0.1, true);

			iMainPlayer = iPlayerCounter;

			iPlayerCounter++;
		}
	}

	iNumOfMoves = 0;

	//Each move limit for each star
	iStar = new int[3];
	fscanf(filePointer, "Star: %d %d %d\n", &iStar[0], &iStar[1], &iStar[2]);

	fclose(filePointer);
}

void SceneManager::Update(float deltaTime)
{
	//Set players movement abitity
	SetPlayerMovement();

	//Active players move
	for (int i = 0; i < iNumPlayer; i++) 
	{
		pPlayer[i]->Move(deltaTime);
		pPlayer[i]->Update(deltaTime);
	}

	for (int i = 0; i < iNumObstacle; i++)
	{
		pObstacles[i]->Update(deltaTime);
	}

	//Move camera according to player if needed
	pCamera->Move(deltaTime);

	//Spawn player if step into spawn square
	if (isSpawnActive) {
		SpawnPlayer();
	}

	//Active nearby players
	ActivatePlayer();

	//Check if the game has ended
	if (!hasEnded) {
		CheckWinCondition();
	}
}

void SceneManager::Key(unsigned char keyPressed)
{
	//Player movement
	if (!hasEnded) 
	{
		//Call Key function of each player
		bool* hasMoved = new bool;
		*hasMoved = false;

		for (int i = 0; i < iNumPlayer; i++) {
			pPlayer[i]->Key(keyPressed, hasMoved);
		}

		//Camera input -> movement
		//Number of square in a row on screen: row = Globals::screenWidth / SQUARE_SIZE, number of square in a column on screen: column = Globals::screenWidth / SQUARE_SIZE
		cameraFixPosition = pPlayer[iMainPlayer]->GetCoordinate();

		//Fix main player in the middle of the screen with row / 2 and column / 2
		if ((keyPressed & (1 << 0)) && (cameraFixPosition.x > (Globals::screenWidth / SQUARE_SIZE) * 0.5f) && (iWidth - cameraFixPosition.x > (Globals::screenWidth / SQUARE_SIZE) * 0.5f - 1.0f))
		{
			pCamera->Inputs(keyPressed);
		}
		if ((keyPressed & (1 << 1)) && (cameraFixPosition.y > (Globals::screenHeight / SQUARE_SIZE) * 0.5f) && (iHeight - cameraFixPosition.y > (Globals::screenHeight / SQUARE_SIZE) * 0.5f - 1.0f))
		{
			pCamera->Inputs(keyPressed);
		}
		if ((keyPressed & (1 << 2)) && (cameraFixPosition.x >= (Globals::screenWidth / SQUARE_SIZE) * 0.5f) && (iWidth - cameraFixPosition.x > (Globals::screenWidth / SQUARE_SIZE) * 0.5f))
		{
			pCamera->Inputs(keyPressed);
		}
		if ((keyPressed & (1 << 3)) && (cameraFixPosition.y >= (Globals::screenHeight / SQUARE_SIZE) * 0.5f) && (iHeight - cameraFixPosition.y > (Globals::screenHeight / SQUARE_SIZE) * 0.5f))
		{
			pCamera->Inputs(keyPressed);
		}

		if (*hasMoved) 
		{
			iNumOfMoves++;
			iNumOfMoves = iNumOfMoves < 999 ? iNumOfMoves : 999;
		}

		delete hasMoved;
	}
}

//Set player movement ability(left, right, up, down)
void SceneManager::SetPlayerMovement()
{
	for (int i = 0; i < iNumPlayer; i++)
	{
		pPlayer[i]->SetMoveRightStatus(true);
		pPlayer[i]->SetMoveLeftStatus(true);
		pPlayer[i]->SetMoveDownStatus(true);
		pPlayer[i]->SetMoveUpStatus(true);

		if (pPlayer[i]->GetActiveStatus())
		{
			Vector3 coordinate = pPlayer[i]->GetCoordinate();

			//Check surrounding map after each move
			if ((p_imapType[(int)std::round(coordinate.x)][(int)std::round(coordinate.y)] == 3))
			{
				pPlayer[i]->SetMoveRightStatus(false);
			}
			else if (p_imapType[(int)std::round(coordinate.x) + 1][(int)std::round(coordinate.y)] == 1 || p_imapType[(int)std::round(coordinate.x) + 1][(int)std::round(coordinate.y)] == 2)
			{
				pPlayer[i]->SetMoveRightStatus(false);
			}

			if ((p_imapType[(int)std::round(coordinate.x)][(int)std::round(coordinate.y)] == 2))
			{
				pPlayer[i]->SetMoveLeftStatus(false);
			}
			else if (p_imapType[(int)std::round(coordinate.x) - 1][(int)std::round(coordinate.y)] == 1 || p_imapType[(int)std::round(coordinate.x) - 1][(int)std::round(coordinate.y)] == 3)
			{
				pPlayer[i]->SetMoveLeftStatus(false);
			}

			if (p_imapType[(int)std::round(coordinate.x)][(int)std::round(coordinate.y) + 1] == 1)
			{
				pPlayer[i]->SetMoveDownStatus(false);
			}

			if (p_imapType[(int)std::round(coordinate.x)][(int)std::round(coordinate.y) - 1] == 1)
			{
				pPlayer[i]->SetMoveUpStatus(false);
			}
		}
	}
}

//Spawn new players in all spawn square
void SceneManager::SpawnPlayer()
{
	for (int i = 0; i < iNumSpawn && isSpawnActive; i++)
	{
		for (int j = 0; j < iNumPlayer && isSpawnActive; j++)
		{
			if (pPlayer[j]->GetActiveStatus() && pPlayer[j]->CheckPosition(pSpawnPosition[i]))
			{
				//Check if any active player step into spawn square
				Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
				Vector3 scale = Vector3(SQUARE_SIZE, SQUARE_SIZE, 1.0f);
				for (int k = 0; k < iNumSpawn; k++)
				{
					pPlayer[iNumPlayer + k] = new Player(ResourceManager::GetInstance()->GetModelById(0), ResourceManager::GetInstance()->GetTextureById(44), pCamera,
						ResourceManager::GetInstance()->GetShaderById(1), pSpawnPosition[k], rotation, scale, 6, 1, 0, 0.1, true);
				}
				iNumPlayer += iNumSpawn;

				pPlayer[j]->SetDrawnStatus(false);
				//Spawn audio
				AudioManager::GetInstance()->GetAudioById(4)->PlayMusic();
				//Disable spawn square
				for (int k = 0; k < iWidth * iHeight; k++)
				{
					for (int l = 0; l < iNumSpawn; l++)
					{
						if (pObjects[k]->CheckPosition(pSpawnPosition[l]))
						{
							pObjects[k]->SetTexture(ResourceManager::GetInstance()->GetTextureById(24));
						}
					}
				}

				isSpawnActive = false;
			}
		}
	}
}

//Check nearby inactive player, activate them
void SceneManager::ActivatePlayer()
{
	for (int i = 0; i < iNumPlayer; i++)
	{
		if (pPlayer[i]->GetActiveStatus())
		{
			for (int j = 0; j < iNumPlayer; j++)
			{
				if (pPlayer[i]->CheckCloseObject(*pPlayer[j]))
				{
					pPlayer[j]->SetActiveStatus(true);
					pPlayer[j]->SetTexture(*pPlayer[i]); // When wake up
				}
			}
		}
	}
}

//If all active players are in target square and all target square has at least one active player, end the game
void SceneManager::CheckWinCondition()
{
	int iNumberTargetReached = 0, iNumberActivePlayerReached = 0, iNumberActivePlayer = 0;

	//Check if all target square has at least one active player
	for (int i = 0; i < iNumTarget; i++)
	{
		for (int j = 0; j < iNumPlayer; j++)
		{
			if (pPlayer[j]->GetActiveStatus() && pPlayer[j]->CheckPosition(pTargetPosition[i]))
			{
				iNumberTargetReached++;
				break;
			}
		}
	}

	// Check if all active players are in target square
	for (int i = 0; i < iNumPlayer; i++) {
		if (pPlayer[i]->GetActiveStatus())
		{
			iNumberActivePlayer++;
			for (int j = 0; j < iNumTarget; j++) {
				if (pPlayer[i]->CheckPosition(pTargetPosition[j]))
				{
					iNumberActivePlayerReached++;
				}
			}
		}
	}

	if (iNumberTargetReached == iNumTarget && iNumberActivePlayer == iNumberActivePlayerReached)
	{
		hasEnded = true;
		AudioManager::GetInstance()->GetAudioById(5)->PlayMusic();
	}
}

int SceneManager::GetNumberOfMoves()
{
	return iNumOfMoves;
}

int* SceneManager::GetStarIndex()
{
	return iStar;
}

bool SceneManager::GetEndedStatus()
{
	return hasEnded;
}

int SceneManager::GetNumberOfStar()
{
	for (int i = 0; i < 3; i++) 
	{
		if (iNumOfMoves > iStar[i]) 
		{
			return i;
		}
	}

	return 3;
}

//Draw floor -> players -> obsticle -> vertical walls
void SceneManager::Draw()
{
	for (int i = 0; i < iWidth * iHeight; i++)
	{
		pObjects[i]->Draw();
	}

	for (int i = 0; i < iNumPlayer; i++) 
	{
		if (pPlayer[i]->GetDrawnStatus()) 
		{
			pPlayer[i]->Draw();
		}
	}

	for (int i = 0; i < iNumObstacle; i++)
	{
		pObstacles[i]->Draw();
	}

	for (int i = 0; i < iNumHorizontalWall; i++) 
	{
		pHorizontalWall[i]->Draw();
	}
}

SceneManager::~SceneManager()
{
	for (int i = 0; i < iWidth * iHeight; i++)
	{
		delete pObjects[i];
	}
	delete pObjects;

	for (int i = 0; i < iWidth; i++)
	{
		delete p_imapType[i];
	}
	delete p_imapType;

	for (int i = 0; i < iNumHorizontalWall; i++)
	{
		delete pHorizontalWall[i];
	}
	delete pHorizontalWall;
	
	for (int i = 0; i < iNumPlayer; i++) 
	{
		delete pPlayer[i];
	}
	delete pPlayer;

	for (int i = 0; i < iNumObstacle; i++)
	{
		delete pObstacles[i];
	}
	delete pObstacles;
	
	delete pTargetPosition;
	delete pSpawnPosition;
	delete iStar;
	delete pCamera;
}