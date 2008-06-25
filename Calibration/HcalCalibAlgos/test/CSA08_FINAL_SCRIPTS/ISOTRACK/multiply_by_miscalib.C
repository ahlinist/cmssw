{   
gROOT->Reset();
ifstream in1,in2,in3,in4;
in1.open("S156_w_HB_scaled_w_MC.txt");
//in2.open("IsoTrackCoef_v1.txt");
in2.open("hcalmiscalib_1pb.txt");

float value1[10000],value2[10000],value4[10000];
int eta1[10000], phi1[10000], dep1[10000];
int eta2[10000], phi2[10000], dep2[10000];
TString det1[10000],det2[10000],detid1[10000],detid2[10000];
int eta4[10000], phi4[10000], dep4[10000];
TString det4[10000],det4[10000],detid4[10000],detid4[10000];
float dijet_eta[10000];
float dijet_value[10000];
int i1 = 0;
int i2 = 0;
int i3 = 0;
int i4 = 0;

while (!in1.eof()){
  in1 >> eta1[i1] >> phi1[i1] >> dep1[i1] >> det1[i1] >> value1[i1] >> detid1[i1];
  i1++;
}

while (!in2.eof()){
  in2 >> eta2[i2] >> phi2[i2] >> dep2[i2] >> det2[i2] >> value2[i2];
  ++i2;
}


for (int i=0;i<i1;++i){
  for (int j=0;j<i2;++j){
    if ( eta1[i] == eta2[j] && phi1[i] == phi2[j] && dep1[i] == dep2[j] && det1[i] == det2[j]) {
	cout << eta1[i] << "  " << phi1[i] << "  "<< dep1[i] << "  " << det1[i] << "   " << value1[i]*value2[j] << "  " << detid1[i] <<endl;
    }
  }
}


}
