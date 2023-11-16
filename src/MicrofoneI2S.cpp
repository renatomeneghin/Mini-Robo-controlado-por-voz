#include "../include/MicrofoneI2S.h"
#include "MicrofoneI2S.h"

Microfone_I2S::Microfone_I2S(int Data, int BCLK, int WS) : Microfone(Data)
{
    this->pino_BCLK = BCLK;
    this->pino_WS = WS;

    i2s_chan_config_t rx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&rx_chan_cfg, NULL, &this->rx_handle));

    i2s_std_config_t rx_std_cfg = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(16000),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_24BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,    // some codecs may require mclk signal, this example doesn't need it
            .bclk = pino_BCLK,
            .ws   = pino_WS,
            .dout = I2S_GPIO_UNUSED,
            .din  = Data,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };
    /* Default is only receiving left slot in mono mode,
     * update to right here to show how to change the default configuration */
    rx_std_cfg.slot_cfg.slot_mask = I2S_STD_SLOT_RIGHT;
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(this->rx_handle, &rx_std_cfg));
    /* Enable the RX channel */
    ESP_ERROR_CHECK(i2s_channel_enable(rx_chan));

}

Microfone_I2S::~Microfone_I2S()
{
    i2s_channel_disable(this->rx_handle);
    i2s_del_channel(this->rx_handle);
}

void Microfone_I2S::setPino_BCLK(int BCLK){this->pino_BCLK=BCLK;}
void Microfone_I2S::setPino_WS(int WS){this->pino_WS=WS;}
int Microfone_I2S::getPino_BCLK(){return this->pino_BCLK;}
int Microfone_I2S::getPino_WS(){return this->pino_WS;}

float Microfone_I2S::hearSound()
{
    uint8_t *r_buf = (uint8_t *)calloc(1, 64000);
    assert(r_buf); // Check if r_buf allocation success
    size_t r_bytes = 0;

    i2s_channel_read(rx_chan, r_buf, 64000, &r_bytes, 1000);

    
    


    return 
}
Microfone_I2S::operator float()
{
}
