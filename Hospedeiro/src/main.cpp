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

//ClockCalendar cc1(11,18,2023,8,0,0,true);
List dados;

void imprimir_calendario();
void ler_arquivo(const char*);
void listar_um();
void listar_um(Dados);
void listar_tudo();
void imprimir_calendario(ClockCalendar cc);
void listar_intervalo();

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
             << "2: Listar Elementos sobre um intervalo."<< endl;

        cin >> i;

        switch (i)
        {
        case 1:
            listar_tudo();
            break;
        case 2:
            listar_intervalo();
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

/* void imprimir_calendario(){
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

} */

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

	//inicio = clock();
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

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
    for(Dados data = dados.removeFirst();data.Operacao[0];data = dados.removeFirst()){
        listar_um(data);
    }
}

void listar_intervalo(){
    ClockCalendar cc1, cc2;
    int dia1, mes1, ano1, hora1, minuto1, segundo1, pm1, dia2, mes2, ano2, hora2, minuto2, segundo2, pm2;
    
    cout << "Digite a Primeira data no formato: " << endl
         << "Dia/Mes/Ano    Hora:Minuto:Segundo (AM/PM = 0/1)" << endl;

    cin >> dia1;  cin >> mes1; cin >> ano1;
    cin >> hora1; cin >> minuto1; cin >> segundo1; cin >> pm1;
    
    cc1 = ClockCalendar(mes1,dia1,ano1,hora1,minuto1,segundo1,pm1);

    cout << "Digite a segunda data: " << endl;

    cin >> dia2; cin >> mes2; cin >> ano2; 
    cin >> hora2; cin >> minuto2; cin >> segundo2; cin >> pm2;
    
    cc2 = ClockCalendar(mes2,dia2,ano2,hora2,minuto2,segundo2,pm2);
    
    List validos;
    
    for(Dados dado = dados.removeFirst();dado.Operacao[0];dado = dados.removeFirst()){  
        ClockCalendar cc = dado.Data_Hora;

        if ((cc > cc1) && (cc < cc2)){
            validos.insertAfterLast(dado);
        }
    }

    cout << "Listando registros de operações válidos:" << endl;
    this_thread::sleep_for(std::chrono::milliseconds(500));
    for(Dados data = validos.removeFirst();data.Operacao[0];data = validos.removeFirst()){
        listar_um(data);
    }
}