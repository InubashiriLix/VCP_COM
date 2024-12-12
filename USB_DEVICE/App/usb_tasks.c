// this is the usb task c file
#include "usb_tasks.h"

// #include "cmsis_os.h"


#include <stdio.h>
#include <stdarg.h>
#include "string.h"

uint8_t received_data[USB_RX_BUFFER_SIZE];  // 定义 received_data
// size_t ProjectileTxSize = sizeof(ProjectileTx_t);
// size_t MotionRxSize = sizeof(MotionRx_t);  

extern const char CRC8_INIT = 0xff;
extern const char CRC8_TABLE[] = {
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
    190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
    219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
    87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53};

extern const uint16_t wCRC16_INIT = 0xffff;
extern const uint16_t wCRC16_Table[256] =
    {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78};

// validation functions first

// ==============================  for PARITY CHECK  ==============================
// ====== ODD ======
// 奇校验
uint8_t calculate_parity(const uint8_t *data, size_t length)
{
    uint8_t parity = 0;
    for (size_t i = 0; i < length; i++)
    {
        parity ^= data[i]; // 每字节进行异或
    }
    return parity & 1; // 返回最低位（0 表示偶校验，1 表示奇校验）
}

uint8_t vertify_parity(const uint8_t *data, size_t length)
{
    // 计算数据部分的校验码（不包括最后两个字节）
    uint8_t calculated_parity_code = calculate_parity(data, length - 2);

    // 获取存储在最后两个字节的校验码
    uint8_t stored_parity_code = data[length - 1];

    // 校验码匹配与否
    if (calculated_parity_code == stored_parity_code)
    {
        return 1; // 校验通过
    }
    else
    {
        return 0; // 校验失败
    }
}

// ================================  for CRC CHECK  ================================
// ========= CRC16 ===========
/*
** Descriptions: CRC16 checksum function
** Input: Data to check,Stream length, initialized checksum
** Output: CRC checksum
*/
uint16_t calculate_crc16(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC)
{
    uint8_t chData;
    if (pchMessage == 0)
    {
        return 0xFFFF;
    }
    while (dwLength--)
    {
        chData = *pchMessage++;
        (wCRC) = ((uint16_t)(wCRC) >> 8) ^ wCRC16_Table[((uint16_t)(wCRC) ^ (uint16_t)(chData)) & 0x00ff];
    }
    return wCRC;
}

