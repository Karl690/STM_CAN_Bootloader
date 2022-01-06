
namespace HyrelCanAnalyzer
{
    partial class Mainform
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Mainform));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.cANToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.connectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemConnectCan1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemConnectCan2 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemDisconnect = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemExit = new System.Windows.Forms.ToolStripMenuItem();
            this.sETTINGSToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sTARTToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sTOPToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripCmbCanChannel = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripButtonConnect = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripCmbFilterHead = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripCmbFilterMessage = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripCapture = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.listView1 = new System.Windows.Forms.ListView();
            this.menuStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cANToolStripMenuItem,
            this.sETTINGSToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(919, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // cANToolStripMenuItem
            // 
            this.cANToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.connectToolStripMenuItem,
            this.toolStripMenuItemDisconnect,
            this.toolStripMenuItemExit});
            this.cANToolStripMenuItem.Name = "cANToolStripMenuItem";
            this.cANToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.cANToolStripMenuItem.Text = "CAN";
            // 
            // connectToolStripMenuItem
            // 
            this.connectToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemConnectCan1,
            this.toolStripMenuItemConnectCan2});
            this.connectToolStripMenuItem.Name = "connectToolStripMenuItem";
            this.connectToolStripMenuItem.Size = new System.Drawing.Size(133, 22);
            this.connectToolStripMenuItem.Text = "Connect";
            // 
            // toolStripMenuItemConnectCan1
            // 
            this.toolStripMenuItemConnectCan1.Name = "toolStripMenuItemConnectCan1";
            this.toolStripMenuItemConnectCan1.Size = new System.Drawing.Size(105, 22);
            this.toolStripMenuItemConnectCan1.Text = "CAN1";
            this.toolStripMenuItemConnectCan1.Click += new System.EventHandler(this.toolStripMenuItemConnectCan1_Click);
            // 
            // toolStripMenuItemConnectCan2
            // 
            this.toolStripMenuItemConnectCan2.Name = "toolStripMenuItemConnectCan2";
            this.toolStripMenuItemConnectCan2.Size = new System.Drawing.Size(105, 22);
            this.toolStripMenuItemConnectCan2.Text = "CAN2";
            this.toolStripMenuItemConnectCan2.Click += new System.EventHandler(this.toolStripMenuItemConnectCan2_Click);
            // 
            // toolStripMenuItemDisconnect
            // 
            this.toolStripMenuItemDisconnect.Name = "toolStripMenuItemDisconnect";
            this.toolStripMenuItemDisconnect.Size = new System.Drawing.Size(133, 22);
            this.toolStripMenuItemDisconnect.Text = "Disconnect";
            this.toolStripMenuItemDisconnect.Click += new System.EventHandler(this.toolStripMenuItemDisconnect_Click);
            // 
            // toolStripMenuItemExit
            // 
            this.toolStripMenuItemExit.Name = "toolStripMenuItemExit";
            this.toolStripMenuItemExit.Size = new System.Drawing.Size(133, 22);
            this.toolStripMenuItemExit.Text = "Exit";
            // 
            // sETTINGSToolStripMenuItem
            // 
            this.sETTINGSToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sTARTToolStripMenuItem,
            this.sTOPToolStripMenuItem});
            this.sETTINGSToolStripMenuItem.Name = "sETTINGSToolStripMenuItem";
            this.sETTINGSToolStripMenuItem.Size = new System.Drawing.Size(70, 20);
            this.sETTINGSToolStripMenuItem.Text = "CAPTURE";
            // 
            // sTARTToolStripMenuItem
            // 
            this.sTARTToolStripMenuItem.Name = "sTARTToolStripMenuItem";
            this.sTARTToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.sTARTToolStripMenuItem.Text = "START";
            // 
            // sTOPToolStripMenuItem
            // 
            this.sTOPToolStripMenuItem.Name = "sTOPToolStripMenuItem";
            this.sTOPToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.sTOPToolStripMenuItem.Text = "STOP";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripCmbCanChannel,
            this.toolStripButtonConnect,
            this.toolStripSeparator1,
            this.toolStripCmbFilterHead,
            this.toolStripCmbFilterMessage,
            this.toolStripCapture,
            this.toolStripButton1});
            this.toolStrip1.Location = new System.Drawing.Point(0, 24);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(919, 31);
            this.toolStrip1.TabIndex = 1;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripCmbCanChannel
            // 
            this.toolStripCmbCanChannel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.toolStripCmbCanChannel.Items.AddRange(new object[] {
            "CAN1",
            "CAN2"});
            this.toolStripCmbCanChannel.Name = "toolStripCmbCanChannel";
            this.toolStripCmbCanChannel.Size = new System.Drawing.Size(121, 31);
            // 
            // toolStripButtonConnect
            // 
            this.toolStripButtonConnect.AutoSize = false;
            this.toolStripButtonConnect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonConnect.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonConnect.Image")));
            this.toolStripButtonConnect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonConnect.Name = "toolStripButtonConnect";
            this.toolStripButtonConnect.Size = new System.Drawing.Size(80, 28);
            this.toolStripButtonConnect.Text = "Connect";
            this.toolStripButtonConnect.Click += new System.EventHandler(this.toolStripButtonConnect_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 31);
            // 
            // toolStripCmbFilterHead
            // 
            this.toolStripCmbFilterHead.Name = "toolStripCmbFilterHead";
            this.toolStripCmbFilterHead.Size = new System.Drawing.Size(121, 31);
            this.toolStripCmbFilterHead.TextChanged += new System.EventHandler(this.toolStripCmbFilterHead_TextChanged);
            // 
            // toolStripCmbFilterMessage
            // 
            this.toolStripCmbFilterMessage.Name = "toolStripCmbFilterMessage";
            this.toolStripCmbFilterMessage.Size = new System.Drawing.Size(220, 31);
            this.toolStripCmbFilterMessage.TextChanged += new System.EventHandler(this.toolStripCmbFilterMessage_TextChanged);
            // 
            // toolStripCapture
            // 
            this.toolStripCapture.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripCapture.Image = global::HyrelCanAnalyzer.Properties.Resources.start_disable;
            this.toolStripCapture.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripCapture.Name = "toolStripCapture";
            this.toolStripCapture.Size = new System.Drawing.Size(23, 28);
            this.toolStripCapture.Text = "Start";
            this.toolStripCapture.Click += new System.EventHandler(this.toolStripCapture_Click);
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.Image = global::HyrelCanAnalyzer.Properties.Resources.clear;
            this.toolStripButton1.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(28, 28);
            this.toolStripButton1.Text = "toolStripButtonClear";
            this.toolStripButton1.Click += new System.EventHandler(this.toolStripButton1_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // listView1
            // 
            this.listView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listView1.GridLines = true;
            this.listView1.HideSelection = false;
            this.listView1.Location = new System.Drawing.Point(0, 55);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(919, 462);
            this.listView1.TabIndex = 2;
            this.listView1.UseCompatibleStateImageBehavior = false;
            // 
            // Mainform
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(919, 517);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Mainform";
            this.Text = "Hyrel CAN Analyzer";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Mainform_FormClosed);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem cANToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem connectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemConnectCan1;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemConnectCan2;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemDisconnect;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemExit;
        private System.Windows.Forms.ToolStripMenuItem sETTINGSToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sTARTToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sTOPToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripComboBox toolStripCmbCanChannel;
        private System.Windows.Forms.ToolStripButton toolStripCapture;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripComboBox toolStripCmbFilterHead;
        private System.Windows.Forms.ToolStripComboBox toolStripCmbFilterMessage;
        private System.Windows.Forms.ToolStripButton toolStripButtonConnect;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.ListView listView1;
    }
}

