# 🤖 Mini Robô Controlado por Voz

Sistema embarcado desenvolvido com **ESP32** para controle de um pequeno robô a partir de **comandos de voz processados localmente**.

O projeto integra aquisição de áudio por **I2S**, processamento e classificação utilizando **TinyML**, controle de motores por **MCPWM**, comunicação **Bluetooth Low Energy (BLE)** e **UART**, além de estruturas de dados desenvolvidas em C++ para registro das operações executadas.

O firmware foi desenvolvido utilizando **ESP-IDF**, com integração ao **PlatformIO** e utilização de C++ para abstração dos principais componentes do sistema.

---

## 🎯 Objetivo

O objetivo do projeto é desenvolver uma plataforma embarcada capaz de:

* adquirir sinais de áudio através de um microfone I2S;
* processar os dados de áudio no ESP32;
* utilizar um modelo de Machine Learning embarcado para reconhecer comandos;
* converter os comandos reconhecidos em ações para o robô;
* controlar os motores utilizando PWM;
* registrar as operações realizadas;
* disponibilizar informações por comunicação BLE;
* utilizar UART para comunicação e depuração.

A arquitetura busca demonstrar a integração entre **aquisição de sinais, processamento digital, Machine Learning e controle de hardware** em um sistema embarcado de baixo custo.

---

## 🧠 Reconhecimento de voz com TinyML

O reconhecimento dos comandos é realizado diretamente no ESP32 utilizando um modelo integrado através do **Edge Impulse SDK**.

O fluxo principal do processamento é:

```text
Microfone I2S
      │
      ▼
Aquisição do áudio
      │
      ▼
Buffer de amostras
      │
      ▼
Modelo TinyML
      │
      ▼
Classificação do comando
      │
      ▼
Ação correspondente
      │
      ▼
Controle dos motores
```

O firmware utiliza `run_classifier_continuous()` para executar a inferência sobre os dados adquiridos pelo microfone.

As classificações são então associadas às operações do robô:

| Operação        | Ação                  |
| --------------- | --------------------- |
| `Para-traz`     | Movimento para trás   |
| `Para-Frente`   | Movimento para frente |
| `Para-Esquerda` | Curva para esquerda   |
| `Para-Direita`  | Curva para direita    |
| `Parar`         | Parada dos motores    |

O processamento ocorre **localmente**, sem depender de um servidor externo para interpretar o comando de voz.

---

## 🎙️ Aquisição de áudio

A aquisição do áudio é realizada através da interface **I2S** do ESP32.

O projeto possui uma implementação própria da classe `Microfone_I2S`, permitindo configurar parâmetros como:

* frequência de amostragem;
* largura dos dados;
* largura do slot I2S;
* modo mono/estéreo;
* seleção do slot;
* fonte de clock;
* multiplicador do clock;
* pinos da interface I2S.

A configuração utilizada no sistema principal inclui:

```text
Sample rate: 16 kHz
Data width: 24 bits
Slot width: 32 bits
Modo: Mono
I2S: Master
```

O áudio é adquirido através do periférico I2S e armazenado em um buffer antes de ser disponibilizado para o classificador.

A implementação também utiliza DMA através do driver I2S do ESP-IDF para realizar a aquisição dos dados.

---

## 🚗 Controle dos motores

O controle dos motores foi implementado utilizando o periférico **MCPWM** do ESP32.

A arquitetura utiliza uma abstração em C++:

```text
Motor
  │
  └── Driver_motor
          │
          └── MCPWM
```

A classe abstrata `Motor` define a interface básica:

```cpp
virtual void init() = 0;
virtual void girar() = 0;
virtual void parar() = 0;
```

Enquanto `Driver_motor` implementa essa interface utilizando os recursos de hardware do ESP32.

O driver permite controlar:

* duty cycle;
* direção;
* frequência;
* período;
* geração do PWM;
* parada do motor.

No projeto principal são utilizados dois motores:

```text
Motor 1 → GPIO 4 / GPIO 5
Motor 2 → GPIO 18 / GPIO 19
```

As ações de movimento são implementadas através de funções dedicadas:

```cpp
Carro_ParaFrente();
Carro_ParaTraz();
Carro_ParaEsquerda();
Carro_ParaDireita();
Carro_Parar();
```

Isso permite separar a decisão produzida pelo classificador da implementação de baixo nível do acionamento dos motores.

---

## 📡 Comunicação BLE

O projeto também implementa um servidor **Bluetooth Low Energy** utilizando **NimBLE**.

A classe `BLE` encapsula:

* criação do servidor;
* criação do serviço;
* características RX/TX;
* advertising;
* conexão e desconexão;
* autenticação;
* envio de informações.

A comunicação utiliza características compatíveis com uma arquitetura semelhante a UART sobre BLE:

```text
RX → comandos / requisições
TX → dados enviados pelo ESP32
```

O ESP32 pode transmitir informações referentes às operações realizadas pelo robô através de notificações BLE.

---

