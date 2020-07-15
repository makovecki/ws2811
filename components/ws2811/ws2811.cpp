#include "ws2811.h"
#include <driver/rmt.h>
#include <driver/gpio.h>
#include <iostream>


uint32_t WS2811::ws2812_t0h_ticks = 0;
uint32_t WS2811::ws2812_t1h_ticks = 0;
uint32_t WS2811::ws2812_t0l_ticks = 0;
uint32_t WS2811::ws2812_t1l_ticks = 0;

void IRAM_ATTR WS2811::WS2811_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size,size_t wanted_num, size_t *translated_size, size_t *item_num)
{
    if (src == NULL || dest == NULL) {
        *translated_size = 0;
        *item_num = 0;
        return;
    }
    const rmt_item32_t bit0 = {{{ ws2812_t0h_ticks, 1, ws2812_t0l_ticks, 0 }}}; //Logical 0
    const rmt_item32_t bit1 = {{{ ws2812_t1h_ticks, 1, ws2812_t1l_ticks, 0 }}}; //Logical 1

    size_t size = 0;
    size_t num = 0;

    uint8_t *psrc = (uint8_t *)src;
    rmt_item32_t *pdest = dest;

    while (size < src_size && num < wanted_num) {
        
        for (int i = 0; i < 8; i++) {
            // MSB first
            if (*psrc & (1 << (7 - i))) {
                pdest->val =  bit1.val;
            } else {
                pdest->val =  bit0.val;
            }
            num++;
            pdest++;
        }
        size++;
        psrc++;
    }
    *translated_size = size;
    *item_num = num;
}

WS2811::WS2811(gpio_num_t pin, uint16_t pixelCount, rmt_channel_t channel) {
    this->pixelCount = pixelCount;
    this->channel    = (rmt_channel_t) channel;
    this->buffer    = (uint8_t*)calloc(pixelCount*3, sizeof(uint8_t));
    Clear();

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(pin, channel);
    config.clk_div = 2; // set counter clock to 40MHz

    ESP_ERROR_CHECK(rmt_config(&config));
	ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    uint32_t counter_clk_hz = 0;

    rmt_get_counter_clock(channel, &counter_clk_hz);
    
    float ratio = (float)counter_clk_hz / 1e9;
    ws2812_t0h_ticks = (uint32_t)(ratio * 350);
    ws2812_t0l_ticks = (uint32_t)(ratio * 1000);
    ws2812_t1h_ticks = (uint32_t)(ratio * 1000);
    ws2812_t1l_ticks = (uint32_t)(ratio * 350);

    rmt_translator_init(channel, WS2811_rmt_adapter);

}

void WS2811::Clear()
{
    memset(buffer, 0, pixelCount * 3);
    Show();
    /*for (uint16_t i = 0; i < this->pixelCount; i++) {
		this->pixels[i].red   = 0;
		this->pixels[i].green = 0;
		this->pixels[i].blue  = 0;
	}*/
}

void WS2811::Show()
{
   rmt_write_sample(this->channel, this->buffer, this->pixelCount *3, true);
   rmt_wait_tx_done(this->channel, pdMS_TO_TICKS(100)); 
}

void WS2811::SetPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t start = index * 3;
    buffer[start + 0] = green;
    buffer[start + 1] = red;
    buffer[start + 2] = blue;
}