using Scripts.ECS;
using Scripts.NativeComponents;
using System;
using System.Collections.Generic;
using System.Text;

namespace Scripts
{
    public static class Actor
    {
        private static List<int> actors_to_destroy = new List<int>();
        private static int next_actor_id = 0;
        private static Queue<int> free_ids = new Queue<int>();
        public static int Instantiate()
        {
            int id_to_use = 0;
            if (free_ids.Count == 0)
                id_to_use = next_actor_id++;
            else
                id_to_use += free_ids.Dequeue();
            ComponentRegistry<Position>.Manager.AddComponent(id_to_use);
            return id_to_use;
        }

        public static void Destroy(int actor_id)
        {
            if (!actors_to_destroy.Contains(actor_id))
            {
                actors_to_destroy.Add(actor_id);
            }
        }

        public static void AddComponent<T>(int actor_id) where T : struct, IComponent
        {
            ComponentRegistry<T>.Manager.QueueComponentAddition(actor_id);
        }

        public static void AddComponent<T>(int actor_id, T comp_data) where T : struct, IComponent
        {
            ComponentRegistry<T>.Manager.QueueComponentAddition(actor_id, comp_data);
        }

        public static ref T GetComponent<T>(int actor_id) where T : struct, IComponent
        {
            return ref ComponentRegistry<T>.Manager.GetActorComponent(actor_id);
        }

        public static bool HasComponent<T>(int actor_id) where T : struct, IComponent
        {
            return ComponentRegistry<T>.Manager.ActorHasComponent(actor_id);
        }

        public static void RemoveComponent<T>(int actor_id) where T : struct, IComponent
        {
            ComponentRegistry<T>.Manager.QueueComponentRemoval(actor_id);
        }

        public static void ProcessDestructionQueue()
        {
            if (actors_to_destroy.Count == 0) return;
            Debug.Log("destroy");
            Debug.Log(actors_to_destroy.Count);
            foreach (int dead_id in actors_to_destroy)
            {
                DestroyActor(dead_id);
                free_ids.Enqueue(dead_id);
            }
            actors_to_destroy.Clear();
        }

        private static void DestroyActor(int actor_id)
        {
            EventManager.ScrubActorFromAllEvents(actor_id);

            foreach (IComponentManager manager in ComponentSystemsManager.component_systems)
                manager.RemoveActor(actor_id);
        }
    }
}
