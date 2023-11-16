#include "Motor.h"

Motor::Motor (int velocidade, int ID){
  this->velocidade = velocidade;
  this->ID = ID;
}

Motor::~Motor (){
  this->velocidade = 0;
  this->ID = 0;
}

void Motor::setVelocidade (int vel) {this->velocidade = vel;}
int Motor::getVelocidade () {return this->velocidade;}
void Motor::setID (int id) {this->ID = id;}
int Motor::getID () {return ID;}

