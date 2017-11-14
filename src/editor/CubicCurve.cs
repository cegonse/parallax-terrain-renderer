using System;
using System.Collections;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;

namespace TerrenosHelper
{
    class CubicCurve
    {
        protected Matrix4 _basisMatrix;
        private ArrayList _controlPoints;

        public CubicCurve()
        {
            _controlPoints = new ArrayList();
            _basisMatrix = Matrix4.Identity;
        }

        public void AddControlPoint(Vector2 cp)
        {
            if (_controlPoints.Count == 4)
            {
                throw new ArithmeticException();
            }
            else
            {
                _controlPoints.Add(cp);
            }
        }

        public void ResetControlPoints()
        {
            _controlPoints.Clear();
        }

        public Vector2 Evaluate(float t)
        {
            if (_controlPoints.Count != 4)
            {
                throw new ArithmeticException();
            }

            if (t == 0f)
            {
                return (Vector2)_controlPoints[0];
            }
            else if (t == 1f)
            {
                return (Vector2)_controlPoints[3];
            }

            Vector2 result = new Vector2();

            float P0x = ((Vector2)_controlPoints[0]).X;
            float P1x = ((Vector2)_controlPoints[1]).X;
            float P2x = ((Vector2)_controlPoints[2]).X;
            float P3x = ((Vector2)_controlPoints[3]).X;

            float P0y = ((Vector2)_controlPoints[0]).Y;
            float P1y = ((Vector2)_controlPoints[1]).Y;
            float P2y = ((Vector2)_controlPoints[2]).Y;
            float P3y = ((Vector2)_controlPoints[3]).Y;

            float Bm11 = _basisMatrix.M11;
            float Bm12 = _basisMatrix.M12;
            float Bm13 = _basisMatrix.M13;
            float Bm14 = _basisMatrix.M14;

            float Bm21 = _basisMatrix.M21;
            float Bm22 = _basisMatrix.M22;
            float Bm23 = _basisMatrix.M23;
            float Bm24 = _basisMatrix.M24;

            float Bm31 = _basisMatrix.M31;
            float Bm32 = _basisMatrix.M32;
            float Bm33 = _basisMatrix.M33;
            float Bm34 = _basisMatrix.M34;

            float Bm41 = _basisMatrix.M41;
            float Bm42 = _basisMatrix.M42;
            float Bm43 = _basisMatrix.M43;
            float Bm44 = _basisMatrix.M44;

            result.X = (Bm14 * P0x + Bm24 * P1x + Bm34 * P2x + Bm44 * P3x) * (t * t * t) +
                       (Bm13 * P0x + Bm23 * P1x + Bm33 * P2x + Bm43 * P3x) * (t * t) +
                       (Bm12 * P0x + Bm22 * P1x + Bm32 * P2x + Bm42 * P3x) * t +
                        Bm11 * P0x + Bm21 * P1x + Bm31 * P2x + Bm41 * P3x;

            result.Y = (Bm14 * P0y + Bm24 * P1y + Bm34 * P2y + Bm44 * P3y) * (t * t * t) +
                       (Bm13 * P0y + Bm23 * P1y + Bm33 * P2y + Bm43 * P3y) * (t * t) + 
                       (Bm12 * P0y + Bm22 * P1y + Bm32 * P2y + Bm42 * P3y) * t + 
                       Bm11 * P0y + Bm21 * P1y + Bm31 * P2y + Bm41 * P3y;

            return result;
        }

