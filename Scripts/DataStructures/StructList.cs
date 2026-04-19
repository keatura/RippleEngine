using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

namespace Scripts.DataStructures
{
    public class StructList<T> where T : struct
    {
        public T[] Data;
        public int Count { get; private set; }

        public StructList(int initial_capacity = 256)
        {
            Data = new T[initial_capacity];
            Count = 0;
        }

        public void Add(T item)
        {
            if (Count >= Data.Length)
            {
                Array.Resize(ref Data, Data.Length * 2);
            }

            Data[Count] = item;
            Count++;
        }

        public void RemoveAt(int index)
        {
            if (index < 0 || index >= Count) return;

            // pretty much just swap and pop
            Data[index] = Data[Count - 1];
            Data[Count - 1] = default(T);
            Count--;
        }

        public ref T this[int index]
        {
            get { return ref Data[index]; }
        }
    }
}
