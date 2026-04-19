using Engine.Physics;
using Scripts.ECS;
using System;
using System.Collections.Generic;
using System.Text;
using Engine.LinearAlgebra;
using Scripts.TimeTracking;

namespace Scripts.NativeComponents
{
    public struct CharacterController : IComponent, IHasUpdate, IHasLateUpdate
    {
        Vector2 target_pos = new Vector2();
        float max_leash_dist = 3f;
        float target_speed_x = 10f;


        public CharacterController() { }
        public void OnUpdate(int self_actor_id)
        {
            ref Rigidbody rb = ref Actor.GetComponent<Rigidbody>(self_actor_id);
            Vector2 curr_vel = rb.GetVelocity();
            Vector2 new_vel = new Vector2(curr_vel.x, curr_vel.y);
            bool space = Input.GetKeyDown(InputCode.Space);
            bool right = Input.GetKey(InputCode.Right);
            bool left = Input.GetKey(InputCode.Left);
            if (space || right || left)
            {
                if (space)
                {
                    new_vel.y = -10;
                }
                if (!(right && left))
                {
                    if (right)
                    {
                        if (Math.Abs(curr_vel.x) < target_speed_x)
                            new_vel.x = curr_vel.x + (target_speed_x - curr_vel.x) * Time.DeltaTime * 2;
                    }
                    if (left)
                    {
                        if (Math.Abs(curr_vel.x) < target_speed_x)
                            new_vel.x = curr_vel.x + (-target_speed_x - curr_vel.x) * Time.DeltaTime * 2;
                    }
                }
                rb.SetVelocity(new_vel);
            } else
            {
                new_vel.x = 0; new_vel.y = 0;
            }
            target_pos = target_pos.Lerp(new Vector2(new_vel.x, new_vel.y).Normalized() * max_leash_dist, Time.DeltaTime);
            if (target_pos.LengthSquared() > max_leash_dist*max_leash_dist)
                target_pos = target_pos.Normalized() * max_leash_dist;
        }

        public void OnLateUpdate(int self_actor_id)
        {
            ref Position pos = ref Actor.GetComponent<Position>(self_actor_id);
            Camera.SetPosition(target_pos.x + pos.x, target_pos.y + pos.y);
        }
    }
}
