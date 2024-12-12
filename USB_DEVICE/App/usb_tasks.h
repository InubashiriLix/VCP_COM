// this is the head file of the usb tasks
//
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "stm32f4xx_hal.h"

#ifndef USB_TASKS_H
#define USB_TASKS_H

#define USB_TX_BUFFER_SIZE 64
#define USB_RX_BUFFER_SIZE 64

// Tx -> out (to be send)
// Rx -> in (to be received)
// structure of the msg that is going to be send to the upper machine
#define PROJECTILE_TX_HEADER 0xA5 
typedef struct ProjectileTx 
{
    uint8_t header;
    float q[4];
    uint8_t vision_mode;
    uint8_t shoot_remote;
    uint8_t armor_color;
    float bullet_speed;
    uint16_t enemy_hp[6];
    uint32_t timestamp;
    float yaw;
    float pitch;
    uint8_t checksum;
} ProjectileTx_t;
extern size_t ProjectileTxSize;
extern ProjectileTx_t projectile_tx;

// structure of the msg that is going to be received form the upper machine
#define MOTION_RX_HEADER 0x5A
typedef struct MotionRx
{
    // the type of the message
    uint8_t header;          // 第 0-3 位
    float yaw;               // 第 4-7 位
    float pitch;             // 第 4-11 位
    float actual1;           // 第 8-15 位
    uint8_t raw1[4];         // 第 12-19 位
    float actual2;           // 第 12-23 位  
    uint8_t raw2[4];         // 第 19～27位
    float linear_x;          // 第 24～31 位
    float linear_y;          // 第 28～35 位
    float angular_z;         // 第 32～39 位
    uint8_t placeholder1;    // 第 41 位
    uint8_t placeholder2;    // 第 42 位
    uint8_t placeholder3;    // 第 43 位
    uint8_t checksum;        // 第 44 位 (校验位)
} MotionRx_t;
// extern size_t MotionRxSize = sizeof(MotionRx_t);  

extern MotionRx_t motion_rx;

extern uint8_t received_data[USB_RX_BUFFER_SIZE];

extern uint8_t calculate_parity(const uint8_t* data, size_t length);
extern uint8_t vertify_parity(const uint8_t* data, size_t length);

extern uint16_t calculate_crc16(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
extern uint8_t Crc16Verify(uint8_t *pchMessage, uint32_t dwLength);

extern uint8_t calculate_crc8(const uint8_t *data, uint16_t length, uint8_t init_value);
extern uint8_t Crc8Vertify(uint8_t* data, uint32_t length);

extern void usb_task(void const * argument);
extern void usb_printf(const char *fmt, ...);
extern void sendFloat(void);
void sendValidatedFloat_single(const float *f1);
void sendValidatedFloat_double(const float *f1, const float *f2);
void sendValidatedFloat_quad(float f1, float f2, float f3, float f4);

// void receive_data_from_PCV(void);

#endif
