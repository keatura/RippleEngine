using Scripts.ECS;
using System;
using System.Collections.Generic;
using System.Text;

namespace Scripts.NativeComponents
{
    public struct Position : IComponent
    {
        public float x = 0;
        public float y = 0;

        public Position() { }
    }
}
