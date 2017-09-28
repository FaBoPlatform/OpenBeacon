// LineBeacon for nRF5 SDK 12.2.0

#include "bsp.h"
#include "boards.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "app_timer.h"

#include "beacon.h"
#include "blestack.h"


#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */

#define UICR_ADDRESS 0x10001080

// HWID
static uint8_t hwid[5] = {0};
// Device Message
static uint8_t message[13] = {0};
static uint8_t message_len = 5;


// エラーハンドラ
__WEAK void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    switch (id)
    {
        case NRF_FAULT_ID_SDK_ASSERT: {
            NRF_LOG_ERROR("**Assert!**\r\n");
            volatile assert_info_t *assert_info = (assert_info_t*)info;
            NRF_LOG_ERROR("Line:%d\r\n", assert_info->line_num);
            NRF_LOG_ERROR("File:%c\r\n", assert_info->p_file_name[0]);
        } break;
        case NRF_FAULT_ID_SDK_ERROR: {
            NRF_LOG_ERROR("**Error!**\r\n");
            volatile error_info_t *error_info = (error_info_t*)info;
            NRF_LOG_ERROR("Line:%d\r\n", error_info->line_num);
            NRF_LOG_ERROR("File:%s\r\n", nrf_log_push((char*)error_info->p_file_name));
            NRF_LOG_ERROR("Code:%d\r\n", error_info->err_code);
        } break;
    }
    NRF_LOG_FINAL_FLUSH();

    NVIC_SystemReset();
}

// UICRから情報を取得
static void load_uicr()
{
    // UICR_ADDRESS
    uint32_t *ptr = (uint32_t *)UICR_ADDRESS;
    hwid[0] = (*ptr & 0x000000FF);
    if (hwid[0] == 0xFF) hwid[0] = 0x01;
    ptr++;
    uint32_t ptr_data = *ptr;
    for (int i = 0; i < 4; i++) {
        hwid[4-i] = ptr_data & 0x000000FF;
        ptr_data = ptr_data >> 8;
    }
    // メッセージ長
    ptr++;
    message_len = (*ptr & 0x000000FF);
    if (message_len > 14) {
        message_len = 1;
        return;
    }
    // メッセージ
    for (int j = 0; j < 3; j++) {
        ptr++;
        ptr_data = *ptr;
        for (int i = 0; i < 4; i++) {
            message[j * 4 + 3 - i] = ptr_data & 0x000000FF;
            ptr_data = ptr_data >> 8;
        }
    }
    ptr++;
    message[12] = (*ptr & 0x000000FF);
}

// BLEイベントハンドラ
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
}


int main()
{
    uint32_t err_code;

    // ログ初期化
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("**started**\r\n");

    // UICRから情報を取得
    load_uicr();

    // Timer初期化
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);

    // BLEStack初期化
    ble_stack_init(on_ble_evt);

    // DCDCを有効化
#if (NRF_SD_BLE_API_VERSION == 3)
    // NRF_POWER->DCDCEN = 1;
    sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
#endif

    // BSP初期化
    err_code = bsp_init(BSP_INIT_LED, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER), NULL);
    APP_ERROR_CHECK(err_code);

    // Beacon情報初期化
    beacon_info_t beacon_info = {0};
    beacon_info.interval = 100;
    beacon_info.channel_mask.ch_37_off = false;
    beacon_info.channel_mask.ch_38_off = false;
    beacon_info.channel_mask.ch_39_off = false;
    beacon_info.data_len = message_len;
    memcpy(beacon_info.hwid, hwid, 5);
    memcpy(beacon_info.data, message, message_len);
    beacon_init(beacon_info);
    beacon_start();

    // LED点滅
    err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
    APP_ERROR_CHECK(err_code);

    while(true){
        if (NRF_LOG_PROCESS() == false) {
            err_code = sd_app_evt_wait();
            APP_ERROR_CHECK(err_code);
        }
    }

    return 0;
}
