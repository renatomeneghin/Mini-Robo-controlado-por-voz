#include "Microfone.h"

Microfone::Microfone(int pino_in){this->pino = pino_in;}
Microfone::~Microfone() { this->pino = 0; }
void Microfone::setPino(int pino) { this->pino = pino; }
int Microfone::getPino() { return pino;}
Microfone::operator float(){return this->hearSound();}