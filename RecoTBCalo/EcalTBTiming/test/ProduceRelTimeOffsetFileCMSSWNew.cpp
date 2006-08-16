#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

#include<vector>
#include<string>

using namespace std;
int main(int argc,  char * argv[]){

  if(argc < 4){cout<<" Usage: executable initial_xml_file_name peak_file.txt out_file(without suffix)"<<endl; 
  return -4;}
  cout<<"Please make sure that the peak time is expected in SAMPLE units"<<endl;
  cout<<"This program will run using the relative timing"<<endl;



 ifstream FileXML(argv[1]);
  if( !(FileXML.is_open()) ){cout<<"Error: file"<<argv[1]<<" not found!!"<<endl;return -2;}
  char Buffer[5000];
  int TimeOffset[71];
  for(int i=0;i<71;i++){TimeOffset[i]=-100;}
  int SMn =0;
  bool find_SMnum = true;
  int HowManyTT =0;
   while( !(FileXML.eof()) ){
    FileXML.getline(Buffer,5000);
    //    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer)))
    string initial(Buffer);
    if( find_SMnum && initial.find("<SUPERMODULE>") != string::npos){
      char stSM[100];
      sscanf(Buffer,"%s",stSM);
      sscanf(stSM,"<SUPERMODULE>%d</SUPERMODULE>",&SMn);
      find_SMnum = false;
    }

    if( initial.find("<DELAY_OFFSET>") != string::npos ){
      FileXML.getline(Buffer,5000);// get the line with SM id
      FileXML.getline(Buffer,5000);// get the line with TT id
      char st1[200];
      int TT = -1;
      sscanf(Buffer,"%s",st1);
      sscanf(st1,"<TRIGGERTOWER>%d</TRIGGERTOWER>",&TT);
      //cout<<"TT: "<<TT<<endl;
      //cout<<"Buffer: "<<Buffer<<"  []TT: "<<TT<<endl;
      if(TT< 1 || TT >70){cout<<"Wrong TT: "<<TT<<endl;}
      else{
	int time_off = -10;
	char st2[200];
	FileXML.getline(Buffer,5000);// line for the delay
	sscanf(Buffer,"%s",st2);
	sscanf(st2,"<TIME_OFFSET>%d</TIME_OFFSET>",&time_off);
	TimeOffset[TT]=time_off;
	if(TimeOffset[TT] >0 ){HowManyTT++;}
	else{cout<<"Error for delays in TT: "<<TT<<" Offsets: "<<TimeOffset[TT] <<endl;}
      }

    }//end of detecting offset of a TT

  }//end of file
  FileXML.close();
  cout<<"Found "<<HowManyTT<<" good TT while reading the file "<<argv[1]<<" (Should be 70)"<<endl;



  // reading the peak file
  ifstream TxtFile(argv[2]);
  if( !(TxtFile.is_open()) ){cout<<"Error: file"<<argv[2]<<" not found!!"<<endl;return -2;}
  int Shift[71];
  for(int i=0;i<71;i++){Shift[i]=-1000;}
  int TTnum,sample;
  float rms,shift,rel_shift, rel_rms;
  int HowManyShifts = 0;
  
  
  while( !(TxtFile.eof()) ){
    TxtFile.getline(Buffer,5000);
    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer)))
      {
	sscanf(Buffer,"%d %f %f %f %f",&TTnum,&shift,&rms,&rel_shift,&rel_rms);
	//cout<<"TT: "<<TTnum<<endl;
	if(TTnum < 1 || TTnum >68){cout<<"Wrong TT in txt file: "<<TTnum<<endl;continue;}
	if(rel_shift <= -10){cout<<" shift <= 0! in TT: "<<TTnum<<" skipped"<<endl;continue;}
	if(rel_rms < 0){ cout<<" rms < 0! in TT: "<<TTnum<<" skipped"<<endl;continue;}
	HowManyShifts++;
	float move = (0. - rel_shift)*25.;
	int temp = int(move);
	if( fabs(rel_shift*25.+temp) < fabs(rel_shift*25.+temp+1) ){Shift[TTnum]=temp;}
	else{Shift[TTnum]=temp+1;}
	if( fabs(move)> 10.){cout<<"!! Large shift ( "<<move<<" samples) required for TT: "<<TTnum<<endl; }
      }
  }//end of file
  TxtFile.close();
  cout<<"Found "<<HowManyShifts<<" tt timing while reading the file "<<argv[2]<<" (should be 68)"<<endl;
  
  // calculate the new values for the offset file.
  int FinalOffsets[70];
  for(int i=1;i<69;i++){
    FinalOffsets[i]=-100;
    if(Shift[i]>-1000){FinalOffsets[i]=TimeOffset[i]-Shift[i];}
    if(FinalOffsets[i] < 1 ){
      cout<<"Offsets in TT: "<<i<<" is <1. It will be set to 0"<<endl;
      FinalOffsets[i]=0;
    }
  }
  
 
  string txtFileName = argv[3]; txtFileName += ".txt";
  ofstream txt_outfile;
  txt_outfile.open(txtFileName.c_str(),ios::out);
  txt_outfile<< "#  Needed shift in terms of samples and fine tuning (ns) for each TT"<<endl;
  txt_outfile<<"#   shift"<<std::endl;
  for(int i=0;i<71;i++){
    if(Shift[i] == -1000){continue;}
    txt_outfile <<"   "<<setw(4)<<i<<"  "<<setw(4)<<Shift[i]<<" \t  "<<endl;  
  }
  txt_outfile.close();

  string xmlFileName = argv[3]; xmlFileName += ".xml";
  ofstream xml_outfile;
  xml_outfile.open(xmlFileName.c_str(),ios::out);
  
  xml_outfile<<"<delayOffsets>"<<endl;
  xml_outfile<<" <DELAY_OFFSET_RELEASE VERSION_ID = \"SM"<<SMn<<"_VER1\"> \n";
  xml_outfile<<"      <RELEASE_ID>RELEASE_1</RELEASE_ID>\n";
  xml_outfile<<"             <SUPERMODULE>" <<SMn<< "</SUPERMODULE>\n";
  xml_outfile<<"     <TIME_STAMP> 270705 </TIME_STAMP>"<<endl;
  //add the time for the mem at the beginning
  xml_outfile<<"   <DELAY_OFFSET>\n";
  xml_outfile<<"             <SUPERMODULE>"<<SMn <<"</SUPERMODULE>\n";
  xml_outfile<<"             <TRIGGERTOWER>69</TRIGGERTOWER>\n";
  xml_outfile<<"             <TIME_OFFSET>48</TIME_OFFSET>\n";
  xml_outfile<<"    </DELAY_OFFSET>"<<endl;
  xml_outfile<<"   <DELAY_OFFSET>\n";
  xml_outfile<<"             <SUPERMODULE>"<<SMn <<"</SUPERMODULE>\n";
  xml_outfile<<"             <TRIGGERTOWER>70</TRIGGERTOWER>\n";
  xml_outfile<<"             <TIME_OFFSET>48</TIME_OFFSET>\n";
  xml_outfile<<"    </DELAY_OFFSET>"<<endl;

  for(int i=1;i<69;i++){
    xml_outfile<<"   <DELAY_OFFSET>\n";
    xml_outfile<<"             <SUPERMODULE>"<<SMn <<"</SUPERMODULE>\n";
    xml_outfile<<"             <TRIGGERTOWER>"<< i <<"</TRIGGERTOWER>\n";
    xml_outfile<<"             <TIME_OFFSET>"<< FinalOffsets[i] <<"</TIME_OFFSET>\n";
    xml_outfile<<"    </DELAY_OFFSET>"<<endl;
  }
  xml_outfile<<" </DELAY_OFFSET_RELEASE>"<<endl;
  xml_outfile<<"</delayOffsets>"<<endl;
  xml_outfile.close();

  
  
  return 0;
}

