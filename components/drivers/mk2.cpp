#include "esp_log.h"
#include "mk2.hpp"

#define GPIO_MOSI 13
#define GPIO_MISO 12
#define GPIO_SCLK 14
#define GPIO_CS 15
#define RCV_HOST    HSPI_HOST
#define DMA_CHAN    2

DRAM_ATTR spi_slave_transaction_t CTAG::DRIVERS::mk2::transaction[2];
DRAM_ATTR uint32_t CTAG::DRIVERS::mk2::currentTransaction;
DMA_ATTR uint8_t CTAG::DRIVERS::mk2::buf0[64];
DMA_ATTR uint8_t CTAG::DRIVERS::mk2::buf1[64];

void CTAG::DRIVERS::mk2::Init() {
    //Configuration for the SPI bus
    spi_bus_config_t buscfg={
            .mosi_io_num=GPIO_MOSI,
            .miso_io_num=GPIO_MISO,
            .sclk_io_num=GPIO_SCLK,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = 64,
            .flags = 0,
            .intr_flags = 0
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
            .spics_io_num=GPIO_CS,
            .flags=0,
            .queue_size=1,
            .mode=0,
            .post_setup_cb=0,
            .post_trans_cb=0
    };

    esp_err_t ret;
    ret=spi_slave_initialize(RCV_HOST, &buscfg, &slvcfg, DMA_CHAN);
    assert(ret==ESP_OK);

    transaction[0].length = 64*8;
    transaction[0].rx_buffer = buf0;
    transaction[0].tx_buffer = NULL;

    transaction[1].length = 64*8;
    transaction[1].rx_buffer = buf1;
    transaction[1].tx_buffer = NULL;

    // queue first transaction
    currentTransaction = 0;
    ret = spi_slave_queue_trans(RCV_HOST, &transaction[currentTransaction], portMAX_DELAY);
    assert(ret==ESP_OK);
}

IRAM_ATTR void * CTAG::DRIVERS::mk2::Update() {
    // get result of last transaction
    spi_slave_transaction_t *ret_trans;
    spi_slave_get_trans_result(RCV_HOST, &ret_trans, portMAX_DELAY);

    // queue next transaction with other buffer, previous buffer can now be consumed
    currentTransaction ^= 0x1;
    spi_slave_queue_trans(RCV_HOST, &transaction[currentTransaction], portMAX_DELAY);
    return ret_trans->rx_buffer;
}
