using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
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
        public Mainform()
        {
            InitializeComponent();
            InitControl();
            canInterface.CapturePacketEvent += CanInterface_CapturePacketEvent;
            this.WindowState = FormWindowState.Maximized;
        }

        

        public void InitControl()
        {
            toolStripCmbCanChannel.SelectedIndex = 0;

            listView1.View = View.Details;
            listView1.FullRowSelect = true;            
            listView1.Columns.Add("Frame ID", 80);
            listView1.Columns.Add("Data", 100);
            listView1.Columns.Add("SourceID", 200);
            listView1.Columns.Add("TargetID", 200);
            listView1.Columns.Add("MSGTYPE", 300);
            listView1.Columns.Add("MSGID", 200);
            listView1.Columns.Add("Content", 500);

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
                canInterface.Disconnect();
            else
                canInterface.Connect((uint)toolStripCmbCanChannel.SelectedIndex);
            UpdateControlStatus();
        }
        private void UpdateControlStatus()
        {
            if(canInterface.IsConnected)
            {
                toolStripMenuItemConnectCan1.Checked = canInterface.Channel == 0 ? true : false;
                toolStripMenuItemConnectCan2.Checked = canInterface.Channel == 0 ? false : true;
                toolStripCmbCanChannel.SelectedIndex = (int)canInterface.Channel;
                toolStripCmbCanChannel.Enabled = false;
                toolStripCapture.Image = Properties.Resources.start_enable;
                toolStripButtonConnect.Text = "Disconnect";
            }else
            {
                toolStripMenuItemConnectCan1.Checked = false;
                toolStripMenuItemConnectCan2.Checked = true;
                toolStripCmbCanChannel.Enabled = true;
                toolStripButtonConnect.Text = "Connect";
                toolStripCapture.Image = Properties.Resources.start_disable;
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

        }

        private void toolStripCapture_Click(object sender, EventArgs e)
        {
            if (!canInterface.IsConnected) return;
            if (canInterface.IsCapturing)
            {
                canInterface.IsCapturing = false;
                toolStripCapture.Image = Properties.Resources.start_enable;
            }
            else
            {
                canInterface.IsCapturing = true;
                toolStripCapture.Image = Properties.Resources.stop;
            }
                
            
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
            item.Tag = e.msgInfo;
            item.Text = e.msgInfo.ID.ToString("X");
            item.SubItems.Add(e.msgInfo.GetDataString());
            item.SubItems.Add(e.msgInfo.SourceID.ToString() + ":" + string.Format("0x{0:X2}", (int)e.msgInfo.SourceID));
            item.SubItems.Add(e.msgInfo.TargetID.ToString() + ":" + string.Format("0x{0:X2}", (int)e.msgInfo.TargetID));
            item.SubItems.Add(e.msgInfo.MsgType.ToString());
            item.SubItems.Add(e.msgInfo.MsgId.ToString());
            item.SubItems.Add(Hyrel.ProcessCanMessage(e.msgInfo.SourceID, e.msgInfo.MsgId, e.msgInfo.data));

            if (listView1.InvokeRequired)
            {
                listView1.Invoke((MethodInvoker)delegate ()
                {
                    listView1.Items.Add(item);
                    listView1.Items[listView1.Items.Count - 1].EnsureVisible();
                });
            }
            else
            {
                listView1.Items.Add(item);
                listView1.Items[listView1.Items.Count - 1].EnsureVisible();
            }
                


        }

        private void Mainform_FormClosed(object sender, FormClosedEventArgs e)
        {
            canInterface.Dispose();
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            listView1.Items.Clear();
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
    }
}