## 📝 Registro das operações

As ações executadas pelo robô são armazenadas em uma estrutura de fila.

O projeto implementa uma estrutura própria baseada em nós:

```text
Fila
 │
 ├── Node
 │     └── Dados
 │
 ├── Node
 │     └── Dados
 │
 └── Node
       └── Dados
```

Cada registro contém informações sobre a operação e o instante em que ela ocorreu.

As operações são armazenadas utilizando a classe `Fila` e posteriormente podem ser:

* impressas pela UART;
* enviadas através do BLE;
* utilizadas para acompanhamento das ações executadas.

---

## 🕒 Registro de data e hora

O projeto possui classes próprias para manipulação de:

* relógio;
* calendário;
* data e hora.

A aplicação mantém um relógio atualizado em uma thread separada e associa o instante da execução a cada operação registrada.

Um registro pode ser convertido para uma representação textual semelhante a:

```text
Operacao;dia;mes;ano;hora;minuto;segundo;AM/PM
```

Isso permite que o histórico das ações seja posteriormente transmitido ou armazenado.

---

## 🔌 UART

A comunicação UART do ESP32 é configurada através dos drivers do ESP-IDF.

A configuração principal utiliza:

```text
Baud rate: 115200
Data bits: 8
Parity: None
Stop bits: 1
Flow control: Disabled
```

A UART é utilizada principalmente para saída de informações e acompanhamento do funcionamento do sistema.

O projeto também possui uma implementação denominada `Virtual_UART`, relacionada à abstração da comunicação serial.

---

## 🧵 Concorrência e threads

O firmware utiliza recursos de concorrência disponíveis no ESP-IDF/FreeRTOS e também a abstração de threads em C++.

Entre as atividades executadas de maneira independente estão:

```text
             ┌─────────────────────┐
             │     ESP32           │
             │                     │
             │  ┌───────────────┐  │
Microfone ──►│  │ Aquisição I2S │  │
             │  └───────┬───────┘  │
             │          │          │
             │          ▼          │
             │  ┌───────────────┐  │
             │  │    TinyML     │  │
             │  └───────┬───────┘  │
             │          │          │
             │          ▼          │
             │  ┌───────────────┐  │
             │  │   Controle    │  │
             │  │    motores    │  │
             │  └───────────────┘  │
             │                     │
             │  ┌───────────────┐  │
             │  │     BLE       │  │
             │  └───────────────┘  │
             │                     │
             │  ┌───────────────┐  │
             │  │     UART      │  │
             │  └───────────────┘  │
             └─────────────────────┘
```

O projeto utiliza ainda `esp_pthread` para configuração das threads criadas pela aplicação.

---

## 🧩 Arquitetura de software

O firmware foi estruturado em diferentes componentes C++.

Principais módulos:

| Módulo               | Função                                           |
| -------------------- | ------------------------------------------------ |
| `main.cpp`           | Aplicação principal e integração dos componentes |
| `MicrofoneI2S.cpp`   | Aquisição de áudio através de I2S                |
| `BLE.cpp`            | Comunicação Bluetooth Low Energy                 |
| `Driver_Motores.cpp` | Implementação do controle dos motores            |
| `Motor.cpp`          | Abstração dos motores                            |
| `Fila.cpp`           | Estrutura de fila para registro das operações    |
| `Lista.cpp`          | Estrutura de dados auxiliar                      |
| `Node.cpp`           | Nós das estruturas encadeadas                    |
| `clock.cpp`          | Controle do relógio                              |
| `calendar.cpp`       | Manipulação do calendário                        |
| `clockcalendar.cpp`  | Integração de data e hora                        |
| `Virtual_UART.cpp`   | Abstração de comunicação serial                  |
| `TinyML.cpp`         | Interface associada ao módulo TinyML             |

O projeto também contém exemplos e implementações experimentais relacionadas a BLE, Wi-Fi e UART.

---

## 📱 Aplicação hospedeira

O repositório contém ainda um diretório `Hospedeiro`, destinado à comunicação com o sistema embarcado.

Essa parte inclui:

* código C++;
* comunicação serial;
* estruturas de dados;
* manipulação de data e hora;
* dados sintéticos para testes;
* aplicativo Android empacotado.

Estrutura simplificada:

```text
Hospedeiro/
├── include/
│   ├── Lista.h
│   ├── Node.h
│   ├── Serial.h
│   ├── calendar.h
│   ├── clock.h
│   └── clockcalendar.h
│
├── src/
│   ├── main.cpp
│   ├── Serial.cpp
│   ├── Lista.cpp
│   ├── Node.cpp
│   ├── calendar.cpp
│   ├── clock.cpp
│   └── clockcalendar.cpp
│
├── Dados_Sinteticos.txt
└── Hospedeiro-Android.zip
```

O hospedeiro permite trabalhar com os dados produzidos pelo sistema embarcado e representa uma extensão da arquitetura além do próprio firmware.

---

## 🛠️ Tecnologias utilizadas

