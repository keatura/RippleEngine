using System;
using System.Collections.Generic;
using System.Text;
using Scripts.Core;
using Scripts.ECS;
using Scripts.Scene;
using Scripts.TimeTracking;

namespace Scripts
{
    public static class Engine
    {
        public static bool DEBUG_MODE { get; private set; } = false;

        public static void Initialize()
        {
            ComponentSystemsManager.InitializeComponentManagers();
            TemplateDB.Initialize();
            Image.Initialize();
        }

        public static void Tick()
        {
            for (int i = 0; i < ComponentSystemsManager.component_systems.Count; i++)
            {
                ComponentSystemsManager.component_systems[i].FlushCommands();
            }
            for (int i = 0; i < EngineLoop.EarlyDataUpdateDelegates.Count; i++)
                EngineLoop.EarlyDataUpdateDelegates[i].Invoke();

            for (int i = 0; i < EngineLoop.UpdateDelegates.Count; i++)
                EngineLoop.UpdateDelegates[i].Invoke();

            for (int i = 0; i < EngineLoop.LateUpdateDelegates.Count; i++)
                EngineLoop.LateUpdateDelegates[i].Invoke();

            Actor.ProcessDestructionQueue();
        }
    }
}
