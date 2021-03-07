#include "stairs.h"
#include "ws2811.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <iostream>
#include <math.h>

static xQueueHandle gpio_evt_queue = NULL;

void IRAM_ATTR Stairs::ISR_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}
void Stairs::ClearLights()
{
    ledStrip->Clear();
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
void Stairs::AnimationTask(void* arg)
{
    Stairs *stairs = static_cast<Stairs *>(arg);
    stairs->Animate();
    vTaskDelete(NULL);
}
void Stairs::AnimateDown()
{
    for (int i = steps-1; i>=0; i--) if (runAnimation) AnimateStepOn(i);
}
void Stairs::AnimateUp()
{
    for (int i = 0; i < steps; i++) if (runAnimation) AnimateStepOn(i);
}

void Stairs::AnimateStepOn(int step)
{
    int height = 0;
    for (int i =1;i<100;i+=2) if (leds4Step>=i) height++; // calculate how many steps to turn all leds
    int m = floor(leds4Step/2.0);
    
    for (int h = 0; h < height; h++)
    {
        int x = 0;
        for (int xl0 = 0; xl0 < m-h; xl0++)
        {
            ledStrip->SetPixel(((step*leds4Step)+x),0,0,0,0);
            x++;
        }
        for (int x1 = 0; x1 < (h*2)+1; x1++)
        {
            ledStrip->SetPixel(((step*leds4Step)+x),126,255,255,255);
            x++;
        }
        for (int xr0 = 0; xr0 < m-h; xr0++)
        {
            ledStrip->SetPixel(((step*leds4Step)+x),0,0,0,0);
            x++;
        }
        if (runAnimation) {
            ledStrip->Show();
            for (int j = 0; j < 10; j++) if (runAnimation) vTaskDelay(10 / portTICK_PERIOD_MS);
           
            
        }

    }
    
    
    //std::cout << "step:" << step << " height:" << height << "-\n";
}

void Stairs::AnimateTestLights()
{

    int tail =0;
    int distance = 10;

    for (int head = 0; head < steps*leds4Step; head++)
    {
        if (i>distance) {
            ledStrip->SetPixel(tail,0,0,0,0);
            tail++:
        }
        
        ledStrip->SetPixel(head,126,255,255,255);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    
}
void Stairs::Animate()
{
    animationFinished = false;
    runAnimation = true;
    if (animateDirection == Up) AnimateUp();
    if (animateDirection == Down) AnimateDown();
    animationFinished = true;
    animateDirection = Off;
}
void Stairs::StartAnimate()
{
    if (runAnimation)
    {
        runAnimation = false;
        vTaskDelay(pdMS_TO_TICKS(100)); //wait for task to end
    }
    xTaskCreate(this->AnimationTask, "Animation Task", 2048, this, 10, NULL);

}
void Stairs::SetNewDirection(gpio_num_t pin)
{
    runAnimation = false;
    this->animateDirection = GetDirection(pin);
}

void Stairs::EndAnimate()
{
    animationFinished = true;
}
Stairs::Stairs(gpio_num_t pinOut,gpio_num_t pinSensorDown,gpio_num_t pinSensorUp,int steps, int leds4Step)
{
    animationFinished = true;
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