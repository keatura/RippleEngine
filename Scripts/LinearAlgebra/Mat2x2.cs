using Engine.LinearAlgebra;
using System;
using System.Collections.Generic;
using System.Data.Common;
using System.Runtime.InteropServices;
using System.Text;

namespace Engine.LinearAlgebra
{
    public class Mat2x2
    {
        Vector2 col0 = new Vector2(0.0f,0.0f);
        Vector2 col1 = new Vector2(0.0f, 0.0f);

        public Mat2x2() { }
        public Mat2x2(Vector2 col0, Vector2 col1) {
            this.col0 = col0;
            this.col1 = col1;
        }

        public Mat2x2(Mat2x2 other)
        {
            this.col0 = other.col0;
            this.col1 = other.col1;
        }

        public static Vector2 operator*(Mat2x2 mat, Vector2 vec)
        {
            return (mat.col0 * vec.x) + (mat.col1 * vec.y);
        }

        public static Mat2x2 operator*(Mat2x2 mat, float scalar)
        {
            return new Mat2x2(mat.col0*scalar, mat.col1*scalar);
        }
        public static Mat2x2 operator*(float scalar, Mat2x2 mat)
        {
            return mat * scalar;
        }

        public static Mat2x2 operator*(Mat2x2 mat1, Mat2x2 mat2)
        {
            return new Mat2x2( (mat1) * mat2.col0, (mat2) * mat2.col1);
        }
    }
}
