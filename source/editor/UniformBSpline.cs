using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;

namespace TerrenosHelper
{
    class UniformBSpline : CubicCurve
    {
        public UniformBSpline()
        {
            _basisMatrix = new Matrix4(new Vector4(1, -3, 3, -1),
                                       new Vector4(4, 0, -6, 3),
                                       new Vector4(1, 3, 3, -3),
                                       new Vector4(0, 0, 0, 1));

            _basisMatrix *= 1/6f;
        }
    }
}
