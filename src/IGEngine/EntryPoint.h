#pragma once

#ifdef IGE_PLATFORM_WINDOWS

extern IGEngine::Application* IGEngine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = IGEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif