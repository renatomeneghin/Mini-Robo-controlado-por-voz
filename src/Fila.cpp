#include "Fila.h"

Fila::Fila(){
   head = 0;
}

Fila::Fila(Dados novo){
  head = new Node(novo,0);
}

Fila::~Fila(){
  if (head){
    while(head->getNext()){
      Node* aux = head;
      head = head->getNext();
      delete aux;
    }
    delete head;
  }
}

void Fila::insert(Dados dat){
  Node* fim = new Node(dat,0);
  
  if (head){
    Node* aux = head;
  
    while(aux->getNext()){
      aux = aux->getNext();
    }
    aux->setNext(fim);  
  }
  else{
    head = fim;
  }
}

Dados Fila::read(){return head->getLog();}
Dados Fila::remove(){
  
  Dados valor = (head)? head->getLog() : Dados(0,ClockCalendar());
  
  if(head->getNext()){
    Node* aux = head;
    head = head->getNext();
    delete aux;
  }
  else{
    delete head;
    head = 0;
  }
  
  return valor;
}
