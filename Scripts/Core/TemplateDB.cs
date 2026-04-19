using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using Newtonsoft.Json.Linq;

namespace Scripts.Core
{
    public static class TemplateDB
    {
        private static bool DEBUG_MODE = true;

        static string template_path = "resources/actor_templates/";
        static Dictionary<string, JObject> actor_templates = new Dictionary<string, JObject>();
        public static void Initialize()
        {
            string[] files = Directory.GetFiles(template_path, "*.template");
            if (DEBUG_MODE)
                Debug.Log($"[TEMPLATEDB] Initializing TemplateDB with {files.Length} found template files");

            foreach (string file in files)
            {
                string content = File.ReadAllText(file);
                JObject root = JObject.Parse(content);
                JToken template_name;
                root.TryGetValue("template_name", out template_name);
                if (template_name == null)
                {
                    Debug.Log($"[TEMPLATEDB] ERROR: Template {file} does not have a name. Aborting.");
                    Application.Quit();
                } else
                {
                    JToken components;
                    root.TryGetValue("components", out components);
                    string temp_name = template_name.ToString();
                    if (actor_templates.ContainsKey(temp_name))
                    {
                        Debug.Log($"[TEMPLATEDB] ERROR: Template {file} has a duplicate name. Aborting.");
                        Application.Quit();
                    }
                    JObject comps = (components != null) ? (JObject)components : new JObject();
                    actor_templates.Add(temp_name, comps);
                }
            }

            if (DEBUG_MODE)
            {
                Debug.Log($"[TEMPLATEDB] Finished loading {actor_templates.Count} valid templates\nTemplate names:");
                foreach (string tname in actor_templates.Keys)
                {
                    Debug.Log($"{tname}");
                }
            }
        }

        public static JObject GetTemplate(string template_name)
        {
            JObject comps;
            actor_templates.TryGetValue(template_name, out comps);
            if (comps == null)
            {
                Debug.Log($"[TEMPLATEDB] ERROR: Template with name {template_name} does not exist. Aborting.");
                Application.Quit();
                return null;
            }
            return (JObject)comps.DeepClone();
        }
    }
}
