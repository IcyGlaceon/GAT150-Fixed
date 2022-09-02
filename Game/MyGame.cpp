#include "MyGame.h"
#include "Engine.h"
#include "GameComponents/EnemyComponent.h"

void TheGame::Initialize(){

	REGISTER_CLASS(EnemyComponent);

	m_scene = std::make_unique<cool::Scene>();
	rapidjson::Document document;

	std::vector<std::string> sceneNames = {"level.txt", "tilemap.txt", "prefabs.txt"};
	for (auto& sceneName : sceneNames) {
		bool success = cool::json::Load(sceneName, document);
		if (!success) {
			LOG("Could not load scene %s", sceneName.c_str());
			continue;
		}
		m_scene->Read(document);
	}

	m_scene->Initialize();

	cool::g_eventManager.Subscribe("EVENT_ADD_POINTS", std::bind(&TheGame::OnNotify, this, std::placeholders::_1));
	cool::g_eventManager.Subscribe("EVENT_PLAYER_DEAD", std::bind(&TheGame::OnNotify, this, std::placeholders::_1));
	cool::g_eventManager.Subscribe("EVENT_DAMAGE", std::bind(&TheGame::OnNotify, this, std::placeholders::_1));
	

}

void TheGame::Shutdown(){

	m_scene->RemoveAll();

}

void TheGame::Update(){

	auto time = m_scene->GetActorFromName("Time");
	auto timecomponent = time->GetComponent<cool::TextComponent>();

	switch (m_gameState) {
	case gameState::titleScreen:
		m_scene->GetActorFromName("GameOver")->SetActive(false);
		if (cool::g_inputSystem.GetKeyState(cool::key_space) == cool::InputSystem::State::Press){
			m_scene->GetActorFromName("Title")->SetActive(false);
			m_gameState = gameState::startLevel;
		}
		break;

	case gameState::startLevel:
		m_scene->GetActorFromName("Player")->SetActive(true);

		for (int i = 0; i < 10; i++) {
			auto actor = cool::Factory::Instance().Create<cool::Actor>("Coin");
			actor->m_transform.position = { cool::randomf(0, 800), 100.0f };
			actor->Initialize();
			m_scene->Add(std::move(actor));
		}

		for (int i = 0; i < 3; i++) {
			auto actor = cool::Factory::Instance().Create<cool::Actor>("Ghost");
			actor->m_transform.position = { cool::randomf(0, 800), 100.0f };
			actor->Initialize();
			m_scene->Add(std::move(actor));
		}
		m_gameState = gameState::game;
		break;

	case gameState::game:
		m_gameTimer -= cool::g_time.deltaTime;

		if (timecomponent)
		{
			timecomponent->SetText(std::to_string((int)(m_gameTimer)));
		}

		if (m_gameTimer <= 0)
		{
			m_gameState = gameState::gameOver;
		}

		m_spawnTimerE -= cool::g_time.deltaTime;
		if (m_spawnTimerE <= 0)
		{
			auto actor = cool::Factory::Instance().Create<cool::Actor>("Ghost");
			actor->m_transform.position = { cool::randomf(0, 800), 100.0f };
			actor->Initialize();
			m_scene->Add(std::move(actor));

			m_spawnTimerE = cool::randomf(5);
		}
		m_spawnTimerC -= cool::g_time.deltaTime;
		if (m_spawnTimerC <= 0)
		{
			auto actor = cool::Factory::Instance().Create<cool::Actor>("Coin");
			actor->m_transform.position = { cool::randomf(0, 800), 100.0f };
			actor->Initialize();
			m_scene->Add(std::move(actor));

			m_spawnTimerC = cool::randomf(2);
		}

		break;

	case gameState::playerDead:
		m_scene->GetActorFromName("Player")->m_transform.position = (300, 400);
		m_scene->GetActorFromName("Player")->SetActive(false);
		m_startTimer -= cool::g_time.deltaTime;
		if (m_startTimer <= 0) {			
			m_gameState = (m_lives > 0) ? gameState::startLevel : gameState::gameOver;


		}
		break;
	case gameState::gameOver:
		m_scene->GetActorFromName("GameOver")->SetActive(true);
		m_scene->GetActorFromName("Player")->SetActive(false);
		auto enemies = m_scene->GetActorsFromTag("Enemy");
		for (auto enemy : enemies)
		{
			enemy->SetDestroy();
		}

		break;

		
	}
	m_scene->Update();
}

void TheGame::Draw(cool::Renderer& renderer){

	m_scene->Draw(renderer);

}

void TheGame::OnAddPoints(const cool::Event& event){

	AddPoints(std::get<int>(event.data));

	std::cout << event.name << std::endl;
	std::cout << GetScore() << std::endl;

}

void TheGame::OnPlayerDead(const cool::Event& event){

	m_gameState = gameState::playerDead;
	m_lives--;
	m_startTimer = 3;

}

void TheGame::OnNotify(const cool::Event& event){

	if (event.name == "EVENT_ADD_POINTS") {
		AddPoints(std::get<int>(event.data)); 
		//std::setprecision(1);
		auto score = m_scene->GetActorFromName("Score");
		auto component = score->GetComponent<cool::TextComponent>();
		if (component)
		{
			component->SetText(std::to_string((GetScore())));
		}
		std::cout << GetScore() << std::endl;

	}

	if (event.name == "EVENT_PLAYER_DEAD") {
		m_gameState = gameState::playerDead;
		auto enemies = m_scene->GetActorsFromTag("Enemy");
		for (auto enemy : enemies)
		{
			enemy->SetDestroy();
		}
		m_lives--;
		m_startTimer = 3;
		std::cout << m_lives << std::endl;
	}
	if (event.name == "EVENT_DAMAGE") {

	}
}