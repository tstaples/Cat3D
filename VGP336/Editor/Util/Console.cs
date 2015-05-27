using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using System.Windows.Forms;
using System.IO;

namespace Editor
{
    public static class Console
    {
        private static ListView listView;

        public enum Priority
        {
            Emergency,	// System unstable
            Alert,		// Action must be taken immediatley
            Critical,	// Critical condition
            Error,		// Error conditions
            Warning,	// Warning conditions
            Notice,		// Normal but significant conditions
            Info,		// Informational
            Debug		// Debug-level messages
        };

        public static void Initialize(ref ListView list)
        {
            listView = list;
        }
        public static void Clear()
        {
            listView.Items.Clear();
        }
        public static void DumpToFile(string filename)
        {
            // TODO
        }

        public static void Log(string service, Priority priority, string msg, params object[] args)
        {
            string formattedMsg = String.Format(msg, args);
            string levelStr = priority.ToString();
            int hash = (service + levelStr + msg).GetHashCode();
            
            // Search for a message already containing this hash
            int index = GetIndex(hash);
            if (index != -1)
            {
                listView.Items[index].SubItems[2].Text = formattedMsg;
                // Increase the count for that message
                string countStr = listView.Items[index].SubItems[3].Text;
                int count = Convert.ToInt32(countStr);
                listView.Items[index].SubItems[3].Text = (++count).ToString();
            }
            else
            {
                // Message doesn't exist; update the count
                ListViewItem item = new ListViewItem(new[] { service, levelStr, formattedMsg, "0" });
                item.Tag = hash; // store hash so we don't have to re-compute later
                listView.Items.Add(item);
            }
        }
        public static void LogEmergency(string service, string msg, params object[] args)
        {
            Log(service, Priority.Emergency, msg, args);
        }
        public static void LogAlert(string service, string msg, params object[] args)
        {
            Log(service, Priority.Alert, msg, args);
        }
        public static void LogCritical(string service, string msg, params object[] args)
        {
            Log(service, Priority.Critical, msg, args);
        }
        public static void LogError(string service, string msg, params object[] args)
        {
            Log(service, Priority.Error, msg, args);
        }
        public static void LogWarning(string service, string msg, params object[] args)
        {
            Log(service, Priority.Warning, msg, args);
        }
        public static void LogNotice(string service, string msg, params object[] args)
        {
            Log(service, Priority.Notice, msg, args);
        }
        public static void LogInfo(string service, string msg, params object[] args)
        {
            Log(service, Priority.Info, msg, args);
        }
        public static void LogDebug(string service, string msg, params object[] args)
        {
            Log(service, Priority.Debug, msg, args);
        }

        private static int GetIndex(int hash)
        {
            foreach (ListViewItem item in listView.Items)
            {
                int itemHash = (int)item.Tag;
                if (itemHash == hash)
                {
                    return item.Index;
                }
            }
            return -1;
        }

        //private void DoWorkHandler(object sender, DoWorkEventArgs e)
        //{
        //    BackgroundWorker worker = sender as BackgroundWorker;
        //    e.Result = WriteAsync((LogEntry[])e.Argument);
        //}
        //private bool WriteAsync(LogEntry[] entries)
        //{
        //    FileStream stream = File.Open(LogFile, FileMode.Append);
        //    if (stream.CanWrite)
        //    {
        //        string nl = Environment.NewLine;
        //        foreach (LogEntry entry in entries)
        //        {
        //            // Append a newline to each entry
        //            byte[] bytes = Util.GetBytes(entry.Format() + nl);
        //            stream.Write(bytes, 0, bytes.Length);
        //        }
        //        stream.Close();
        //        return true;
        //    }
        //    return false;
        //}
    }
}
