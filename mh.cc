#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <sstream>
#include<map>
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

//Configuracio d'un Equip: nombre de jugadors per cada posició
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
map<string, bool> USAT;

string outputFile;

//Llegida de dades d'entrada
void input(int argc, char** argv, int& J, ConfEquip& confEquip) {
  string filename = argv[2];
  ifstream f(filename);
  f >> confEquip.def >> confEquip.mig >> confEquip.dav >> confEquip.pressupost >> J;
	f.close();

  confEquip.por = 1;
  PRESSUPOST = confEquip.pressupost;
}

//Escriptura del resultat al fitxer de sortida
void output(clock_t start, ConfEquip confEquip){
  double temps = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  int puntsTotal = 0; int preuTotal = 0;
  ofstream File;
  File.open (outputFile);

  File << temps << endl;
  int i = 0;
  File << "POR: " << RESULTAT[0].nom << endl;
  puntsTotal += RESULTAT[0].punts;
  preuTotal += RESULTAT[0].preu;
  ++i;

  File << "DEF: ";
  bool primer = true;
  while(i<11 and RESULTAT[i].posicio == "def"){
    File << (primer ? "" : ";") << RESULTAT[i].nom;
    primer = false;
    puntsTotal += RESULTAT[i].punts;
    preuTotal += RESULTAT[i].preu;
    ++i;
  }

  File << endl << "MIG: ";
  primer = true;
  while(i<11 and RESULTAT[i].posicio == "mig" and i<11){
    File << (primer ? "" : ";") << RESULTAT[i].nom;
    primer = false;
    puntsTotal += RESULTAT[i].punts;
    preuTotal += RESULTAT[i].preu;
    ++i;
  }

  File << endl << "DAV: ";
  primer = true;
  while(i<11 and RESULTAT[i].posicio == "dav" and i<11){
    File << (primer ? "" : ";") << RESULTAT[i].nom;
    primer = false;
    puntsTotal += RESULTAT[i].punts;
    preuTotal += RESULTAT[i].preu;
    ++i;
  }

  File << endl;
  File << "Punts: " << puntsTotal << endl;
  File << "Preu: " << preuTotal << endl;

  File.close();
}

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
      USAT.insert({J.nom, true});
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
      USAT.insert({J.nom, true});
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
      USAT.insert({J.nom, true});
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
      USAT.insert({J.nom, true});
    }
  }
}

//Crear un equip vei canviant un jugador en una posició especifica
void nouEquip(int i, vector <Jugador>& EquipVei, int& puntsEquip, int& preuEquip, bool& millor){
  Jugador J;
  if(EquipVei[i].posicio == "por") J = POR[rand()%(POR.size())];
  else if(EquipVei[i].posicio == "def") J = DEF[rand()%(DEF.size())];
  else if(EquipVei[i].posicio == "mig") J = MIG[rand()%(MIG.size())];
  else if(EquipVei[i].posicio == "dav") J = DAV[rand()%(DAV.size())];
  if(PRESSUPOST - preuEquip + EquipVei[i].preu - J.preu > 0 and not USAT.count(J.nom)){
    if(puntsEquip < (puntsEquip - EquipVei[i].punts + J.punts)) millor = true;
    USAT.erase(EquipVei[i].nom);
    EquipVei[i] = J;
    USAT.insert({J.nom, true});
  }
  puntsEquip = preuEquip = 0;
  for(Jugador N : EquipVei){
    puntsEquip += N.punts; preuEquip += N.preu;
  }
}

//Retorna True amb probabilitat T
bool probabilitat(double T){
  T = int(T*100);
  return (rand() % 100) < T;
}

void metaheuristica(vector <Jugador>& Equip, int& puntsEquip, int& preuEquip, double& T){
  //Triar posicio aleatoria per fer el canvi de Jugador
  int i = rand()%(Equip.size());
  //Crear l'equip vei
  bool millorat = false; //indica si el nou equip es millor.
  vector<Jugador> EquipVei = Equip;
  nouEquip(i, EquipVei, puntsEquip, preuEquip, millorat);

  //Acceptar o rebutjar l'equip
  if(millorat) Equip = EquipVei;
  else if(probabilitat(T)) Equip = EquipVei;
}

int main(int argc, char** argv){
  srand(time(NULL));
  clock_t start = std::clock();
  outputFile = argv[3];

  //Input de l'enunciat
  ConfEquip confEquip;
  int J; // preu maxim per jugador
  input(argc, argv, J, confEquip);

  parser(argc, argv, J);

  //Crear l'equip aleatrori inicial
  int puntsEquip = 0;
  int preuEquip = 0;
  vector<Jugador>Equip(11);
  generarEquip(Equip, confEquip, puntsEquip, preuEquip);

  //metaheuristica
  double T = 1; //temperatura
  int iteracions = 100000; //iteracions per cada temperatura
  double canviTenperatura = 0.01; //Decreixement per cada iteració de temperatura

  int puntsMax = 0;
  while(T > 0){
    for(int i = 0; i < iteracions; ++i){
      metaheuristica(Equip, puntsEquip, preuEquip, T);
      if(puntsMax < puntsEquip){
        puntsMax = puntsEquip;
        RESULTAT = Equip;
        output(start, confEquip);
      }
    }
    T -= canviTenperatura;
  }

  output(start, confEquip);
}
