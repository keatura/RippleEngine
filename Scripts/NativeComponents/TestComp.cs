using Scripts.ECS;
using System;
using System.Collections.Generic;
using System.Text;

namespace Scripts.NativeComponents
{
    public struct TestComp : IComponent, IHasUpdate
    {
        public bool enabled = true;
        public int offset = 2;
        public string my_string = "Booyah!";

        public TestComp() { }

        public void OnUpdate(int actor_id)
        {
            Debug.Log(my_string);
        }
    }
}
