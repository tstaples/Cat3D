using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace Editor
{
    [TypeConverter(typeof(ComponentConverter))]
    [CategoryAttribute("Components")]
    public class Component
    {
        public string DisplayName;
        public string Name;
        public string Description;
        public Field[] Fields;

        public Field GetField(string name)
        {
            foreach (Field field in Fields)
            {
                if (field.name == name)
                {
                    return field;
                }
            }
            return null;
        }

        public virtual void OnModify(string propertyName, object newVal)
        {
            GetField(propertyName).value = newVal;
        }
    }

    internal class ComponentConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is Component)
            {
                return "";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }

    public class ComponentCollectionConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is ComponentCollection)
            {
                return "";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }

    public class ComponentCollection : CollectionBase, ICustomTypeDescriptor
    {
        #region CollectionBase impl
        public void Add(Component c)
        {
            this.List.Add(c);
        }
        public void Remove(Component c)
        {
            this.List.Remove(c);
        }
        public Component this[int index]
        {
            get { return (Component)this.List[index]; }
        }
        #endregion
        #region ICustomTypeDescriptor impl
        public String GetClassName()
        {
            return TypeDescriptor.GetClassName(this, true);
        }

        public AttributeCollection GetAttributes()
        {
            return TypeDescriptor.GetAttributes(this, true);
        }

        public String GetComponentName()
        {
            return TypeDescriptor.GetComponentName(this, true);
        }

        public TypeConverter GetConverter()
        {
            return TypeDescriptor.GetConverter(this, true);
        }

        public EventDescriptor GetDefaultEvent()
        {
            return TypeDescriptor.GetDefaultEvent(this, true);
        }

        public PropertyDescriptor GetDefaultProperty()
        {
            return TypeDescriptor.GetDefaultProperty(this, true);
        }

        public object GetEditor(Type editorBaseType)
        {
            return TypeDescriptor.GetEditor(this, editorBaseType, true);
        }

        public EventDescriptorCollection GetEvents(Attribute[] attributes)
        {
            return TypeDescriptor.GetEvents(this, attributes, true);
        }

        public EventDescriptorCollection GetEvents()
        {
            return TypeDescriptor.GetEvents(this, true);
        }

        public object GetPropertyOwner(PropertyDescriptor pd)
        {
            return this;
        }
        #endregion

        public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
        {
            return GetProperties();
        }

        public PropertyDescriptorCollection GetProperties()
        {
            PropertyDescriptorCollection pds = new PropertyDescriptorCollection(null);
            for (int i=0; i < this.List.Count; ++i)
            {
                ComponentCollectionPropertyDescriptor pd = new ComponentCollectionPropertyDescriptor(this, i);
                pds.Add(pd);
            }
            return pds;
        }
    }

    public class ComponentCollectionPropertyDescriptor : PropertyDescriptor
    {
        private ComponentCollection collection = null;
        private int index = -1;

        public ComponentCollectionPropertyDescriptor(ComponentCollection col, int idx)
            : base("#" + idx.ToString(), null)
        {
            collection = col;
            index = idx;
        }

        public override AttributeCollection Attributes
        {
            get
            { return new AttributeCollection(null); }
        }

        public override bool CanResetValue(object component)
        {
            return true;
        }

        public override Type ComponentType
        {
            get { return this.collection.GetType(); }
        }

        public override string DisplayName
        {
            get
            {
                // Component name
                return collection[index].DisplayName;
            }
        }

        public override string Description
        {
            get { return collection[index].Description; }
        }

        public override object GetValue(object component)
        {
            return this.collection[index];
        }

        public override bool IsReadOnly
        {
            get { return true; }
        }

        public override string Name
        {
            get { return "#" + index.ToString(); }
        }

        public override Type PropertyType
        {
            get { return this.collection[index].GetType(); }
        }

        public override void ResetValue(object component) { }

        public override bool ShouldSerializeValue(object component)
        {
            return true;
        }

        public override void SetValue(object component, object value)
        {
            // this.collection[index] = value;
        }
    }
}
