#ifndef __SIMPLE_ADV_H
#define __SIMPLE_ADV_H

#include "ble_advdata.h"
#include "ble_types.h"

// Functions
void simple_adv_only_name();
void simple_adv_service(ble_uuid_t* service_uuid);
void simple_adv_manuf_data(ble_advdata_manuf_data_t* manuf_specific_data);
void simple_adv_service_manuf_data (ble_uuid_t* service_uuid,
                                    ble_advdata_manuf_data_t* manuf_specific_data);
#endif
