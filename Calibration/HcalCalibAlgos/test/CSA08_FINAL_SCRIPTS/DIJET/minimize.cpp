#include<iostream>
#include<vector>
#include <fstream>
#include "GenericMinL3Algorithm.h"
using namespace std; 

int main(){

   vector< vector<float> > eventMatrix;
   vector<float> energyVector;


  int tow_array = 1944;

  ifstream in("/tmp/krohotin/data_veto5.txt");
  float EJet;
while(in >> EJet){
    energyVector.push_back(EJet);
    vector<float> en;
    for(int i=0; i<tow_array; i++){en.push_back(0.);}
    int ntowers;
    in >> ntowers;
    for(int i=0; i<ntowers; i++){
     int tower_number;
     float tower_energy;
     in >> tower_number >> tower_energy;
     en[tower_number-1]  = tower_energy;
    }
   eventMatrix.push_back(en);
}
   int nIter = 10; 
   GenericMinL3Algorithm* minL3 = new GenericMinL3Algorithm();
   vector<float> calib = minL3->iterate(eventMatrix,energyVector,nIter);

}



