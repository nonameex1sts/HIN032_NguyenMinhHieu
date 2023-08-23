#include "stdafx.h"
#include "GSHelp.h"

GSHelp::GSHelp()
{
	GameStateBase::GameStateBase(StateType::STATE_HELP);
	Init();
}

GSHelp::~GSHelp()
{
	Exit();
}

void GSHelp::Init()
{
	// NOTE: read camera
	FILE* filePointer = fopen("../TrainingFramework/GSHelp.txt", "r");
	float fovY, nearPlane, farPlane, speed;
	fscanf(filePointer, "#CAMERA\n");
	fscanf(filePointer, "NEAR %f\n", &nearPlane);
	fscanf(filePointer, "FAR %f\n", &farPlane);
	fscanf(filePointer, "FOV %f\n", &fovY);
	fscanf(filePointer, "SPEED %f\n", &speed);
	pCamera = new Camera(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), fovY, nearPlane, farPlane, speed);

	int id, modelId, textureId, shaderId, buttonType, isActive;
	Vector3 position, rotation, scale;
	// NOTE: read buttons
	fscanf(filePointer, "#Buttons: %d\n", &inumButtons);
	pButtons = new Button * [inumButtons];
	for (int i = 0; i < inumButtons; i++)
	{
		fscanf(filePointer, "ID %d\n", &id);
		fscanf(filePointer, "MODEL %d\n", &modelId);
		fscanf(filePointer, "TEXTURE %d\n", &textureId);
		fscanf(filePointer, "SHADER %d\n", &shaderId);
		fscanf(filePointer, "POSITION %f, %f, %f\n", &position.x, &position.y, &position.z);
		fscanf(filePointer, "ROTATION %f, %f, %f\n", &rotation.x, &rotation.y, &rotation.z);
		fscanf(filePointer, "SCALE %f, %f, %f\n", &scale.x, &scale.y, &scale.z);
		fscanf(filePointer, "TYPE %d\n", &buttonType);
		fscanf(filePointer, "ACTIVE %d\n", &isActive);
		pButtons[i] = new Button(ResourceManager::GetInstance()->GetModelById(modelId), ResourceManager::GetInstance()->GetTextureById(textureId), pCamera,
			ResourceManager::GetInstance()->GetShaderById(shaderId), position, rotation, scale, buttonType, isActive);
	}

	fscanf(filePointer, "#Pictures: %d\n", &inumPics);
	pPictures = new Picture * [inumPics];
	for (int i = 0; i < inumPics; i++)
	{
		fscanf(filePointer, "ID %d\n", &id);
		fscanf(filePointer, "MODEL %d\n", &modelId);
		fscanf(filePointer, "TEXTURE %d\n", &textureId);
		fscanf(filePointer, "SHADER %d\n", &shaderId);
		fscanf(filePointer, "POSITION %f, %f, %f\n", &position.x, &position.y, &position.z);
		fscanf(filePointer, "ROTATION %f, %f, %f\n", &rotation.x, &rotation.y, &rotation.z);
		fscanf(filePointer, "SCALE %f, %f, %f\n", &scale.x, &scale.y, &scale.z);
		fscanf(filePointer, "ACTIVE %d\n", &isActive);
		pPictures[i] = new Picture(ResourceManager::GetInstance()->GetModelById(modelId), ResourceManager::GetInstance()->GetTextureById(textureId), pCamera,
			ResourceManager::GetInstance()->GetShaderById(shaderId), position, rotation, scale, isActive);
	}

	fclose(filePointer);
	printf("GSHelp init\n");
}

void GSHelp::Exit()
{
	// NOTE: Delete camera
	delete pCamera;
	// NOTE: Delete button
	for (int i = 0; i < inumButtons; i++) {
		delete pButtons[i];
	}
	delete pButtons;
	for (int i = 0; i < inumPics; i++) {
		delete pPictures[i];
	}
	delete pPictures;
	printf("GSHelp exit\n");
}

void GSHelp::Pause()
{
	// NOTE: blank
}

void GSHelp::Resume()
{
	// NOTE: blank
}

void GSHelp::Update(GLfloat deltatime)
{
	//NOTE: check current iCurrentImage to show that image
}

void GSHelp::Key(int iKeyPressed)
{
	// NOTE: blank
}

void GSHelp::MouseClick(int x, int y, bool isPressed)
{
	// NOTE: check back button if it got click or not
	if (isPressed)
	{
		for (int i = 0; i < inumButtons; i++)
		{
			pButtons[i]->MouseClick(x, y, &iCurrentPic, iHelpPic);
		}
	}
}

void GSHelp::MouseMove(int x, int y)
{
	// NOTE: blank
}

void GSHelp::Draw()
{
	printf("%d\n", iCurrentPic);
	for (int i = 0; i < inumPics; i++)
	{
		if (pPictures[i]->getActive())
		{
			pPictures[i]->Draw();
		}
	}
	for (int i = 0; i < inumButtons; i++)
	{
		if (pButtons[i]->getActive())
		{
			pButtons[i]->Draw();
		}
	}
}