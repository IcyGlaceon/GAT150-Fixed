#include "Engine.h"
#include "TheGame.h"
#include <iostream>

int main(){

	cool::InitializeMemory();
	cool::SetFilePath("../Assets");

	cool::g_inputSystem.Initialize();
	cool::g_renderer.Initialize();
	cool::g_audioSystem.Initialize();
	cool::g_resources.Initialize();
	cool::g_physicsSystem.Initialize();
	cool::g_eventManager.Initialize();

	cool::Engine::Instance().Register();

	cool::g_renderer.CreateWindow("Game", 800, 600);
	cool::g_renderer.SetClearColor(cool::Color{ 10, 10, 10, 255 });

	std::unique_ptr<TheGame> game = std::make_unique<TheGame>();
	
	game->Initialize();

	float angle = 0;

	bool quit = false;
	while (!quit){
		
		cool::g_time.Tick();
		cool::g_inputSystem.Update();
		cool::g_audioSystem.Update();
		cool::g_physicsSystem.Update();
		cool::g_eventManager.Update();

		if (cool::g_inputSystem.GetKeyDown(cool::key_escape)) quit = true;

		game->Update();

		cool::g_renderer.BeginFrame();

		angle += 360.0f * cool::g_time.deltaTime;

		game->Draw(cool::g_renderer);

		cool::g_renderer.EndFrame();
	}

	game->Shutdown();
	game.reset();

	cool::Factory::Instance().Shutdown();

	cool::g_eventManager.Shutdown();
	cool::g_physicsSystem.Shutdown();
	cool::g_resources.Shutdown();
	cool::g_inputSystem.Shutdown();
	cool::g_renderer.Shutdown();
	cool::g_audioSystem.Shutdown();
}