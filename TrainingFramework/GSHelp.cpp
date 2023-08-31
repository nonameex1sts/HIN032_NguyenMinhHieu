#include "stdafx.h"
#include "GSHelp.h"

GSHelp::GSHelp()
{
	Init("../TrainingFramework/GSHelp.txt", StateType::STATE_HELP);
}

GSHelp::~GSHelp()
{
	Exit();
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
	// NOTE: check all button if it got click or not
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
	for (int i = 0; i < inumButtons; i++)
	{
		pButtons[i]->MouseMove(x, y);
	}
}