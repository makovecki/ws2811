#pragma once
#include "ws2811.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

enum Direction { Off,Up,Down };

class Stairs
{
private:
    WS2811 *ledStrip;
    int steps;
    int leds4Step;
    gpio_num_t pinSensorDown;
    gpio_num_t pinSensorUp;
    Direction animateDirection;
    static void SensorTrigger(void* arg);
    static void IRAM_ATTR ISR_handler(void* arg);
    Direction GetDirection(gpio_num_t pin);
public:
    Stairs(gpio_num_t pinOut,gpio_num_t pinSensorDown,gpio_num_t pinSensorUp ,int steps, int leds4Step);
    void StartAnimate();
    void SetNewDirection(gpio_num_t pin);
    void ClearLights();
    void EndAnimate();
    bool HasDirectionChanged(gpio_num_t pin);
    ~Stairs();
};


