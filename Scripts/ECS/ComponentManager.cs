using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Scripts.DataStructures;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Net;
using System.Reflection;
using System.Text;


namespace Scripts.ECS
{
    public static class EngineLoop
    {
        public static List<Action> StartDelegates = new List<Action>();
        public static List<Action> UpdateDelegates = new List<Action>();
        public static List<Action> LateUpdateDelegates = new List<Action>();
        public static List<Action> DestroyDelegates = new List<Action>();
        public static List<Action> EarlyDataUpdateDelegates = new List<Action>();
    }

    public interface IComponentManager
    {
        void RemoveActor(int actor_id);
        void AddComponent(int actor_id);

        void DeserializeAndApply(int actor_id, JToken json_values);
        bool ActorHasComponent(int actor_id);

        void QueueComponentRemoval(int actor_id);

        void QueueComponentAddition(int actor_id);
        void FlushCommands();
    }

    public class ComponentManager<T> : IComponentManager  where T : struct, IComponent
    {
        public static readonly bool HasStart = typeof(IHasStart).IsAssignableFrom(typeof(T));
        public static readonly bool HasUpdate = typeof(IHasUpdate).IsAssignableFrom(typeof(T));
        public static readonly bool HasLateUpdate = typeof(IHasLateUpdate).IsAssignableFrom(typeof(T));
        public static readonly bool HasDestroy = typeof(IHasDestroy).IsAssignableFrom(typeof(T));
        public static readonly bool HasEarlyDataUpdate = typeof(IHasEarlyDataUpdate).IsAssignableFrom(typeof(T));


        public StructList<T> components = new StructList<T>();
        private List<int> comp_to_actor_id = new List<int>();
        private Dictionary<int, int> actor_id_to_comp_id = new Dictionary<int, int>();

        private Action<StructList<T>, int, int> single_start_method;

        private List<Command> command_queue = new List<Command>();
        private enum CommandType { ADD, REMOVE }
        private struct Command
        {
            public int actor_id;
            public CommandType type;
            public T component_data;
        }

        public ComponentManager()
        {
            if (HasStart)
            {
                var single_method = typeof(SystemBinder).GetMethod("CreateSingleStart").MakeGenericMethod(typeof(T));
                single_start_method = (Action<StructList<T>, int, int>)single_method.Invoke(null, null);
            }

            if (HasUpdate)
            {
                var method = typeof(SystemBinder).GetMethod("CreateUpdate").MakeGenericMethod(typeof(T));
                Action action = (Action)method.Invoke(null, new object[] { components, comp_to_actor_id });
                EngineLoop.UpdateDelegates.Add(action);
            }

            if (HasLateUpdate)
            {
                var method = typeof(SystemBinder).GetMethod("CreateLateUpdate").MakeGenericMethod(typeof(T));
                Action action = (Action)method.Invoke(null, new object[] { components, comp_to_actor_id });
                EngineLoop.LateUpdateDelegates.Add(action);
            }

            if (HasDestroy)
            {
                var method = typeof(SystemBinder).GetMethod("CreateDestroy").MakeGenericMethod(typeof(T));
                Action action = (Action)method.Invoke(null, new object[] { components, comp_to_actor_id });
                EngineLoop.DestroyDelegates.Add(action);
            }

            if (HasEarlyDataUpdate)
            {
                var method = typeof(SystemBinder).GetMethod("CreateEarlyDataUpdate").MakeGenericMethod(typeof(T));
                Action action = (Action)method.Invoke(null, new object[] { components, comp_to_actor_id });
                EngineLoop.EarlyDataUpdateDelegates.Add(action);
            }

            ComponentRegistry<T>.Manager = this;
        }

        public void AddComponent(int actor_id)
        {
            if (actor_id_to_comp_id.ContainsKey(actor_id))
            {
                if (Engine.DEBUG_MODE)
                    Debug.Log($"[ECS] Warning, actor {actor_id} already has component of type {typeof(T)}, ignoring request to add component to actor");
                return;
            }

            components.Add(default(T));
            comp_to_actor_id.Add(actor_id);
            actor_id_to_comp_id[actor_id] = components.Count - 1;

            if (HasStart)
            {
                single_start_method(components, components.Count - 1, actor_id);
            }
        }

        public void RemoveActor(int actor_id)
        {
            if (!actor_id_to_comp_id.TryGetValue(actor_id, out int comp_idx))
                return; // no actor no problem

            EventManager.ScrubComponentForActor(actor_id, typeof(T));
            int last_idx = components.Count - 1;
            if (comp_idx != last_idx)
            {
                int moving_actor_id = comp_to_actor_id[last_idx];

                components[comp_idx] = components[last_idx];
                comp_to_actor_id[comp_idx] = moving_actor_id;

                actor_id_to_comp_id[moving_actor_id] = comp_idx;
            }

            components.RemoveAt(last_idx);
            comp_to_actor_id.RemoveAt(last_idx);
            actor_id_to_comp_id.Remove(actor_id);
        }

