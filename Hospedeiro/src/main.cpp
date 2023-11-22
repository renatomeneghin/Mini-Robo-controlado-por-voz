#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
using namespace std;
#include "../include/clockcalendar.h"
#include "../include/Lista.h"

//ClockCalendar cc1(11,18,2023,8,0,0,true);
List dados;

void imprimir_calendario();
void ler_arquivo(const char*);
void listar_um();
void listar_um(Dados);
void listar_tudo();

int main(){

    try{
        ler_arquivo("Dados_Sinteticos.txt");
    }
    catch(const char* s){
        std::cout << s << endl;
    }

    listar_tudo();
    
    return 0;
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
        ClockCalendar tempo(stoi(data[1]),stoi(data[2]),stoi(data[3]),stoi(data[4]),stoi(data[5]),stoi(data[6]),data[7].compare("AM"));
        Dados novo_item(op,tempo);
        dados.insertAfterLast(novo_item);
    }
    leitura.close();
}
/* 
void imprimir_calendario(){
    int mes, dia, ano, hora, minuto, segundo, pm, is_meiodia = 0;
    
    cc1.readCalendar(&mes, &dia, &ano);
    cc1.readClock(&hora, &segundo, &minuto, &pm);
    is_meiodia = (pm)? 1 : 0;
    char meiodia[2][3] = {"AM", "PM"};
    setfill('0');
    setw(2);
    std::cout << "Data: " <<  dia << "/"  << mes << "/" << ano
      << "\t Hora: " <<  hora << ":" << minuto << ":"  << segundo << " " 
      << meiodia[is_meiodia] << std::endl;

	//inicio = clock();
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}
 */
void listar_um(){

    Dados data = dados.removeFirst();

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
    List copia_dados = dados;
    for(Dados data = copia_dados.removeFirst();data.Operacao[0];data = copia_dados.removeFirst()){
        listar_um(data);
    }
}