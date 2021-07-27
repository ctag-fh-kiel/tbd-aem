#pragma once

#include "driver/spi_slave.h"
#include "esp_attr.h"

namespace CTAG {
    namespace DRIVERS {
        class mk2 final{
        public:
            mk2() = delete;
            static void Init();
            IRAM_ATTR static void * Update();
        private:
            DRAM_ATTR static spi_slave_transaction_t transaction[2];
            DRAM_ATTR static uint32_t currentTransaction;
            DMA_ATTR static uint8_t buf0[64], buf1[64];
        };
    }
}
