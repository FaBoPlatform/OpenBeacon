#include "beacon.h"
#include "nordic_common.h"
#include "app_error.h"

#define APP_BEACON_INFO_LENGTH        20    // データサイズ
#define APP_CFG_NON_CONN_ADV_TIMEOUT  0                                 /**< Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. */

#define HWID_OFFSET_IN_BEACON_INFO 1
#define DATA_OFFSET_IN_BEACON_INFO 7

static ble_gap_adv_params_t m_adv_params;    // アドバタイズパラメータ
static bool is_starting = false;

// ビーコン初期化
void beacon_init(beacon_info_t info)
{
    uint32_t        err_code;
    ble_advdata_t   advdata;

    uint8_t m_beacon_info[APP_BEACON_INFO_LENGTH] =
    {
        0x02,
        0, 0, 0, 0, 0, // LINE Simple Beacon FrameのHWID
        0x7f, // LINE Simple Beacon FrameのMeasured TxPower
        0,0,0,0,0,0,0,0,0,0,0,0,0 // LINE Simple Beacon FrameのDevice Message
    };
    memcpy(m_beacon_info + HWID_OFFSET_IN_BEACON_INFO, info.hwid, 5);
    memcpy(m_beacon_info + DATA_OFFSET_IN_BEACON_INFO, info.data, 13);

    // データ設定
    memset(&advdata, 0, sizeof(advdata));

    ble_uuid_t adv_uuids[] = 
	{
		{0xFE6F, BLE_UUID_TYPE_BLE}
	};
	advdata.uuids_complete.uuid_cnt = 1;
	advdata.uuids_complete.p_uuids  = adv_uuids;

    ble_advdata_service_data_t service_data_struct;
    advdata.p_service_data_array = &service_data_struct; 
    advdata.p_service_data_array->service_uuid = 0xFE6F;
    advdata.p_service_data_array->data.p_data = m_beacon_info;
    advdata.p_service_data_array->data.size   = APP_BEACON_INFO_LENGTH - (13 - info.data_len);
    advdata.service_data_count = 1;

    advdata.name_type               = BLE_ADVDATA_NO_NAME;
    advdata.flags = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED |
                                BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE;
    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

    // アドバタイジングの設定
    memset(&m_adv_params, 0, sizeof(m_adv_params));
    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;
    m_adv_params.p_peer_addr = NULL;
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = MSEC_TO_UNITS(info.interval, UNIT_0_625_MS);
    m_adv_params.timeout     = APP_CFG_NON_CONN_ADV_TIMEOUT;

    m_adv_params.channel_mask.ch_37_off = info.channel_mask.ch_37_off;
    m_adv_params.channel_mask.ch_38_off = info.channel_mask.ch_38_off;
    m_adv_params.channel_mask.ch_39_off = info.channel_mask.ch_39_off;
}


// ビーコン開始
void beacon_start(void)
{
    if (is_starting) return;
    
    uint32_t err_code;
    err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);
    
    is_starting = true;
}

// ビーコン停止
void beacon_stop(void)
{
    if (!is_starting) return;
    
	uint32_t err_code;
	err_code = sd_ble_gap_adv_stop();
	APP_ERROR_CHECK(err_code);
    
    is_starting = false;
}
