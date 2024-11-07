#include "IGEngine.h"

class Sandbox : public IGEngine::Application
{
public:

	Sandbox(){}
	~Sandbox(){}
};

IGEngine::Application* IGEngine::CreateApplication()
{
	return new Sandbox();
}