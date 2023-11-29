#include "Driver_Motores.h"

Driver_motor::Driver_motor(uint16_t Duty, uint8_t Direcao, uint32_t freq, uint32_t period, int *GPIO_PINS) : 
Motor(Duty,Direcao,freq,period)
{
    this->temporizador = NULL;
    this->operador = NULL;
    this->comparador = NULL;
    this->geradores[2] = {};
    this->PWM_gpios[0] = GPIO_PINS[0];
    this->PWM_gpios[1] = GPIO_PINS[1];
}

Driver_motor::Driver_motor(uint16_t Duty, uint8_t Direcao, uint32_t freq, uint32_t period, int GPIO_PIN_A,int GPIO_PIN_B) : 
Motor(Duty,Direcao,freq,period)
{
    this->temporizador = NULL;
    this->operador = NULL;
    this->comparador = NULL;
    this->geradores[2] = {};
    this->PWM_gpios[0] = GPIO_PIN_A;
    this->PWM_gpios[1] = GPIO_PIN_B;
}

Driver_motor::Driver_motor(int pinos[2]) :
Motor()
{
    this->temporizador = NULL;
    this->operador = NULL;
    this->comparador = NULL;
    this->geradores[2] = {};
    this->PWM_gpios[0] = pinos[0];
    this->PWM_gpios[1] = pinos[1];
}

Driver_motor::Driver_motor(int pinoA, int pinoB) : 
Motor()
{
    this->temporizador = NULL;
    this->operador = NULL;
    this->comparador = NULL;
    this->geradores[2] = {};
    this->PWM_gpios[0] = pinoA;
    this->PWM_gpios[1] = pinoB;
}

Driver_motor::~Driver_motor()
{
    ESP_ERROR_CHECK(mcpwm_del_timer(this->temporizador));
    ESP_ERROR_CHECK(mcpwm_del_operator(this->operador));
    ESP_ERROR_CHECK(mcpwm_del_comparator(this->comparador));
    ESP_ERROR_CHECK(mcpwm_del_generator(this->geradores[0]));
    ESP_ERROR_CHECK(mcpwm_del_generator(this->geradores[1]));
}


void Driver_motor::init(void){
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = this->Frequencia,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
        .period_ticks = this->Periodo,
    };
    
    mcpwm_operator_config_t operator_config = {.group_id = 0,};
    mcpwm_comparator_config_t compare_config = {{true,true,true}};
    mcpwm_generator_config_t gen_config = {};
    
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config,&this->temporizador));
    
    
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &this->operador));
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(this->operador, this->temporizador));
    ESP_ERROR_CHECK(mcpwm_new_comparator(this->operador, &compare_config, &this->comparador));
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(this->comparador, this->Duty_Cicle));

    for (int j = 0; j < 2; j++) {
        gen_config.gen_gpio_num = this->PWM_gpios[j];
        ESP_ERROR_CHECK(mcpwm_new_generator(this->operador, &gen_config, &this->geradores[j]));

    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(geradores[j],
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, 
                                     MCPWM_TIMER_EVENT_EMPTY, 
                                     MCPWM_GEN_ACTION_HIGH)));
    
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(geradores[j],
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, 
        this->comparador, 
        MCPWM_GEN_ACTION_LOW)));
    }

    ESP_ERROR_CHECK(mcpwm_timer_enable(this->temporizador));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(this->temporizador, MCPWM_TIMER_START_NO_STOP));
    ESP_ERROR_CHECK(mcpwm_generator_set_force_level(geradores[0], 0, true));
    ESP_ERROR_CHECK(mcpwm_generator_set_force_level(geradores[1], 0, true));
}

void Driver_motor::girar()
{   
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(this->comparador,this->Duty_Cicle));
    ESP_ERROR_CHECK(mcpwm_generator_set_force_level(geradores[this->Direcao], -1, true));
}

void Driver_motor::parar()
{
    ESP_ERROR_CHECK(mcpwm_generator_set_force_level(geradores[this->Direcao], 0, true));
}
