#include "Debug.h"

#include "mono/metadata/object.h"
#include "mono/metadata/appdomain.h"

#include <iostream>

void Debug::Initialize() {
	mono_add_internal_call("Scripts.Debug::Log", &Debug::Log);
}

void Debug::Log(MonoString* mono_string) {
	char* cstr = mono_string_to_utf8(mono_string);
	std::string str = std::string(cstr);
	std::cout << str << "\n";
	mono_free(cstr);
}