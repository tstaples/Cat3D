using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.Diagnostics;

namespace Editor
{
    [StructLayout(LayoutKind.Sequential),
     TypeConverterAttribute(typeof(VectorConverter))]
    public class Vector3
    {
        public float x, y, z;

        #region accessors
        [CategoryAttribute("X")]
        public float X
        {
            get { return x; }
            set { x = value; }
        }
        [CategoryAttribute("Y")]
        public float Y
        {
            get { return y; }
            set { y = value; }
        }
        [CategoryAttribute("Z")]
        public float Z
        {
            get { return z; }
            set { z = value; }
        }
        #endregion

        #region constructor
        public Vector3()
        {
        }

        public Vector3(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }
        #endregion

        public static implicit operator Vector3(byte[] bytes)
        {
            Debug.Assert(bytes.Length == 12);
            float[] m = new float[3];
            int size = 3 * sizeof(float);
            Buffer.BlockCopy(bytes, 0, m, 0, size);
            return new Vector3(m[0], m[1], m[2]);
        }

        public override string ToString()
        {
            return x.ToString() + " " + y.ToString() + " " + z.ToString();
        }
    }

    #region Vector expandable object converter
    public class VectorConverter : ExpandableObjectConverter
    {
        #region property sheet conversion methods
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(Vector3))
            {
                return true;
            }
            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
        {
            if (destinationType == typeof(string) && value is Vector3)
            {
                Vector3 v = (Vector3)value;
                return v.ToString();
            }
            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
            {
                return true;
            }
            return base.CanConvertFrom(context, sourceType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value)
        {
            if (value is string)
            {
                try
                {
                    string s = (string)value;
                    int space1 = s.IndexOf(" ");
                    int space2 = s.IndexOf(" ", space1 + 1);
                    string sx = s.Substring(0, space1);
                    string sy = s.Substring(space1 + 1, space2 - (space1 + 1));
                    string sz = s.Substring(space2 + 1, s.Length - (space2 + 1));

                    // Resets invalid values to 0; much nicer than annoying exception
                    float x = 0.0f, y = 0.0f, z = 0.0f;
                    float.TryParse(sx, out x);
                    float.TryParse(sy, out y);
                    float.TryParse(sz, out z);

                    return new Vector3(x, y, z);
                }
                catch
                {
                    throw new ArgumentException("Cannot convert '" + (string)value + "' to type Vector3");
                }
            }
            return base.ConvertFrom(context, culture, value);
        }
        #endregion
    }
    #endregion Vector expandable object converter
}
