

#include "include/headers.h"

//using namespace std;


void blink_LED1(){
    volatile int *GPIO_OUTPUT = (volatile int *)0x3ff44004;
    while(1){
        *(GPIO_OUTPUT) ^= (1<<5);
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
} 
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

static void PWM_Class(void *args){    
    Motor *motor = new Driver_motor(4,5);

    motor->init();
    while(1){
        motor->setDirecao(0);
        motor->girar();
        vTaskDelay(500/portTICK_PERIOD_MS);
        motor->parar();

        motor->setDirecao(1);
        motor->girar();
        vTaskDelay(500/portTICK_PERIOD_MS);
        motor->parar();
    }

    vTaskDelete(NULL);
}


static void I2S_data(void *args){
    #define BUFFER_LEN 256
    i2s_chan_handle_t rx_handle;

    //uint8_t *r_buf = (uint8_t *)calloc(1, BUFFER_LEN*sizeof(u_int32_t));
    int *r_buf = (int *)malloc(BUFFER_LEN*sizeof(int));
    assert(r_buf); // Check if r_buf allocation success
    size_t r_bytes = 0;

    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    i2s_new_channel(&chan_cfg, NULL, &rx_handle);

    i2s_std_config_t std_cfg = {
        .clk_cfg = { .sample_rate_hz = 16000, .clk_src = I2S_CLK_SRC_DEFAULT, .mclk_multiple = I2S_MCLK_MULTIPLE_384, },
        .slot_cfg = { 
            .data_bit_width = I2S_DATA_BIT_WIDTH_24BIT, 
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_32BIT, 
            .slot_mode = I2S_SLOT_MODE_MONO, 
            .slot_mask = I2S_STD_SLOT_RIGHT, 
            .ws_width = I2S_SLOT_BIT_WIDTH_32BIT, 
            .ws_pol = false, 
            .bit_shift = true, 
            .msb_right = (I2S_DATA_BIT_WIDTH_24BIT <= I2S_DATA_BIT_WIDTH_16BIT) ? true : false, 
            },
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
    i2s_channel_init_std_mode(rx_handle, &std_cfg);

    /* Before reading data, start the RX channel first */
    i2s_channel_enable(rx_handle);
    while (1) {
        /* Read i2s data */
        if (i2s_channel_read(rx_handle, r_buf, BUFFER_LEN*sizeof(int), &r_bytes, 1000) == ESP_OK) {
            puts("\n\n\n\n\n\n\n\n\n\n\n\n");
            float data[BUFFER_LEN];
            
            for (int i = 0; i < BUFFER_LEN; i++){
                data[i] = float((r_buf[i] & ~(0b11)))/(0x7FFFFF00); // 24bit MSB to float 
            } //<!used for u_int32_t buffer>
            
            puts("Read Task: i2s read %d bytes\n----------------------------------------------------\n");
            printf("[0] %.5f [1] %.5f [2] %.5f [3] %.5f [4] %.5f [5] %.5f [6] %.5f [7] %.5f\n",
                   data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
        } else {
            puts("Read Task: i2s read failed\n");
        }
        vTaskDelay(50/portTICK_PERIOD_MS);
    }
    free(r_buf);
    /* Have to stop the channel before deleting it */
    i2s_channel_disable(rx_handle);
    /* If the handle is not needed any more, delete it to release the channel resources */
    i2s_del_channel(rx_handle);
    
    vTaskDelete(NULL);
}


static void I2S_data2(void *args){
    #define BUFFER_LEN 512
    i2s_chan_handle_t rx_handle;

    //uint8_t *r_buf = (uint8_t *)calloc(1, BUFFER_LEN*sizeof(u_int32_t));
    int *r_buf = (int *)calloc(BUFFER_LEN,sizeof(int));
    assert(r_buf); // Check if r_buf allocation success
    size_t r_bytes = 0;

    static const i2s_port_t i2s_num = I2S_NUM_0; // i2s port number

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Interrupt level 1, default 0
        .dma_buf_count = 4,
        .dma_buf_len = BUFFER_LEN,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .mclk_multiple = I2S_MCLK_MULTIPLE_384,
        .bits_per_chan = I2S_BITS_PER_CHAN_32BIT
    };

    static const i2s_pin_config_t pin_config = {
        .bck_io_num = GPIO_NUM_26,
        .ws_io_num = GPIO_NUM_27,
        .data_out_num = GPIO_NUM_NC,
        .data_in_num = GPIO_NUM_25
    };

    i2s_driver_install(i2s_num, &i2s_config, 0, NULL);   //install and start i2s driver
    i2s_set_pin(i2s_num, &pin_config);

    
    while (1) {
        //Read i2s data 
        i2s_read(i2s_num, (void*) r_buf, BUFFER_LEN*sizeof(int), &r_bytes, portMAX_DELAY);            
        puts("\n\n\n\n\n\n\n\n\n\n\n\n");
        float data[BUFFER_LEN];
        
        for (int i = 0; i < BUFFER_LEN; i++){
            data[i] = float((r_buf[i] & ~(0b11)))/(0x7FFFFF00); // 24bit MSB to float 
        } //<!used for u_int32_t buffer>
        
        printf("Read Task: i2s read %d bytes\n----------------------------------------------------\n",r_bytes);
        printf("[0] %d [1] %d [2] %d [3] %d [4] %d [5] %d [6] %d [7] %d \n",
                   r_buf[0], r_buf[1], r_buf[2], r_buf[3], r_buf[4], r_buf[5], r_buf[6], r_buf[7]);
        printf("[0] %.5f [1] %.5f [2] %.5f [3] %.5f [4] %.5f [5] %.5f [6] %.5f [7] %.5f\n",
                data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
        vTaskDelay(100);
    }
    free(r_buf);
    i2s_channel_disable(rx_handle);
    i2s_del_channel(rx_handle);
    
    vTaskDelete(NULL);
}

/* static void I2S_data_Class(void *args){
    Microfone *Mic = new Microfone_I2S(16000,GPIO_NUM_26, GPIO_NUM_27, GPIO_NUM_NC, GPIO_NUM_25,
                                       I2S_NUM_AUTO,I2S_ROLE_MASTER,I2S_DATA_BIT_WIDTH_24BIT,
                                       I2S_SLOT_BIT_WIDTH_32BIT,I2S_SLOT_MODE_MONO,I2S_STD_SLOT_RIGHT,
                                       I2S_CLK_SRC_DEFAULT,I2S_MCLK_MULTIPLE_384);

    vTaskDelete(NULL);
} */


extern "C" void app_main(void) {
    //volatile int *GPIO_BASE_REG = (volatile int *)0x3ff44000;
    //Ativando as saídas dos pinos 0 até 31 (OFFSETS)
    volatile int *GPIO_OUT_ENABLE = (volatile int *)0x3ff44020;

    *(GPIO_OUT_ENABLE) |= (0b11 << 4);

    // Create a thread using default values that can run on any core
    auto cfg = esp_pthread_get_default_config();
    esp_pthread_set_cfg(&cfg);


    std::thread any_core(blink_LED1);

    xTaskCreate(I2S_data2,"I2S",4096,NULL,5,NULL);
    vTaskDelay(500/ portTICK_PERIOD_MS);
    //xTaskCreate(PWM_Class,"PWM",4096, NULL, 5, NULL);
    //while(1);
}