#include "GameEngine.h"

using namespace GE;


GameEngine* GameEngine::m_GameEngineInstance = nullptr;

GameEngine* GameEngine::Get()
{
	if (!m_GameEngineInstance)
		m_GameEngineInstance = new GameEngine();

	return m_GameEngineInstance;
}

void GameEngine::InitializeEngine()
{
	
}