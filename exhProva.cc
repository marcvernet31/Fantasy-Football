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

/*
bool sortVal(const Jugador &A, const Jugador &B) return A.punts < B.punts;
*/

int parser(int argc, char** argv, int J, int& valorJugadorMax, int& preuJugadorMin) {
  if (argc != 4) {
    cout << "Syntax: " << argv[0] << " data_base.txt" << endl;
    exit(1);
  }

  priority_queue<pair<int,string> > valorJugadors;

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
      if(P.punts > 0) valorJugadors.push({P.punts, P.nom});
      if(preuJugadorMin > P.preu and P.preu != 0) preuJugadorMin = P.preu;
    }
  }
  //sort(database.begin(), database.end(), sortVal);
  valorJugadorMax = valorJugadors.top().first;
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

/*
bool checkEquip(Jugador J, int puntsActual, int puntsMax, Equip confEquip,
  priority_queue < pair<int, string> >& valorJugador){
  int n = 11 - confEquip.POR - confEquip.DEF - confEquip.MIG - confEquip.DAV;

  int actualMax;
  if(valorJugador.empty()) actualMax = 0;
  else actualMax = valorJugador.top().first;

  if(J.nom == valorJugador.top().second)valorJugador.pop();

  if((n*actualMax+puntsActual) <= puntsMax) return false;

  return true;
}
*/

bool checkPunts(const Equip& confEquip, const int& valorJugadorMax, const int& puntsActual, const int& puntsMax){
  if(((11 - confEquip.POR - confEquip.DEF - confEquip.MIG - confEquip.DAV)*valorJugadorMax+puntsActual) >= puntsMax){
    return true;
  }
  return false;
}

bool checkPreu(const Equip& confEquip, const Equip& restriccions, const int& preuJugadorMin){
  if(((11 - confEquip.POR - confEquip.DEF - confEquip.MIG - confEquip.DAV)*preuJugadorMin + confEquip.pressupost) < restriccions.pressupost){
    return true;
  }
  return false;
}

//Cerca exhaustiva i poda
void exhaustive(int i, vector<Jugador>& iteracioActual, vector<bool>& usat,
    const Equip& restriccions, Equip confEquip, int puntsActual,
    int& puntsMax, const string& outputFile, const int& valorJugadorMax, const int& preuJugadorMin){

    if(i == 11){
      ////////////////////////////////////////////////
      for(Jugador J : iteracioActual) cout << J.nom << " ";
      cout << endl;
      /*
      if(puntsActual > puntsMax){
        puntsMax = puntsActual;
        output(current, puntsActual, outputFile);
      }
      */
      ///////////////////////////////////////////////
    }
    else{
      int n = database.size();
      for(unsigned int j = 0; j < n; ++j){
        if(not usat[j]){
          iteracioActual[i] = database[j];
          usat[j] = true;
          puntsActual += iteracioActual[i].punts;
          confEquip.pressupost += iteracioActual[i].preu;
          int num = 11;
          if(  checkPunts(confEquip, valorJugadorMax, puntsActual, puntsMax)
                and checkPreu(confEquip, restriccions, preuJugadorMin)
               and confEquip.pressupost <= restriccions.pressupost){

            if(iteracioActual[i].posicio == "por"){
              ++confEquip.POR;
              if(confEquip.POR <= restriccions.POR){
                exhaustive(i+1, iteracioActual, usat, restriccions, confEquip, puntsActual, puntsMax, outputFile, valorJugadorMax, preuJugadorMin);
              }
              --confEquip.POR;
            }
            else if(iteracioActual[i].posicio == "dav"){
              ++confEquip.DAV;
              if(confEquip.DAV <= restriccions.DAV){
                exhaustive(i+1, iteracioActual, usat, restriccions, confEquip, puntsActual, puntsMax, outputFile, valorJugadorMax, preuJugadorMin);
              }
              --confEquip.DAV;
            }
            else if(iteracioActual[i].posicio == "mig"){
              ++confEquip.MIG;
              confEquip.pressupost += iteracioActual[i].preu;
              if(confEquip.MIG <= restriccions.MIG){
                exhaustive(i+1, iteracioActual, usat, restriccions, confEquip, puntsActual, puntsMax, outputFile, valorJugadorMax, preuJugadorMin);
              }
              --confEquip.MIG;
            }
            else if(iteracioActual[i].posicio == "def"){
              ++confEquip.DEF;
              if(confEquip.DEF <= restriccions.DEF){
                exhaustive(i+1, iteracioActual, usat, restriccions, confEquip, puntsActual, puntsMax, outputFile, valorJugadorMax, preuJugadorMin);
              }
              --confEquip.DEF;
            }
          }
          puntsActual -= iteracioActual[i].punts;
          confEquip.pressupost -= iteracioActual[i].preu;
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
  string outputFile = argv[3];
  int N1, N2, N3, T, J;
  input(argc, argv, N1, N2, N3, T, J);

  int valorJugadorMax;
  int preuJugadorMin = 500000000;
  parser(argc, argv, J, valorJugadorMax, preuJugadorMin); //inserir jugadors a database i filtrar per J(pressupost jugador)
  cout << preuJugadorMin << endl;
  vector<Jugador> iteracioActual(11);  //string actual a la recursivitat
  vector<bool> usat(database.size(), false); //vector de bool per no repetir jugadors
  Equip restriccions {1, N1, N2, N3, T}; //Per comprovar que la configuracio de l'equip sigui correcte
  Equip confEquip {0,0,0,0,0};
  int puntsMax = 0; //puntacio maxim obtinguda per un equip

  //startTime = std::clock();
  exhaustive(0, iteracioActual, usat, restriccions, confEquip, 0, puntsMax, outputFile, valorJugadorMax, preuJugadorMin);

}
