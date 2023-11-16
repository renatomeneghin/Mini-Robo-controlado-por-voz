

//#include <cstdlib.h>
//#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "driver/ledc.h"
#include "driver/mcpwm_prelude.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_pthread.h"
#include "esp_timer.h"
#include <thread>

//using namespace std;


/* void blink_LED1(){
    volatile int *GPIO_OUTPUT = (volatile int *)0x3ff44004;
    while(1){
        *(GPIO_OUTPUT) ^= (1<<5);
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
} */
/* 
void blink_LED2(){
    volatile int *GPIO_OUTPUT = (volatile int *)0x3ff44004;
    while(1){
        *(GPIO_OUTPUT) ^= (1<<4);
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
}
 */
/* static void PWM_LED(){
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
        //A = (*A == 1000)? (*A & ~(0xFF)):(*A+i);
        //vTaskDelay(10/ portTICK_PERIOD_MS);
    }
}
 */

static void PWM_LED(void *args){

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

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel2 = {
        .gpio_num       = 5,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_1,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel2));

    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    
    while(1){
        for (int i = 0; i < 1 << 13; i++){
            ESP_ERROR_CHECK(ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, i));
            // Update duty to apply the new value
            ESP_ERROR_CHECK(ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel));
            vTaskDelay(1/ portTICK_PERIOD_MS);
        }
        for (int i = 1 << 13; i ; i--){
            ESP_ERROR_CHECK(ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, i));
            // Update duty to apply the new value
            ESP_ERROR_CHECK(ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel));
            vTaskDelay(1/ portTICK_PERIOD_MS);
        }

        for (int i = 0; i < 1 << 13; i++){
            ESP_ERROR_CHECK(ledc_set_duty(ledc_channel2.speed_mode, ledc_channel2.channel, i));
            // Update duty to apply the new value
            ESP_ERROR_CHECK(ledc_update_duty(ledc_channel2.speed_mode, ledc_channel2.channel));
            vTaskDelay(1/ portTICK_PERIOD_MS);
        }
        for (int i = 1 << 13; i ; i--){
            ESP_ERROR_CHECK(ledc_set_duty(ledc_channel2.speed_mode, ledc_channel2.channel, i));
            // Update duty to apply the new value
            ESP_ERROR_CHECK(ledc_update_duty(ledc_channel2.speed_mode, ledc_channel2.channel));
            vTaskDelay(1/ portTICK_PERIOD_MS);
        }
    }

    vTaskDelete(NULL);
}

static void PWM(void *args){    
    mcpwm_timer_handle_t temporizador = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = 1000000,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
        .period_ticks = 100,
    };

    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config,&temporizador));
    
    mcpwm_oper_handle_t operadores[2];
    mcpwm_operator_config_t operator_config = {
        .group_id = 0,
    };
    
    for(uint8_t i = 0; i < 2; i++){
        ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &operadores[i]));
        ESP_ERROR_CHECK(mcpwm_operator_connect_timer(operadores[i], temporizador));
    }

    mcpwm_cmpr_handle_t comparadores[2];
    mcpwm_comparator_config_t compare_config = {
        {true,true,true}
    };
    
    for(uint8_t i = 0; i < 2; i++){
        ESP_ERROR_CHECK(mcpwm_new_comparator(operadores[i], &compare_config, &comparadores[i]));
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparadores[i], 50));
    }
    
    mcpwm_gen_handle_t geradores[2][2] = {};
    mcpwm_generator_config_t gen_config = {};
    const int PWM_gpios[2][2] = {
        {GPIO_NUM_4, GPIO_NUM_5},
        {GPIO_NUM_18, GPIO_NUM_19},
    };
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            gen_config.gen_gpio_num = PWM_gpios[i][j];
            ESP_ERROR_CHECK(mcpwm_new_generator(operadores[i], &gen_config, &geradores[i][j]));
        }
    }

    for (int i = 0; i < 2; i++) {
        //Configura os geradores de PWM
        ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(geradores[i][0],
                                                                  MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
        ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(geradores[i][0],
                                                                    MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparadores[i], MCPWM_GEN_ACTION_LOW)));
        ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(geradores[i][1],
                                                                  MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
        ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(geradores[i][1],
                                                                    MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparadores[i], MCPWM_GEN_ACTION_LOW)));
        //Força as saídas para 0
        //ESP_ERROR_CHECK(mcpwm_generator_set_force_level(geradores[i][0], 0, true));
        //ESP_ERROR_CHECK(mcpwm_generator_set_force_level(geradores[i][1], 0, true));
    }

    ESP_ERROR_CHECK(mcpwm_timer_enable(temporizador));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(temporizador, MCPWM_TIMER_START_NO_STOP));
    ESP_ERROR_CHECK(mcpwm_generator_set_force_level(geradores[0][0], -1, true));
    ESP_ERROR_CHECK(mcpwm_generator_set_force_level(geradores[0][1], 0, true));
        
    while(1){
        vTaskDelay(10);
    }

    vTaskDelete(NULL);
}

