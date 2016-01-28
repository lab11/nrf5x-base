PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c

APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_scheduler_serconn.c
APPLICATION_SRCS += app_timer.c

APPLICATION_SRCS += nrf_assert.c
APPLICATION_SRCS += nrf_delay.c
APPLICATION_SRCS += nrf_drv_common.c

APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += ble_dtm.c
APPLICATION_SRCS += ble_srv_common.c

# Codecs files
APPLICATION_SRCS += conn_mw.c
APPLICATION_SRCS += ble_dtm_init.c
APPLICATION_SRCS += ble_enable.c
APPLICATION_SRCS += ble_event.c
APPLICATION_SRCS += ble_evt_tx_complete.c
APPLICATION_SRCS += ble_evt_user_mem_release.c
APPLICATION_SRCS += ble_evt_user_mem_request.c
APPLICATION_SRCS += ble_gap_address_get.c
APPLICATION_SRCS += ble_gap_address_set.c
APPLICATION_SRCS += ble_gap_adv_data_set.c
APPLICATION_SRCS += ble_gap_adv_start.c
APPLICATION_SRCS += ble_gap_adv_stop.c
APPLICATION_SRCS += ble_gap_appearance_get.c
APPLICATION_SRCS += ble_gap_appearance_set.c
APPLICATION_SRCS += ble_gap_auth_key_reply.c
APPLICATION_SRCS += ble_gap_authenticate.c
APPLICATION_SRCS += ble_gap_conn_param_update.c
APPLICATION_SRCS += ble_gap_conn_sec_get.c
APPLICATION_SRCS += ble_gap_connect.c
APPLICATION_SRCS += ble_gap_connect_cancel.c
APPLICATION_SRCS += ble_gap_device_name_get.c
APPLICATION_SRCS += ble_gap_device_name_set.c
APPLICATION_SRCS += ble_gap_disconnect.c
APPLICATION_SRCS += ble_gap_encrypt.c
APPLICATION_SRCS += ble_gap_evt_adv_report.c
APPLICATION_SRCS += ble_gap_evt_auth_key_request.c
APPLICATION_SRCS += ble_gap_evt_auth_status.c
APPLICATION_SRCS += ble_gap_evt_conn_param_update.c
APPLICATION_SRCS += ble_gap_evt_conn_param_update_request.c
APPLICATION_SRCS += ble_gap_evt_conn_sec_update.c
APPLICATION_SRCS += ble_gap_evt_connected.c
APPLICATION_SRCS += ble_gap_evt_disconnected.c
APPLICATION_SRCS += ble_gap_evt_passkey_display.c
APPLICATION_SRCS += ble_gap_evt_rssi_changed.c
APPLICATION_SRCS += ble_gap_evt_scan_req_report.c
APPLICATION_SRCS += ble_gap_evt_sec_info_request.c
APPLICATION_SRCS += ble_gap_evt_sec_params_request.c
APPLICATION_SRCS += ble_gap_evt_sec_request.c
APPLICATION_SRCS += ble_gap_evt_timeout.c
APPLICATION_SRCS += ble_gap_ppcp_get.c
APPLICATION_SRCS += ble_gap_ppcp_set.c
APPLICATION_SRCS += ble_gap_rssi_get.c
APPLICATION_SRCS += ble_gap_rssi_start.c
APPLICATION_SRCS += ble_gap_rssi_stop.c
APPLICATION_SRCS += ble_gap_scan_start.c
APPLICATION_SRCS += ble_gap_scan_stop.c
APPLICATION_SRCS += ble_gap_sec_info_reply.c
APPLICATION_SRCS += ble_gap_sec_params_reply.c
APPLICATION_SRCS += ble_gap_tx_power_set.c
APPLICATION_SRCS += ble_gattc_char_value_by_uuid_read.c
APPLICATION_SRCS += ble_gattc_char_values_read.c
APPLICATION_SRCS += ble_gattc_characteristics_discover.c
APPLICATION_SRCS += ble_gattc_descriptors_discover.c
APPLICATION_SRCS += ble_gattc_evt_char_disc_rsp.c
APPLICATION_SRCS += ble_gattc_evt_char_val_by_uuid_read_rsp.c
APPLICATION_SRCS += ble_gattc_evt_char_vals_read_rsp.c
APPLICATION_SRCS += ble_gattc_evt_desc_disc_rsp.c
APPLICATION_SRCS += ble_gattc_evt_hvx.c
APPLICATION_SRCS += ble_gattc_evt_prim_srvc_disc_rsp.c
APPLICATION_SRCS += ble_gattc_evt_read_rsp.c
APPLICATION_SRCS += ble_gattc_evt_rel_disc_rsp.c
APPLICATION_SRCS += ble_gattc_evt_timeout.c
APPLICATION_SRCS += ble_gattc_evt_write_rsp.c
APPLICATION_SRCS += ble_gattc_hv_confirm.c
APPLICATION_SRCS += ble_gattc_primary_services_discover.c
APPLICATION_SRCS += ble_gattc_read.c
APPLICATION_SRCS += ble_gattc_relationships_discover.c
APPLICATION_SRCS += ble_gattc_write.c
APPLICATION_SRCS += ble_gatts_characteristic_add.c
APPLICATION_SRCS += ble_gatts_descriptor_add.c
APPLICATION_SRCS += ble_gatts_evt_hvc.c
APPLICATION_SRCS += ble_gatts_evt_rw_authorize_request.c
APPLICATION_SRCS += ble_gatts_evt_sc_confirm.c
APPLICATION_SRCS += ble_gatts_evt_sys_attr_missing.c
APPLICATION_SRCS += ble_gatts_evt_timeout.c
APPLICATION_SRCS += ble_gatts_evt_write.c
APPLICATION_SRCS += ble_gatts_hvx.c
APPLICATION_SRCS += ble_gatts_include_add.c
APPLICATION_SRCS += ble_gatts_rw_authorize_reply.c
APPLICATION_SRCS += ble_gatts_service_add.c
APPLICATION_SRCS += ble_gatts_service_changed.c
APPLICATION_SRCS += ble_gatts_sys_attr_get.c
APPLICATION_SRCS += ble_gatts_sys_attr_set.c
APPLICATION_SRCS += ble_gatts_value_get.c
APPLICATION_SRCS += ble_gatts_value_set.c
APPLICATION_SRCS += ble_l2cap_cid_register.c
APPLICATION_SRCS += ble_l2cap_cid_unregister.c
APPLICATION_SRCS += ble_l2cap_evt_rx.c
APPLICATION_SRCS += ble_l2cap_tx.c
APPLICATION_SRCS += ble_opt_get.c
APPLICATION_SRCS += ble_opt_id_pre_decoder.c
APPLICATION_SRCS += ble_opt_set.c
APPLICATION_SRCS += ble_tx_buffer_count_get.c
APPLICATION_SRCS += ble_user_mem_reply.c
APPLICATION_SRCS += ble_uuid_decode.c
APPLICATION_SRCS += ble_uuid_encode.c
APPLICATION_SRCS += ble_uuid_vs_add.c
APPLICATION_SRCS += ble_version_get.c
APPLICATION_SRCS += conn_ble_gap_sec_keys.c
APPLICATION_SRCS += conn_ble_user_mem.c
APPLICATION_SRCS += conn_mw_ble.c
APPLICATION_SRCS += conn_mw_ble_gap.c
APPLICATION_SRCS += conn_mw_ble_gattc.c
APPLICATION_SRCS += conn_mw_ble_gatts.c
APPLICATION_SRCS += conn_mw_ble_l2cap.c
APPLICATION_SRCS += conn_mw_nrf_soc.c
APPLICATION_SRCS += power_system_off.c
APPLICATION_SRCS += temp_get.c

