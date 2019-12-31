#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <ctime>
#include <time.h>
#include <cassert>
using namespace std;

struct Jugador{
  string nom;
  string posicio;
  int preu;
  string equip;
  int punts;
};

struct ConfEquip{
  int por;
  int def;
  int mig;
  int dav;
  int pressupost;
};

int PRESSUPOST;
vector<Jugador> POR;
vector<Jugador> DEF;
vector<Jugador> MIG;
vector<Jugador> DAV;

vector<Jugador>RESULTAT;

double DBL_MAX = std::numeric_limits<double>::max();

//Importar la base de dades en un vector de Jugadors per cada posicio
int parser(int argc, char** argv, const int& J) {
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

    //Utilitzem nomes els jugadors amb cost <  J
    Jugador P{nom,posicio,preu,club,punts};
    if(P.preu <= J){
      if(P.posicio == "por") POR.push_back(P);
      else if(P.posicio == "def") DEF.push_back(P);
      else if(P.posicio == "mig") MIG.push_back(P);
      else if(P.posicio == "dav") DAV.push_back(P);
    }
  }
  in.close();
  return 0;
}

//Generar un Equip aleatori amb la configuració demanada
void generarEquip(vector<Jugador>& Equip, ConfEquip confEquip, int& puntsEquip, int& preuEquip){
  int i = 0;
  Jugador J;
  //POR
  while(confEquip.por > 0){
    Jugador J = POR[rand()%(POR.size())];
    if(confEquip.pressupost - J.preu > 0){
      Equip[i] = J;
      --confEquip.por;
      confEquip.pressupost -= J.preu;
      puntsEquip += J.punts;
      preuEquip += J.preu;
      ++i;
    }
  }
  //DEF
  while(confEquip.def > 0){
    Jugador J = DEF[rand()%(DEF.size())];
    if(confEquip.pressupost - J.preu > 0){
      Equip[i] = J;
      --confEquip.def;
      confEquip.pressupost -= J.preu;
      puntsEquip += J.punts;
      preuEquip += J.preu;
      ++i;
    }
  }
  //MIG
  while(confEquip.mig > 0){
    Jugador J = MIG[rand()%(MIG.size())];
    if(confEquip.pressupost - J.preu > 0){
      Equip[i] = J;
      --confEquip.mig;
      confEquip.pressupost -= J.preu;
      puntsEquip += J.punts;
      preuEquip += J.preu;
      ++i;
    }
  }
  //DAV
  while(confEquip.dav > 0){
    Jugador J = DAV[rand()%(DAV.size())];
    if(confEquip.pressupost - J.preu > 0){
      Equip[i] = J;
      --confEquip.dav;
      confEquip.pressupost -= J.preu;
      puntsEquip += J.punts;
      preuEquip += J.preu;
      ++i;
    }
  }
}
/*
//Crear un equip vei canviant un jugador en una posició especifica
void nouEquip(int i, vector <Jugador>& EquipVei, int& puntsEquip, int& preuEquip, bool& millor){
  Jugador J;
  if(EquipVei[i].posicio == "por") J = POR[rand()%(POR.size())];
  else if(EquipVei[i].posicio == "def") J = DEF[rand()%(DEF.size())];
  else if(EquipVei[i].posicio == "mig") J = MIG[rand()%(MIG.size())];
  else if(EquipVei[i].posicio == "dav") J = DAV[rand()%(DAV.size())];
  if(PRESSUPOST - preuEquip + EquipVei[i].preu - J.preu > 0){
    if(puntsEquip < (puntsEquip - EquipVei[i].punts + J.punts)) millor = true;
    puntsEquip = puntsEquip - EquipVei[i].punts + J.punts;
    preuEquip = preuEquip - EquipVei[i].preu + J.preu;
    EquipVei[i] = J;
  }

}
*/