/*
** Descriptions: CRC16 Verify function
** Input: Data to Verify,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
uint8_t Crc16Verify(uint8_t *pchMessage, uint32_t dwLength)
{
    uint16_t wExpected = 0;
    if ((pchMessage == 0) || (dwLength <= 2))
    {
        return 0;
    }
    wExpected = calculate_crc16(pchMessage, dwLength - 2, wCRC16_INIT);
    return ((wExpected & 0xff) == pchMessage[dwLength - 2] && ((wExpected >> 8) & 0xff) == pchMessage[dwLength - 1]);
}

// ========= CRC8 ===========
uint8_t calculate_crc8(const uint8_t *data, uint16_t length, uint8_t init_value)
{
    uint8_t crc = 0;
    for (uint16_t i = 0; i < length; i++)
    {
        crc = CRC8_TABLE[crc ^ data[i]];
    }
    return crc;
}

// the data should be an array of uint8_t
// the length should be the number of bytes in the data array (sizeof(data)?)
uint8_t Crc8Vertify(uint8_t *data, uint32_t length)
{
    if (data == NULL || length <= 1)
    {
        return 0;
    }
    uint8_t crc8_calculated = calculate_crc8(data, length - 1, CRC8_INIT);
    return ((crc8_calculated & 0xff) == data[length - 1]) ? 1 : 0;
}

// tasks functions may be included there
// #include "detect_task.h"
// #include "voltage_task.h"

static uint8_t usb_buf[256];

void usb_task(void const *argument)
{
    MX_USB_DEVICE_Init();
    // todo what is the error list???
    // error_list_usb_local = NULL;

    while (1)
    {
        // osDelay(1000);
        HAL_Delay(1000);
        srand(HAL_GetTick());
        float randF = (float)rand() / 16000.0f;
        usb_printf("random float: %f", randF);
    }
}

void usb_printf(const char *fmt, ...)
{
    static va_list ap;
    uint16_t len = 0;

    va_start(ap, fmt);
    len = vsnprintf((char *)usb_buf, sizeof(usb_buf), fmt, ap);
    if (len > sizeof(usb_buf))
    {
        len = sizeof(usb_buf); // 防止溢出
    }
    va_end(ap);

    CDC_Transmit_FS(usb_buf, len);
}

void sendTestSting()
{
    usb_printf("acuting, acuting\n");
}

void sendFloat()
{
    srand(HAL_GetTick());                   // 初始化随机数种子
    float randF = (float)rand() / 16000.0f; // 生成随机浮点数

    uint8_t data[sizeof(float)];         // 用于存储浮点数字节
                                         // 发送的数据是小端模式的
    memcpy(data, &randF, sizeof(float)); // 将浮点数内容拷贝到字节数组中

    CDC_Transmit_FS(data, sizeof(float)); // 发送完整浮点数数据
}

// 发送单个浮点数（使用奇偶校验）
void sendValidatedFloat_single(const float *f1)
{
    uint8_t data[sizeof(float) + 1]; // 数据 + 奇偶校验
    memcpy(data, f1, sizeof(float)); // 复制浮点数到数据数组

    // 计算奇偶校验
    uint8_t parity = calculate_parity(data, sizeof(float));
    data[sizeof(float)] = parity; // 将校验位放到数据末尾

    // 通过 USB 发送
    CDC_Transmit_FS(data, sizeof(data)); // 数据 + 奇偶校验
}

// 发送两个浮点数（使用奇偶校验）
void sendValidatedFloat_double(const float *f1, const float *f2)
{
    uint8_t data[2 * sizeof(float) + 1]; // 数据 + 奇偶校验
    memcpy(&data[0], f1, sizeof(float));
    memcpy(&data[sizeof(float)], f2, sizeof(float));

    // 计算奇偶校验
    uint8_t parity = calculate_parity(data, 2 * sizeof(float));
    data[2 * sizeof(float)] = parity; // 将校验位放到数据末尾

    // 通过 USB 发送
    CDC_Transmit_FS(data, sizeof(data)); // 数据 + 奇偶校验
}

// 发送四个浮点数（使用 CRC16 校验）
void sendValidatedFloat_quad(float f1, float f2, float f3, float f4)
{
    uint8_t data[4 * sizeof(float) + 2]; // 数据 + CRC16
    memcpy(&data[0], &f1, sizeof(float));
    memcpy(&data[sizeof(float)], &f2, sizeof(float));
    memcpy(&data[2 * sizeof(float)], &f3, sizeof(float));
    memcpy(&data[3 * sizeof(float)], &f4, sizeof(float));

    // 计算 CRC16
    uint16_t crc = calculate_crc16(data, 4 * sizeof(float), wCRC16_INIT); // 假设 CRC16 初始值为 0xFFFF
    data[4 * sizeof(float)] = crc & 0xFF;                                 // CRC 低字节
    data[4 * sizeof(float) + 1] = (crc >> 8);                             // CRC 高字节

    // 通过 USB 发送
    CDC_Transmit_FS(data, sizeof(data)); // 数据 + CRC16
}

// DEPRECATED because of the CDC_Receive_FS cannot be called in the context
// void receive_data_from_PCV()
// {
//     int bytes_received = CDC_Receive_FS(received_data, USB_RX_BUFFER_SIZE);

//     if (bytes_received > 0)
//     {
//         // vertify the data according to different header
//         switch (received_data[0])
//         {
//         case MOTION_RX_HEADER:
//             // sir! crc8 please!
//             if (Crc8Vertify(received_data, bytes_received))
//             {
//                 // if it pass the crc8 test, update the data
//                 memcpy(&motion_rx, received_data, sizeof(MotionRx_t));
//             }
//             break;

//         default:
//             break;
//         }
//     }
// }
