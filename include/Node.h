
//#include "headers.h"
#include "clockcalendar.h"

struct Dados{
  int Operacao;
  ClockCalendar Data_Hora;
  Dados(int Op,ClockCalendar dt) : Operacao(Op) , Data_Hora(dt){};
  Dados(): Operacao(-1), Data_Hora(){};
};

class Node {
    Dados Log;
    Node* next;
  public:      
    Node(Dados Novo_Log, Node* nxt);
    Dados getLog();
    Node* getNext();
    void setLog(Dados NLog);
    void setNext(Node* nxt);
};