APPLICATION_SRCS += ble_serialization.c
APPLICATION_SRCS += cond_field_serialization.c


APPLICATION_SRCS += ble_gap_struct_serialization.c
APPLICATION_SRCS += ble_gattc_struct_serialization.c
APPLICATION_SRCS += ble_gatts_struct_serialization.c
APPLICATION_SRCS += ble_struct_serialization.c

APPLICATION_SRCS += ser_conn_cmd_decoder.c
APPLICATION_SRCS += ser_conn_dtm_cmd_decoder.c
APPLICATION_SRCS += ser_conn_error_handling.c
APPLICATION_SRCS += ser_conn_event_encoder.c
APPLICATION_SRCS += ser_conn_handlers.c
APPLICATION_SRCS += ser_conn_pkt_decoder.c
APPLICATION_SRCS += ser_conn_reset_cmd_decoder.c

## TRANSPORT LAYER
APPLICATION_SRCS += dtm_uart.c

APPLICATION_SRCS += ser_hal_transport.c
APPLICATION_SRCS += ser_phy_nrf51_uart.c

APPLICATION_SRCS += app_uart.c
APPLICATION_SRCS += nrf_drv_uart.c

APPLICATION_SRCS += app_util_platform.c

APPLICATION_SRCS += led.c






# APPLICATION_SRCS += simple_ble.c
# APPLICATION_SRCS += eddystone.c

LIBRARY_PATHS += . ../../include
SOURCE_PATHS += ../../src

SOFTDEVICE_MODEL = s110

SERIALIZATION_MODE = connectivity

NRF_BASE_PATH ?= ../..
include $(NRF_BASE_PATH)/make/Makefile
