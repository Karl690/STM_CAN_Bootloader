using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CanREL
{
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

    public struct CHGDESIPANDPORT
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
        public byte[] szpwd;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
        public byte[] szdesip;
        public Int32 desport;

        public void Init()
        {
            szpwd = new byte[10];
            szdesip = new byte[20];
        }
    }

    public enum BOOTLOADER_HEADS
    {
        [Description("hot head 1")]
        HEAD_01 = 0x11,
        [Description("hot head 2")]
        HEAD_02 = 0x12,
        [Description("hot head 3")]
        HEAD_03 = 0x13,
        [Description("hot head 4")]
        HEAD_04 = 0x14,
        [Description("Can Analyst-2")]
        CAN_ANALYST = 0xFC,
        [Description("Unplugged(for Self Test)")]
        HEAD_UPPLUGGED = 0xFE,

    }
    public enum BOOTLOADER_FUNCTIONS
    {
        [Description("Ping")]
        CAN_FUNCTION_PING = 0x10,

        [Description("Blink HeartBeat")]
        CAN_FUNCTION_BLINK_HEARTBEAT = 0x20,

        [Description("Jump To Application")]
        CAN_FUNCTION_JUMP_TO_APPLICATION = 0x30,
        
        [Description("CRC Region")]
        CAN_FUNCTION_CRC_REGION= 0x40,

        [Description("Erase Flash")]
        CAN_FUNCTION_ERASE_FLASH = 0x50,

        [Description("Write Flash")]
        CAN_FUNCTION_WRITE_FLASH = 0x60,

        [Description("Read Flash")]
        CAN_FUNCTION_READ_FLASH = 0x70,

        [Description("Custom Message")]
        FUNCTION_CUSTIOM = 0xF0,
        [Description("Unkown Message")]
        FUNCTION_UNKNOW = 0xFF,
    }
    public class CANLib
    {
        public const int USB_CAN = 3;
        public const int USB_CAN2 = 4;
        public const uint MAX_RECEVIEDPACKETS = 1000;

        public const int FILTER_METHOD_ALLFRAME = 1;
        public const int FILTER_METHOD_STANDARDFRAME = 2;
        public const int FILTER_METHOD_EXTENDEDFRAME = 3;

        public const int CAN_REQUEST_PING = 0x10;
        public const int CAN_RESPONSE_PING = 0x80;

        public const int CAN_NORMAL_MESSAGE = 0x00;

        public const int CAN_FIRMWARE_UPLOADDOWNLOAD_ERROR = 0x08;
        public const int CAN_FIRMWARE_UPLOADDOWNLOAD_SUCCESS = 0x09;

        public const int CAN_UPLOAD_START_FIRMWARE = 0x1A;
        public const int CAN_UPLOADING_FIRMWARE = 0x1B;
        public const int CAN_UPLOAD_END_FIRMWARE = 0x1C;

        public const int CAN_DOWNLOAD_START_FIRMWARE = 0x1D;
        public const int CAN_DOWNLOADING_FIRMWARE = 0x1E;
        public const int CAN_DOWNLOAD_END_FIRMWARE	=0x1F;

    }
}
