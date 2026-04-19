#ifndef SCENEDB_H
#define SCENEDB_H


#include <string>
#include <memory>
#include <unordered_set>
#include <map>
#include <set>

#include "mono/metadata/object.h"
#include "mono/metadata/appdomain.h"

class SceneDB
{

    using ActorID = uint64_t;
public:

    static void Initialize(MonoImage* img, MonoDomain* domain);

    static void Load(const std::string& scene_name);
    static void OpenScene(const std::string& sn);

    //void OnStart();

    static inline std::string GetSceneName() { return scene_name; }
    static inline uint64_t GetNumActors() { return num_actors; }

    //static inline std::vector<ActorData> actors;

    // scene table stuff
    static void AnswerLoadQuery();

private:
    static inline MonoImage* m_image;
    static inline MonoDomain* m_domain;

    static inline uint64_t num_actors = 0;
    //std::vector<ECS::ActorID> free_list; // the recycling bin

    static inline std::string scene_name = "";
    static inline std::string scene_load_request = "";

};

#endif