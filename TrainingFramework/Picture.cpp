#include "stdafx.h"
#include "Picture.h"
#include "GameStateMachine.h"

Picture::Picture() {}

Picture::Picture(Model* pModel, Texture* pTexture, Camera* pCamera, Shaders* pShader, Vector3 position, Vector3 rotation, Vector3 scale, int isActive)
	: Object(pModel, pTexture, pCamera, pShader, position, rotation, scale)
{
	if (isActive == 1)
	{
		this->isActive = true;
	}
	else
	{
		this->isActive = false;
	}
}

void Picture::Update()
{
}


void Picture::Key()
{
}

void Picture::Move()
{
}

void Picture::Rotate()
{
}

bool Picture::getActive()
{
	return isActive;
}

void Picture::MouseClick(int x, int y)
{
}

void Picture::MouseMove()
{
}


Picture::~Picture() {}