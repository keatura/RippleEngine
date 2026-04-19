main:
	clang++ main.cpp first_party/*/*.cpp third_party/box2d/src/*/*.cpp -std=c++17 -Wno-deprecated-declarations \
	-I./first_party/ \
	-I./first_party/Audio/ \
	-I./first_party/Core/ \
	-I./first_party/DB/ \
	-I./first_party/Graphics/ \
	-I./first_party/Math/ \
	-I./first_party/Scene/ \
	-I./first_party/Utils/ \
	-I./third_party/ \
	-I./third_party/rapidjson/include \
	-I./third_party/SDL2/ -I./third_party/SDL2_image/ -I./third_party/SDL2_mixer/ -I./third_party/SDL2_ttf/ \
	-I./lua/ -I./third_party/LuaBridge/ \
	-I./third_party/box2d/ -I./third_party/box2d/src/ \
	-llua5.4 \
	-lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image -o game_engine_linux -O3
