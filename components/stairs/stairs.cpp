#include "stairs.h"
#include "ws2811.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <iostream>

static xQueueHandle gpio_evt_queue = NULL;

void IRAM_ATTR Stairs::ISR_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}
void Stairs::ClearLights()
{
    for (int i = 0; i <= steps*leds4Step; i++) ledStrip->SetPixel(i,255,0,0,0);
}
Direction Stairs::GetDirection(gpio_num_t pin)
{
    if (pin == pinSensorUp) return Down; else return Up;
}
bool Stairs::HasDirectionChanged(gpio_num_t pin)
{
    Direction dir = GetDirection(pin);
    return dir != animateDirection;
}
void Stairs::SensorTrigger(void* arg)
{
    Stairs *stairs = static_cast<Stairs *>(arg);
    gpio_num_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {

            if (stairs->HasDirectionChanged(io_num) && gpio_get_level(io_num) == 1) 
            {
                stairs->SetNewDirection(io_num);
                stairs->StartAnimate();
            }
            
        }
    }
}
void Stairs::StartAnimate()
{
    if (animateDirection == Up) std::cout << "Going UP";
    if (animateDirection == Down) std::cout << "Going DOWN";
    std::cout << "\n";
}
void Stairs::SetNewDirection(gpio_num_t pin)
{
    this->animateDirection = GetDirection(pin);
}

void Stairs::EndAnimate()
{

}
Stairs::Stairs(gpio_num_t pinOut,gpio_num_t pinSensorDown,gpio_num_t pinSensorUp,int steps, int leds4Step)
{
    ledStrip = new WS2811(pinOut, steps*leds4Step);
    this->steps = steps;
    this->leds4Step = leds4Step;
    this->pinSensorUp = pinSensorUp;
    this->pinSensorDown = pinSensorDown;
    gpio_config_t io_conf;
    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO17/5 here
    io_conf.pin_bit_mask = ((1ULL<<pinSensorDown) | (1ULL<<pinSensorUp));
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    gpio_config(&io_conf);
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    xTaskCreate(this->SensorTrigger, "SensorTrigger", 2048, this, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(pinSensorDown, this->ISR_handler, (void*) pinSensorDown);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(pinSensorUp, this->ISR_handler, (void*) pinSensorUp);
}

Stairs::~Stairs()
{
}