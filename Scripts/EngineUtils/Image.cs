using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Scripts
{
    public static class Image
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void DrawUI(string image_name, float x, float y);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void DrawUIEx(string image_name, float x, float y, float r, float g, float b, float a, float sorting_order);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Draw(string image_name, float x, float y);

        public static void DrawEx(string image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, int sorting_order)
        {
            DrawEx_Internal(image_to_idx[image_name], x, y, rotation_degrees, scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void DrawEx_Internal(int image_idx, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, int sorting_order);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void DrawPixel(int x, int y, float r, float g, float b, float a);

        private static Dictionary<string, int> image_to_idx = new Dictionary<string, int>();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetImageCount();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetImageName(int idx);

        public static void Initialize()
        {
            image_to_idx.Clear();
            int img_count = GetImageCount();

            for (int i = 0; i < img_count; i++)
            {
                string name = GetImageName(i);
                image_to_idx[name] = i;
            }
        }
    }
}
