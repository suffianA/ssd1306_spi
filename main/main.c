#include <stdio.h> 
#include <string.h>
#include "driver/spi_common.h"
#include "freertos/FreeRTOS.h" 
#include "driver/spi_master.h"
#include "driver/gpio.h" 
#include "freertos/idf_additions.h"
#include "hal/spi_types.h"

#define LCD_HOST    HSPI_HOST

#define PIN_NUM_MOSI 23
#define PIN_NUM_MISO -1     
#define PIN_NUM_SCLK 18
#define PIN_NUM_CS    5
#define PIN_NUM_DC    16
#define PIN_NUM_RST   17

#define SSD1306_WIDTH   128
#define SSD1306_HEIGHT   64
#define SPI_CLOCK_HZ  (4 * 1000 * 1000)

spi_device_handle_t spi;


static inline void oled_cmd(const uint8_t cmd){
    
    spi_transaction_t t;

    gpio_set_level(PIN_NUM_DC, 0); // data/command set to 0
    memset(&t, 0, sizeof(t));
    t.flags = 0;
    t.length = 8;
    t.tx_buffer = &cmd;
    
    spi_device_transmit(spi, &t);

}

static inline void oled_data(const uint8_t *data, int len){

    spi_transaction_t t;

    if(!len) return;

    gpio_set_level(PIN_NUM_DC, 1);
    memset(&t, 0, sizeof(t));
    t.flags = 0;
    t.length = len*8;
    t.tx_buffer = data;
    spi_device_polling_transmit(spi, &t);

}

static void oled_reset(void)
{
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(20));
}



static void oled_init(void){
    oled_reset();

    oled_cmd(0xAE);
    oled_cmd(0xD5); oled_cmd(0x80);
    oled_cmd(0xA8); oled_cmd(0x3F);
    oled_cmd(0xD3); oled_cmd(0x00);
    oled_cmd(0x40 | 0x00);
    oled_cmd(0x8D); oled_cmd(0x14);
    oled_cmd(0x20); oled_cmd(0x00);
    oled_cmd(0xA1);
    oled_cmd(0xC8);
    oled_cmd(0xDA); oled_cmd(0x12);
    oled_cmd(0x81); oled_cmd(0x7F);
    oled_cmd(0xD9); oled_cmd(0xF1);
    oled_cmd(0xDB); oled_cmd(0x40);
    oled_cmd(0xA4);
    oled_cmd(0xA6);
    oled_cmd(0x2E);
    oled_cmd(0xAF);

}

static void oled_set_addr_window(uint8_t col_start, uint8_t col_end, uint8_t page_start, uint8_t page_end)
{
    oled_cmd(0x21);
    oled_cmd(col_start);
    oled_cmd(col_end);
    oled_cmd(0x22);
    oled_cmd(page_start);
    oled_cmd(page_end);

}

static void oled_fill(uint8_t color){
    oled_set_addr_window(0, SSD1306_WIDTH - 1, 0, (SSD1306_HEIGHT/8) - 1);
    uint8_t line[SSD1306_WIDTH];
    memset(line, color, sizeof(line));
    for (int page = 0; page < SSD1306_HEIGHT/8; ++page){
        oled_data(line, sizeof(line));
    }
}

void app_main(void)
{
    gpio_config_t io = {
        .pin_bit_mask = (1ULL<<PIN_NUM_DC) | (1ULL<<PIN_NUM_RST),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&io));

    // bus, device interface configurations
    spi_bus_config_t buscfg_t = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = SSD1306_WIDTH
    };

    spi_device_interface_config_t devicecfg = {
        .clock_speed_hz = SPI_CLOCK_HZ,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 4
    };

    // init bus and add device
    spi_bus_initialize(HSPI_HOST, &buscfg_t, SPI_DMA_CH_AUTO);
    spi_bus_add_device(HSPI_HOST, &devicecfg, &spi);

    oled_init();

    while(1){
        oled_fill(0x00);
        vTaskDelay(pdMS_TO_TICKS(1000));
        oled_fill(0xFF);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}