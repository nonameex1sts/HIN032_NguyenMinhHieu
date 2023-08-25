#include "stdafx.h"
#include "GSCredit.h"

GSCredit::GSCredit()
{
	Init("../TrainingFramework/GSCredit.txt", "GSCredit");
	GameStateBase::GameStateBase(StateType::STATE_CREDIT);
}

GSCredit::~GSCredit()
{
	Exit("GSCredit");
}

void GSCredit::Pause()
{
	// NOTE: blank
}

void GSCredit::Resume()
{
	// NOTE: blank
}

void GSCredit::Update(GLfloat deltatime)
{
	//NOTE: blank
}

void GSCredit::Key(int iKeyPressed)
{
	// NOTE: blank
}

void GSCredit::MouseClick(int x, int y, bool isPressed)
{
	// NOTE: check back button if it got click or not
	if (isPressed)
	{
		for (int i = 0; i < inumButtons; i++)
		{
			pButtons[i]->MouseClick(x, y);
		}
	}
}

void GSCredit::MouseMove(int x, int y)
{
	// NOTE: blank
}