#include "Core/Engine.h"
#include "Core/Input.h"

#include "DB/SceneDB.h"
#include "Graphics/Renderer.h"
#include "Audio/AudioManager.h"
#include "Math/Physics.h"

#include "Utils/EngineUtils.h"
#include "Utils/Application.h"
#include "Utils/Debug.h"

#include <iostream>
#include <algorithm>
#include <filesystem>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

void CppDebugLog(std::string msg) { std::cout << msg << "\n"; }

// Base initialize function that gets called by game loop
void Engine::Initialize() {
	// make sure resources exists in the first place
	if (!std::filesystem::exists("resources/")) {
		std::cout << "error: resources/ missing";
		exit(0);
	}
	// load members from game config
	
	rapidjson::Document
		game_cfg;
	EngineUtils::LoadConfigFile("resources/game.config", game_cfg, true);
	if (!game_cfg.HasMember("initial_scene")) {
		std::cout << "error: initial_scene unspecified";
		exit(0);
	}
	game_title = EngineUtils::GetString(game_cfg, "game_title", "");
	scene_to_load = EngineUtils::GetString(game_cfg, "initial_scene", "");
	//
	//lua_state = luaL_newstate();
	//luaL_openlibs(lua_state);

	//luabridge::getGlobalNamespace(lua_state)
	//	.beginNamespace("Debug")
	//	.addFunction("Log", CppDebugLog)
	//	.endNamespace();



	// load in mono
	mono_set_dirs("./third_party/Mono/lib", ".");

	m_domain = mono_jit_init("CsGame");
	if (!m_domain) {
		std::cout << "Error with m_ptrMonoDomain" << "\n";
		exit(0);
	}

	m_assembly = mono_domain_assembly_open(m_domain, "./Scripts/bin/Debug/netstandard2.0/Scripts.dll");
	if (!m_assembly) {
		std::cout << "Error with m_ptrGameAssembly" << "\n";
		exit(0);
	}

	m_assembly_image = mono_assembly_get_image(m_assembly);
	if (!m_assembly_image) {
		std::cout << "Error with m_ptrGameAssemblyImage" << "\n";
		exit(0);
	}

	if (!SDL_Init(0)) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
		exit(0);
	}

	InitializeSystems();
}

void Engine::InitializeSystems() {
	Physics::Initialize();
	Application::Initialize(m_assembly_image);
	Debug::Initialize();
	Input::Initialize();
	AudioManager::Initialize();
	Renderer::Initialize(game_title);
	SceneDB::Initialize(m_assembly_image, m_domain);

	time_manager.Initialize(m_assembly_image);
}

Engine::~Engine()
{
	Shutdown();
}

// Engine game loop
void Engine::GameLoop()
{
	// Initialize scene, all actors
	//SceneDB::OpenScene(scene_to_load);

	engine_running = true;
	Engine::Initialize();

	MonoClass* engine_class = mono_class_from_name(m_assembly_image, "Scripts", "Engine");
	MonoMethodDesc* init_method_desc = mono_method_desc_new(".Engine::Initialize()", false);
	MonoMethod* init_method = mono_method_desc_search_in_class(init_method_desc, engine_class);

	if (!init_method) {
		std::cout << "[ENGINE] ERROR: Could not find engine initialize method, aborting now.";
		exit(0);
	}

	MonoObject* exception_object = nullptr;
	mono_runtime_invoke(init_method, nullptr, nullptr, &exception_object);

	if (exception_object) {
		std::cout << mono_string_to_utf8(mono_object_to_string(exception_object, nullptr));
		std::cout << "[ENGINE] ERROR: Engine initialize did not run successfully, aborting now.";
		exit(0);
	}

	MonoMethodDesc* tick_method_desc = mono_method_desc_new(".Engine::Tick()", false);
	MonoMethod* tick_method = mono_method_desc_search_in_class(tick_method_desc, engine_class);
	SceneDB::OpenScene(scene_to_load);
	float total_time = 0;
	while (engine_running) {
		total_time += time_manager.GetDeltaTime();
		time_manager.Update();

	//	// Input step
		Input::ProcessEvents();
	//	SceneDB::UpdateActors();
		Physics::PhysicsStep(time_manager.GetDeltaTime());

		mono_runtime_invoke(tick_method, nullptr, nullptr, &exception_object);

		Renderer::ClearRenderer(); 
		Renderer::ProcessCommandBuffer();
		Renderer::Present();
		Application::IncrementFrame();

		Input::LateUpdate();
		//std::cout << "Avg frame time: " << total_time / Application::GetFrame() << "\n";
	//	SceneDB::AnswerLoadQuery();
	}
}

void Engine::Shutdown() {
	Input::Shutdown();
	Renderer::Shutdown();

	mono_jit_cleanup(m_domain);

	SDL_Quit();
}