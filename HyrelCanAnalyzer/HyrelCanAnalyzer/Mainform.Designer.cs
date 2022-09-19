
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
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripCmbCanChannel = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripButtonConnect = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripCmbFilterHead = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripCmbFilterMessage = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripRunStop = new System.Windows.Forms.ToolStripButton();
            this.toolStripRecord = new System.Windows.Forms.ToolStripButton();
            this.toolStripClear = new System.Windows.Forms.ToolStripButton();
            this.toolStripOpenCSV = new System.Windows.Forms.ToolStripButton();
            this.toolStripSaveCSV = new System.Windows.Forms.ToolStripButton();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.listviewCanTraffic = new System.Windows.Forms.ListView();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.AutoSize = false;
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(48, 48);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripCmbCanChannel,
            this.toolStripButtonConnect,
            this.toolStripSeparator1,
            this.toolStripCmbFilterHead,
            this.toolStripCmbFilterMessage,
            this.toolStripRunStop,
            this.toolStripRecord,
            this.toolStripClear,
            this.toolStripOpenCSV,
            this.toolStripSaveCSV});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(1378, 36);
            this.toolStrip1.TabIndex = 1;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripCmbCanChannel
            // 
            this.toolStripCmbCanChannel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.toolStripCmbCanChannel.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripCmbCanChannel.Items.AddRange(new object[] {
            "CAN1",
            "CAN2"});
            this.toolStripCmbCanChannel.Name = "toolStripCmbCanChannel";
            this.toolStripCmbCanChannel.Size = new System.Drawing.Size(90, 36);
            this.toolStripCmbCanChannel.ToolTipText = "PIck Channel to monoitor";
            // 
            // toolStripButtonConnect
            // 
            this.toolStripButtonConnect.AutoSize = false;
            this.toolStripButtonConnect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonConnect.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripButtonConnect.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.toolStripButtonConnect.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripButtonConnect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonConnect.Name = "toolStripButtonConnect";
            this.toolStripButtonConnect.Size = new System.Drawing.Size(100, 28);
            this.toolStripButtonConnect.Text = "Connect";
            this.toolStripButtonConnect.Click += new System.EventHandler(this.toolStripButtonConnect_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 36);
            // 
            // toolStripCmbFilterHead
            // 
            this.toolStripCmbFilterHead.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripCmbFilterHead.Name = "toolStripCmbFilterHead";
            this.toolStripCmbFilterHead.Size = new System.Drawing.Size(179, 36);
            this.toolStripCmbFilterHead.TextChanged += new System.EventHandler(this.toolStripCmbFilterHead_TextChanged);
            // 
            // toolStripCmbFilterMessage
            // 
            this.toolStripCmbFilterMessage.Name = "toolStripCmbFilterMessage";
            this.toolStripCmbFilterMessage.Size = new System.Drawing.Size(328, 36);
            this.toolStripCmbFilterMessage.TextChanged += new System.EventHandler(this.toolStripCmbFilterMessage_TextChanged);
            // 
            // toolStripRunStop
            // 
            this.toolStripRunStop.AutoSize = false;
            this.toolStripRunStop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripRunStop.Image = global::HyrelCanAnalyzer.Properties.Resources.stop;
            this.toolStripRunStop.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripRunStop.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripRunStop.Name = "toolStripRunStop";
            this.toolStripRunStop.Size = new System.Drawing.Size(40, 36);
            this.toolStripRunStop.Text = "Start";
            this.toolStripRunStop.ToolTipText = "Run/Stop";
            this.toolStripRunStop.Click += new System.EventHandler(this.toolStripCapture_Click);
            // 
            // toolStripRecord
            // 
            this.toolStripRecord.AutoSize = false;
            this.toolStripRecord.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripRecord.Image = global::HyrelCanAnalyzer.Properties.Resources.record_start;
            this.toolStripRecord.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripRecord.ImageTransparentColor = System.Drawing.Color.Lime;
            this.toolStripRecord.Name = "toolStripRecord";
            this.toolStripRecord.Size = new System.Drawing.Size(40, 36);
            this.toolStripRecord.Text = "toolStripButton2";
            this.toolStripRecord.ToolTipText = "Record";
            this.toolStripRecord.Click += new System.EventHandler(this.toolStripRecord_Click);
            // 
            // toolStripClear
            // 
            this.toolStripClear.AutoSize = false;
            this.toolStripClear.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripClear.Image = global::HyrelCanAnalyzer.Properties.Resources.clear;
            this.toolStripClear.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripClear.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripClear.Name = "toolStripClear";
            this.toolStripClear.Size = new System.Drawing.Size(40, 36);
            this.toolStripClear.Text = "toolStripButtonClear";
            this.toolStripClear.ToolTipText = "Remove all items in List";
            this.toolStripClear.Click += new System.EventHandler(this.toolStripButton1_Click);
            // 
            // toolStripOpenCSV
            // 
            this.toolStripOpenCSV.AutoSize = false;
            this.toolStripOpenCSV.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripOpenCSV.Image = global::HyrelCanAnalyzer.Properties.Resources.open;
            this.toolStripOpenCSV.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripOpenCSV.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripOpenCSV.Name = "toolStripOpenCSV";
            this.toolStripOpenCSV.Size = new System.Drawing.Size(40, 36);
            this.toolStripOpenCSV.Text = "toolStripButton3";
            this.toolStripOpenCSV.ToolTipText = "Open CSV";
            this.toolStripOpenCSV.Click += new System.EventHandler(this.toolStripOpenCSV_Click);
            // 
            // toolStripSaveCSV
            // 
            this.toolStripSaveCSV.AutoSize = false;
            this.toolStripSaveCSV.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripSaveCSV.Image = global::HyrelCanAnalyzer.Properties.Resources.save;
            this.toolStripSaveCSV.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripSaveCSV.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSaveCSV.Name = "toolStripSaveCSV";
            this.toolStripSaveCSV.Size = new System.Drawing.Size(40, 36);
            this.toolStripSaveCSV.Text = "toolStripButton4";
            this.toolStripSaveCSV.ToolTipText = "Save CSV";
            this.toolStripSaveCSV.Click += new System.EventHandler(this.toolStripSaveCSV_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // listviewCanTraffic
            // 
            this.listviewCanTraffic.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listviewCanTraffic.GridLines = true;
            this.listviewCanTraffic.HideSelection = false;
            this.listviewCanTraffic.Location = new System.Drawing.Point(0, 36);
            this.listviewCanTraffic.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.listviewCanTraffic.Name = "listviewCanTraffic";
            this.listviewCanTraffic.Size = new System.Drawing.Size(1378, 799);
            this.listviewCanTraffic.TabIndex = 2;
            this.listviewCanTraffic.UseCompatibleStateImageBehavior = false;
            // 
            // Mainform
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 21F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1378, 835);
            this.Controls.Add(this.listviewCanTraffic);
            this.Controls.Add(this.toolStrip1);
            this.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "Mainform";
            this.Text = "Hyrel CAN Analyzer";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Mainform_FormClosed);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripComboBox toolStripCmbCanChannel;
        private System.Windows.Forms.ToolStripButton toolStripRunStop;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripComboBox toolStripCmbFilterHead;
        private System.Windows.Forms.ToolStripComboBox toolStripCmbFilterMessage;
        private System.Windows.Forms.ToolStripButton toolStripButtonConnect;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ToolStripButton toolStripClear;
        private System.Windows.Forms.ListView listviewCanTraffic;
        private System.Windows.Forms.ToolStripButton toolStripRecord;
        private System.Windows.Forms.ToolStripButton toolStripOpenCSV;
        private System.Windows.Forms.ToolStripButton toolStripSaveCSV;
    }
}

