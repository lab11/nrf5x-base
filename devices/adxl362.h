#pragma once

#include <stdint.h>

#include "nrf_drv_spi.h"

typedef enum {
    adxl362_DISABLE_FIFO,
    adxl362_OLDEST_SAVED_FIFO,
    adxl362_STREAM_FIFO,
    adxl362_TRIGGERED_FIFO
} adxl362_fifo_mode;

typedef enum {
    adxl362_INTERRUPT_DEFAULT,
    adxl362_INTERRUPT_LINKED,
    adxl362_INTERRUPT_LOOP
} adxl362_interrupt_mode;

typedef enum {
    adxl362_NOISE_NORMAL,
    adxl362_NOISE_LOW,
    adxl362_NOISE_ULTRALOW
} adxl362_noise_mode;

typedef enum {
    adxl362_MEAS_RANGE_2G,
    adxl362_MEAS_RANGE_4G,
    adxl362_MEAS_RANGE_8G
} adxl362_measurement_range;


typedef struct {
    bool DATA_READY;
    bool FIFO_READY;
    bool FIFO_WATERMARK;
    bool FIFO_OVERRUN;
    bool ACT;
    bool INACT;
    bool AWAKE;
    bool INT_LOW;

} adxl362_interrupt_map_t;

void adxl362_accelerometer_init(nrf_drv_spi_t* spi,
                                adxl362_noise_mode n_mode,
                                bool measure,
                                bool autosleep_en,
                                bool wakeup_en);

void adxl362_config_interrupt_mode(adxl362_interrupt_mode i_mode,
                                   bool use_referenced_activity,
                                   bool use_referenced_inactivity);
void adxl362_config_measurement_range(adxl362_measurement_range m_range);

void adxl362_set_activity_threshold(uint16_t act_threshold);
void adxl362_set_inactivity_threshold(uint16_t inact_threshold);

//ignored if device is on wake-up mode
void adxl362_set_activity_time(uint8_t act_time);
void adxl362_set_inactivity_time(uint16_t inact_time);

//if intmap_1 = 1, config for intpin 1
// if intmap_1 = 0, config for intpin 2
void adxl362_config_INTMAP(adxl362_interrupt_map_t * int_map, bool intmap_1);

void adxl362_activity_interrupt_enable();
void adxl362_inactivity_interrupt_enable();
void adxl362_activity_inactivity_interrupt_enable();

void adxl362_sample_accel_word(uint8_t * x_data, uint8_t * y_data, uint8_t * z_data);
void adxl362_sample_accel_byte(uint8_t * x_data, uint8_t * y_data, uint8_t * z_data);

void adxl362_num_FIFO_samples_ready(uint16_t *num_ready);
void adxl362_config_FIFO(adxl362_fifo_mode f_mode, bool store_temp, uint16_t num_samples);\
void adxl362_read_FIFO(uint8_t * buf, uint16_t num_samples);
void adxl362_parse_FIFO(uint8_t * buf_in, int16_t * buf_out, uint16_t num_samples);

uint8_t adxl362_read_status_reg();
void adxl362_read_dev_id(uint8_t *buf);

void adxl362_accelerometer_reset ();
void adxl362_autosleep ();
void adxl362_measurement_mode ();
