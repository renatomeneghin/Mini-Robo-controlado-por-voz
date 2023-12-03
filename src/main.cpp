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


void app_main(void) {
    Mic->init();
    motor1->init();
    motor2->init();
    BLE_Servidor.initFull();
    Operacoes.insert({"Inicializacao",*cc});

    xTaskCreate(AtualizarClock,"ClockCalendar",1024,NULL,20,NULL);
    //vTaskDelay(500/ portTICK_PERIOD_MS);
    //xTaskCreatePinnedToCore(PWM_Class,"PWM",4096, NULL, 5, NULL,1);
    //vTaskDelay(1000/portTICK_PERIOD_MS);
    int i = 0;
    while(1){
        AplicacaoPrincipal();
        i++;
        if (i > 10){
            xTaskCreate(enviarFilaBLE,"Fila",4096,NULL,2,NULL);
            i = 0;
        }

        /* if (BLE_Servidor.getBLEEnvio){
            BLE_Servidor.setBLEEnvio(false);
            xTaskCreate(enviarFilaBLE,"UART",4096,NULL,2,NULL);
        } */
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

static void imprimirfila(void *args)
{
    for(Dados data = Operacoes.remove();data.Operacao[0];data = Operacoes.remove()){
        imprimirDado(data);
    }
    vTaskDelete(NULL);
}


void imprimirDado(Dados data)
{
    char meiodia[2][3] = {"AM", "PM"};
    int mes, dia, ano, hora, minuto, segundo, pm, is_meiodia = 0;

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
        << meiodia[is_meiodia];

    return ss;
}

static void enviarFilaBLE(void *args){
     
    for(Dados data = Operacoes.remove();data.Operacao[0];data = Operacoes.remove()){
        BLE_Servidor.send(preparar_dado(data).str());
        vTaskDelay(10/portTICK_PERIOD_MS);
    } 
}