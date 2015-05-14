using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data;

namespace Editor
{
    public class Inspector
    {
        private PropertyGrid GridView;
        private List<Component> Components;

        public Inspector(ref PropertyGrid gridView)
        {
            Components = new List<Component>();
            GridView = gridView;
        }

        public void Clear()
        {
            Components.Clear();
        }

        public void AddComponent(Component component)
        {
            GridView.SelectedObject = component;

            //Dictionary<string, string> keyVals = component.GetFormattedFields();
            //foreach (KeyValuePair<string, string> pair in keyVals)
            //{
            //    string[] data = new string[]{pair.Key, pair.Value};
            //    OutlookGridRow row = new OutlookGridRow();
            //    row.CreateCells(GridView, data);
            //    GridView.Rows.Add(row);
            //}
            //GridView.BindData(arrayList, component.Name);
            Components.Add(component);
        }

        public void OnComponentModified(string name, string propertyName, object newVal)
        {
            foreach (Component c in Components)
            {
                // Hack: match based on substr. Won't work (likely) later when there are more properties
                if (c.Name.Contains(name))
                {
                    c.OnModify(propertyName, newVal);

                    byte[] buffer = ComponentReader.WriteComponent(c);
                    NativeMethods.UpdateComponent(buffer, (uint)buffer.Length);
                    GridView.SelectedObject = c;
                    break;
                }
            }
        }
    }
}
