using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HyrelCanAnalyzer
{
    public class Hyrel
    {
        public const float TEMP_SCALE = 32.0f;
        public static string ProcessCanMessage(HEADPOSITION pos,  CANMSGID msgId, byte[] data)
        {
            string result = "";
            switch(msgId)
            {
                case CANMSGID.CAN_MSG_EVENT_MESSAGE:
                    result = $"TEMP: {(data[0] + data[1] * 0xff) / TEMP_SCALE}";
                    break;
                case CANMSGID.CAN_MSG_HSS_CONTROL:
                    result =  ProcessHSS(data);
                    break;
            }
            return result;
        }
        public static string ProcessHSS(byte[] data)
        {
            string result = "Mode:";
            short mode = (short)(data[0] + data[1] * 10);
            switch(mode)
            {
                case (short)SWITCHMODE.SWITCH_CONTROL_OFF:
                    result += SWITCHMODE.SWITCH_CONTROL_OFF.ToString();
                    break;
                case (short)SWITCHMODE.SWITCH_CONTROL_ON:
                    result += SWITCHMODE.SWITCH_CONTROL_ON.ToString();
                    break;
                case (short)SWITCHMODE.SWITCH_CONTROL_DUTY_CYCLE:
                    result += SWITCHMODE.SWITCH_CONTROL_DUTY_CYCLE.ToString();
                    result += $" Prescaler:{data[2] + data[3] * 0xFF} DutyCycle: {data[4] + data[5] * 0xff}";
                    break;
                case (short)SWITCHMODE.SWITCH_CONTROL_PWM:
                    result += SWITCHMODE.SWITCH_CONTROL_PWM.ToString();
                    result += $" Prescaler:{data[2] + data[3] * 0xFF} DutyCycle: {data[4] + data[5] * 0xff}";
                    break;
                case (short)SWITCHMODE.SWITCH_CONTROL_BY_TEMP:
                    result += SWITCHMODE.SWITCH_CONTROL_BY_TEMP.ToString();
                    result += $" Prescaler:{data[2] + data[3] * 0xFF}  Target Temp: {(data[4] + data[5] * 0xff) / TEMP_SCALE}";
                    break;
            }
            return result;
        }
    }

    public enum SWITCHMODE
    {
        SWITCH_CONTROL_OFF = 0,
        SWITCH_CONTROL_ON,
        SWITCH_CONTROL_DUTY_CYCLE,
        SWITCH_CONTROL_PWM,
        SWITCH_CONTROL_BY_TEMP,
    }
    public enum CANMSGID
    {
        CAN_MSG_DEVICE_POSITION = ((byte)0x00),
        CAN_MSG_DEVICE_INFO = ((byte)0x01),
        CAN_MSG_FLASH_CONFIG = ((byte)0x02),
        CAN_MSG_UNIQUE_ID = ((byte)0x03),
        CAN_MSG_PRE_DEFINED_ALIASES = ((byte)0x04),
        CAN_MSG_USER_DEFINED_ALIASES = ((byte)0x05),
        CAN_MSG_STATUS = ((byte)0x06),
        //#define CAN_MSG_HISTORY_RANGE_DEF         ((byte)0x07)    //NUKE DEPRECATE
        //#define CAN_MSG_HISTORY_RANGES                ((byte)0x08)    //NUKE DEPRECATE
        //#define CAN_MSG_HISTORY_COUNTERS          ((byte)0x09)    //NUKE DEPRECATE
        CAN_MSG_PAGE_DEF = ((byte)0x0a),
        //#define CAN_MSG_TABLE_START_OFFSETS           ((byte)0x0b)    // not used in V1 -- tables are not loadable
        //#define CAN_MSG_GLOBAL_STRUCT_INFO            ((byte)0x0c)    //NUKE DEPRECATE
        CAN_MSG_DEVICE_INIT = ((byte)0x0d),   // V1 - multi-page initialization based on deviceType

        CAN_MSG_CONTROL_WORD = ((byte)0x10),
        CAN_MSG_ADD_ALIAS = ((byte)0x11),
        CAN_MSG_REMOVE_ALIAS = ((byte)0x12),
        CAN_MSG_AUTO_STATUS_CONTROL = ((byte)0x13),
        //#define CAN_MSG_LED_CONTROL                   ((byte)0x14)    //NUKE DEPRECATE

        //#define CAN_MSG_EXTREMES                  ((byte)0x16)    // V0
        CAN_MSG_EXTRUSION_TEMP_RANGES = ((byte)0x16),    // V1
        //#define CAN_MSG_ERROR_REPORT_INTERVAL     ((byte)0x17)    //NUKE DEPRECATE
        //#define CAN_MSG_SWITCH_DUTY                   ((byte)0x18)    // V0 only
        //#define CAN_MSG_SWITCH_PWM                    ((byte)0x19)    // V0 only
        //#define CAN_MSG_SWITCH_TEMP                   ((byte)0x1a)    // V0 only
        CAN_MSG_KARL_FACTORS = ((byte)0x1b),
        CAN_MSG_HSS_CONTROL = ((byte)0x1c),    // V1
        //#define CAN_MSG_HSS_FLAGS                 ((byte)0x1d)    // V1
        CAN_MSG_MOTION_CONTROLS = ((byte)0x1e),

        CAN_MSG_MOTOR_ENABLE = ((byte)0x20),    //NUKE DEPRICATED (USED IN V4, remove for V5) LEGACY
        //#define CAN_MSG_MICROSTEPS                  ((byte)0x21)    // aliased to CAN_MSG_JET_INDEX - not used in V1
        //#define CAN_MSG_JET_INDEX                   ((byte)0x21)    // aliased to CAN_MSG_MICROSTEPS - not used in V1
        //#define CAN_MSG_CURRENT_BOOST             ((byte)0x22)    // not used in v1
        CAN_MSG_FLOW_SCALE_FACTORS = ((byte)0x23),    // was CAN_MSG_fudge_factor
        //#define CAN_MSG_EXTRUSION_STEPS_PER_NL        ((byte)0x24)    //NUKE DEPRICATED
        //#define CAN_MSG_EXTRUSION_MAIN                ((byte)0x25)    // V0 only
        CAN_MSG_EXTRUSION_UNPRIME = ((byte)0x26),
        CAN_MSG_EXTRUSION_PRIME = ((byte)0x27),
        //#define CAN_MSG_EXTRUSION_MANUAL          ((byte)0x28)    // V0 only
        CAN_MSG_EXTRUSION_DWELL = ((byte)0x29),    //NUKE DEPRICATED
        CAN_MSG_PRIME_AND_RUN_PARAMS = ((byte)0x2a),
        //NUKE #define CAN_MSG_VELOCITY_SCALE_FACTOR          ((byte)0x2b)    //NUKE DEPRICATED
        CAN_MSG_TOUCH_PROBE_OLD = ((byte)0x2c), // aliased with CAN_MSG_PICKNPLACE_CONTROLTOUCH PROBE -- BOO HISS LEGACY....
        CAN_MSG_PICKNPLACE_CONTROL = ((byte)0x2c),
        CAN_MSG_PICKNPLACE_DATA = ((byte)0x2d),
        //#define CAN_MSG_CONTROL_VIA_PAGE            ((byte)0x2e)  // V1
        CAN_MSG_TOUCH_PROBE = ((byte)0x2f),


        CAN_MSG_FILL_BUFFER = ((byte)0x30),
        CAN_MSG_ACCESS_BUFFER = ((byte)0x31),
        //NUKE #define CAN_MSG_COPY_ADDR_TO_BUFFER          ((byte)0x32)
        CAN_MSG_COPY_PAGE_TO_BUFFER = ((byte)0x33),
        CAN_MSG_COPY_BUFFER_TO_PAGE = ((byte)0x34),
        CAN_MSG_PAGE_CHECKSUM = ((byte)0x35),
        //#define CAN_MSG_ACCESS_OPTION_BYTES           ((byte)0x36) //NUKE DEPRECATE
        CAN_MSG_START_PRIMARY_PROGRAM = ((byte)0x37),
        //#define CAN_MSG_READ_DEVICE_MEMORY            ((byte)0x38)    //NUKE DEPRECATE        //reads the device memory at location argument[0] for up to argument[1] words

        CAN_MSG_CLOSED_LOOP_MOTOR_CONTROL = ((byte)0x3a),

        CAN_MSG_EVENT_MESSAGE = ((byte)0x40),
        CAN_MSG_LOOPBACK = ((byte)0x41),
        CAN_MSG_REPORT_ERROR = ((byte)0x42),
        //#define CAN_MSG_AUTO_STATUS                   ((byte)0x43)    //NUKE DEPRECATE
        CAN_MSG_PRIME_RUN = ((byte)0x44),
        CAN_MSG_UNPRIME = ((byte)0x45),
        CAN_MSG_PRIME = ((byte)0x46),
        CAN_MSG_RUN = ((byte)0x47),
        CAN_MSG_STOP = ((byte)0x48),
        // all 0x2a are aliased and must have the same number of data bytes
        //#define CAN_MSG_V0_HEAD_CONTROL             ((byte)0x49)  // V0
        //#define CAN_MSG_SET_LASER_POWER               ((byte)0x49)    // V0
        //#define CAN_MSG_SPINDLE_CONTROL               ((byte)0x49)    // V0
        // all 0x2a are aliased and must have the same number of data bytes
        CAN_MSG_V1_HEAD_FUNCTION_CONTROL = ((byte)0x49),    // V1
        CAN_MSG_HEAD_EXTRUDE_CONTROL = ((byte)0x49),    // V1
        CAN_MSG_HEAD_LASER_CONTROL = ((byte)0x49),    // V1
        CAN_MSG_HEAD_INKJET_CONTROL = ((byte)0x49),    // V1
        CAN_MSG_HEAD_SPINDLE_CONTROL = ((byte)0x49),   // V1

        CAN_MSG_SET_POWER_LEVEL_ONLY = ((byte)0x4a),    // for laser and inkjet raster modes
        CAN_MSG_STEP_MOTOR = ((byte)0x4b),
        CAN_MSG_STRING = ((byte)0x4c),
        CAN_MSG_DISPLAY_CONTROL = ((byte)0x4d),

        CAN_MSG_DIAG_IO_INIT = ((byte)0x50),    // 8 bytes; page 0xff
        CAN_MSG_DIAG_IO_WRITE = ((byte)0x51),    // 1 byte;  page 0xff
        CAN_MSG_DIAG_IO_READ = ((byte)0x52),   // 1 byte;  page 0xff
        CAN_MSG_DIAG_ADC_READ = ((byte)0x53),    // 2 bytes; page 0xff
        CAN_MSG_DIAG_STEP_COUNTER = ((byte)0x54),    // 8 bytes; page 0


        CAN_MSG_UNDEFINED = ((byte)0xff),
    }

    public enum HEADPOSITION
    {
        HOTBED1 = 91,
        AUX2_YOKE2 = 26,
        AUX2_YOKE1 = 16,
        HOTHEAD1_YOKE2 = 21,
        HOTHEAD1_YOKE1 = 11,
        HOTHEAD2_YOKE2 = 22,
        HOTHEAD2_YOKE1 = 12,
        HOTHEAD3_YOKE2 = 23,
        HOTHEAD3_YOKE1 = 13,
        HOTHEAD4_YOKE2 = 24,
        HOTHEAD4_YOKE1 = 14,
        HOTHEAD5_YOKE1 = 15,

        AUX1_YOKE2 = 25,
        AUX1_YOKE1 = 15,
        HOTBED2 = 92,

        HOTHEAD1_YOKE4 = 41,
        HOTHEAD1_YOKE3 = 31,
        HOTHEAD2_YOKE4 = 42,
        HOTHEAD2_YOKE3 = 32,
        HOTHEAD3_YOKE4 = 43,
        HOTHEAD3_YOKE3 = 33,
        HOTHEAD4_YOKE4 = 44,
        HOTHEAD4_YOKE3 = 34,

        AUX1_YOKE4 = 45,
        AUX1_YOKE3 = 35,

        HYDRA407 = 0x40,
        UNPLUGGED = 0xFF,
    }
}