### Hardware

* ESP32
* Microfone digital I2S
* Motores DC
* Driver de motores
* Interface UART
* Comunicação Bluetooth Low Energy

### Software

* C++
* ESP-IDF
* PlatformIO
* FreeRTOS
* NimBLE
* Edge Impulse SDK
* TinyML
* I2S
* MCPWM
* UART

---

## 📁 Estrutura do repositório

```text
Mini-Robo-controlado-por-voz/
│
├── include/
│   ├── BLE.h
│   ├── Driver_Motores.h
│   ├── Motor.h
│   ├── MicrofoneI2S.h
│   ├── Fila.h
│   ├── Node.h
│   ├── Virtual_UART.h
│   ├── clock.h
│   ├── calendar.h
│   ├── clockcalendar.h
│   └── headers.h
│
├── src/
│   ├── main.cpp
│   ├── BLE.cpp
│   ├── MicrofoneI2S.cpp
│   ├── Driver_Motores.cpp
│   ├── Motor.cpp
│   ├── Fila.cpp
│   ├── Lista.cpp
│   ├── Node.cpp
│   ├── TinyML.cpp
│   ├── Virtual_UART.cpp
│   ├── clock.cpp
│   ├── calendar.cpp
│   ├── clockcalendar.cpp
│   └── ...
│
├── lib/
│   ├── Rede_Neural.zip
│   └── esp-nimble-cpp-master.zip
│
├── Hospedeiro/
│   ├── include/
│   ├── src/
│   ├── Dados_Sinteticos.txt
│   └── Hospedeiro-Android.zip
│
├── test/
│
├── platformio.ini
├── CMakeLists.txt
├── sdkconfig.esp32dev
└── default.csv
```

---

## ⚙️ Ambiente de desenvolvimento

O projeto utiliza **PlatformIO** com a plataforma ESP32 e framework **ESP-IDF**.

Configuração principal:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = espidf
monitor_speed = 115200
```

As bibliotecas específicas do projeto são mantidas localmente no diretório `lib/`.

Entre elas estão:

```text
Rede_Neural.zip
esp-nimble-cpp-master.zip
```

O projeto também utiliza uma tabela de partições personalizada através de:

```text
default.csv
```

---

## 🚀 Compilação

Com o PlatformIO instalado:

```bash
pio run
```

Para gravar o firmware:

```bash
pio run --target upload
```

Para abrir o monitor serial:

```bash
pio device monitor
```

A velocidade configurada para o monitor é:

```text
115200 baud
```

---

## 🔬 Aspectos técnicos explorados

Este projeto foi desenvolvido como uma plataforma prática para explorar diferentes conceitos de sistemas embarcados:

* programação C++ em microcontroladores;
* orientação a objetos aplicada a drivers;
* abstração de hardware;
* aquisição de sinais digitais;
* interface I2S;
* processamento de áudio;
* Machine Learning embarcado;
* controle PWM;
* MCPWM;
* comunicação BLE;
* UART;
* estruturas de dados;
* filas e listas encadeadas;
* concorrência;
* FreeRTOS;
* integração entre firmware e aplicação hospedeira.

Um dos aspectos centrais do projeto é a integração entre **processamento de sinais e controle embarcado**:

```text
       Sinal físico
           │
           ▼
     ┌─────────────┐
     │ Microfone   │
     └──────┬──────┘
            │ I2S
            ▼
     ┌─────────────┐
     │ ESP32       │
     │             │
     │  TinyML     │
     └──────┬──────┘
            │
      comando reconhecido
            │
            ▼
     ┌─────────────┐
     │ Controle    │
     │ dos motores │
     └──────┬──────┘
            │
            ▼
        🤖 Robô
```

---

## 📚 Contexto do projeto

O projeto foi desenvolvido no contexto da disciplina **Programação em Sistemas Embarcados**, com o objetivo de aplicar conceitos de desenvolvimento de software e hardware em uma plataforma embarcada real.

Além da aplicação final, o repositório mantém diferentes experimentos realizados durante o desenvolvimento, incluindo exemplos de BLE, Wi-Fi, UART, estruturas de dados e processamento de sinais.

Por isso, o repositório deve ser entendido não apenas como o firmware de um robô, mas também como um registro do desenvolvimento de uma plataforma experimental baseada em ESP32.

---

## 📌 Status

**Projeto funcional / experimental**

A implementação principal integra:

* aquisição de áudio;
* inferência TinyML;
* controle dos motores;
* registro das operações;
* BLE;
* UART.

O repositório também mantém códigos auxiliares e experimentais utilizados durante o desenvolvimento.

---

## 👤 Autor

**Renato Augusto Schenkel Meneghin**

Projeto desenvolvido para estudos e desenvolvimento de sistemas embarcados.

---

## 🔗 Repositório

[Mini-Robo-controlado-por-voz no GitHub](https://github.com/renatomeneghin/Mini-Robo-controlado-por-voz?utm_source=chatgpt.com)
