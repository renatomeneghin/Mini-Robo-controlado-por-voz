#include "clockcalendar.h"
#include <string.h>

struct Dados{
  char Operacao[20];
  ClockCalendar Data_Hora;
  Dados(const char *Op,ClockCalendar dt){strcpy(Operacao,Op);Data_Hora=dt;}
  Dados(): Operacao(""), Data_Hora(){};
  bool operator==(Dados& a){return ( !strcmp(Operacao,a.Operacao) && Data_Hora == a.Data_Hora);}
  bool operator!=(Dados& a){return ( strcmp(Operacao,a.Operacao) || (Data_Hora != a.Data_Hora));}

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

