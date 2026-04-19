using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Scripts
{
    public class Camera
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetPosition(float x, float y);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern float GetPositionX();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern float GetPositionY();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetZoom(float zf);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern float GetZoom();

    }
}
