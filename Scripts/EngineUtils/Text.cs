using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Scripts
{
    public class Text
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Draw(string str_content, float x, float y, string font_name, float font_size, float r, float g, float b, float a);
    }
}
