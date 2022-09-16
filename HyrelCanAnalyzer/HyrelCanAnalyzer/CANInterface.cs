using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HyrelCanAnalyzer
{
    public class CANInterface: IDisposable
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
        public const int USB_CAN = 3;
        public const int USB_CAN2 = 4;
        public const int MAX_RECEVIEDPACKETS = 100;

        public const int FILTER_METHOD_ALLFRAME = 1;
        public const int FILTER_METHOD_STANDARDFRAME = 2;
        public const int FILTER_METHOD_EXTENDEDFRAME = 3;
        public bool IsConnected { get; set; }
        public uint Channel { get; set; }
        public VCI_INIT_CONFIG CanConfig = new VCI_INIT_CONFIG();

        public VCI_CAN_OBJ[] CanReceivedrecobj = new VCI_CAN_OBJ[MAX_RECEVIEDPACKETS];

        public bool IsCapturing { get; set; }
        public bool IsDispose { get; private set; }

        public Thread CaptureThread = null;

        public delegate void CapturePacketEventHandler(object sender, CanMessageEventArgs e);
        public event CapturePacketEventHandler CapturePacketEvent;
        public CANInterface()
        {
            CaptureThread = new Thread(() => CaptureCanPacketCallback(this));
            CaptureThread.Start();
        }
        public bool Connect(uint CanChannel)
        {
            if(IsConnected)
            {
                MessageBox.Show("Can Interface already connected.");
                return false;
            }
            if (VCI_OpenDevice(USB_CAN2, 0, 0) == 0)
            {
                MessageBox.Show("Failed to open the device, please check whether the device type and device index number are correct", "Error",
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            CanConfig.AccCode = 0x00000000; //Acceptance Code
            CanConfig.AccMask = 0xFFFFFFFF; //mask Code
            CanConfig.Timing0 = 0x00;          // Timer0
            CanConfig.Timing1 = 0x1C;          //Timer 1
            CanConfig.Filter = FILTER_METHOD_ALLFRAME; //Filtering Method
            CanConfig.Mode = (byte)0; //0:Normal Mode, 1: Just listen, 2: Self test.
            if (VCI_InitCAN(USB_CAN2, 0, CanChannel, ref CanConfig) == 0)
            {
                MessageBox.Show("Can' change the channel.(ERROR: VCI_InitCAN)");
                return false;
            }
            if (VCI_StartCAN(USB_CAN2, 0, CanChannel) == 0)
            {
                MessageBox.Show("Can' change the channel.(ERROR: VCI_StartCAN)");
                return false;
            }
            Channel = CanChannel;
            IsConnected = true;
            return true;
        }

        public void Disconnect()
        {
            if (!IsConnected) return;
            VCI_CloseDevice(USB_CAN2, 0);
            IsConnected = false;
        }

        unsafe public void CaptureCanPacketCallback(CANInterface ifc)
        {
            while(!IsDispose)
            {
                if (!IsCapturing) continue;
                UInt32 res = new UInt32();
                res = VCI_Receive(USB_CAN2, 0, Channel, ref CanReceivedrecobj[0], MAX_RECEVIEDPACKETS, 100);
                for (UInt32 i = 0; i < res; i++)
                {
                    if (CanReceivedrecobj[i].RemoteFlag == 1) continue;
                    if (CanReceivedrecobj[i].ExternFlag != 1) continue;
                    byte len = (byte)CanReceivedrecobj[i].DataLen;
                    CANMESSAGEINFO msg = new CANMESSAGEINFO();
                    msg.DataLen = len;
                    fixed (VCI_CAN_OBJ* recobj = &CanReceivedrecobj[i])
                    {
                        msg.ID = recobj->ID;
                        for (int k = 0; k < len; k++)
                            msg.data[k] = recobj->Data[k]; 
                    }
                    CanMessageEventArgs e = new CanMessageEventArgs();
                    e.msgInfo = msg;

                    CapturePacketEvent?.Invoke(ifc, e);
                    break;

                }
                Thread.Sleep(100);
            }
        }

        public void Dispose()
        {
            IsDispose = true;
        }
    }

    public class CanMessageEventArgs: EventArgs
    {
        public CANMESSAGEINFO msgInfo = null;
    }
    public class CANMESSAGEINFO
    {
        public uint ID;
        public byte[] data = new byte[8];
        public string description = "";
        public int DataLen = 0;
        public string GetDataString()
        {
            string sz = "";
            for(int i = 0; i< DataLen; i ++)
            {
                sz = data[i].ToString("X2") + " " + sz;
            }
            return sz;
        }
        public HEADPOSITION SourceID
        {
            get
            {
                int pos = (int)(ID & 0x00ff0000) >> 16; //0b0000011111111000000000000000
                if (Enum.IsDefined(typeof(HEADPOSITION), pos))
                {
                    return (HEADPOSITION)pos;
                }
                return HEADPOSITION.UNPLUGGED;
            }
        }

        public HEADPOSITION TargetID
        {
            get
            {
                int pos = (int)(ID & 0x0000ff00) >> 8;
                if (Enum.IsDefined(typeof(HEADPOSITION), pos))
                {
                    return (HEADPOSITION)pos;
                }
                return HEADPOSITION.UNPLUGGED;
            }
        }
        public CANMSGTYPE MsgType
        {
            get
            {
                return (ID & 0b0000001000000000000000000000) == 0 ? CANMSGTYPE.CAN_READ : CANMSGTYPE.CAN_WRITE;
            }
        }
        public CANMSGID MsgId
        {
            get
            {
                int id = (int)(ID & 0x0000FF00) >> 8;
                if (Enum.IsDefined(typeof(CANMSGID), id)) {
                    return (CANMSGID)id;
                }
                return CANMSGID.CAN_MSG_UNDEFINED;
                
            }
        }
        
    }
    public enum CANMSGTYPE { CAN_READ, CAN_WRITE}
    

    /*------------Compatible with ZLG data types---------------------------------*/

    //1.Data type of ZLGCAN series interface card information。
    public struct VCI_BOARD_INFO
    {
        public UInt16 hw_Version;
        public UInt16 fw_Version;
        public UInt16 dr_Version;
        public UInt16 in_Version;
        public UInt16 irq_Num;
        public byte can_Num;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)] public byte[] str_Serial_Num;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40)]
        public byte[] str_hw_Type;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] Reserved;
    }

    /////////////////////////////////////////////////////
    //2.Define the data type of CAN message frame。
    unsafe public struct VCI_CAN_OBJ  //Use unsafe code
    {
        public uint ID;
        public uint TimeStamp;        //Time stamp
        public byte TimeFlag;         //Whether to use time stamp
        public byte SendType;         //Send flag. Reserved, unused
        public byte RemoteFlag;       //Is it a remote frame
        public byte ExternFlag;       //Is it an extended frame
        public byte DataLen;          //Data length
        public fixed byte Data[8];    //Data
        public fixed byte Reserved[3];//Reserved bit

    }

    //3.Define the data type for initializing CAN
    public struct VCI_INIT_CONFIG
    {
        public UInt32 AccCode;
        public UInt32 AccMask;
        public UInt32 Reserved;
        public byte Filter;   //0 or 1 to receive all frames. 2 standard frame filter, 3 is extended frame filter.
        public byte Timing0;  //For the baud rate parameters and specific configuration, please refer to the manual of the secondary development library function.
        public byte Timing1;
        public byte Mode;     //Mode, 0 means normal mode, 1 means listening only mode, 2 self-test mode
    }

    /*------------Other data structure description---------------------------------*/
    //4. The data type 1 of the USB-CAN bus adapter board information, which is the return parameter of the VCI_FindUsbDevice function
    public struct VCI_BOARD_INFO1
    {
        public UInt16 hw_Version;
        public UInt16 fw_Version;
        public UInt16 dr_Version;
        public UInt16 in_Version;
        public UInt16 irq_Num;
        public byte can_Num;
        public byte Reserved;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)] public byte[] str_Serial_Num;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] str_hw_Type;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] str_Usb_Serial;
    }

    /*------------Data structure description completed---------------------------------*/
}
