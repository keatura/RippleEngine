using Scripts.ECS;
using System;
using System.Collections.Generic;
using System.Text;

namespace Scripts.NativeComponents
{
    public struct Sprite : IComponent, IHasLateUpdate
    {
        public string image_name = "";
        public Sprite() { }
        public void OnLateUpdate(int self_actor_id)
        {
            ref Position pos = ref Actor.GetComponent<Position>(self_actor_id);
            Image.DrawEx(image_name, pos.x, pos.y, 0, 1.0f, 1.0f, 0.5f, 0.5f, 255, 255, 255, 255, 1);
        }
    }
}
