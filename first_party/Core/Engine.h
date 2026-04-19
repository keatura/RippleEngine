#ifndef ENGINE_H
#define ENGINE_H

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/debug-helpers.h"

#include "Utils/TimeManager.h"

#include <string>

class Engine
{
private:
	std::string font_name;
	std::string 
		game_title = "",
		scene_to_load;


	static inline const char* BASE_FILEPATH = "resources/component_types/";

	MonoDomain* m_domain = nullptr;
	MonoAssembly* m_assembly = nullptr;
	MonoImage* m_assembly_image = nullptr;

	TimeManager time_manager;

	// Base initialize function that gets called by game loop
	void Initialize();
	void InitializeSystems();
	void Shutdown();

public:
	~Engine();
	static inline bool engine_running = false;
	void GameLoop(); // Engine game loop
};

#endif
