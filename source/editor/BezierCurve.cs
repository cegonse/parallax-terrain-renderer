using System;
using System.Collections;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;

namespace TerrenosHelper
{
    class BezierCurve : CubicCurve
    {

        public BezierCurve()
        {
            _basisMatrix = new Matrix4(new Vector4(1, -3, 3, -1),
                                    new Vector4(0, 3, -6, 3),
                                    new Vector4(0, 0, 3, -3),
                                    new Vector4(0, 0, 0, 1));
        }
    }
}
