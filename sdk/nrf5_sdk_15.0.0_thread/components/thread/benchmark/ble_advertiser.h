#ifndef BLE_ADVERTISER_H_
#define BLE_ADVERTISER_H_

/**************************************************************************************************
 * Public API.
 *************************************************************************************************/

/**@brief   Start Bluetooth advertising. */
void ble_advertiser_start(void);

/**@brief   Stop Bluetooth advertising. */
void ble_advertiser_stop(void);

/**@brief   Get current advertising state.
 *
 * @retval true    If Bluetooth is advertising.
 * @retval false   If Bluetooth is not advertising.
 */
bool ble_advertiser_is_started(void);

/**@brief   Function to set advertising intrval. This interval will be used after next call to 
 *          @ref ble_advertiser_start()
 *
 * @param[in] interval    Advertising interval in ms.
 */
void ble_advertiser_interval_set(uint32_t interval);

/**@brief   Get advertising interval that was last requested by @ref ble_advertiser_interval_set()
 *
 * @returns Requested advertising interval in ms.
 */
uint32_t ble_advertiser_interval_get_requested(void);

/**@brief   Get advertising interval that is currently used.
 *
 * @returns Current advertising interval in ms.
 */
uint32_t ble_advertiser_interval_get_current(void);

/**@brief   Set advertising data length.
 *
 * @param[in] len  Advertising interval in ms.
 * 
 * @returns true    If the request data len was correct.
 */
bool ble_advertiser_adv_data_len_set(size_t len);

/**@brief   Get maximum length of data that can be advertised.
 *
 * @returns Maximum length of data that can be advertised.
 */
size_t ble_advertiser_adv_data_len_max(void);

/**@brief   Get advertising data length.
 *
 * @returns Advertising data length.
 */
size_t ble_advertiser_adv_data_len_get(void);

#endif //BLE_ADVERTISER_H_
