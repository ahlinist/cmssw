#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

#include<vector>
#include<string>

using namespace std;
int main(int argc,  char * argv[]){

  if(argc < 3){cout<<" Usage: executable initialLaserFile.txt  out_file.txt"<<endl; 
  return -4;}
  cout<<"Please note that the peak time is expected in SAMPLE units"<<endl;

  float speed = 200.; // mm/ns
  float EcalR = 1290.; //mm
  float fiberL1[4]={493, 500, 711, 727};//mm
  float fiberL2[4]={2573, 2723, 2723, 3123};//mm
  float fiberLtot[4];
  for(int i=0;i<4;i++){fiberLtot[i] = fiberL1[i]+fiberL2[i];}


  // reading the Laser CH  file
  ifstream TxtFile(argv[1]);
  if( !(TxtFile.is_open()) ){cout<<"Error: file "<<argv[1]<<" not found!!"<<endl;return -2;}
  int Crynum;
  float Ltime;
  int HowManyShifts = 0;
  int SM = 0;
  float cryTime[1701];
  char Buffer[5500];
  for(int i=0;i < 1701; i++){ cryTime[i]=-1000; }
  while( !(TxtFile.eof()) ){
    TxtFile.getline(Buffer,5000);
    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer)))
      {
	sscanf(Buffer,"%d %d %f",&SM, &Crynum,&Ltime);
	//cout<<"TT: "<<TTnum<<endl;
	if(Crynum < 1 || Crynum >1700){cout<<"Wrong Cry in txt file: "<<Crynum<<endl;continue;}
	float eta = float(Crynum/20)*0.0175 + 0.0175/2.;
	float theta = 2.*atan( exp( -1.*eta));
	float travel = EcalR/sin(theta);
	int mod =0;
	if(Crynum<501){mod =0;}
	else if(Crynum<901){mod =1;}
	else if(Crynum<1301){mod =2;}
	else {mod =3;}
	float tcorr = travel/300. - fiberLtot[mod]/200.+ 10.62;// ns (so that the average of tcorr is ~0)
	cryTime[Crynum]=Ltime+tcorr/25.;
      }
  }//end of file
  TxtFile.close();
  
  string txtFileName = argv[2];
  ofstream txt_outfile;
  txt_outfile.open(txtFileName.c_str(),ios::out);
  
  for(int i=1;i<1701;i++){
    txt_outfile <<SM<<setw(4)<<"   "<<i<<" \t  "<<setw(4)<<cryTime[i]<<endl;  
  }
  txt_outfile.close();

  return 0;
}

