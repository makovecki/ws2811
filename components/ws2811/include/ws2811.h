#pragma once
#include <stdint.h>
#include <driver/rmt.h>



/*typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} pixel_t;*/
typedef void *led_strip_dev_t;
typedef struct {
    uint32_t max_leds;   /*!< Maximum LEDs in a single strip */
    led_strip_dev_t dev; /*!< LED strip device (e.g. RMT channel, PWM channel, etc) */

} led_strip_config_t;

class WS2811 {
    private:
	uint16_t       pixelCount;
	rmt_channel_t  channel;
	uint8_t*       buffer;
    static uint32_t ws2812_t0h_ticks;
    static uint32_t ws2812_t1h_ticks;
    static uint32_t ws2812_t0l_ticks;
    static uint32_t ws2812_t1l_ticks;
    void Clear();
    static void IRAM_ATTR WS2811_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size,size_t wanted_num, size_t *translated_size, size_t *item_num);
    public:
    WS2811(gpio_num_t gpioNum, uint16_t pixelCount, rmt_channel_t channel = RMT_CHANNEL_0);
    void Show();
    void SetPixel(uint16_t index, uint8_t dim, uint8_t red, uint8_t green, uint8_t blue);
};