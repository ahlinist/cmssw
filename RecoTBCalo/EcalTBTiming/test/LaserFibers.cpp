#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

#include<vector>
#include<string>

using namespace std;
int getM( int tt);
int main(int argc,  char * argv[]){
  float speed = 200.; // mm/ns
  float EcalR = 1290.; //mm
  float fiberL1[4]={493, 500, 711, 727};//mm
  float fiberL2[4]={2573, 2723, 2723, 3123};//mm
  float fiberLtot[4];
  for(int i=0;i<4;i++){fiberLtot[i] = fiberL1[i]+fiberL2[i];}
  
  float etaTT[68], lTT[68];
  for(int TT=0;TT<68;TT++){ etaTT[TT] = float (2 + TT/4 *5) * 0.0175 + 0.0175/2.;}
  for(int TT=0;TT<68;TT++){ 
    float theta = 2.*atan( exp( -1.*etaTT[TT]));
    lTT[TT] = 1330./sin(theta);
  }
  
  float tcorr[68];// samples 
  float mean = 0;
  for(int TT=0;TT<68;TT++){
    int mod = getM(TT+1);
    tcorr[TT] = ( lTT[TT]/300. - fiberLtot[mod-1]/speed )/25. ;
    mean += tcorr[TT];
  }
  mean /= 68.;
  for(int TT=0;TT<68;TT++){tcorr[TT] -= mean;}
  
  ofstream txt_outfile;
  txt_outfile.open("TimeCorrectionLaserTB.txt",ios::out);
  txt_outfile<< "#  Needed shift (sample units) for each TT to adjust the tof wrt to the laser fiber lenght"<<endl;
  for(int i=1;i<69;i++){ txt_outfile <<"   "<<setw(4)<<i<<"  "<<setw(4)<< tcorr[i-1] <<endl;  }
  txt_outfile.close();
        
}

int getM(int tt){
  if(tt<1 || tt >68){return -1;}
  if(tt < 21){return 1;}
  else if(tt < 37){return 2;}
  else if(tt < 53){return 3;}
  else {return 4;}
  return 0;
}