        private static readonly JsonSerializer _serializer = new JsonSerializer();
        public void DeserializeAndApply(int actor_id, JToken json_values)
        {
            if (actor_id_to_comp_id.TryGetValue(actor_id, out int comp_id)) return;

            T new_data = new T();
            object boxed_data = new_data;

            using (var reader = json_values.CreateReader())
            {
                _serializer.Populate(reader, boxed_data);
            }
            QueueComponentAddition(actor_id, (T)boxed_data);
        }

        public bool ActorHasComponent(int actor_id)
        {
            return actor_id_to_comp_id.ContainsKey(actor_id);
        }

        public ref T GetActorComponent(int actor_id)
        {
            int index = actor_id_to_comp_id[actor_id];
            return ref components.Data[index];
        }
        public void QueueComponentRemoval(int actor_id)
        {
            command_queue.Add(new Command { actor_id = actor_id, type = CommandType.REMOVE });
        }
        public void QueueComponentAddition(int actor_id)
        {
            command_queue.Add(new Command { actor_id = actor_id, type = CommandType.ADD, component_data = new T() });
        }

        public void QueueComponentAddition(int actor_id, T initial_data)
        {
            command_queue.Add(new Command { actor_id = actor_id, type = CommandType.ADD, component_data = initial_data });
        }

        public void FlushCommands()
        {
            if (command_queue.Count == 0) return;
            for (int i = 0; i < command_queue.Count; i++)
            {
                Command cmd = command_queue[i];
                if (cmd.type == CommandType.ADD)
                    AddComponent(cmd.actor_id, cmd.component_data);
                else if (cmd.type == CommandType.REMOVE)
                    RemoveActor(cmd.actor_id);
            }
            command_queue.Clear();
        }

        private void AddComponent(int actor_id, T component_data)
        {
            if (ActorHasComponent(actor_id)) return;

            components.Add(component_data);
            int new_index = components.Count - 1;

            actor_id_to_comp_id[actor_id] = new_index;
            comp_to_actor_id.Add(actor_id);

            if (HasStart)
            {
                single_start_method(components, new_index, actor_id);
            }
        }
    }
    public static class StartSystem<T> where T : struct, IComponent, IHasStart
    {
        public static void Execute(StructList<T> components, List<int> comp_to_actor_id)
        {
            for (int i = 0; i < components.Count; i++)
            {
                ref T component = ref components[i];
                component.OnStart(comp_to_actor_id[i]);
            }
        }
    }

    public static class EarlyDataUpdateSystem<T> where T : struct, IComponent, IHasEarlyDataUpdate
    {
        public static void Execute(StructList<T> components, List<int> comp_to_actor_id)
        {
            for (int i = 0; i < components.Count; i++)
            {
                ref T component = ref components[i];
                component.OnEarlyDataUpdate(comp_to_actor_id[i]);
            }
        }
    }

    public static class UpdateSystem<T> where T : struct, IComponent, IHasUpdate
    {
        public static void Execute(StructList<T> components, List<int> comp_to_actor_id)
        {
            for (int i = 0; i < components.Count; i++)
            {
                ref T component = ref components[i];
                component.OnUpdate(comp_to_actor_id[i]);
            }
        }
    }
    public static class LateUpdateSystem<T> where T : struct, IComponent, IHasLateUpdate
    {
        public static void Execute(StructList<T> components, List<int> comp_to_actor_id)
        {
            for (int i = 0; i < components.Count; i++)
            {
                ref T component = ref components[i];
                component.OnLateUpdate(comp_to_actor_id[i]);
            }
        }
    }

    public static class DestroySystem<T> where T: struct, IComponent, IHasDestroy
    {
        public static void Execute(StructList<T> components, List<int> comp_to_actor_id)
        {
            for (int i = 0; i < components.Count; i++)
            {
                ref T component = ref components[i];
                component.OnDestroy(comp_to_actor_id[i]);
            }
        }
    }

    public static class SystemBinder
    {
        public static Action CreateStart<T>(StructList<T> components, List<int> ids) where T : struct, IComponent, IHasStart
        {
            return () => StartSystem<T>.Execute(components, ids);
        }

        public static Action CreateUpdate<T>(StructList<T> components, List<int> ids) where T : struct, IComponent, IHasUpdate
        {
            return () => UpdateSystem<T>.Execute(components, ids);
        }

        public static Action CreateLateUpdate<T>(StructList<T> components, List<int> ids) where T : struct, IComponent, IHasLateUpdate
        {
            return () => LateUpdateSystem<T>.Execute(components, ids);
        }

        public static Action CreateDestroy<T>(StructList<T> components, List<int> ids) where T : struct, IComponent, IHasDestroy
        {
            return () => DestroySystem<T>.Execute(components, ids);
        }

        public static Action CreateEarlyDataUpdate<T>(StructList<T> components, List<int> ids) where T : struct, IComponent, IHasEarlyDataUpdate
        {
            return () => EarlyDataUpdateSystem<T>.Execute(components, ids);
        }

        public static Action<StructList<T>, int, int> CreateSingleStart<T>() where T : struct, IComponent, IHasStart
        {
            return (StructList<T> list, int index, int actor_id) =>
            {
                list.Data[index].OnStart(actor_id);
            };
        }
    }
}
