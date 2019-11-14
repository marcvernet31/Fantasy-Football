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

//Restriccions de configuració de l'equip
struct Equip{
  int POR;
  int DEF;
  int MIG;
  int DAV;
  int pressupost;
};

vector<Jugador> database;

bool sortVal(const Jugador &A, const Jugador &B)
{
    return B.punts < A.punts;
}

//string outputFile;
int parser(int argc, char** argv, int J, priority_queue < pair<int, string> >& valorJugador) {
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
      database.push_back(P);
      valorJugador.push({P.punts, P.nom});
    }
  }
  sort(database.begin(), database.end(), sortVal);

  in.close();
  return 0;
}

//Output del resultat(a terminal)
void output(const vector<Jugador>& current, int punts, const string& outputFile){
  /*
  finishTime = std::clock();
  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  */
  Jugador POR;
  vector<Jugador> DEF;
  vector<Jugador> MIG;
  vector<Jugador> DAV;
  int preu = 0;
  for(Jugador J : current){
    preu += J.preu;
    if (J.posicio == "por") POR = J;
    else if(J.posicio == "def") DEF.push_back(J);
    else if(J.posicio == "mig") MIG.push_back(J);
    else if(J.posicio == "dav") DAV.push_back(J);
  }

  cout << "POR: " << POR.nom << endl;
  cout << "DEF:";
  for(Jugador J : DEF) cout << J.nom << ";";
  cout << endl << "MIG:";
  for(Jugador J : MIG) cout << J.nom << ";";
  cout << endl << "DAV:";
  for(Jugador J : DAV) cout << J.nom << ";";
  cout << endl;
  cout << "Punts: " << punts << endl;
  cout << "Preu: " << preu << endl;
}

bool checkEquip(Jugador J, int puntsActual, int puntsMax, Equip confEquip,
priority_queue < pair<int, string> >& valorJugador){
  int n = 11 - confEquip.POR - confEquip.DEF - confEquip.MIG - confEquip.DAV;
  int actualMax = valorJugador.top().first;

  if(J.nom == valorJugador.top().second)valorJugador.pop();

  if((n*actualMax+puntsActual) <= puntsMax) return false;

  return true;
}

//Cerca exhaustiva i poda
void exhaustive(int i, vector<Jugador>& current,
  vector<bool>& usat, const Equip& restriccions, Equip confEquip, int puntsActual,
  int& puntsMax, const string& outputFile, priority_queue < pair<int, string> > valorJugador){
    if(i == 11){

      for(Jugador J : current) cout << J.nom << " ";
      cout << endl;
      /*
      if(puntsActual > puntsMax){
        puntsMax = puntsActual;
        output(current, puntsActual, outputFile);
      }
      */
    }
    else{
      for(unsigned int j = 0; j < database.size(); ++j){
        if(not usat[j]){
          current[i] = database[j];
          usat[j] = true;
          puntsActual += current[i].punts;

          confEquip.pressupost += current[i].preu;
          if( (checkEquip(current[i], puntsActual, puntsMax, confEquip, valorJugador) and confEquip.pressupost <= restriccions.pressupost)){

            if(current[i].posicio == "por"){
              ++confEquip.POR;
              if(confEquip.POR <= restriccions.POR){
                exhaustive(i+1, current, usat, restriccions, confEquip, puntsActual, puntsMax, outputFile, valorJugador);
              }
              --confEquip.POR;
            }

            else if(current[i].posicio == "dav"){
              ++confEquip.DAV;
              if(confEquip.DAV <= restriccions.DAV){
                exhaustive(i+1, current, usat, restriccions, confEquip, puntsActual, puntsMax, outputFile, valorJugador);
              }
              --confEquip.DAV;
            }

            else if(current[i].posicio == "mig"){
              ++confEquip.MIG;
              confEquip.pressupost += current[i].preu;
              if(confEquip.MIG <= restriccions.MIG){
                exhaustive(i+1, current, usat, restriccions, confEquip, puntsActual, puntsMax, outputFile, valorJugador);
              }
              --confEquip.MIG;
            }

            else if(current[i].posicio == "def"){
              ++confEquip.DEF;
              if(confEquip.DEF <= restriccions.DEF){
                exhaustive(i+1, current, usat, restriccions, confEquip, puntsActual, puntsMax, outputFile, valorJugador);
              }
              --confEquip.DEF;
            }
          }
          puntsActual -= current[i].punts;

          confEquip.pressupost -= current[i].preu;
          usat[j] = false;
        }
      }
    }
  }


void input(int argc, char** argv, int& N1, int& N2, int& N3, int& T, int& J) {
  string filename = argv[2];
  ifstream f(filename);
  f >> N1 >> N2 >> N3 >> T >> J;
	f.close();
}


int main(int argc, char** argv){
  int N1, N2, N3, T, J;
  input(argc, argv, N1, N2, N3, T, J);


  string outputFile = argv[3];
  priority_queue < pair<int, string> > valorJugador;
  parser(argc, argv, J, valorJugador); //inserir jugadors a database i filtrar per J(pressupost jugador)

/*
  while(not valorJugador.empty()){
    cout << valorJugador.top().first << " " << valorJugador.top().second << endl;
    valorJugador.pop();
  }
*/

  vector<Jugador> current(11);  //string actual a la recursivitat
  vector<bool> usat(database.size(), false); //vector de bool per no repetir jugadors
  Equip restriccions {1, N1, N2, N3, T}; //Per comprovar que la configuracio de l'equip sigui correcte
  Equip confEquip {0,0,0,0,0};
  int puntsMax = 0;

  //startTime = std::clock();
  exhaustive(0, current, usat, restriccions, confEquip, 0, puntsMax, outputFile, valorJugador); //crea totes les combinacions possibles

}