//Crear un equip vei canviant un jugador en una posició especifica
void nouEquip(int i, vector <Jugador>& EquipVei, int& puntsEquip, int& preuEquip, bool& millor){
  Jugador J;
  if(EquipVei[i].posicio == "por") J = POR[rand()%(POR.size())];
  else if(EquipVei[i].posicio == "def") J = DEF[rand()%(DEF.size())];
  else if(EquipVei[i].posicio == "mig") J = MIG[rand()%(MIG.size())];
  else if(EquipVei[i].posicio == "dav") J = DAV[rand()%(DAV.size())];
  if(PRESSUPOST - preuEquip + EquipVei[i].preu - J.preu > 0){
    if(puntsEquip < (puntsEquip - EquipVei[i].punts + J.punts)) millor = true;
    EquipVei[i] = J;
  }
  puntsEquip = 0;
  preuEquip = 0;
  for(Jugador N : EquipVei){
    puntsEquip += N.punts;
    preuEquip += N.preu;
  }
}

bool probabilitat(double P){
  P = int(P*100);
  return (rand() % 100) < P;
}

void updateProbabilitat(double& P, double& T, int f1, int f2){

  //Update T
  //double alpha = ((double) rand() / (RAND_MAX));
  double fMax = 0.999;
  double fMin = 0.8;
  double f = (double)rand() / RAND_MAX;
  double alpha =  fMin + f * (fMax - fMin);

  T = alpha * T;
  //Update P
  P = exp(-T);

}

void metaheuristica(vector <Jugador>& Equip, int& puntsEquip, int& preuEquip, double& T, double& P){
  //Triar posicio aleatoria per fer el canvi
  int i = rand()%(Equip.size());
  int f1 = puntsEquip;
  //Crear l'equip vei
  bool millorat = false; //indica si el nou equip es millor.
  vector<Jugador> EquipVei = Equip;
  nouEquip(i, EquipVei, puntsEquip, preuEquip, millorat);

  int f2 = puntsEquip;
  //Acceptar o rebutjar l'equip
  if(millorat) Equip = EquipVei;
  else if(probabilitat(P)) Equip = EquipVei;

  //Actualitzar la temperatura i la probabilitat
  updateProbabilitat(P, T, f1, f2);
}

int main(int argc, char** argv){
  srand(time(NULL));

  //Input de l'enunciat///////////////////////s'ha de canviar a input per fitxer
  ConfEquip confEquip;
  int J; // preu maxim per jugador
  confEquip.por = 1;
  cin >> confEquip.def >> confEquip.mig >> confEquip.dav >> confEquip.pressupost >> J;
  PRESSUPOST = confEquip.pressupost;
  ////////////////////////////////////////////

  parser(argc, argv, J);

  //Crear l'equip aleatrori inicial
  int puntsEquip = 0;
  int preuEquip = 0;
  vector<Jugador>Equip(11);
  generarEquip(Equip, confEquip, puntsEquip, preuEquip);

  //metaheuristica
  double T = DBL_MAX; //temperatura
  double P = 1; //Probabilitat d'accceptar una solucio pitjor


  int iteracions = 0;
  int puntsMax = 0;
  while(T > 1){
    metaheuristica(Equip, puntsEquip, preuEquip, T, P);
    if(puntsMax < puntsEquip){
      puntsMax = puntsEquip;
       RESULTAT = Equip;
       cout << "punts: " << puntsEquip << endl;
       //cout << "preu: " << preuEquip << endl;
       //cout << "P: " << P << endl;
       //cout << "T: " << T << endl;
    }
    ++iteracions;
    ////////////////////////////////////////////

    ///////////////////////////////////////////
  }
  cout << "iteracions: " << iteracions << endl;

}

/*
for(Jugador J : Equip) cout << J.nom << " "  << J.posicio << " pr:" << J.preu  << " pt:" << J.punts << endl;
cout << "punts: " << puntsEquip << endl;
cout << "preu: " << preuEquip << endl;

*/
