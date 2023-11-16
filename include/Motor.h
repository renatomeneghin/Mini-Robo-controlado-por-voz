#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED
#pragma once

class Motor {
protected:
  int velocidade;
  int ID;
public:
   Motor (int velocidade, int ID);
   ~Motor ();
   void setVelocidade (int vel);
   int getVelocidade ();
   void setID (int id);
   int getID ();
};


#endif