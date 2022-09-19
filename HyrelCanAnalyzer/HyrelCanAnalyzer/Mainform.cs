using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HyrelCanAnalyzer
{
    public partial class Mainform : Form
    {
        CANInterface canInterface = new CANInterface();

        public List<HEADPOSITION> FilterHeads = new List<HEADPOSITION>();
        public List<CANMSGID> FilterMessageIds = new List<CANMSGID>();
        public string FilterHeadString = "";
        public string FilterMsgIdString = "";
        public bool IsRecording = false;
        public string defalutLogFile = "canbuslog.csv";
        public Mainform()
        {
            InitializeComponent();
            InitControl();
            this.Text = $"{RevisionHistory.RevisionHeader} {RevisionHistory.MajorStep}.{RevisionHistory.MinorStep} - {RevisionHistory.RevisionDate}";
            canInterface.CapturePacketEvent += CanInterface_CapturePacketEvent;
            this.WindowState = FormWindowState.Maximized;
            toolStripRunStop.Image = Properties.Resources.run;
            toolStripRecord.Image = Properties.Resources.record_start;
            defalutLogFile = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "/canbuslog.csv";
        }

        

        public void InitControl()
        {
            toolStripCmbCanChannel.SelectedIndex = 0;

            listviewCanTraffic.View = View.Details;
            listviewCanTraffic.FullRowSelect = true;
            listviewCanTraffic.Columns.Add("TIME", 80);
            listviewCanTraffic.Columns.Add("Frame ID", 100);
            listviewCanTraffic.Columns.Add("Data", 200);
            listviewCanTraffic.Columns.Add("SourceID", 180);
            listviewCanTraffic.Columns.Add("TargetID", 180);
            listviewCanTraffic.Columns.Add("MSGTYPE", 300);
            listviewCanTraffic.Columns.Add("MSGID", 200);
            listviewCanTraffic.Columns.Add("Content", 500);

            foreach (var head in Enum.GetValues(typeof(HEADPOSITION)).Cast<HEADPOSITION>().ToList())
            {
                toolStripCmbFilterHead.Items.Add(head.ToString());
            }
            foreach (var msg in Enum.GetValues(typeof(CANMSGID)).Cast<CANMSGID>().ToList())
            {
                toolStripCmbFilterMessage.Items.Add(msg.ToString());
            }
            toolStripCmbFilterMessage.Width = 200;
        }

        private void toolStripMenuItemConnectCan1_Click(object sender, EventArgs e)
        {
            canInterface.Connect(0);
            UpdateControlStatus();
        }

        private void toolStripMenuItemConnectCan2_Click(object sender, EventArgs e)
        {
            canInterface.Connect(1);
            UpdateControlStatus();
        }

        private void toolStripMenuItemDisconnect_Click(object sender, EventArgs e)
        {
            canInterface.Disconnect();
            UpdateControlStatus();
        }

        private void toolStripButtonConnect_Click(object sender, EventArgs e)
        {
            if (canInterface.IsConnected)
            {
                canInterface.Disconnect();
                canInterface.IsRunning = false;
                toolStripRunStop.Image = Properties.Resources.run;
            }
            else
            {
                canInterface.Connect((uint)toolStripCmbCanChannel.SelectedIndex);
                canInterface.IsRunning = true;
                toolStripRunStop.Image = Properties.Resources.stop;
            }
            UpdateControlStatus();
        }
        private void UpdateControlStatus()
        {
            if(canInterface.IsConnected)
            {
                toolStripCmbCanChannel.SelectedIndex = (int)canInterface.Channel;
                toolStripCmbCanChannel.Enabled = false;                
                toolStripButtonConnect.Text = "Disconnect";

            }else
            {
                toolStripCmbCanChannel.Enabled = true;
                toolStripButtonConnect.Text = "Connect";
                
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

        }

        private void toolStripCapture_Click(object sender, EventArgs e)
        {   
            if (canInterface.IsRunning)
            {
                canInterface.IsRunning = false;
                toolStripRunStop.Image = Properties.Resources.run;
            }
            else
            {
                canInterface.IsRunning = true;
                toolStripRunStop.Image = Properties.Resources.stop;
            }
        }

        private void RecordDataToFile(string line)
        {
            if (!IsRecording) return;
            bool bFirst = true;
            
            if (!File.Exists(defalutLogFile))
            {   
                File.WriteAllText(defalutLogFile, "TIME,Frame ID,Data,SourceID,TargetID,MSGTYPE,MSGID,Content");
                
            }
            File.AppendAllText(defalutLogFile, line);            
        }
        private void CanInterface_CapturePacketEvent(object sender, CanMessageEventArgs e)
        {   
            if (FilterHeadString != "")
            {
                int pos = FilterHeads.IndexOf(e.msgInfo.SourceID);
                if (pos < 0) return;
            }

            if (FilterMsgIdString != "")
            {
                int pos = FilterMessageIds.IndexOf(e.msgInfo.MsgId);
                if (pos < 0) return;
            }

            ListViewItem item = new ListViewItem();
            string line = "\n";
            item.Tag = e.msgInfo;
            string time = DateTime.Now.ToString("hh:mm:ss");
            string id = e.msgInfo.ID.ToString("X");
            string Data = e.msgInfo.GetDataString();
            string sourceId = e.msgInfo.SourceID.ToString() + ":" + string.Format("0x{0:X2}", (int)e.msgInfo.SourceID);
            string targetId = e.msgInfo.TargetID.ToString() + ":" + string.Format("0x{0:X2}", (int)e.msgInfo.TargetID);
            string msgType = e.msgInfo.MsgType.ToString();
            string msgId = e.msgInfo.MsgId.ToString();
            string Content = Hyrel.ProcessCanMessage(e.msgInfo.SourceID, e.msgInfo.MsgId, e.msgInfo.data);
            item.Text = time;
            item.SubItems.Add(id);
            item.SubItems.Add(Data);
            item.SubItems.Add(sourceId);
            item.SubItems.Add(targetId);
            item.SubItems.Add(msgType);
            item.SubItems.Add(msgId);
            item.SubItems.Add(Content);

            if (listviewCanTraffic.InvokeRequired)
            {
                listviewCanTraffic.Invoke((MethodInvoker)delegate ()
                {
                    listviewCanTraffic.Items.Add(item);
                    listviewCanTraffic.Items[listviewCanTraffic.Items.Count - 1].EnsureVisible();
                });
            }
            else
            {
                listviewCanTraffic.Items.Add(item);
                listviewCanTraffic.Items[listviewCanTraffic.Items.Count - 1].EnsureVisible();
            }

            if (IsRecording)
            {
                RecordDataToFile($"\n{time},{id},{Data},{sourceId},{targetId},{msgType},{msgId},{Content}");
            }

        }

        private void Mainform_FormClosed(object sender, FormClosedEventArgs e)
        {
            canInterface.Dispose();
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            File.Delete(defalutLogFile);
            listviewCanTraffic.Items.Clear();
        }

        private void toolStripCmbFilterHead_TextChanged(object sender, EventArgs e)
        {
            FilterHeads.Clear();
            FilterHeadString = toolStripCmbFilterHead.Text;
            foreach (var head in Enum.GetValues(typeof(HEADPOSITION)).Cast<HEADPOSITION>().ToList())
            {
                if(head.ToString().IndexOf(FilterHeadString) >= 0)
                {
                    FilterHeads.Add(head);
                }
            }
        }

        private void toolStripCmbFilterMessage_TextChanged(object sender, EventArgs e)
        {
            FilterMessageIds.Clear();
            FilterMsgIdString = toolStripCmbFilterMessage.Text;
            foreach (var msgid in Enum.GetValues(typeof(CANMSGID)).Cast<CANMSGID>().ToList())
            {
                if (msgid.ToString().IndexOf(FilterMsgIdString) >= 0)
                {
                    FilterMessageIds.Add(msgid);
                }
            }
        }

        private void toolStripRecord_Click(object sender, EventArgs e)
        {
            if(IsRecording)
            {
                IsRecording = false;
                File.Delete(defalutLogFile);
                toolStripRecord.Image = Properties.Resources.record_start;
            }else
            {
                IsRecording = true;
                toolStripRecord.Image = Properties.Resources.record_stop;
            }
            
        }

        private void toolStripOpenCSV_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "CSV files (*.csv)|*.csv|All files (*.*)|*.*";
            
            
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                IsRecording = false;
                toolStripRecord.Image = Properties.Resources.record_start;
                canInterface.IsRunning = false;
                toolStripRunStop.Image = Properties.Resources.run;
                string[] lines = File.ReadAllLines(ofd.FileName);
                bool bFirst = true;
                listviewCanTraffic.Items.Clear();
                foreach(var line in lines)
                {
                    if (bFirst)
                    {
                        bFirst = false;
                        continue;
                    }
                    string[] items = line.Split(',');
                    if (items.Length != 8) continue;
                    ListViewItem item = new ListViewItem();
                    item.Text = items[0];
                    item.SubItems.Add(items[1]);
                    item.SubItems.Add(items[2]);
                    item.SubItems.Add(items[3]);
                    item.SubItems.Add(items[4]);
                    item.SubItems.Add(items[5]);
                    item.SubItems.Add(items[6]);
                    item.SubItems.Add(items[7]);
                    listviewCanTraffic.Items.Add(item);
                    bFirst = false;
                }
                if(listviewCanTraffic.Items.Count > 0) listviewCanTraffic.Items[listviewCanTraffic.Items.Count - 1].EnsureVisible();
                listviewCanTraffic.Update();
            }
        }

        private void toolStripSaveCSV_Click(object sender, EventArgs e)
        {
            if(!File.Exists(defalutLogFile))
            {
                MessageBox.Show("There is no recorded data");
                return; 
            }
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "CSV files (*.csv)|*.csv|All files (*.*)|*.*";
            if(sfd.ShowDialog() == DialogResult.OK)
            {
                File.Copy(defalutLogFile, sfd.FileName, true);
            }
        }
    }
}
