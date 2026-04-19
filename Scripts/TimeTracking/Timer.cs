using System;
using System.Collections.Generic;
using System.Text;

namespace Scripts.TimeTracking
{
    public class Timer
    {
        public float TimeRemaining { get; private set; }
        public Action OnComplete { get; private set; }
        public bool IsDone => TimeRemaining <= 0;

        public Timer(float duration, Action on_complete)
        {
            TimeRemaining = duration;
            OnComplete = on_complete;
        }

        public void Tick(float delta)
        {
            if (IsDone) return;
            TimeRemaining -= delta;
            if (IsDone) OnComplete?.Invoke();
        }
    }

    public static class TimerManager
    {
        private static List<Timer> active_timers = new List<Timer>();
        private static void CreateTimer(float duration, Action on_complete)
        {
            active_timers.Add(new Timer(duration, on_complete));
        }

        internal static void UpdateTImers()
        {
            for (int i = active_timers.Count - 1; i >= 0; i--)
            {
                active_timers[i].Tick(Time.DeltaTime);
                if (active_timers[i].IsDone)
                {
                    active_timers.RemoveAt(i);
                }
            }
        }
    }
}
