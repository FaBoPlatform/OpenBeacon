#ifndef BEACON_H__
#define BEACON_H__

#include "ble_advdata.h"

typedef struct
{
    uint8_t ch_37_off : 1;
    uint8_t ch_38_off : 1;
    uint8_t ch_39_off : 1;
} beacon_channel_mask_t;

typedef struct
{
    uint8_t hwid[5];
    uint8_t data_len;
    uint8_t data[13];
    uint16_t interval;
    beacon_channel_mask_t channel_mask;
} beacon_info_t;


void beacon_init(beacon_info_t info);
void beacon_start(void);
void beacon_stop(void);

#endif // BEACON_H__
