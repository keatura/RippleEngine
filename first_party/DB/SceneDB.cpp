#include "DB/SceneDB.h"

#include "EngineUtils.h"

#include "glm/glm.hpp"
#include "mono/metadata/assembly.h"
#include "mono/metadata/appdomain.h"


#include <sstream>
#include <fstream>
#include <iostream>

void SceneDB::Initialize(MonoImage* img, MonoDomain* domain) {
    m_image = img;
    m_domain = domain;
}

void SceneDB::Load(const std::string& scene_name) {
    scene_load_request = scene_name;
}

void SceneDB::OpenScene(const std::string& sn) {
    scene_name = sn;
    std::string json_path = "resources/scenes/" + scene_name + ".scene";



    // ResetActorStorage();

    std::ifstream json_file(json_path);
    if (!json_file.is_open()) {
        std::cout << "Error loading scene: " << scene_name << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << json_file.rdbuf();
    std::string json_data = buffer.str();

    MonoClass* scene_loader_class = mono_class_from_name(m_image, "Scripts.Scene", "SceneLoader");
    MonoMethod* load_method = mono_class_get_method_from_name(scene_loader_class, "LoadScene", 1);
    MonoString* json_str = mono_string_new(m_domain, json_data.c_str());

    void* args[1];
    args[0] = json_str;
    MonoObject* exception = nullptr;
    mono_runtime_invoke(load_method, nullptr, args, &exception);

    if (exception) {
        MonoString* ex_str = mono_object_to_string(exception, nullptr);
        char* c_str = mono_string_to_utf8(ex_str);
        std::cout << "Scene load failed on C# side: " << c_str << "\n";
        mono_free(c_str);
    }
    else {
        std::cout << "[SCENE] Successfully loaded scene: " << scene_name << "\n";
    }
}

void SceneDB::AnswerLoadQuery()
{
    if (scene_load_request.length() == 0) return;
    SceneDB::OpenScene(scene_load_request);
    scene_load_request = "";
}
