#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
using namespace std;
//#include "../include/clockcalendar.h"
#include "../include/Lista.h"
#include "../include/Serial.h"

//ClockCalendar cc1(11,18,2023,8,0,0,true);
mySerial Serial("/dev/ttyUSB0",115200);
List dados;

void ler_arquivo(const char*);
void listar_um(Dados);
void listar_tudo();
void imprimir_calendario(ClockCalendar cc);
void listar_intervalo();
void listarTempoFuncionamento();

int main(){
    
    while(1){
        try{
            ler_arquivo("Dados_Sinteticos.txt");
        }
        catch(const char* s){
            std::cout << s << endl;
        }        

        int i;
        cout << "Escolha uma opção:" << endl
             << "1: Listar Todos os elementos da lista." << endl
             << "2: Listar Elementos sobre um intervalo."<< endl
             << "3: Imprimir tempo de funcionamento."<< endl;

        cin >> i;

        switch (i)
        {
        case 1:
            listar_tudo();
            break;
        case 2:
            listar_intervalo();
            break;
        case 3:
            listarTempoFuncionamento();
            break;
        default:
            cout << "Comando invalido!";
            break;
        }

        this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void ler_arquivo(const char *s){
    ifstream leitura;
    string str;
    string data[8];
    leitura.open(s,ios::in);
    if (!leitura.is_open()){
        throw "Erro na Abertura do arquivo";
    }
    while(!leitura.eof()){
        leitura >> str;
        stringstream ss(str);

        for(int i = 0; getline(ss,data[i],';');i++);
        const char* op = data[0].c_str();
        ClockCalendar tempo(stoi(data[2]),stoi(data[1]),stoi(data[3]),stoi(data[4]),stoi(data[5]),stoi(data[6]),data[7].compare("AM"));
        Dados novo_item(op,tempo);
        dados.insertAfterLast(novo_item);
    }
    leitura.close();
}

void imprimir_calendario(ClockCalendar cc){
    int mes, dia, ano, hora, minuto, segundo, pm, is_meiodia = 0;
    
    cc.readCalendar(&mes, &dia, &ano);
    cc.readClock(&hora, &segundo, &minuto, &pm);
    is_meiodia = (pm)? 1 : 0;
    char meiodia[2][3] = {"AM", "PM"};
    setfill('0');
    setw(2);
    std::cout << "Data: " << std::setfill('0') << std::setw(2) << dia 
            << "/" << std::setfill('0') << std::setw(2) << mes << "/" << ano
	        << "\t Hora: " << std::setfill('0') << std::setw(2) << hora << ":" 
            << std::setfill('0') << std::setw(2) << minuto << ":" 
            << std::setfill('0') << std::setw(2) << segundo << " " 
	      	<< meiodia[is_meiodia] << std::endl << std::endl;
}

void listar_um(Dados data){
    int mes, dia, ano, hora, minuto, segundo, pm, is_meiodia = 0;
    
    data.Data_Hora.readCalendar(&mes, &dia, &ano);
    data.Data_Hora.readClock(&hora, &segundo, &minuto, &pm);
    is_meiodia = (pm)? 1 : 0;
    char meiodia[2][3] = {"AM", "PM"};
    setfill('0');
    setw(2);

    std::cout << "Operação Executada: " << data.Operacao << std::endl 
            << "Data: " << std::setfill('0') << std::setw(2) << dia 
            << "/" << std::setfill('0') << std::setw(2) << mes << "/" << ano
	        << "\t Hora: " << std::setfill('0') << std::setw(2) << hora << ":" 
            << std::setfill('0') << std::setw(2) << minuto << ":" 
            << std::setfill('0') << std::setw(2) << segundo << " " 
	      	<< meiodia[is_meiodia] << std::endl << std::endl;
}

void listar_tudo(){
    for(Dados data = Operacoes.remove();data.Operacao[0];data = Operacoes.remove()){
        listar_um(data);
    }
}

void listarTempoFuncionamento(){
    std::stringstream Tempo;
    try{bool lista = strcmp(dados.readFirst().Operacao,"");}
    catch (const char *s){
        ;
    }
    int hora, minuto, segundo, pm;
    ClockCalendar cc1 = dados.readLast().Data_Hora;

    cc1.subtract(dados.readFirst().Data_Hora);
    cc1.readClock(&hora,&segundo,&minuto,&pm);

    Tempo << "Tempo de funcionamento: "
    << std::setfill('0') << std::setw(2) << hora
    << ":" << std::setfill('0') << std::setw(2) << minuto
    << ":" << std::setfill('0') << std::setw(2) << segundo;

}