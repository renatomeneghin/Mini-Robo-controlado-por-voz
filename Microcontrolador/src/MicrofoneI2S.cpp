#include "../include/MicrofoneI2S.h"


#define MAX_DMA_BUFFER 1000

Microfone_I2S::Microfone_I2S(gpio_num_t BCLK, gpio_num_t WS, gpio_num_t LR, gpio_num_t DA)
{
    this->pino_BCLK = BCLK;
    this->pino_WS = WS;
    this->pino_LR = LR;
    this->pino_DA = DA;
    this->periferico = I2S_NUM_AUTO;
    this->funcao = I2S_ROLE_MASTER;
    this->comprimeto_dados = I2S_DATA_BIT_WIDTH_16BIT;
    this->comprimeto_pacote = I2S_SLOT_BIT_WIDTH_AUTO;
    this->tipo_pacote = I2S_SLOT_MODE_STEREO;
    this->mascara_pacote = I2S_STD_SLOT_BOTH;
    this->clock_base = I2S_CLK_SRC_DEFAULT;
    this->multiplicador_clock = I2S_MCLK_MULTIPLE_256;
}

Microfone_I2S::Microfone_I2S(uint32_t freq,gpio_num_t BCLK, gpio_num_t WS, gpio_num_t LR, gpio_num_t DA, 
    i2s_port_t i2s, i2s_role_t role, i2s_data_bit_width_t
    comprimento, i2s_slot_bit_width_t pacote, i2s_slot_mode_t modo, 
    i2s_std_slot_mask_t mascara, soc_periph_i2s_clk_src_t clock, 
    i2s_mclk_multiple_t multiplicador) : Microfone(freq)
{
    this->pino_BCLK = BCLK;
    this->pino_WS = WS;
    this->pino_LR = LR;
    this->pino_DA = DA;
    this->periferico = i2s;
    this->funcao = role;
    this->comprimeto_dados = comprimento;
    this->comprimeto_pacote = pacote;
    this->tipo_pacote = modo;
    this->mascara_pacote = mascara;
    this->clock_base = clock;
    this->multiplicador_clock = multiplicador;
}

Microfone_I2S::~Microfone_I2S()
{
    i2s_channel_disable(this->rx_handle);
    i2s_del_channel(this->rx_handle);
}

void Microfone_I2S::setPeriferico(i2s_port_t per){this->periferico = per;}
void Microfone_I2S::setFuncao(i2s_role_t func){this->funcao = func;}
void Microfone_I2S::setComprimentoDados(i2s_data_bit_width_t data){this->comprimeto_dados = data;}
void Microfone_I2S::setComprimentoPacote(i2s_slot_bit_width_t pacote){this->comprimeto_pacote = pacote;}
void Microfone_I2S::setModoPacote(i2s_slot_mode_t modo){this->tipo_pacote = modo;}
void Microfone_I2S::setMascara(i2s_std_slot_mask_t mascara){this->mascara_pacote = mascara;}
void Microfone_I2S::setClockBase(soc_periph_i2s_clk_src_t clk){this->clock_base = clk;}
void Microfone_I2S::setMultiplicadorClock(i2s_mclk_multiple_t mclk){this->multiplicador_clock = mclk;}

void Microfone_I2S::reconfig()
{
    i2s_channel_disable(this->rx_handle);
    i2s_del_channel(this->rx_handle);
    this->init();
}

void Microfone_I2S::init()
{
    i2s_chan_config_t rx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(this->periferico, this->funcao);
    
    i2s_std_config_t rx_std_cfg = {
        .clk_cfg = {.sample_rate_hz = this->frequencia, .clk_src = I2S_CLK_SRC_DEFAULT, .mclk_multiple = I2S_MCLK_MULTIPLE_384, },
        .slot_cfg = { 
            .data_bit_width = this->comprimeto_dados, 
            .slot_bit_width = this->comprimeto_pacote, 
            .slot_mode = this->tipo_pacote, 
            .slot_mask = this->mascara_pacote, 
            .ws_width = this->comprimeto_pacote, 
            .ws_pol = false, 
            .bit_shift = true, 
            .msb_right = (this->comprimeto_dados <= I2S_DATA_BIT_WIDTH_16BIT) ? true : false, 
        },
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,    // some codecs may require mclk signal, this example doesn't need it
            .bclk = this->pino_BCLK,
            .ws   = this->pino_WS,
            .dout = I2S_GPIO_UNUSED,
            .din  = this->pino_DA,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };
    
    
    ESP_ERROR_CHECK(i2s_new_channel(&rx_chan_cfg, NULL, &this->rx_handle));
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(this->rx_handle, &rx_std_cfg));
    ESP_ERROR_CHECK(i2s_channel_enable(this->rx_handle));
}

void Microfone_I2S::hearSound()
{
    int *r_buf = (int *)calloc(1,MAX_DMA_BUFFER*sizeof(int));
    assert(r_buf); // Check if r_buf allocation success
    size_t r_bytes = 0;

    int itr = BUFFER_LEN/MAX_DMA_BUFFER;
    for(int i = 0; i < itr ; i++){
        if(i2s_channel_read(this->rx_handle, r_buf, MAX_DMA_BUFFER*sizeof(int), &r_bytes, 1000) == ESP_OK){
            for (int k = 0; k < MAX_DMA_BUFFER; k++){
                    this->Som_Buffer[k+i*1000] = (int)((r_buf[k] & ~(0b11))/0x1000); // 24bit MSB to float 
            }
        //vTaskDelay(10);
        }
    }
    free(r_buf);
    //return this->Som_Buffer;
}
