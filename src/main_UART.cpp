#include "headers.h"

void AplicacaoPrincipal(){  
    ei_impulse_result_t result = { 0 };

    void (*action[5])(void *args) = {Carro_ParaTraz,Carro_ParaFrente,Carro_ParaEsquerda,Carro_ParaDireita,Carro_Parar};
    uint8_t i;
    // the features are stored into flash, and we don't want to load everything into RAM
    signal_t features_signal;
    features_signal.total_length = BUFFER_LEN;
    features_signal.get_data = &raw_feature_get_data;

    // invoke the impulse
    EI_IMPULSE_ERROR res = run_classifier_continuous(&features_signal, &result, false /* debug */);
    
    if (res) return;

    if ((result.classification[2].value > 0.5) || (result.classification[3].value > 0.5)){i = 0;}
    else if ((result.classification[4].value > 0.5) || (result.classification[10].value > 0.5)){i = 1;}
    else if (result.classification[5].value > 0.5){i = 2;}
    else if (result.classification[7].value > 0.5){i = 3;}
    else{i = 4;}
    xTaskCreate(action[i],Operations[i],1024,NULL,1,NULL);
    Operacoes.insert({Operations[i],*cc});
    Mic->hearSound();
}



extern "C" void app_main(void) {
    Mic->init();
    motor1->init();
    motor2->init();
    Operacoes.insert({"Inicializacao",*cc});
    UART_init();

    auto cfg = esp_pthread_get_default_config();
    esp_pthread_set_cfg(&cfg);


    std::thread Relogio(AtualizarClock);
    
    int i = 0;
    while(1){
        AplicacaoPrincipal();
        i++;
        if (i > 10){
            Imprimir_UART();
            imprimirFila(NULL);
            i = 0;
        }
    }    
}




int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, Mic->getSom() + offset, length * sizeof(float));
    return 0;
}

static void Carro_ParaFrente(void *args){
    motor1->setDirecao(0);
    motor1->setDuty(100);
    motor1->girar();
    motor2->setDirecao(0);
    motor2->setDuty(100);
    motor2->girar();
    vTaskDelete(NULL);    
}

static void Carro_ParaTraz(void *args){
    motor1->setDirecao(1);
    motor1->setDuty(100);
    motor1->girar();
    motor2->setDirecao(1);
    motor2->setDuty(100);
    motor2->girar();
    vTaskDelete(NULL);     
}

static void Carro_ParaEsquerda(void *args){
    motor1->setDirecao(0);
    motor1->setDuty(100);
    motor1->girar();
    motor2->setDirecao(0);
    motor2->setDuty(5);
    motor2->girar();
    vTaskDelete(NULL);     
}

static void Carro_ParaDireita(void *args){
    motor1->setDirecao(0);
    motor1->setDuty(5);
    motor1->girar();
    motor2->setDirecao(0);
    motor2->setDuty(100);
    motor2->girar();
    vTaskDelete(NULL);     
}

static void Carro_Parar(void *args){
    motor1->parar();
    motor2->parar();
    vTaskDelete(NULL);     
}

static void AtualizarClock(void *args){
    while(1){
        ++(*cc);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    delete cc;
    vTaskDelete(NULL);
}

void imprimirFila()
{
    char meiodia[2][3] = {"AM", "PM"};
    int mes, dia, ano, hora, minuto, segundo, pm, is_meiodia = 0;
    Dados data;
    for(int i = 0; i < 10; i++){
        data = Operacoes.remove();

        data.Data_Hora.readCalendar(&mes, &dia, &ano);
	    data.Data_Hora.readClock(&hora, &segundo, &minuto, &pm);
	    is_meiodia = (pm)? 1 : 0;

	    std::cout << "Operação Executada: " << data.Operacao << std::endl 
            << "Data: " << std::setfill('0') << std::setw(2) << dia 
            << "/" << std::setfill('0') << std::setw(2) << mes << "/" << ano
	        << "\t Hora: " << std::setfill('0') << std::setw(2) << hora << ":" 
            << std::setfill('0') << std::setw(2) << minuto << ":" 
            << std::setfill('0') << std::setw(2) << segundo << " " 
	      	<< meiodia[is_meiodia] << std::endl << std::endl;
    }
    vTaskDelete(NULL);
}

void UART_init(){
    const uart_port_t uart_num = UART_NUM_2;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
};
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    
    // Configure UART parameters
    // Set UART pins(TX: IO4, RX: IO5, RTS: IO18, CTS: IO19)
    // Setup UART buffered IO with event queue
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, 16, 17, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size, uart_buffer_size, 0, NULL, 0));
}


void Imprimir_UART(){
    //uint8_t *data = (uint8_t *) malloc(2048);

    char meiodia[2][3] = {"AM", "PM"};
    int mes, dia, ano, hora, minuto, segundo, pm, is_meiodia = 0;
    Dados data;
    for(int i = 0; i < 10; i++){
        data = Operacoes.read();
        stringstream ss;

        data.Data_Hora.readCalendar(&mes, &dia, &ano);
	    data.Data_Hora.readClock(&hora, &segundo, &minuto, &pm);
	    is_meiodia = (pm)? 1 : 0;

	    ss  << "Operação Executada: " << data.Operacao << std::endl 
            << "Data: " << std::setfill('0') << std::setw(2) << dia 
            << "/" << std::setfill('0') << std::setw(2) << mes << "/" << ano
	        << "\t Hora: " << std::setfill('0') << std::setw(2) << hora << ":" 
            << std::setfill('0') << std::setw(2) << minuto << ":" 
            << std::setfill('0') << std::setw(2) << segundo << " " 
	      	<< meiodia[is_meiodia] << std::endl << std::endl;
    
        // Read data from the UART
        int len = ss.gcount();
        // Write data back to the UART
        uart_write_bytes(UART_NUM_2, (const char *) ss.str().c_str(), len);
        
    }
    vTaskDelete(NULL);
}




    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    /* uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));
 */
