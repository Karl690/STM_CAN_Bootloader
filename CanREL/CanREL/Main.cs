using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CanREL
{
    public partial class Main : Form
    {
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_OpenDevice(UInt32 DeviceType, UInt32 DeviceInd, UInt32 Reserved);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_CloseDevice(UInt32 DeviceType, UInt32 DeviceInd);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_InitCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_INIT_CONFIG pInitConfig);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ReadBoardInfo(UInt32 DeviceType, UInt32 DeviceInd, ref VCI_BOARD_INFO pInfo);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_GetReceiveNum(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ClearBuffer(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_StartCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ResetCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_Transmit(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_CAN_OBJ pSend, UInt32 Len);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_Receive(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_CAN_OBJ pReceive, UInt32 Len, Int32 WaitTime);

        /*------------Other function description---------------------------------*/

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ConnectDevice(UInt32 DevType, UInt32 DevIndex);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_UsbDeviceReset(UInt32 DevType, UInt32 DevIndex, UInt32 Reserved);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_FindUsbDevice(ref VCI_BOARD_INFO1 pInfo);

        public static Main main = null;
        public int SelectedDeviceId = 0;
        public int SelectedFunctionId = 0;
        public bool IsCanDeviceOpen = false;
        public bool IsCaptureCanPackets = false;
        public uint SelectedCanChannel = 0;
        public bool IsCanFirmwareUpdating = false;
        public bool IsCanFirmwareDownloading = false;
        public int ProgressFirmwareCounter = 0;
        public int ProgressFirmwareDataSize = 0;
        public bool ProcessFirmwareResult = true;

        public string DownloadFirmwareFilePath = "";
        public int DownloadFirmwareOffsetAddress = 0;
        public int DownloadFirmwareSize = 0;
        public byte[] DownloadDataBuffer = null;
        public VCI_INIT_CONFIG CanConfig = new VCI_INIT_CONFIG();
        
        public VCI_CAN_OBJ[] CanReceivedrecobj = new VCI_CAN_OBJ[CANLib.MAX_RECEVIEDPACKETS];
        public Main()
        {
            main = this;
            InitializeComponent();
            cmbHeadIDs.DisplayMember = "Text";
            cmbHeadIDs.ValueMember = "Value";
            cmbFunctions.DisplayMember = "Text";
            cmbFunctions.ValueMember = "Value";
            RdoCan1.Checked = true;

            
            CmbCanMode.SelectedIndex = 0;
            lstReceivedMessage.View = View.Details;
            lstReceivedMessage.FullRowSelect = true;
            lstReceivedMessage.Columns.Add("STD/EXT", 100);
            lstReceivedMessage.Columns.Add("Frame ID", 150);
            lstReceivedMessage.Columns.Add("Data", 200);
            lstReceivedMessage.Columns.Add("Target ID", 100);
            lstReceivedMessage.Columns.Add("Source ID", 100);
            lstReceivedMessage.Columns.Add("Function", 300);
            lstReceivedMessage.Columns.Add("Content", 400);

            foreach (BOOTLOADER_HEADS head in Enum.GetValues(typeof(BOOTLOADER_HEADS)))
            {
                cmbHeadIDs.Items.Add(new { Text = StringValueOfEnum(head), Value = head });
            }

            foreach (BOOTLOADER_FUNCTIONS function in Enum.GetValues(typeof(BOOTLOADER_FUNCTIONS)))
            {
                if(function != BOOTLOADER_FUNCTIONS.FUNCTION_UNKNOW)
                    cmbFunctions.Items.Add(new { Text=StringValueOfEnum(function), Value=function });
            }
            timer.Start();
        }

        static string StringValueOfEnum(Enum value)
        {
            FieldInfo fi = value.GetType().GetField(value.ToString());
            DescriptionAttribute[] attributes = (DescriptionAttribute[])fi.GetCustomAttributes(typeof(DescriptionAttribute), false);
            if (attributes.Length > 0)
            {
                return attributes[0].Description;
            }
            else
            {
                return value.ToString();
            }
        }
        private void btnConnect_Click(object sender, EventArgs e)
        {
            if(IsCanDeviceOpen == true)
            {
                VCI_CloseDevice(CANLib.USB_CAN2, 0);
                IsCanDeviceOpen = false;
            }else
            {
                if (VCI_OpenDevice(CANLib.USB_CAN2, 0, 0) == 0)
                {
                    MessageBox.Show("Failed to open the device, please check whether the device type and device index number are correct", "Error",
                            MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return;
                }
                IsCanDeviceOpen = true;

                //VCI_INIT_CONFIG config = new VCI_INIT_CONFIG();
                CanConfig.AccCode = 0x00000000; //Acceptance Code
                CanConfig.AccMask = 0xFFFFFFFF; //mask Code
                CanConfig.Timing0 = 0x00;          // Timer0
                CanConfig.Timing1 = 0x1C;          //Timer 1
                CanConfig.Filter = CANLib.FILTER_METHOD_ALLFRAME; //Filtering Method
                CanConfig.Mode = (byte)CmbCanMode.SelectedIndex;
                SelectCanChannel(SelectedCanChannel);
                IsCaptureCanPackets = false;
                btnCapture.Enabled = false;
                //VCI_InitCAN(CANLib.USB_CAN, 0, SelectedCanChannel, ref CanConfig);
            }
            if(IsCanDeviceOpen)
            {
                btnConnect.Text = "Disconnect";                
                btnCapture.Enabled = true;
                CmbCanMode.Enabled = false;
            }else
            {
                btnConnect.Text = "Connect";
                btnCapture.Enabled = false;
                CmbCanMode.Enabled = true;
            }
            btnCapture.Text = "Capture";
        }

        public int GenerateFrameID(int HeadId, int funcId)
        {
            int FrameID = 0;
            bool isEnable = false;
            if(funcId == (int)BOOTLOADER_FUNCTIONS.FUNCTION_CUSTIOM ||
                funcId == (int)BOOTLOADER_FUNCTIONS.CAN_FUNCTION_READ_FLASH)
            {
                isEnable = true; 
            }

            //frame ID format: 0x00[TargetID 2Bytes] [SourceID 2 Bytes] [Function ID 2Bytes]
            //Can Analyst -> Boot Loader Dev
            FrameID = (HeadId << 16) + ((int)BOOTLOADER_HEADS.CAN_ANALYST << 8)  + funcId;
            return FrameID;
        }
        
        private void cmbHeadFunction_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            if (cmbHeadIDs.SelectedItem != null)
                SelectedDeviceId = (int)(cmbHeadIDs.SelectedItem as dynamic).Value;
            if (cmbFunctions.SelectedItem != null)
                SelectedFunctionId = (int)(cmbFunctions.SelectedItem as dynamic).Value;

            int FrameID = GenerateFrameID(SelectedDeviceId, SelectedFunctionId);
            string sz = FrameID.ToString("X8");// "0x" + Convert.ToString(FrameID, 16);
            txtFrameID.Text = sz;
        }
        unsafe public bool SendCanMessage(uint frameId, byte[] data, int len)
        {
            if (!IsCanDeviceOpen) return false;
            VCI_CAN_OBJ sendObj = new VCI_CAN_OBJ();
            sendObj.RemoteFlag = 0;
            sendObj.ExternFlag = 1;
            sendObj.ID = frameId; 
            sendObj.DataLen = (byte)len;
            //for(int i = 0; i < len; i ++)
            //    sendObj.Data[i] = data[i];
            Marshal.Copy(data, 0, (IntPtr)sendObj.Data, len);

            if (VCI_Transmit(CANLib.USB_CAN2, 0, 0, ref sendObj, 1) == 0)
            {
                return false;
            }
            return true;
        }
        private void btnSendMessage_Click(object sender, EventArgs e)
        {
            if (txtFrameID.Text == "")
            {
                MessageBox.Show("Please select the Head ID and Function ID.");
                return;
            }

            uint frameId = System.Convert.ToUInt32(txtFrameID.Text, 16);
            byte[] data = new byte[8];


            data[0] = txtData0.Text == "" ? (byte)0 : System.Convert.ToByte("0x" + txtData0.Text, 16);
            data[1] = txtData1.Text == ""? (byte)0: System.Convert.ToByte("0x" + txtData1.Text, 16);
            data[2] = txtData2.Text == "" ? (byte)0 : System.Convert.ToByte("0x" + txtData2.Text, 16);
            data[3] = txtData3.Text == "" ? (byte)0 : System.Convert.ToByte("0x" + txtData3.Text, 16);
            data[4] = txtData4.Text == "" ? (byte)0 : System.Convert.ToByte("0x" + txtData4.Text, 16);
            data[5] = txtData5.Text == "" ? (byte)0 : System.Convert.ToByte("0x" + txtData5.Text, 16);
            data[6] = txtData6.Text == "" ? (byte)0 : System.Convert.ToByte("0x" + txtData6.Text, 16);
            data[7] = txtData7.Text == "" ? (byte)0 : System.Convert.ToByte("0x" + txtData7.Text, 16);

            if (!SendCanMessage(frameId, data, 8)) 
            {
                MessageBox.Show("Failed to send", "Error",
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        public void SelectCanChannel(uint channel)
        {
            if (!IsCanDeviceOpen)
            {
                SelectedCanChannel = channel;
                return;
            }

            if(VCI_InitCAN(CANLib.USB_CAN2, 0, channel, ref CanConfig) == 0)
            {
                MessageBox.Show("Can' change the channel.(ERROR: VCI_InitCAN)");
                return;
            }
            

            if(VCI_StartCAN(CANLib.USB_CAN2, 0, channel) == 0)
            {
                MessageBox.Show("Can' change the channel.(ERROR: VCI_StartCAN)");
                return;
            }
            SelectedCanChannel = channel;
        }
        private void RdoCan1_CheckedChanged(object sender, EventArgs e)
        {
            
            SelectCanChannel(0);
        }

        private void RdoCan2_CheckedChanged(object sender, EventArgs e)
        {
            SelectCanChannel(1);
        }

        unsafe private void timer_Tick(object sender, EventArgs e)
        {
            UInt32 res = new UInt32();
            res = VCI_Receive(CANLib.USB_CAN2, 0, SelectedCanChannel, ref CanReceivedrecobj[0], CANLib.MAX_RECEVIEDPACKETS, 100);
            string str = "";
            byte[] data = new byte[8];
            for(UInt32 i = 0; i < res; i ++)
            {
                if (CanReceivedrecobj[i].RemoteFlag == 1) continue;

                ListViewItem item = new ListViewItem();
                if (CanReceivedrecobj[i].ExternFlag == 1) item.Text = "Ext";
                else item.Text = "Std";
                if(IsCaptureCanPackets)
                    item.SubItems.Add("0x" + CanReceivedrecobj[i].ID.ToString("X8"));

                byte len = (byte)(CanReceivedrecobj[i].DataLen % 9);
                byte j = 0;
                str = "";
                fixed (VCI_CAN_OBJ* m_recobj1 = &CanReceivedrecobj[i])
                {
                    if (j++ < len)
                        str += String.Format("{0:X2}", (int)m_recobj1->Data[0]);
                    if (j++ < len)
                        str += String.Format(" {0:X2}", (int)m_recobj1->Data[1]);
                    if (j++ < len)
                        str += String.Format(" {0:X2}", (int)m_recobj1->Data[2]);
                    if (j++ < len)
                        str += String.Format(" {0:X2}", (int)m_recobj1->Data[3]);
                    if (j++ < len)
                        str += String.Format(" {0:X2}", (int)m_recobj1->Data[4]);
                    if (j++ < len)
                        str += String.Format(" {0:X2}", (int)m_recobj1->Data[5]);
                    if (j++ < len)
                        str += String.Format(" {0:X2}", (int)m_recobj1->Data[6]);
                    if (j++ < len)
                        str += String.Format(" {0:X2}", (int)m_recobj1->Data[7]);
                }
                if (IsCaptureCanPackets)
                {
                    item.SubItems.Add(str); //Data field.
                    int TargetId = (int)((CanReceivedrecobj[i].ID & 0x00ff0000) >> 16);
                    item.SubItems.Add("0x" + TargetId.ToString("X2"));
                    int SourceId = (int)((CanReceivedrecobj[i].ID & 0x0000ff00) >> 8);
                    item.SubItems.Add("0x" + SourceId.ToString("X2"));
                }

                    

                int funcId = (int)(CanReceivedrecobj[i].ID & 0x000000ff);
                BOOTLOADER_FUNCTIONS FunctionId;
                try
                {
                    FunctionId = (BOOTLOADER_FUNCTIONS)funcId;
                }
                catch
                {
                    FunctionId = BOOTLOADER_FUNCTIONS.FUNCTION_CUSTIOM;
                }
                str = AnalysisPacketContents(funcId, str);
                if(IsCanFirmwareDownloading)
                {

                }else if (IsCaptureCanPackets )
                {
                    item.SubItems.Add("0x" + funcId.ToString("X2") + ":" + FunctionId.ToString());
                    item.SubItems.Add(str);
                    lstReceivedMessage.Items.Add(item);
                    lstReceivedMessage.Items[lstReceivedMessage.Items.Count - 1].EnsureVisible();
                }
                
                
            }
        }
        //Analysis the Can Message and it works 
        public string AnalysisPacketContents(int funcId, string datastring)
        {
            int frameId = 0;
            string[] data = datastring.Split(' ');
            string contents = "";
            switch(funcId)
            {
                case (int)BOOTLOADER_FUNCTIONS.CAN_FUNCTION_PING:
                    contents = "Ping Response";
                    break;
                case CANLib.CAN_FIRMWARE_UPLOADDOWNLOAD_ERROR:
                    contents = "ERROR: Download/Upload fireware.";
                    IsCanFirmwareUpdating = false;
                    break;
                case CANLib.CAN_FIRMWARE_UPLOADDOWNLOAD_SUCCESS:
                    contents = "SUCCESS: Download/Upload fireware.";
                    IsCanFirmwareDownloading = false;
                    break;
                case CANLib.CAN_DOWNLOAD_START_FIRMWARE:
                    contents = "Start: Download fireware.";
                    DownloadFirmwareSize = (Convert.ToByte(data[3], 16) << 24) + (Convert.ToByte(data[2], 16) << 16) 
                                            + (Convert.ToByte(data[1], 16) << 8) + (Convert.ToByte(data[0], 16) & 0xFF);
                    if (DownloadDataBuffer != null)
                    {
                        DownloadDataBuffer = null;
                        GC.Collect();
                        GC.WaitForFullGCComplete();
                    }
                    if(DownloadFirmwareSize>= 0xFFFF)
                    {
                        frameId = (CANLib.CAN_DOWNLOAD_END_FIRMWARE << 24) + (SelectedDeviceId << 16); 
                        SendCanMessage((uint)frameId, new byte[8], 0);
                        IsCanFirmwareDownloading = false;
                    }else
                    {
                        ProgressFirmwareDataSize = DownloadFirmwareSize;
                        DownloadDataBuffer = new byte[DownloadFirmwareSize];
                        IsCanFirmwareDownloading = true;
                        DownloadFirmwareOffsetAddress = 0;
                        ProgressFirmwareCounter = 0;
                        RequestDownloadApplicationData(SelectedDeviceId, DownloadFirmwareOffsetAddress);
                    }
                    break;
                case CANLib.CAN_DOWNLOADING_FIRMWARE:
                    if(IsCanFirmwareDownloading)
                    {
                        if(DownloadFirmwareSize <= DownloadFirmwareOffsetAddress)
                        {
                            IsCanFirmwareDownloading = false; // The Downloading End.
                            frameId = (CANLib.CAN_DOWNLOAD_END_FIRMWARE << 24) + (SelectedDeviceId << 16);
                            SendCanMessage((uint)frameId, new byte[8], 0);
                        }
                        else
                        {
                            for(int i = 0; i < 8; i++)
                            {
                                if (DownloadFirmwareOffsetAddress + i < DownloadFirmwareSize)
                                    DownloadDataBuffer[DownloadFirmwareOffsetAddress + i] = Convert.ToByte(data[i], 16);
                                else
                                    break;
                            }
                            ProgressFirmwareCounter += 8; 
                            DownloadFirmwareOffsetAddress += 8; //Increase the Offset Address as 8 Bytes.
                            RequestDownloadApplicationData(SelectedDeviceId, DownloadFirmwareOffsetAddress);
                        }
                        
                    }
                    break;
                case CANLib.CAN_DOWNLOAD_END_FIRMWARE:
                    IsCanFirmwareDownloading = false;
                    contents = "End: Download fireware.";
                    File.WriteAllBytes(DownloadFirmwareFilePath, DownloadDataBuffer);                    
                    break;
            }
            return contents;
        }
        void RequestDownloadApplicationData(int DeviceId, int OffsetAddress)
        {
            int frameId = (CANLib.CAN_DOWNLOADING_FIRMWARE << 24) + (DeviceId << 16) + OffsetAddress;
            if(!SendCanMessage((uint)frameId, new byte[8], 0))
            {
                IsCanFirmwareDownloading = false;
            }
        }
        private void btnCapture_Click(object sender, EventArgs e)
        {
            if(!IsCaptureCanPackets)
            {
                btnCapture.Text = "PAUSE";                
                IsCaptureCanPackets = true;
            }
            else
            {
                btnCapture.Text = "CAPTURE";
                IsCaptureCanPackets = false;
            }
            
        }

        private void btnReadFirmware_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Title = "Please select the firmware file(.hex)";
            saveFileDialog.Filter = "Firmware files (*.bin)|*.bin|Firmware files(*.hex)|*.hex|All files (*.*)|*.*";
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                IsCanFirmwareDownloading = true;
                
                //uploading frame Id format : 0x[1B][Target ID (2B)]0000;
                int frameId = (CANLib.CAN_DOWNLOAD_START_FIRMWARE << 24) + (SelectedDeviceId << 16);
                if (!SendCanMessage((uint)frameId, new byte[8], 0))
                {
                    IsCanFirmwareDownloading = false;
                    MessageBox.Show("The errors occur while updating the firmware.");
                }else
                {
                    IsCanFirmwareDownloading = true;
                    DownloadFirmwareFilePath = saveFileDialog.FileName;
                    ProgressForm progressForm = new ProgressForm();
                    progressForm.ShowDialog();
                }
            }
        }

        private void btnWriteFirmware_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Title = "Please select the Application file";
            openFileDialog.Filter = "Application files (*.bin)|*.bin| Hex files (*.hex)|*.hex|All files (*.*)|*.*";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                byte[] fileBytes = File.ReadAllBytes(openFileDialog.FileName);
                IsCanFirmwareUpdating = true;                
                int HeadId = 0;
                if (cmbHeadIDs.SelectedItem != null)
                    HeadId = (int)(cmbHeadIDs.SelectedItem as dynamic).Value;

                Thread myNewThread = new Thread(() => UploadFirmware(this, HeadId, fileBytes));
                myNewThread.Start();
                ProgressForm progressForm = new ProgressForm();
                progressForm.ShowDialog();

            }
        }

        public void UploadFirmware(Main main, int TargetId, byte[] fileBytes)
        {
            int fileSize = fileBytes.Length;

            //upload start frame Id format : 0x[1A][Target ID (2B)][file size{4B}]
            int frameId  = (CANLib.CAN_UPLOAD_START_FIRMWARE << 24) + (TargetId << 16) +  fileSize;
            byte[] data = new byte[8];
            IsCanFirmwareUpdating = true;
            ProgressFirmwareDataSize = fileSize;
            ProgressFirmwareCounter = 0;
            //write the file size at the 0x08003000 address.
            if (!SendCanMessage((uint)frameId, data, 0))
            {
                IsCanFirmwareUpdating = false;
                MessageBox.Show("The errors occur while updating the firmware.");
                return;
            }
            Thread.Sleep(500);
            
            int len = 8;
            int offsetAddress = 0;
            
            while (IsCanFirmwareUpdating)
            {
                //uploading frame Id format : 0x[1B][Target ID (2B)][[Offset {4B}]
                frameId = (CANLib.CAN_UPLOADING_FIRMWARE << 24) + (TargetId << 16) + offsetAddress;
                if (ProgressFirmwareCounter + 8 < fileSize)
                {
                    len = 8;
                    Buffer.BlockCopy(fileBytes, ProgressFirmwareCounter, data, 0, len);
                }   
                else {
                    len = fileSize - ProgressFirmwareCounter;                    
                    Buffer.BlockCopy(fileBytes, ProgressFirmwareCounter, data, 0, len);
                }
                if (!SendCanMessage((uint)frameId, data, len))
                {
                    IsCanFirmwareUpdating = false;
                    MessageBox.Show("The errors occur while updating the firmware.");
                }
                ProgressFirmwareCounter += len;
                offsetAddress += len;
                if (ProgressFirmwareCounter >= fileSize)
                {
                    IsCanFirmwareUpdating = false;
                    MessageBox.Show("Completed Updating firmware.");
                }   
                Thread.Sleep(50);
            }

            //upload start frame Id format : 0x[1C][Target ID (2B)][written size {4B}]
            frameId = (CANLib.CAN_UPLOAD_END_FIRMWARE << 24) + (TargetId << 16) + offsetAddress;
            if (!SendCanMessage((uint)frameId, data, 0))
            {
                IsCanFirmwareUpdating = false;
                MessageBox.Show("The errors occur while updating the firmware.");
                return;
            }
            IsCanFirmwareUpdating = false;
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            lstReceivedMessage.Items.Clear();
            lstReceivedMessage.Refresh();
        }
    }
}
