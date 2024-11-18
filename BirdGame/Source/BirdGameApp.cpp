#include "igpch.h"
#include "IGEngine.h"

class BirdGame : public IGEngine::Application
{
public:

	BirdGame(){}
	~BirdGame(){}
};

IGEngine::Application* IGEngine::CreateApplication()
{
	return new BirdGame();
}