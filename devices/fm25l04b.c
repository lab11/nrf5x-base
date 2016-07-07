#include "stdint.h"

#include "nrf_gpio.h"
#include "nrf_drv_spi.h"
#include "app_util_platform.h"

#include "fm25l04b.h"


static void spi_init (fm25l04b_t* dev) {
    uint32_t err;

    // Configure !CS pin
    nrf_gpio_cfg_output(dev->ss_pin);
    nrf_gpio_pin_set(dev->ss_pin);

    // Un init the SPI just in case. It doesn't like being re-inited.
    nrf_drv_spi_uninit(dev->spi);

    nrf_drv_spi_config_t spi_config = {
        .sck_pin      = dev->sck_pin,
        .mosi_pin     = dev->mosi_pin,
        .miso_pin     = dev->miso_pin,
        .ss_pin       = 0,
        .irq_priority = APP_IRQ_PRIORITY_LOW,
        .orc          = 0xff,
        .frequency    = NRF_DRV_SPI_FREQ_1M,
        .mode         = NRF_DRV_SPI_MODE_3,
        .bit_order    = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
    };

    // We want blocking mode
    err = nrf_drv_spi_init(dev->spi, &spi_config, NULL);
    APP_ERROR_CHECK(err);
}

/**
 * \brief         Read from the FRAM chip.
 * \param address The index of the byte to start reading from.
 * \param len     The number of bytes to read.
 * \param buf     A buffer to put the return data in.
 * \return        0 on success, -1 on error
 *
 *                Reads len bytes from the FRAM chip starting at address.
 */
int fm25l04b_read (fm25l04b_t* dev, uint16_t address, uint8_t *buf, uint16_t len) {
    uint32_t err;

    // Init in case something else used this spi bus
    spi_init(dev);

    nrf_gpio_pin_clear(dev->ss_pin);

    // Setup that we want a read and to the correct address
    uint8_t header[2];
    header[0] = FM25L04B_ADD_ADDRESS_BIT(address, FM25L04B_READ_COMMAND);
    header[1] = address & 0xFF;

    err = nrf_drv_spi_transfer(dev->spi, header, 2, NULL, 0);
    if (err != NRF_SUCCESS) goto err;

    // Now do the actual read
    err = nrf_drv_spi_transfer(dev->spi, NULL, 0, buf, len);
    if (err != NRF_SUCCESS) goto err;

    nrf_gpio_pin_set(dev->ss_pin);
    return 0;

err:
    nrf_gpio_pin_set(dev->ss_pin);
    return -1;
}

/**
 * \brief         Write to the FRAM chip.
 * \param address The index of the byte to start writing to.
 * \param len     The number of bytes to write.
 * \param buf     A buffer of values to write.
 * \return        0 on success, -1 on error
 *
 *                Writes len bytes to the FRAM chip starting at address.
 */
int fm25l04b_write(fm25l04b_t* dev, uint16_t address, uint8_t *buf, uint16_t len) {
    uint32_t err;
    uint8_t header[2];

    // Init in case something else used this spi bus
    spi_init(dev);

    nrf_gpio_pin_clear(dev->ss_pin);

    // Enable writing to flash
    header[0] = FM25L04B_WRITE_ENABLE_COMMAND;
    err = nrf_drv_spi_transfer(dev->spi, header, 1, NULL, 0);
    if (err != NRF_SUCCESS) goto err;

    nrf_gpio_pin_set(dev->ss_pin);

    // Setup that we want a read and to the correct address
    header[0] = FM25L04B_ADD_ADDRESS_BIT(address, FM25L04B_WRITE_COMMAND);
    header[1] = address & 0xFF;

    nrf_gpio_pin_clear(dev->ss_pin);

    // Setup that this is a write and the address
    err = nrf_drv_spi_transfer(dev->spi, header, 2, NULL, 0);
    if (err != NRF_SUCCESS) goto err;

    // Now do the actual write
    err = nrf_drv_spi_transfer(dev->spi, buf, len, NULL, 0);
    if (err != NRF_SUCCESS) goto err;


    nrf_gpio_pin_set(dev->ss_pin);
    return 0;

err:
    nrf_gpio_pin_set(dev->ss_pin);
    return -1;
}
