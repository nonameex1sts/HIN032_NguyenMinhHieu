#include "stdafx.h"
#include "GameStateBase.h"

GameStateBase::GameStateBase()
{
}

GameStateBase::GameStateBase(StateType e_type)
{
	this->e_type = e_type;
}

GameStateBase::~GameStateBase()
{
}

void GameStateBase::Init()
{
}

void GameStateBase::Exit()
{
}

void GameStateBase::Pause()
{
}

void GameStateBase::Resume()
{
}

void GameStateBase::Update(GLfloat deltatime)
{
}

void GameStateBase::Draw()
{
}

StateType GameStateBase::GetStateType()
{
	return e_type;
}

GameStateBase* GameStateBase::CreateState(StateType type)
{
	return nullptr;
}