        public Vector2 Derive(float t)
        {
            if (_controlPoints.Count != 4)
            {
                throw new ArithmeticException();
            }

            Vector2 result = new Vector2();

            float P0x = ((Vector2)_controlPoints[0]).X;
            float P1x = ((Vector2)_controlPoints[1]).X;
            float P2x = ((Vector2)_controlPoints[2]).X;
            float P3x = ((Vector2)_controlPoints[3]).X;

            float P0y = ((Vector2)_controlPoints[0]).Y;
            float P1y = ((Vector2)_controlPoints[1]).Y;
            float P2y = ((Vector2)_controlPoints[2]).Y;
            float P3y = ((Vector2)_controlPoints[3]).Y;

            float Bm11 = _basisMatrix.M11;
            float Bm12 = _basisMatrix.M12;
            float Bm13 = _basisMatrix.M13;
            float Bm14 = _basisMatrix.M14;

            float Bm21 = _basisMatrix.M21;
            float Bm22 = _basisMatrix.M22;
            float Bm23 = _basisMatrix.M23;
            float Bm24 = _basisMatrix.M24;

            float Bm31 = _basisMatrix.M31;
            float Bm32 = _basisMatrix.M32;
            float Bm33 = _basisMatrix.M33;
            float Bm34 = _basisMatrix.M34;

            float Bm41 = _basisMatrix.M41;
            float Bm42 = _basisMatrix.M42;
            float Bm43 = _basisMatrix.M43;
            float Bm44 = _basisMatrix.M44;

            result.X = (3 * t * t) * (Bm14 * P0x + Bm24 * P1x + Bm34 * P2x + Bm44 * P3x) + 
                       (Bm12 * P0x + Bm22 * P1x + Bm32 * P2x + Bm42 * P3x) + 
                       (2 * t) * (Bm13 * P0x + Bm23 * P1x + Bm33 * P2x + Bm43 * P3x);

            result.Y = (3 * t * t) * (Bm14 * P0y + Bm24 * P1y + Bm34 * P2y + Bm44 * P3y) + 
                       Bm12 * P0y + Bm22 * P1y + Bm32 * P2y + Bm42 * P3y + 
                       (2 * t) * (Bm13 * P0y + Bm23 * P1y + Bm33 * P2y + Bm43 * P3y);

            return result;
        }

        public float Integrate(float t)
        {
            if (_controlPoints.Count != 4)
            {
                throw new ArithmeticException();
            }

            float P0x = ((Vector2)_controlPoints[0]).X;
            float P1x = ((Vector2)_controlPoints[1]).X;
            float P2x = ((Vector2)_controlPoints[2]).X;
            float P3x = ((Vector2)_controlPoints[3]).X;

            float P0y = ((Vector2)_controlPoints[0]).Y;
            float P1y = ((Vector2)_controlPoints[1]).Y;
            float P2y = ((Vector2)_controlPoints[2]).Y;
            float P3y = ((Vector2)_controlPoints[3]).Y;

            float Bm11 = _basisMatrix.M11;
            float Bm12 = _basisMatrix.M12;
            float Bm13 = _basisMatrix.M13;
            float Bm14 = _basisMatrix.M14;

            float Bm21 = _basisMatrix.M21;
            float Bm22 = _basisMatrix.M22;
            float Bm23 = _basisMatrix.M23;
            float Bm24 = _basisMatrix.M24;

            float Bm31 = _basisMatrix.M31;
            float Bm32 = _basisMatrix.M32;
            float Bm33 = _basisMatrix.M33;
            float Bm34 = _basisMatrix.M34;

            float Bm41 = _basisMatrix.M41;
            float Bm42 = _basisMatrix.M42;
            float Bm43 = _basisMatrix.M43;
            float Bm44 = _basisMatrix.M44;

            Vector2 temp = new Vector2();

            temp.X = ((Bm14 * P0x) / 4 + (Bm24 * P1x) / 4 + (Bm34 * P2x) / 4 + 
                     (Bm44 * P3x) / 4) * (t * t * t * t) + ((Bm13 * P0x) / 3 + 
                     (Bm23 * P1x) / 3 + (Bm33 * P2x) / 3 + (Bm43 * P3x) / 3) * (t * t * t) + 
                     ((Bm12 * P0x) / 2 + (Bm22 * P1x) / 2 + (Bm32 * P2x) / 2 + 
                     (Bm42 * P3x) / 2) * (t * t) + 
                     (Bm11 * P0x + Bm21 * P1x + Bm31 * P2x + Bm41 * P3x) * t;

            temp.Y = ((Bm14 * P0y) / 4 + (Bm24 * P1y) / 4 + (Bm34 * P2y) / 4 + 
                     (Bm44 * P3y) / 4) * (t * t * t * t) + ((Bm13 * P0y) / 3 + 
                     (Bm23 * P1y) / 3 + (Bm33 * P2y) / 3 + (Bm43 * P3y) / 3) * (t * t * t) + 
                     ((Bm12 * P0y) / 2 + (Bm22 * P1y) / 2 + (Bm32 * P2y) / 2 + 
                     (Bm42 * P3y) / 2) * (t * t) + 
                     (Bm11 * P0y + Bm21 * P1y + Bm31 * P2y + Bm41 * P3y) * t;

            return temp.Length;
        }
    }
}
