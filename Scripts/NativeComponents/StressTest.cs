using Scripts.ECS;
using Scripts.TimeTracking;
using System;
using System.Collections.Generic;
using System.Text;

namespace Scripts.NativeComponents
{
    public struct SpriteRenderer : IComponent, IHasStart, IHasUpdate
    {
        public string sprite;
        public float pos_x;
        public float pos_y;

        private float _originX;
        private float _originY;
        private float _phaseShift;
        private float _amplitude;
        private int _sortingOrder;
        public byte r, g, b, a;

        private const float _freq = (float)Math.PI / 120.0f;

        public void OnStart(int id)
        {
            _originX = pos_x;
            _originY = pos_y;

            float delay = (float)Math.Sqrt(pos_x * pos_x + pos_y * pos_y);

            float translatedX = (pos_x * 0.5f) - (pos_y * 0.5f);
            float translatedY = (pos_x * 0.25f) + (pos_y * 0.25f);

            pos_x = translatedX * (15f / 16f);
            pos_y = translatedY * (15f / 16f);

            _phaseShift = delay * 16f;
            _amplitude = -(10f / (float)Math.Sqrt(1f + delay)) / 1.5f;

            _sortingOrder = (int)((_originX + _originY) * 4);

            if (a == 0) a = 255;
        }

        public void OnUpdate(int id)
        {
            float frame = (float)Application.GetFrame();
            float wave = (float)Math.Sin((frame + _phaseShift) * _freq);
            float yToUse = (wave * _amplitude) + pos_y;

            Image.DrawEx(
                sprite,
                pos_x,
                yToUse,
                0,      
                5.0f,   
                5.0f,   
                0.5f,   
                0.5f,   
                r, g, b, a,
                _sortingOrder
            );
        }
    }
}
