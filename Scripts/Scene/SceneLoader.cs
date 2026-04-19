using System;
using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Scripts.Core;
using Scripts.ECS;
using Scripts.NativeComponents;

namespace Scripts.Scene
{
    public static class SceneLoader
    {
        public static void LoadScene(string json_content)
        {
            JObject root = JObject.Parse(json_content);

            if (root == null) return;
            if (!root.ContainsKey("actors")) return;

            JArray actors_array = (JArray)root["actors"];

            foreach (JToken actor_token in actors_array)
            {
                JObject actor_obj = (JObject)actor_token;

                int new_actor_id = Actor.Instantiate();

                JObject components_obj = null;
                if (actor_obj.ContainsKey("components"))
                {
                    components_obj = (JObject)actor_obj["components"];
                } else
                {
                    components_obj = new JObject();
                }

                // template
                JToken template_name_token;
                JObject template_object = null;
                actor_obj.TryGetValue("template_name", out template_name_token);
                if (template_name_token != null)
                {
                    string template_name = template_name_token.ToString();
                    template_object = TemplateDB.GetTemplate(template_name);


                    template_object.Merge(components_obj);
                }

                JObject obj_to_use = (template_object != null) ? template_object : components_obj;

                foreach (var kvp in obj_to_use)
                {
                    string comp_name = kvp.Key;
                    JToken comp_data = kvp.Value;

                    if (comp_name == "Position")
                    {
                        ref Position actor_pos = ref Actor.GetComponent<Position>(new_actor_id);
                        var pos_x = comp_data["x"];
                        var pos_y = comp_data["y"];
                        if (pos_x != null)
                            actor_pos.x = (float)pos_x;
                        if (pos_y != null)
                            actor_pos.y = (float)pos_y;
                    }

                    if (ComponentSystemsManager.manager_by_name.TryGetValue(comp_name, out IComponentManager comp_manager))
                    {
                        comp_manager.DeserializeAndApply(new_actor_id, comp_data);
                    }
                    else
                    {
                        Debug.Log("[SCENELOADER] ERROR: Component does not exist of type" + comp_name);
                    }
                }
            }
        }

        public static int InstantiateFromJSON(string json_content)
        {
            int new_actor_id = Actor.Instantiate();
            JObject root = JObject.Parse(json_content);
            JObject components_obj = (JObject)root["components"];
            foreach(var kvp in components_obj)
            {
                string component_name = kvp.Key;
                JToken component_data = kvp.Value;


                if (ComponentSystemsManager.manager_by_name.TryGetValue(component_name, out IComponentManager manager))
                {
                    if (component_name == "Position")
                    {
                        ref Position actor_pos = ref Actor.GetComponent<Position>(new_actor_id);
                        var pos_x = component_data["x"];
                        var pos_y = component_data["y"];
                        if (pos_x != null)
                            actor_pos.x = (float)pos_x;
                        if (pos_y != null)
                            actor_pos.y = (float)pos_y;
                    }
                    manager.AddComponent(new_actor_id);
                    manager.DeserializeAndApply(new_actor_id, component_data);
                }
            }

            return new_actor_id;
;        }
    }
}
