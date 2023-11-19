#include "../include/Microfone.h"
#include "Microfone.h"

Microfone::Microfone(){this->frequencia = 48000;}
Microfone::Microfone(uint32_t freq){this->frequencia = freq;}
float *Microfone::getSom() {return this->Som_Buffer;}
Microfone::operator float *() { return this->Som_Buffer; }
void Microfone::set_freq(uint32_t freq){this->frequencia = freq;}
