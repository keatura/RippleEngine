using System;
using System.Collections.Generic;

namespace Scripts
{
    public static class EventManager
    {
        private static readonly List<Action<int>> actor_scrubbers = new List<Action<int>>();
        private static readonly List<Action<int, Type>> component_scrubbers = new List<Action<int, Type>>();

        public static void RegisterBusScrubber(Action<int> actor_scrubber, Action<int, Type> component_scrubber)
        {
            actor_scrubbers.Add(actor_scrubber);
            component_scrubbers.Add(component_scrubber);
        }

        public static void ScrubActorFromAllEvents(int actor_id)
        {
            for (int i = 0; i < actor_scrubbers.Count; i++)
            {
                actor_scrubbers[i](actor_id);
            }
        }

        public static void ScrubComponentForActor(int actor_id, Type component_type)
        {
            for (int i = 0; i < component_scrubbers.Count; i++)
            {
                component_scrubbers[i].Invoke(actor_id, component_type);
            }
        }
    }
    public static class EventBus<T> where T : struct
    {
        public const bool DEBUG_MODE = true;
        private static readonly List<(int actor_id, Action<int, T> callback)> listeners = new List<(int, Action<int, T>)>();

        static EventBus()
        {
            EventManager.RegisterBusScrubber(RemoveAllForActor, RemoveAllForActorAndComponent);
        }

        public static void Subscribe(int actor_id, Action<int, T> static_callback)
        {
            var new_listener = (actor_id, static_callback);
            if (!listeners.Contains(new_listener))
            {
                listeners.Add(new_listener);
            } else if (DEBUG_MODE) {
                Debug.Log($"[SUBSCRIBE] Warning: Method {static_callback.Method.Name} is already subscribed to {typeof(T).Name}. Disregarding subscription attempt");
            }
        }

        public static void Publish(T published_event)
        {
            if (DEBUG_MODE)
                Debug.Log($"[PUBLISH] Executing {typeof(T).Name}...");

            for (int i = listeners.Count - 1; i >= 0; i--)
            {
                var listener = listeners[i];
                listener.callback(listener.actor_id, published_event);
            }
        }

        public static void Unsubscribe(int actor_id, Action<int, T> static_callback)
        {
            listeners.Remove((actor_id, static_callback));
        }

        public static void Clear()
        {
            listeners.Clear();
        }

        private static void RemoveAllForActor(int actor_id)
        {
            listeners.RemoveAll(x => x.actor_id == actor_id);
        }

        private static void RemoveAllForActorAndComponent(int actor_id, Type component_type)
        {
            listeners.RemoveAll(x => x.actor_id == actor_id && x.callback.Method.DeclaringType == component_type);
        }
    }
}