#include "../include/clockcalendar.h"

ClockCalendar::ClockCalendar() : Clock(-1, -1, -1, -1), Calendar(-1, -1, -1) {}
ClockCalendar::ClockCalendar(int mt, int d, int y, int h, int m, int s, int pm)
    : Clock(h, s, m, pm), Calendar(mt, d, y) {}
ClockCalendar::~ClockCalendar() {}

void ClockCalendar::advance() {
  int wasPm = is_pm; // mude de AM para PM.
  Clock::advance();
  if (wasPm && !is_pm)
    Calendar::advance();
}

void operator++(ClockCalendar &cc) { cc.advance(); }

bool ClockCalendar::operator==(const ClockCalendar &cc) const
{
  int dia, mes, ano, hora, minuto, segundo, pm;
  ClockCalendar c = cc;
  c.readCalendar(&mes,&dia,&ano);  
  c.readClock(&hora,&segundo,&minuto,&pm);
  //cc2.readCalendar(&m2,&d2,&a2);  
  //cc2.readClock(&h2,&s2,&mn2,&p2);
  return ((hr == hora) && (min == minuto) && (sec == segundo) && (is_pm == pm) && (yr == ano) && (mo == mes) && (day == dia));
}

bool ClockCalendar::operator!=(const ClockCalendar &cc) const
{
  int dia, mes, ano, hora, minuto, segundo, pm;
  ClockCalendar c(cc);
  c.readCalendar(&mes,&dia,&ano);  
  c.readClock(&hora,&segundo,&minuto,&pm);
  //cc2.readCalendar(&m2,&d2,&a2);  
  //cc2.readClock(&h2,&s2,&mn2,&p2);
  return ((hr != hora) || (min != minuto) || (sec != segundo) || (is_pm != pm) || (yr != ano) || (mo != mes) || (day != dia));
}

bool ClockCalendar::operator<(const ClockCalendar &cc) const
{
  bool resultado = false;
  int dia, mes, ano, hora, minuto, segundo, pm;
  ClockCalendar c(cc);
  c.readCalendar(&mes,&dia,&ano);  
  c.readClock(&hora,&segundo,&minuto,&pm);
  //cc2.readCalendar(&m2,&d2,&a2);  
  //cc2.readClock(&h2,&s2,&mn2,&p2);
  Calendar c1(mo,day,yr),c2(mes,dia,ano); 
  Clock cl1(hr,sec,min,is_pm), cl2(hora,segundo,minuto,pm);
  if(c1 < c2){
    resultado = true;
  } else if(c1 == c2){
    resultado = (cl1 < cl2);
  }
  return resultado;
}

bool ClockCalendar::operator>(const ClockCalendar &cc) const
{
  bool resultado = false;
  int dia, mes, ano, hora, minuto, segundo, pm;
  ClockCalendar c(cc);
  c.readCalendar(&mes,&dia,&ano);  
  c.readClock(&hora,&segundo,&minuto,&pm);
  Calendar c1(mo,day,yr),c2(mes,dia,ano); 
  Clock cl1(hr,sec,min,is_pm), cl2(hora,segundo,minuto,pm);
  if(c1 > c2){
    resultado = true;
  } else if(c1 == c2){
    resultado = (cl1 > cl2);
  }
  return resultado;
}

bool ClockCalendar::greater_than(ClockCalendar cc1){
  bool resultado = false;
  int dia = day, mes= mo, ano=yr, hora=hr, minuto=min, segundo=sec, pm=is_pm;
  int dia2, mes2, ano2, hora2, minuto2, segundo2, pm2;
  cc1.readCalendar(&mes2,&dia2,&ano2);  
  cc1.readClock(&hora2,&segundo2,&minuto2,&pm2);
  
  if(ano > ano2){
    resultado = true;
  }
  else if (ano == ano2){
    if(mes > mes2){
      resultado = true;
    }
    else if (mes == mes2){
      if (dia > dia2){
        resultado = true;
        }
      else if(dia == dia2){ 
        if(pm && !pm2){
            resultado = true;
        }
        else if (pm == pm2){
          if(hora > hora2){
              resultado = true;
            }
          else if (hora == hora2){
            if(minuto > minuto2){
              resultado = true;
            }
            else if (minuto == minuto2){
              if (segundo > segundo2){
                resultado = true;
              }
            }
          }
        }
      }
    }
  }
  return resultado;
}

bool ClockCalendar::smaller_than(ClockCalendar cc1){
  bool resultado = false;
  int dia = day, mes= mo, ano=yr, hora=hr, minuto=min, segundo=sec, pm=is_pm;
  int dia2, mes2, ano2, hora2, minuto2, segundo2, pm2;
  cc1.readCalendar(&mes2,&dia2,&ano2);  
  cc1.readClock(&hora2,&segundo2,&minuto2,&pm2);
  
  if(ano < ano2){
    resultado = true;
  }
  else if (ano == ano2){
    if(mes < mes2){
      resultado = true;
    }
    else if (mes == mes2){
      if (dia < dia2){
        resultado = true;
        }
        else if(dia == dia2){ 
          if(pm && !pm2){
            resultado = true;
          }
          else if (pm == pm2){
            if(hora < hora2){
              resultado = true;
              }
              else if (hora == hora2){
                if(minuto < minuto2){
                  resultado = true;
                  }
                  else if (minuto == minuto2){
                    if (segundo < segundo2){
                      resultado = true;
                      }
                  }
              }
          }
        }
    }
  }
  return resultado;
}
