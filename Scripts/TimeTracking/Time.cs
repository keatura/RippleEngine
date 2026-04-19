using System;
using System.Collections.Generic;
using System.Text;

namespace Scripts.TimeTracking
{
    public static class Time
    {
        public static float DeltaTime { get; private set; }
        public static double TotalTime { get; private set; }

        internal static void UpdateTime(float new_dt, double new_total)
        {
            DeltaTime = new_dt;
            TotalTime = new_total;
        }
    }
}
