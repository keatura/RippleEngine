#include <iostream>
#include "Core/Engine.h"

int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr); // remember that you have to flush manually for input

	// Game Stuff Here
	Engine engine;
	engine.GameLoop();

	// Game Stuff Here
	//// Engine engine; 
	//// engine.GameLoop();
	//std::cout << "Success! Mono is linked and running from your project root." << "\n";
	//mono_jit_cleanup(root_domain);
	return 0;
}
