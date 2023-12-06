#include "headers.h"

void AplicacaoPrincipal(){  
    uint8_t current_operation;
    static uint8_t previous_operarion = 10;

    ei_impulse_result_t result = {0};

    void (*action[5])() = {Carro_ParaTraz,Carro_ParaFrente,Carro_ParaEsquerda,Carro_ParaDireita,Carro_Parar};
    // the features are stored into flash, and we don't want to load everything into RAM
    signal_t features_signal;
    features_signal.total_length = BUFFER_LEN;
    features_signal.get_data = &raw_feature_get_data;

    // invoke the impulse
    EI_IMPULSE_ERROR res = run_classifier_continuous(&features_signal, &result, false /* debug */);
    
    if (res) return;

    if ((result.classification[2].value > 0.5) || (result.classification[3].value > 0.5)){current_operation = 0;}
    else if ((result.classification[4].value > 0.5) || (result.classification[10].value > 0.5)){current_operation = 1;}
    else if (result.classification[5].value > 0.5){current_operation = 2;}
    else if (result.classification[7].value > 0.5){current_operation = 3;}
    else{current_operation = 4;}
    
    
    //if(previous_operarion != current_operation){
        previous_operarion = current_operation;
        Operacoes.insert({Operations[current_operation],*cc});
        std::thread Operating(action[current_operation]);
        Operating.detach();
    //}
    Mic->hearSound();
}


void app_main(void) {
    Mic->init();
    motor1->init();
    motor2->init();
    BLE_Servidor.initFull();
    UART_init();

    auto cfg = esp_pthread_get_default_config();
    cfg.stack_size = 4096;
    esp_pthread_set_cfg(&cfg);

    std::thread Clock(AtualizarClock);
    
    Operacoes.insert({"Inicializacao",*cc});
    
    while(1){
        std::thread Main_app(AplicacaoPrincipal);

        if (BLE_Servidor.getBLEEnvio()){
            BLE_Servidor.setBLEEnvio(false);
            enviarFilaBLE();
            //std::thread BluetoothEnviar(enviarFilaBLE);
            //BluetoothEnviar.detach();
        }

        Main_app.join();
    }    
}

int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, Mic->getSom() + offset, length * sizeof(float));
    return 0;
}
void Carro_ParaFrente(){
    motor1->setDirecao(0);
    motor1->setDuty(100);
    motor1->girar();

    motor2->setDirecao(0);
    motor2->setDuty(100);
    motor2->girar();    
}

void Carro_ParaTraz(){
    motor1->setDirecao(1);
    motor1->setDuty(100);
    motor1->girar();
    motor2->setDirecao(1);
    motor2->setDuty(100);
    motor2->girar();     
}

void Carro_ParaEsquerda(){
    motor1->setDirecao(0);
    motor1->setDuty(100);
    motor1->girar();
    motor2->setDirecao(0);
    motor2->setDuty(60);
    motor2->girar();
}

void Carro_ParaDireita(){
    motor1->setDirecao(0);
    motor1->setDuty(60);
    motor1->girar();
    motor2->setDirecao(0);
    motor2->setDuty(100);
    motor2->girar();
}

void Carro_Parar(){
    motor1->parar();
    motor2->parar();     
}

void AtualizarClock(){
    while(1){
        ++(*cc);
        std::this_thread::sleep_for(1000ms);
    }
    delete cc;
}

static void imprimirfila(void *args)
{
    try{
        for(Dados data = Operacoes.remove();data.Operacao[0];data = Operacoes.remove()){
            Imprimir_UART(data);
        }
    }
    catch (const char *s){;}
    catch(...){;}
    
    vTaskDelete(NULL);
}


void imprimirDado(Dados data)
{
    char meiodia[2][3] = {"AM", "PM"};
    int mes, dia, ano, hora, minuto, segundo, pm, is_meiodia = 0;
    
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

stringstream preparar_dado(Dados data){
    char meiodia[2][3] = {"AM", "PM"};
    int mes, dia, ano, hora, minuto, segundo, pm, is_meiodia = 0;
    stringstream ss;
    data = Operacoes.remove();

    data.Data_Hora.readCalendar(&mes, &dia, &ano);
    data.Data_Hora.readClock(&hora, &segundo, &minuto, &pm);
    is_meiodia = (pm)? 1 : 0;

    ss << data.Operacao << ";" 
        << dia << ";" << mes << ";" << ano << ";" 
        << hora << ";" << minuto << ";" << segundo << ";" 
        << meiodia[is_meiodia] << endl;

    return ss;
}

void enviarFilaBLE(){
    try{ 
        for(Dados data = Operacoes.remove();data.Operacao[0];data = Operacoes.remove()){
            BLE_Servidor.send(preparar_dado(data).str());
            //std::this_thread::sleep_for(1ms);
        }
    }
    catch (const char *s){;}
    catch(...){;}
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


void Imprimir_UART(Dados data){
    stringstream ss = preparar_dado(data);
    // Write data back to the UART
    std::cout << ss.str().c_str();    
}