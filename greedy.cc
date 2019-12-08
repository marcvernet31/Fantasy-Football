#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include<map>
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

//Caracteristiques de l'equip demanat
Equip CONF_EQUIP;
//Dades de tots els jugadors necessaris
vector<Jugador> DATABASE;
map<string, vector<Jugador>> resultat;
string outputFile;


//Llegida de dades d'entrada
void input(int argc, char** argv, int& J) {
  CONF_EQUIP.por = 1;

  string filename = argv[2];
  ifstream f(filename);
  f >> CONF_EQUIP.def >> CONF_EQUIP.mig >> CONF_EQUIP.dav >> CONF_EQUIP.pressupost >> J;
	f.close();
}

bool sortVal(const Jugador& A, const Jugador& B){
  /*
  if(A.preu == 0 or B.preu == 0) return A.punts > B.punts;
  else return (0.8*A.punts)/(0.2*A.preu) > (0.8*B.punts)/(0.2*B.preu);
  */
  return A.punts > B.punts;
}

//Importar la base de dades en un vector de Jugadors
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
    if(P.preu <= J) DATABASE.push_back(P);
  }
  sort(DATABASE.begin(), DATABASE.end(), sortVal);
  in.close();
  return 0;
}

void greedy(){

  for(Jugador J : DATABASE)if(CONF_EQUIP.pressupost - J.preu > 0){
    if(J.posicio == "por" and CONF_EQUIP.por > 0){
      resultat["por"].push_back(J);
      --CONF_EQUIP.por;
      CONF_EQUIP.pressupost -= J.preu;
    }
    else if(J.posicio == "dav" and CONF_EQUIP.dav > 0){
      resultat["dav"].push_back(J);
      --CONF_EQUIP.dav;
      CONF_EQUIP.pressupost -= J.preu;
    }
    else if(J.posicio == "def" and CONF_EQUIP.def > 0){
      resultat["def"].push_back(J);
      --CONF_EQUIP.def;
      CONF_EQUIP.pressupost -= J.preu;
    }
    else if(J.posicio == "mig" and CONF_EQUIP.mig > 0){
      resultat["mig"].push_back(J);
      --CONF_EQUIP.mig;
      CONF_EQUIP.pressupost -= J.preu;
    }
  }
}

void outputTerminal(clock_t start){
  double temps = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  int puntsTotal = 0; int preuTotal = 0;

  cout << temps << endl;

  cout << "POR: " << resultat["por"][0].nom << endl;
  cout << "DEF: ";
  bool primer = true;
  for(Jugador J : resultat["def"]){
    cout << (primer ? "" : ";") << J.nom;
    primer = false;
    puntsTotal += J.punts;
    preuTotal += J.preu;
  }
  cout << endl << "MIG: ";
  primer = true;
  for(Jugador J : resultat["mig"]){
    cout << (primer ? "" : ";") << J.nom;
    primer = false;
    puntsTotal += J.punts;
    preuTotal += J.preu;
  }
  cout << endl << "DAV: ";
  primer = true;
  for(Jugador J : resultat["dav"]){
    cout << (primer ? "" : ";") << J.nom;
    primer = false;
    puntsTotal += J.punts;
    preuTotal += J.preu;
  }
  cout << endl;
  cout << "Punts: " << puntsTotal << endl;
  cout << "Preu: " << preuTotal << endl;
}

void output(clock_t start){
  double temps = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  int puntsTotal = 0; int preuTotal = 0;

  ofstream File;
  File.open (outputFile);

  File << temps << endl;

  File << "POR: " << resultat["por"][0].nom << endl;
  File << "DEF: ";
  bool primer = true;
  for(Jugador J : resultat["def"]){
    File << (primer ? "" : ";") << J.nom;
    primer = false;
    puntsTotal += J.punts;
    preuTotal += J.preu;
  }
  File << endl << "MIG: ";
  primer = true;
  for(Jugador J : resultat["mig"]){
    File << (primer ? "" : ";") << J.nom;
    primer = false;
    puntsTotal += J.punts;
    preuTotal += J.preu;
  }
  File << endl << "DAV: ";
  primer = true;
  for(Jugador J : resultat["dav"]){
    File << (primer ? "" : ";") << J.nom;
    primer = false;
    puntsTotal += J.punts;
    preuTotal += J.preu;
  }

  File << endl;
  File << "Punts: " << puntsTotal << endl;
  File << "Preu: " << preuTotal << endl;

  File.close();
}

int main(int argc, char** argv){
  clock_t start = std::clock();
  outputFile = argv[3];
  int J; // preu maxim per jugador

  //Inicialització del diccionari de resultats
  vector<Jugador> v;
  resultat.insert({"por", v});
  resultat.insert({"def", v});
  resultat.insert({"dav", v});
  resultat.insert({"mig", v});


  input(argc, argv, J);
  parser(argc, argv, J);

  greedy();
  //outputTerminal(start);
  output(start);

}
