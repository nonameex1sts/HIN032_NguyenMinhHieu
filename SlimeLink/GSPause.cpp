#include "stdafx.h"
#include "GSPause.h"

GSPause::GSPause()
{
	Init("../Resources/StateData/GSPause.txt", StateType::STATE_PAUSE);
}

GSPause::~GSPause()
{
	Exit();
}

void GSPause::MouseClick(int x, int y, bool isPressed)
{
	if (isPressed)
	{
		for (int i = 0; i < inumButtons; i++)
		{
			if (inumButtons != 8 || pButtons[i] == nullptr) {
				break;
			}
			pButtons[i]->MouseClick(x, y, isPressed);
		}
	}
}

void GSPause::MouseMove(int x, int y)
{
	for (int i = 0; i < inumButtons; i++)
	{
		pButtons[i]->MouseMove(x, y);
	}
}