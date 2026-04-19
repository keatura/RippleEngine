using Engine.LinearAlgebra;
using Engine.Physics;
using Scripts.ECS;
using System;
using System.Collections.Generic;
using System.Text;

namespace Scripts.NativeComponents {
    public struct SRenderer : IComponent, IHasUpdate
    {
        public string image_name = "iso_cube";
        public float scale_x = 10.0f;
        public float scale_y = 10.0f;

        public SRenderer() { }

        public void OnUpdate(int self_actor_id)
        {
            Vector2 pos = new Vector2();
            if (Actor.HasComponent<Rigidbody>(self_actor_id))
            {
                ref Rigidbody rb = ref Actor.GetComponent<Rigidbody>(self_actor_id);
                pos = rb.GetPosition();
            }
            Image.DrawEx(image_name, pos.x, pos.y, 0, scale_x, scale_y, 0.5f, 0.5f, 255, 255, 255, 255, 0);
        }
    }
}
