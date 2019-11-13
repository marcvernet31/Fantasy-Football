#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
using namespace std;

struct Jugador{
  string nom;
  string posicio;
  int preu;
  string equip;
  int punts;
};



string outputFile;

int parser(int argc, char** argv, vector<Jugador>& database, int J) {
  if (argc != 2) {
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

////////////////FET PER MI///////////////////////

    Jugador P{nom,posicio,preu,club,punts};
    if(P.preu <= J) database.push_back(P);
////////////////////////////////////////////////
  }
  in.close();
}

/*
N1 :defenses
N2 : migcampistes
N3 : davanters
T :pressupost total
J :pressupost per jugador
*/

/*
void generacio(int n, int i, vector<int>& v, vector<bool>& usat){
  if(i == n) escriu(v);
  else{
    for (int j = 1; j<=n; ++j){
      if(not usat[j-1]){
        v[i] = j;
        usat[j-1] = true;
        generacio(n, i+1, v, usat);
        usat[j-1] = false;
      }
    }
  }
}
*/
//n = 11
void ex(int i, const vector<Jugador>& database, vector<Jugador>& current,
  vector<bool>& usat, vector<vector<Jugador>>& resultat){
    if(i == 10){
      resultat.push_back(current);
      for(Jugador J : current) cout << J.nom << " ";
      cout << endl;
    }
    else{
      for(int j = 0; j < database.size(); ++j){
        if(not usat[j]){
          current[i] = database[j];
          usat[j] = true;
          ex(i+1, database, current, usat, resultat);
          usat[j] = false;
        }
      }
    }
  }


/*

void exhaustive(int i, const vector<Jugador>& database,   vector<bool>& usat,
   vector<vector<Jugador>>& resultat, vector<Jugador>& current){
  if(i == 11){
    resultat.push_back(current);
    for(Jugador J : current) cout << J.nom << " ";
    cout << endl;
  }
  else{
    for(int j = 0; j < database.size(); ++j){
      if(not usat[j]){
        current[i] = database[i];
        usat[j] = true;
        exhaustive(i+1, database, usat, resultat, current);
        usat[j] = false;
      }
    }
  }
}

*/

int main(int argc, char** argv){
  int N1, N2, N3, T, J;
  cin >> N1 >> N2 >> N3 >> T >> J;
  vector<Jugador> database; //vector amb tots els jugadors
  parser(argc, argv, database, J); //inserir jugadors a database i filtrar per J(pressupost jugador)
  vector<vector<Jugador>> resultat;
  vector<Jugador> current(11);
  vector<bool> usat(database.size(), false);
  ex(0, database, current, usat, resultat); //crea totes les combinacions possibles
  //for(Jugador P : database) cout << P.nom << " " << P.preu << endl;

}

/*
vector<Jugador> POR;
vector<Jugador> DEF;
vector<Jugador> MIG;
vector<Jugador> DAV;
*/