static void I2S_data(void *args){
    #define BUFFER_LEN 32
    i2s_chan_handle_t rx_handle;

    //uint8_t *r_buf = (uint8_t *)calloc(1, BUFFER_LEN*sizeof(u_int32_t));
    int *r_buf = (int *)calloc(1, BUFFER_LEN*sizeof(int));
    assert(r_buf); // Check if r_buf allocation success
    size_t r_bytes = 0;

    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    i2s_new_channel(&chan_cfg, NULL, &rx_handle);

    i2s_std_config_t std_cfg = {
        .clk_cfg = { .sample_rate_hz = 32000, .clk_src = I2S_CLK_SRC_DEFAULT, .mclk_multiple = I2S_MCLK_MULTIPLE_384, },
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_24BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = GPIO_NUM_26,
            .ws = GPIO_NUM_27,
            .dout = I2S_GPIO_UNUSED,
            .din = GPIO_NUM_25,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };
    /* Initialize the channel */
    std_cfg.slot_cfg.slot_mask = I2S_STD_SLOT_RIGHT;
    i2s_channel_init_std_mode(rx_handle, &std_cfg);

    /* Before reading data, start the RX channel first */
    i2s_channel_enable(rx_handle);
    while (1) {
        /* Read i2s data */
        if (i2s_channel_read(rx_handle, r_buf, BUFFER_LEN*sizeof(int), &r_bytes, 1000) == ESP_OK) {
            puts("\n\n\n\n\n\n\n\n\n\n\n\n");
            float data[BUFFER_LEN];
            
            for (int i = 0; i < BUFFER_LEN; i++){
                data[i] = float((r_buf[2*i+1] & ~(0b11)))/(0x7FFFFF00); // 24bit MSB to float 
            } //<!used for u_int32_t buffer>
            
            puts("Read Task: i2s read %d bytes\n----------------------------------------------------\n");
            printf("[0] %.5f [1] %.5f [2] %.5f [3] %.5f [4] %.5f [5] %.5f [6] %.5f [7] %.5f\n",
                   data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
        } else {
            puts("Read Task: i2s read failed\n");
        }
        vTaskDelay(10);
    }
    free(r_buf);
    /* Have to stop the channel before deleting it */
    i2s_channel_disable(rx_handle);
    /* If the handle is not needed any more, delete it to release the channel resources */
    i2s_del_channel(rx_handle);
    
    vTaskDelete(NULL);
}



extern "C" void app_main(void) {
    //volatile int *GPIO_BASE_REG = (volatile int *)0x3ff44000;
    //Ativando as saídas dos pinos 0 até 31 (OFFSETS)
    volatile int *GPIO_OUT_ENABLE = (volatile int *)0x3ff44020;

    *(GPIO_OUT_ENABLE) |= (0b11 << 4);

    // Create a thread using default values that can run on any core
    auto cfg = esp_pthread_get_default_config();
    esp_pthread_set_cfg(&cfg);


    //std::thread any_core(blink_LED1);
    xTaskCreatePinnedToCore(I2S_data,"I2S",4096,NULL,5,NULL,0);
    vTaskDelay(500/ portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(PWM,"PWM",4096, NULL, 5, NULL,1);
    while(1);
}