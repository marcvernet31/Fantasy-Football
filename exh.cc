#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
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
struct Restriccions{
  int POR;
  int DEF;
  int MIG;
  int DAV;
  int pressupost;
};

//string outputFile;
int parser(int argc, char** argv, vector<Jugador>& database, int J) {
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
    if(P.preu <= J) database.push_back(P);
  }
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

  ofstream File;
  File.open (outputFile);


  File << "POR: " << POR.nom << endl;
  File << "DEF:";
  for(Jugador J : DEF) File << J.nom << ";";
  File << endl << "MIG:";
  for(Jugador J : MIG) File << J.nom << ";";
  File << endl << "DAV:";
  for(Jugador J : DAV) File << J.nom << ";";
  File << endl;
  File << "Punts: " << punts << endl;
  File << "Preu: " << preu << endl;

  File.close();
}

//Actualització de les restriccions
void modify(const Jugador& J, Restriccions& R, const int& var){
  string pos = J.posicio;
  if(pos == "por") R.POR += 1*var;
  else if(pos == "def") R.DEF += 1*var;
  else if(pos == "mig") R.MIG += 1*var;
  else R.DAV += 1*var;
  R.pressupost += J.preu*var;
}

//comprovar que les restriccions es compleixen
bool checkConf(const Restriccions& R){
  if((R.pressupost >= 0) and
     (R.POR >= 0) and (R.DEF >= 0) and
     (R.MIG >= 0) and (R.DAV >= 0)) return true;
  return false;
}

//Cerca exhaustiva i poda
void exhaustive(int i, const vector<Jugador>& database, vector<Jugador>& current,
  vector<bool>& usat, Restriccions R, int puntsActual, int& puntsMax, const string& outputFile){
    if(i == 11){
      if(puntsActual > puntsMax){
        puntsMax = puntsActual;
        output(current, puntsActual, outputFile);
      }
    }
    else{
      for(unsigned int j = 0; j < database.size(); ++j){
        if(not usat[j]){
          current[i] = database[j];
          modify(database[j], R, -1);
          usat[j] = true;
          puntsActual += current[i].punts;
          if(checkConf(R)) exhaustive(i+1, database, current, usat, R, puntsActual, puntsMax, outputFile);
          modify(database[j], R, 1);
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
  vector<Jugador> database; //vector amb tots els jugadors
  parser(argc, argv, database, J); //inserir jugadors a database i filtrar per J(pressupost jugador)

  vector<Jugador> current(11);  //string actual a la recursivitat
  vector<bool> usat(database.size(), false); //vector de bool per no repetir jugadors
  Restriccions R{1, N1, N2, N3, T}; //Per comprovar que la configuracio de l'equip sigui correcte
  int puntsMax = 0;

  //startTime = std::clock();
  exhaustive(0, database, current, usat, R, 0, puntsMax, outputFile); //crea totes les combinacions possibles

}
