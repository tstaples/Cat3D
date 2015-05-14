using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Diagnostics;

namespace Editor
{
    // https://msdn.microsoft.com/en-us/library/aa302326.aspx
    [DescriptionAttribute("Object's Transform")]
    public class TransformComponent : Component
    {
        private Vector3 pos;
        private Vector3 rot;
        private Vector3 scale;

        #region accessors
        [CategoryAttribute("Transform")]
        public Vector3 Position
        {
            get { return pos; }
            set { pos = value; }
        }

        [CategoryAttribute("Transform")]
        public Vector3 Rotation
        {
            get { return rot; }
            set { rot = value; }
        }

        [CategoryAttribute("Transform")]
        public Vector3 Scale
        {
            get { return scale; }
            set { scale = value; }
        }
        #endregion accessors

        public TransformComponent()
        {
            base.Name = "TransformComponent";
        }

        public override void Load(Field[] fields)
        {
            // Store fields in base class
            base.Load(fields);

            float[] m = new float[3];
            Buffer.BlockCopy(fields[0].data, 0, m, 0, 3 * sizeof(float));
            pos = new Vector3(m[0], m[1], m[2]);
            Buffer.BlockCopy(fields[1].data, 0, m, 0, 3 * sizeof(float));
            rot = new Vector3(m[0], m[1], m[2]);
            Buffer.BlockCopy(fields[2].data, 0, m, 0, 3 * sizeof(float));
            scale = new Vector3(m[0], m[1], m[2]);
        }

        public override void OnModify(string propertyName, object newVal)
        {
            int fieldIndex = -1;
            Vector3 v = (Vector3)newVal;
            if (propertyName.ToLower() == "position")
            {
                Position = v;
                fieldIndex = 0;
            }
            else if (propertyName.ToLower() == "rotation")
            {
                Rotation = v;
                fieldIndex = 1;
            }
            else if (propertyName.ToLower() == "scale")
            {
                Scale = v;
                fieldIndex = 2;
            }
            // Update the field data so it can be serialized back out
            UpdateField(fieldIndex, v);
        }

        public override bool OnChildModified(string propertyName, string childName, object newVal)
        {
            float val = (float)newVal;
            if (propertyName.ToLower() == "position")
            {
                ParseVector(ref pos, childName, val);
                UpdateField(0, pos);
                return true;
            }
            else if (propertyName.ToLower() == "rotation")
            {
                ParseVector(ref rot, childName, val);
                UpdateField(1, rot);
                return true;
            }
            else if (propertyName.ToLower() == "scale")
            {
                ParseVector(ref scale, childName, val);
                UpdateField(2, scale);
                return true;
            }
            return false;
        }

        private void ParseVector(ref Vector3 v, string component, float val)
        {
            if (component == "X")
            {
                v.x = val;
            }
            else if (component == "Y")
            {
                v.y = val;
            }
            else if (component == "Z")
            {
                v.z = val;
            }
        }

        private void UpdateField(int index, Vector3 v)
        {
            float[] m = { v.x, v.y, v.z };
            Buffer.BlockCopy(m, 0, Fields[index].data, 0, 3 * sizeof(float));
        }
    }
}
