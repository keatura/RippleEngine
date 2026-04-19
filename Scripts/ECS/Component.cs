using System;
using System.Collections.Generic;
using System.Text;

namespace Scripts.ECS
{
    public interface IComponent { }

    public interface IHasStart
    {
        void OnStart(int self_actor_id);
    }
    public interface IHasUpdate
    {
        void OnUpdate(int self_actor_id);
    }

    public interface IHasLateUpdate
    {
        void OnLateUpdate(int self_actor_id);
    }

    public interface IHasDestroy
    {
        void OnDestroy(int self_actor_id);
    }

    public interface IHasEarlyDataUpdate
    {
        void OnEarlyDataUpdate(int self_actor_id);
    }
}
