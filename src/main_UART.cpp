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


    xTaskCreate(AtualizarClock,"Relogio",1024,NULL,25,NULL);
    
    int i = 0;
    puts("Cheguei no loop");
    while(1){
        AplicacaoPrincipal();
        i++;
        //ESP_ERROR_CHECK(uart_write_bytes(UART_NUM_0, "Hello World", 12));
        //printf("%d",i);
        if (i > 10){
            xTaskCreate(Imprimir_UART,"UART",8192,NULL,5,NULL);
            //imprimirFila();
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
        try{
            data = Operacoes.remove();
        }
        catch(const char *s){
            ;
        }
        catch(...){
            ;
        }
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
    //vTaskDelete(NULL);
}

void UART_init(){
    const uart_port_t uart_num = UART_NUM_0;
    const int uart_buffer_size = 256;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // Configure UART parameters
    
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, GPIO_NUM_1, GPIO_NUM_3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size,uart_buffer_size, 0, NULL, 0));
    
    
}


static void Imprimir_UART(void *args){
    char meiodia[2][3] = {"AM", "PM"};
    int mes, dia, ano, hora, minuto, segundo, pm, is_meiodia = 0;
    Dados data;
    stringstream ss;
    
    try{
        data = Operacoes.remove();
    }
    catch(const char *s){
        ;
    }
    catch(...){
        ;
    }
    
    data.Data_Hora.readCalendar(&mes, &dia, &ano);
    data.Data_Hora.readClock(&hora, &segundo, &minuto, &pm);
    is_meiodia = (pm)? 1 : 0;

    ss  << data.Operacao << ";" 
    << dia << ";" << mes << ";" << ano
    << ";" << hora << ";" << minuto << ";" << segundo 
    << ";" << meiodia[is_meiodia] << std::endl;

    // Read data from the UART
    int len = ss.gcount();
    
    // Write data back to the UART
    std::cout << ss.str().c_str();
    //ESP_ERROR_CHECK(uart_write_bytes(UART_NUM_0, (const char *) ss.str().c_str(), len));

    vTaskDelete(NULL);
}