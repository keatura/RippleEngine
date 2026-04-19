using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace Scripts.ECS
{
    public static class ComponentSystemsManager
    {
        public static List<IComponentManager> component_systems = new List<IComponentManager>();
        public static Dictionary<Type, IComponentManager> manager_lookup = new Dictionary<Type, IComponentManager>();

        public static void InitializeComponentManagers()
        {
            Assembly game_assembly = Assembly.GetExecutingAssembly();

            foreach (Type type in game_assembly.GetTypes())
            {
                if (type.IsValueType && typeof(IComponent).IsAssignableFrom(type))
                {
                    Type generic_manager_type = typeof(ComponentManager<>).MakeGenericType(type);

                    IComponentManager manager_instance = (IComponentManager)Activator.CreateInstance(generic_manager_type);

                    manager_by_name.Add(type.Name, manager_instance);
                    component_systems.Add(manager_instance);
                    manager_lookup[type] = manager_instance;

                    Debug.Log($"[ECS] Auto-Generated Manager for: {type.Name}");
                }
            }
        }

        public static void AddComponent<T>(int actor_id) where T : struct, IComponent
        {
            if (manager_lookup.TryGetValue(typeof(T), out IComponentManager manager))
            {
                var casted_manager = (ComponentManager<T>)manager;
                casted_manager.AddComponent(actor_id);
            }
        }

        public static Dictionary<string, IComponentManager> manager_by_name = new Dictionary<string, IComponentManager>();
    }

    public static class ComponentRegistry<T> where T : struct, IComponent
    {
        public static ComponentManager<T> Manager;
    }
}
