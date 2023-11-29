#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include <sstream>
#include <string>
#include <memory>
#include <esp_pthread.h>

#include <edge-impulse-sdk/classifier/ei_run_classifier.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_pthread.h"
#include "esp_timer.h"
#include "driver/uart.h"
#include "../include/Driver_Motores.h"
#include "../include/MicrofoneI2S.h"
#include "../include/Fila.h"
#include "../include/clockcalendar.h"

#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

using namespace std;

ClockCalendar *cc = new ClockCalendar(11,18,2023,8,0,0,true);
Fila Operacoes;
Motor *motor1 = new Driver_motor(4,5);
Motor *motor2 = new Driver_motor(18,19);
Microfone *Mic = new Microfone_I2S(16000,GPIO_NUM_26, GPIO_NUM_27, GPIO_NUM_NC, GPIO_NUM_25,
                                       I2S_NUM_AUTO,I2S_ROLE_MASTER,I2S_DATA_BIT_WIDTH_24BIT,
                                       I2S_SLOT_BIT_WIDTH_32BIT,I2S_SLOT_MODE_MONO,I2S_STD_SLOT_RIGHT,
                                       I2S_CLK_SRC_DEFAULT,I2S_MCLK_MULTIPLE_384);

char Operations[5][20] = {"Para traz","Para Frente","Para Esquerda","Para Direita","Parar"};

int raw_feature_get_data(size_t offset, size_t length, float *out_ptr);
static void Carro_ParaFrente(void *args);
static void Carro_ParaTraz(void *args);
static void Carro_ParaEsquerda(void *args);    
static void Carro_ParaDireita(void *args);
static void Carro_Parar(void *args);
void AplicacaoPrincipal();
static void AtualizarClock(void *args); 
void imprimirFila();
static void Imprimir_UART(void *args);
void UART_init();