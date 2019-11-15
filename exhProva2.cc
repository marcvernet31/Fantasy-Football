#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <ctime>
#include <cassert>
using namespace std;

struct Jugador{
  string nom;
  string posicio;
  int preu;
  string equip;
  int punts;
};

struct Equip{
  int por;
  int def;
  int mig;
  int dav;
  int pressupost;
};


Equip CONF_EQUIP;
int PREU_MIN;
int PUNTS_MAX;
int MILLOR_PUNTUACIO = 0;
vector<Jugador> DATABASE;

void input(int argc, char** argv, int& J) {
  CONF_EQUIP.por = 1;
  string filename = argv[2];
  ifstream f(filename);
  f >> CONF_EQUIP.def >> CONF_EQUIP.mig >> CONF_EQUIP.dav >> CONF_EQUIP.pressupost >> J;
	f.close();
}

int parser(int argc, char** argv, const int& J) {
  PREU_MIN = std::numeric_limits<int>::max();
  PUNTS_MAX = std::numeric_limits<int>::min();

  if (argc != 4) {
    cout << "Syntax: " << argv[0] << " data_base.txt" << endl;
    exit(1);
  }
  ifstream in(argv[1]);
  while (not in.eof()) {
    string nom, posicio, club;
    int punts, preu;
    getline(in,nom,';');    if (nom == "") break;
    getline(in,posicio,';');
    in >> preu;
    char aux; in >> aux;
    getline(in,club,';');
    in >> punts;
    string aux2;
    getline(in,aux2);

    Jugador P{nom,posicio,preu,club,punts};
    if(P.preu <= J){
      DATABASE.push_back(P);
      if(PREU_MIN > P.preu and P.preu != 0) PREU_MIN = P.preu;
      if(PUNTS_MAX < P.punts) PUNTS_MAX = P.punts;
    }
  }
  in.close();
  return 0;
}

/*
//Comprovar que la configuració de l'equip sigui correcte
bool checkConf(const int& contador, const string& posicio, const int& costEquip){
  if(CONF_EQUIP.pressupost > costEquip){
    if(posicio == "por" and CONF_EQUIP.por >= contador)return true;
    if(posicio == "def" and CONF_EQUIP.def >= contador) return true;
    if(posicio == "mig" and CONF_EQUIP.mig >= contador) return true;
    if(posicio == "dav" and CONF_EQUIP.dav >= contador) return true;
  }
  return false;
}
*/

//Comprova la possibilitat de que el preu es passi del pressupost
bool checkPreu(const int& contPOR, const int& contDEF, const int& contMIG, const int& contDAV, int costEquip){
  int n = (11 - contPOR - contDEF - contMIG - contDAV);
  if(n*PREU_MIN+costEquip > CONF_EQUIP.pressupost) return false;
  return true;
}

bool checkPunts(const int& contPOR, const int& contDEF, const int& contMIG, const int& contDAV, int puntsEquip){
  int n = (11 - contPOR - contDEF - contMIG - contDAV);
  if(n*PUNTS_MAX+puntsEquip < MILLOR_PUNTUACIO) return false;
  return true;
}

void exhaustiu(int i, const int& contPOR, const int& contDEF, const int& contMIG, const int& contDAV, int costEquip, int puntsEquip,
     vector<Jugador>& iteracioActual, vector<bool>& usat){
  if(i == 10){
    //output de prova
    for(Jugador J : iteracioActual) cout << J.nom << " ";
    cout << endl;

  }
  else{
    int n = DATABASE.size();
    for(unsigned int j = 0; j < n; ++j){
      if(not usat[j]){
        usat[j] = true;
        iteracioActual[i] = DATABASE[j];
        costEquip = costEquip+iteracioActual[i].preu;
        puntsEquip = puntsEquip+iteracioActual[i].punts;
        if(MILLOR_PUNTUACIO < puntsEquip) MILLOR_PUNTUACIO = puntsEquip;

        if(checkPreu(contPOR, contDEF, contMIG, contDAV, costEquip) and checkPunts(contPOR, contDEF, contMIG, contDAV, puntsEquip)
            and CONF_EQUIP.pressupost > costEquip){

          if(iteracioActual[i].posicio == "por" and CONF_EQUIP.por >= contPOR){
            exhaustiu(i+1, contPOR+1, contDEF, contMIG, contDAV, costEquip, puntsEquip, iteracioActual, usat);
          }
          else if(iteracioActual[i].posicio == "def" and CONF_EQUIP.def >= contDEF){
            exhaustiu(i+1, contPOR, contDEF+1, contMIG, contDAV, costEquip, puntsEquip, iteracioActual, usat);
          }
          else if(iteracioActual[i].posicio == "mig" and CONF_EQUIP.mig >= contMIG){
            exhaustiu(i+1, contPOR, contDEF, contMIG+1, contDAV, costEquip, puntsEquip, iteracioActual, usat);
          }
          else if(iteracioActual[i].posicio == "dav" and CONF_EQUIP.dav >= contDAV){
            exhaustiu(i+1, contPOR, contDEF, contMIG, contDAV+1, costEquip, puntsEquip, iteracioActual, usat);
          }
        }
        usat[j] = false;
        costEquip = costEquip-iteracioActual[i].preu;
        puntsEquip = puntsEquip-iteracioActual[i].punts;

      }
    }
  }
}



int main(int argc, char** argv){
  int J; // preu maxim per jugador
  input(argc, argv, J);
  parser(argc, argv, J);

  vector<Jugador> iteracioActual(11);  //string actual a la recursivitat
  vector<bool> usat(DATABASE.size(), false); //vector de bool per no repetir jugadors
  exhaustiu(0,0,0,0,0,0,0, iteracioActual, usat);
}
//  string outputFile = argv[3];
