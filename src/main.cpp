

//#include <cstdlib.h>
//#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_pthread.h"
#include <thread>

//using namespace std;

void blink_LED1(){
    volatile int *GPIO_OUTPUT = (volatile int *)0x3ff44004;
    while(1){
        *(GPIO_OUTPUT) ^= (1<<5);
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
}

void blink_LED2(){
    volatile int *GPIO_OUTPUT = (volatile int *)0x3ff44004;
    while(1){
        *(GPIO_OUTPUT) ^= (1<<4);
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
}

void PWM_LED(){
    volatile int *GPIO_FUNC_12 = (volatile int *)0x3FF44540;
    *GPIO_FUNC_12 = (*GPIO_FUNC_12 & ~0b111111111111) | 32 | 1<<10;
    volatile int *PWM_ = (volatile int *)0x3FF5E000;
    *PWM_ |= 0b10011111; PWM_ = (volatile int *)0x3FF5E004;
    *(PWM_) |= 10<<8; PWM_ = (volatile int *)0x3FF5E008;
    *PWM_ |= 0b1<<3;
    PWM_ = (volatile int *)0x3FF5E00c; *PWM_ |= 2<<2;
    PWM_ = (volatile int *)0x3FF5E050; *PWM_ |= 2<<4 | 1<<0;
    
    volatile int *A = (volatile int *) 0x3FF5E040;
    *A |= 8;

    int8_t i = 1;

    while(1){
        ;
        //*A = (*A == 1000)? (*A & ~(0xFF)):(*A+i);
        //vTaskDelay(10/ portTICK_PERIOD_MS);
    }
}

void PWM_LED2(){

/*  Warning:
 *  For ESP32, ESP32S2, ESP32S3, ESP32C3, ESP32C2, ESP32C6, ESP32H2, ESP32P4 targets,
 *  when LEDC_DUTY_RES selects the maximum duty resolution (i.e. value equal to SOC_LEDC_TIMER_BIT_WIDTH),
 *  100% duty cycle is not reachable (duty cannot be set to (2 ** SOC_LEDC_TIMER_BIT_WIDTH)).
 */
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 4000,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = 4,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    while(1){
        for (int i = 0; i < 1<<13; i++){
            ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i));
            // Update duty to apply the new value
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
            vTaskDelay(1/ portTICK_PERIOD_MS);
        }
        for (int i = 1<<13; i ; i--){
            ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i));
            // Update duty to apply the new value
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
            vTaskDelay(1/ portTICK_PERIOD_MS);
        }
    }
}



extern "C" void app_main(void) {
    //volatile int *GPIO_BASE_REG = (volatile int *)0x3ff44000;

    //Ativando as saídas dos pinos 0 até 31 (OFFSETS)
    volatile int *GPIO_OUT_ENABLE = (volatile int *)0x3ff44020;

    *(GPIO_OUT_ENABLE) |= (0b11 << 4);

    // Create a thread using default values that can run on any core
    auto cfg = esp_pthread_get_default_config();
    esp_pthread_set_cfg(&cfg);

    std::thread any_core(blink_LED1);
    vTaskDelay(500/ portTICK_PERIOD_MS);
    std::thread any_core_2(PWM_LED2);
    while (1){
        ;
        //vTaskDelay(500/ portTICK_PERIOD_MS);
        //printf("Código em funcionamento!\n\r");
    }
}