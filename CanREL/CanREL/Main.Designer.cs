
namespace CanREL
{
    partial class Main
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
            this.btnConnect = new System.Windows.Forms.Button();
            this.lstReceivedMessage = new System.Windows.Forms.ListView();
            this.btnSendMessage = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.cmbFunctions = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.cmbHeadIDs = new System.Windows.Forms.ComboBox();
            this.txtData0 = new System.Windows.Forms.TextBox();
            this.txtFrameID = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txtData1 = new System.Windows.Forms.TextBox();
            this.txtData2 = new System.Windows.Forms.TextBox();
            this.txtData3 = new System.Windows.Forms.TextBox();
            this.txtData4 = new System.Windows.Forms.TextBox();
            this.txtData5 = new System.Windows.Forms.TextBox();
            this.txtData6 = new System.Windows.Forms.TextBox();
            this.txtData7 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.RdoCan1 = new System.Windows.Forms.RadioButton();
            this.RdoCan2 = new System.Windows.Forms.RadioButton();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.btnCapture = new System.Windows.Forms.Button();
            this.CmbCanMode = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.btnReadFirmware = new System.Windows.Forms.Button();
            this.btnWriteFirmware = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(18, 18);
            this.btnConnect.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(130, 33);
            this.btnConnect.TabIndex = 0;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // lstReceivedMessage
            // 
            this.lstReceivedMessage.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lstReceivedMessage.GridLines = true;
            this.lstReceivedMessage.HideSelection = false;
            this.lstReceivedMessage.Location = new System.Drawing.Point(18, 61);
            this.lstReceivedMessage.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.lstReceivedMessage.Name = "lstReceivedMessage";
            this.lstReceivedMessage.Size = new System.Drawing.Size(1192, 346);
            this.lstReceivedMessage.TabIndex = 1;
            this.lstReceivedMessage.UseCompatibleStateImageBehavior = false;
            // 
            // btnSendMessage
            // 
            this.btnSendMessage.Location = new System.Drawing.Point(904, 436);
            this.btnSendMessage.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnSendMessage.Name = "btnSendMessage";
            this.btnSendMessage.Size = new System.Drawing.Size(98, 55);
            this.btnSendMessage.TabIndex = 0;
            this.btnSendMessage.Text = "Send";
            this.btnSendMessage.UseVisualStyleBackColor = true;
            this.btnSendMessage.Click += new System.EventHandler(this.btnSendMessage_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(257, 432);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 20);
            this.label1.TabIndex = 3;
            this.label1.Text = "Function List";
            // 
            // cmbFunctions
            // 
            this.cmbFunctions.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbFunctions.FormattingEnabled = true;
            this.cmbFunctions.Location = new System.Drawing.Point(249, 457);
            this.cmbFunctions.Name = "cmbFunctions";
            this.cmbFunctions.Size = new System.Drawing.Size(203, 28);
            this.cmbFunctions.TabIndex = 4;
            this.cmbFunctions.SelectedIndexChanged += new System.EventHandler(this.cmbHeadFunction_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(24, 434);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(69, 20);
            this.label2.TabIndex = 3;
            this.label2.Text = "Head ID";
            // 
            // cmbHeadIDs
            // 
            this.cmbHeadIDs.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbHeadIDs.FormattingEnabled = true;
            this.cmbHeadIDs.Location = new System.Drawing.Point(27, 457);
            this.cmbHeadIDs.Name = "cmbHeadIDs";
            this.cmbHeadIDs.Size = new System.Drawing.Size(216, 28);
            this.cmbHeadIDs.TabIndex = 4;
            this.cmbHeadIDs.SelectedIndexChanged += new System.EventHandler(this.cmbHeadFunction_SelectedIndexChanged);
            // 
            // txtData0
            // 
            this.txtData0.Location = new System.Drawing.Point(572, 459);
            this.txtData0.Name = "txtData0";
            this.txtData0.Size = new System.Drawing.Size(34, 26);
            this.txtData0.TabIndex = 5;
            this.txtData0.Text = "00";
            this.txtData0.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtFrameID
            // 
            this.txtFrameID.Location = new System.Drawing.Point(458, 459);
            this.txtFrameID.Name = "txtFrameID";
            this.txtFrameID.ReadOnly = true;
            this.txtFrameID.Size = new System.Drawing.Size(108, 26);
            this.txtFrameID.TabIndex = 5;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(464, 436);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(76, 20);
            this.label3.TabIndex = 3;
            this.label3.Text = "Frame ID";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(573, 432);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(153, 20);
            this.label4.TabIndex = 3;
            this.label4.Text = "Frame Data(8Bytes)";
            // 
            // txtData1
            // 
            this.txtData1.Location = new System.Drawing.Point(612, 459);
            this.txtData1.Name = "txtData1";
            this.txtData1.Size = new System.Drawing.Size(34, 26);
            this.txtData1.TabIndex = 5;
            this.txtData1.Text = "00";
            this.txtData1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtData2
            // 
            this.txtData2.Location = new System.Drawing.Point(652, 459);
            this.txtData2.Name = "txtData2";
            this.txtData2.Size = new System.Drawing.Size(34, 26);
            this.txtData2.TabIndex = 5;
            this.txtData2.Text = "00";
            this.txtData2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtData3
            // 
            this.txtData3.Location = new System.Drawing.Point(692, 459);
            this.txtData3.Name = "txtData3";
            this.txtData3.Size = new System.Drawing.Size(34, 26);
            this.txtData3.TabIndex = 5;
            this.txtData3.Text = "00";
            this.txtData3.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtData4
            // 
            this.txtData4.Location = new System.Drawing.Point(732, 459);
            this.txtData4.Name = "txtData4";
            this.txtData4.Size = new System.Drawing.Size(34, 26);
            this.txtData4.TabIndex = 5;
            this.txtData4.Text = "00";
            this.txtData4.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtData5
            // 
            this.txtData5.Location = new System.Drawing.Point(772, 459);
            this.txtData5.Name = "txtData5";
            this.txtData5.Size = new System.Drawing.Size(34, 26);
            this.txtData5.TabIndex = 5;
            this.txtData5.Text = "00";
            this.txtData5.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtData6
            // 
            this.txtData6.Location = new System.Drawing.Point(812, 459);
            this.txtData6.Name = "txtData6";
            this.txtData6.Size = new System.Drawing.Size(34, 26);
            this.txtData6.TabIndex = 5;
            this.txtData6.Text = "00";
            this.txtData6.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtData7
            // 
            this.txtData7.Location = new System.Drawing.Point(852, 459);
            this.txtData7.Name = "txtData7";
            this.txtData7.Size = new System.Drawing.Size(34, 26);
            this.txtData7.TabIndex = 5;
            this.txtData7.Text = "00";
            this.txtData7.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(583, 488);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(18, 20);
            this.label5.TabIndex = 3;
            this.label5.Text = "0";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(619, 488);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(18, 20);
            this.label6.TabIndex = 3;
            this.label6.Text = "1";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(659, 488);
            this.label7.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(18, 20);
            this.label7.TabIndex = 3;
            this.label7.Text = "2";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(698, 488);
            this.label8.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(18, 20);
            this.label8.TabIndex = 3;
            this.label8.Text = "3";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(740, 488);
            this.label9.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(18, 20);
            this.label9.TabIndex = 3;
            this.label9.Text = "4";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(780, 486);
            this.label10.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(18, 20);
            this.label10.TabIndex = 3;
            this.label10.Text = "5";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(820, 486);
            this.label11.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(18, 20);
            this.label11.TabIndex = 3;
            this.label11.Text = "6";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(859, 486);
            this.label12.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(18, 20);
            this.label12.TabIndex = 3;
            this.label12.Text = "7";
            // 
            // RdoCan1
            // 
            this.RdoCan1.AutoSize = true;
            this.RdoCan1.Location = new System.Drawing.Point(368, 22);
            this.RdoCan1.Name = "RdoCan1";
            this.RdoCan1.Size = new System.Drawing.Size(69, 24);
            this.RdoCan1.TabIndex = 6;
            this.RdoCan1.TabStop = true;
            this.RdoCan1.Text = "CAN1";
            this.RdoCan1.UseVisualStyleBackColor = true;
            this.RdoCan1.CheckedChanged += new System.EventHandler(this.RdoCan1_CheckedChanged);
            // 
            // RdoCan2
            // 
            this.RdoCan2.AutoSize = true;
            this.RdoCan2.Location = new System.Drawing.Point(448, 22);
            this.RdoCan2.Name = "RdoCan2";
            this.RdoCan2.Size = new System.Drawing.Size(69, 24);
            this.RdoCan2.TabIndex = 6;
            this.RdoCan2.TabStop = true;
            this.RdoCan2.Text = "CAN2";
            this.RdoCan2.UseVisualStyleBackColor = true;
            this.RdoCan2.CheckedChanged += new System.EventHandler(this.RdoCan2_CheckedChanged);
            // 
            // timer
            // 
            this.timer.Interval = 30;
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // btnCapture
            // 
            this.btnCapture.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCapture.Location = new System.Drawing.Point(1080, 18);
            this.btnCapture.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnCapture.Name = "btnCapture";
            this.btnCapture.Size = new System.Drawing.Size(130, 33);
            this.btnCapture.TabIndex = 0;
            this.btnCapture.Text = "Capture";
            this.btnCapture.UseVisualStyleBackColor = true;
            this.btnCapture.Click += new System.EventHandler(this.btnCapture_Click);
            // 
            // CmbCanMode
            // 
            this.CmbCanMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CmbCanMode.FormattingEnabled = true;
            this.CmbCanMode.Items.AddRange(new object[] {
            "Normal",
            "Just listen",
            "Self test"});
            this.CmbCanMode.Location = new System.Drawing.Point(211, 21);
            this.CmbCanMode.Name = "CmbCanMode";
            this.CmbCanMode.Size = new System.Drawing.Size(121, 28);
            this.CmbCanMode.TabIndex = 7;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(156, 26);
            this.label13.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(53, 20);
            this.label13.TabIndex = 3;
            this.label13.Text = "Mode:";
            // 
            // btnReadFirmware
            // 
            this.btnReadFirmware.Location = new System.Drawing.Point(600, 22);
            this.btnReadFirmware.Name = "btnReadFirmware";
            this.btnReadFirmware.Size = new System.Drawing.Size(186, 31);
            this.btnReadFirmware.TabIndex = 8;
            this.btnReadFirmware.Text = "Read Firmware";
            this.btnReadFirmware.UseVisualStyleBackColor = true;
            this.btnReadFirmware.Click += new System.EventHandler(this.btnReadFirmware_Click);
            // 
            // btnWriteFirmware
            // 
            this.btnWriteFirmware.Location = new System.Drawing.Point(792, 21);
            this.btnWriteFirmware.Name = "btnWriteFirmware";
            this.btnWriteFirmware.Size = new System.Drawing.Size(186, 31);
            this.btnWriteFirmware.TabIndex = 8;
            this.btnWriteFirmware.Text = "Write Firmware";
            this.btnWriteFirmware.UseVisualStyleBackColor = true;
            this.btnWriteFirmware.Click += new System.EventHandler(this.btnWriteFirmware_Click);
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(1112, 415);
            this.btnClear.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(98, 37);
            this.btnClear.TabIndex = 0;
            this.btnClear.Text = "Clear";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1223, 535);
            this.Controls.Add(this.btnWriteFirmware);
            this.Controls.Add(this.btnReadFirmware);
            this.Controls.Add(this.CmbCanMode);
            this.Controls.Add(this.RdoCan2);
            this.Controls.Add(this.RdoCan1);
            this.Controls.Add(this.txtFrameID);
            this.Controls.Add(this.txtData7);
            this.Controls.Add(this.txtData6);
            this.Controls.Add(this.txtData5);
            this.Controls.Add(this.txtData4);
            this.Controls.Add(this.txtData3);
            this.Controls.Add(this.txtData2);
            this.Controls.Add(this.txtData1);
            this.Controls.Add(this.txtData0);
            this.Controls.Add(this.cmbHeadIDs);
            this.Controls.Add(this.cmbFunctions);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lstReceivedMessage);
            this.Controls.Add(this.btnClear);
            this.Controls.Add(this.btnSendMessage);
            this.Controls.Add(this.btnCapture);
            this.Controls.Add(this.btnConnect);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "Main";
            this.Text = "CAN Message Analysister";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.ListView lstReceivedMessage;
        private System.Windows.Forms.Button btnSendMessage;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmbFunctions;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cmbHeadIDs;
        private System.Windows.Forms.TextBox txtData0;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        public System.Windows.Forms.TextBox txtFrameID;
        private System.Windows.Forms.TextBox txtData1;
        private System.Windows.Forms.TextBox txtData2;
        private System.Windows.Forms.TextBox txtData3;
        private System.Windows.Forms.TextBox txtData4;
        private System.Windows.Forms.TextBox txtData5;
        private System.Windows.Forms.TextBox txtData6;
        private System.Windows.Forms.TextBox txtData7;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.RadioButton RdoCan1;
        private System.Windows.Forms.RadioButton RdoCan2;
        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.Button btnCapture;
        private System.Windows.Forms.ComboBox CmbCanMode;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Button btnReadFirmware;
        private System.Windows.Forms.Button btnWriteFirmware;
        private System.Windows.Forms.Button btnClear;
    }
}

