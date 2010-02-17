#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <cstdlib>

//#include "OnlineDB/Oracle/interface/Oracle.h"
#include "OnlineDB/EcalCondDB/interface/EcalCondDBInterface.h"
//#include "OnlineDB/EcalCondDB/interface/all_od_types.h"
#include "OnlineDB/ESCondDB/interface/ESMonRunIOV.h"
#include "OnlineDB/ESCondDB/interface/ODESFEPedestalOffsetInfo.h"
#include "OnlineDB/ESCondDB/interface/ODESFEPedestalOffsetsDat.h"
#include "OnlineDB/EcalCondDB/interface/RunDat.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"
#include "OnlineDB/EcalCondDB/interface/Tm.h"

bool dbg;
using namespace std;

class CondDBApp {
public: 
//friend class EcalCondDBInterface;
  /**
   *   App constructor; Makes the database connection
   */
  CondDBApp(string host, string sid, string user, string pass)
  {
    try {
      cout << "Making connection..." << flush;
      econn = new EcalCondDBInterface( sid, user, pass );
      cout << "Done." << endl;
    } catch (runtime_error &e) {
      cerr << e.what() << endl;
      exit(-1);
    }
  }

  /**
   *  App destructor;  Cleans up database connection
   */
  ~CondDBApp() 
  {
    delete econn;
  }


void printInfo( ODESFEPedestalOffsetInfo info)
{
  
  cout << "Version="  << info.getVersion() << " Iov+= " << info.getIov_pl() << " Iov-= " << info.getIov_mi() << endl;
  cout << "COMMENT= " <<  info.getUser_comment() << "  Id= " << info.getId() << " Tag=" << info.getConfigTag()  << endl;
}
 
void printResult(std::vector< ODESFEPedestalOffsetsDat >* p)
{
  int l;

  // for(unsigned i=0; i< p->size() ; ++i)
  cerr << " Display Size= " << p->size() << endl << endl;
  for(unsigned i=0; i< p->size() ; ++i)
  {
    l=(*p)[i].getPedestal();
    cerr << "ID= " << (*p)[i].getId() << " FED= " << (*p)[i].getFedId() << endl;
    cerr <<" ped=" << l << " Strip= " << (*p)[i].getStripId() << " Fiber= " << (*p)[i].getFiberId() <<" Kchip= " << (*p)[i].getKchipId() << endl;
    cerr << " Rms= " << (*p)[i].getRms() <<  endl;
  }
}


void testWrite(const bool ped,const float ZSV,const float CMCV,const string tag,const string hlg,const string comment )
{ 
  

  /** cmsRun ... cmsRun es_dqm_sourceclient-localfile_cfg.py **/

  int version=0;
  ODESFEPedestalOffsetInfo run_info;
  std::map< int,int > Gain;
 
  // Get Last Version for tag
  run_info.setId(0);
  run_info.setConfigTag(tag);
  run_info.setVersion(0);
  run_info.setIov_pl(0);
  run_info.setIov_mi(0);
  run_info.setUser_comment("");
  econn->fetchConfigSet(&run_info);
  
//this->printInfo(run_info);
  
   version=run_info.getVersion()+1;

  cout << endl;
  cout << endl <<  "The TAG=" << tag <<  "  Is now set with the version number: " << version << endl;
  cout << endl;
  Gain.clear();
  Gain=run_info.fetchGain(hlg);
  
  int i=0;
  if(Gain.size()<2)
  {
    cout << " No Run entry with this gain" << endl;
    cout << " Run Aborted" << endl;
    return;
  }
  else
  {

    cout << "There are " << Gain.size() << " entries for this gain" << endl;
    cout << endl << endl;
    cout << "----------------------- Run Numbers for Negativ Wheel -----------------------" << endl<<endl;
    for( std::map<int,int>::iterator it=Gain.begin(); it!=Gain.end(); ++it)
    {
      if((*it).second>500)
      {
        cout <<  (*it).first << " | ";
        if (i==7)
        {
          i=0;
          cout << endl;
        }
        else
          i++;
      }
    }
    i=0;
    cout << endl << endl ;
    cout << "----------------------- Run Numbers for Positiv Wheel -----------------------" << endl<<endl;
    for( std::map<int,int>::iterator it=Gain.begin(); it!=Gain.end(); ++it)
    {
      if((*it).second<500)
      {
        cout <<  (*it).first << " | ";
        if (i==7)
        {
          i=0;
          cout << endl;
        }
        else
          i++;
      }
    }
  }
  cout << endl<<endl;
  string runplus;
  string runminus;
  string ok;
  int nbP;
  int nbM;

  while(1)
  {
    cin.clear();
    cout << "Enter the run_num # for RUN PLUS" << endl;
    cin >> runplus;
    cout << "Enter the run_num # for RUN MINUS" << endl;
    cin >> runminus;
    nbP=atoi(runplus.c_str());
    nbM=atoi(runminus.c_str());
    cout << "Run plus=" << nbP << " Run minus=" << nbM << endl;
    int match=0;
    for( std::map<int,int>::iterator it=Gain.begin(); it!=Gain.end(); ++it)
    {
      if( ((*it).first == nbP) || ((*it).first == nbM) )
        match++;
      if(match==2)
        break;
    }
    if(match==2)
      break;
    else
    {
      cout << endl;
      cout << " Run number are Wrong!!" << endl;
      cout << "You need to enter 2 valid run number:Run for Wheel_Plus and Run for Wheeel_Minus !!" << endl;
      cout << "Do you want to continue and select 2 other Runs # ? (Y)/(y)es or (N)/(n)o ?" << endl;
      cin >> ok;
      if((ok.at(0)=='Y')||(ok.at(0)=='y'))
        continue;
      else
      {
        cout << " Run Aborted ..." << endl;
        return;
      }
    }
  }
  cerr << nbP << " " << nbM << endl;
 
  // return;

  // Now insert new Info entry
  run_info.setConfigTag(tag);
  run_info.setVersion(version);
  run_info.setIov_pl(nbP);
  run_info.setIov_mi(nbM);
  run_info.setUser_comment(comment);
  
  ZSVal=ZSV;
  CMCVal=CMCV;
  Isped=ped;
  cout << "CMCVal=" << CMCVal << "  ZSVal=" << ZSVal << "  Isped=" << Isped << endl;
  cout << "Inserting in DB the ODRunConfigInfo..." << flush;
  econn->insertConfigSet(&run_info);
  cout << "Done." << endl;

  vector<ODESFEPedestalOffsetsDat> dataset;
  dataset.clear();

  econn->fetchConfigDataSet(&dataset,&run_info);
  if( dataset.size()==137216)
    ;//this->printResult(&dataset);
  else
  {
    cout << "Strips not Completed" << endl;
    return;
  }
  this->writeDb(dataset,&run_info);
}


void writeDb(std::vector< ODESFEPedestalOffsetsDat > dataset,ODESFEPedestalOffsetInfo * runinfo) {
 
  cout << "CMCVal=" << CMCVal << "  ZSVal=" << ZSVal << "  Isped=" << Isped << endl;
  for (unsigned int i=0; i< dataset.size(); ++i) 
  {
    if(Isped==false)
      dataset[i].setPedestal(0);

    int rms=dataset[i].getRms();

    if((rms>20) || (rms<3))
      dataset[i].setMasked(1);
    else
      dataset[i].setMasked(0);

    if(CMCVal!=0.0)        
      dataset[i].setCmRange(CMCVal*rms);
    else
      dataset[i].setCmRange(0);

    if(rms>8) 
       dataset[i].setCmMasked(1);
    else 
       dataset[i].setCmMasked(0);

    dataset[i].setGain(1);

    if(ZSVal!=1.0)
      dataset[i].setZs(ZSVal*rms);
    else
      dataset[i].setZs(1);
  }

  /** 
  std::vector< ODESFEPedestalOffsetsDat >::iterator it;
  for(it=dataset.begin(); it< dataset.end() ; ++it)
  {
    //  if(( it->getMasked()==1)||( it->getCmMasked()==1)) {
    cerr << "Fed=" << it->getFedId() <<" KCHIP=" << it->getKchipId() << " Strip= " << it->getStripId() << " Pace= " << it->getPaceId() << " Fiber=" << it->getFiberId() << " RX=" << it->getRxId() << " Ped=" << it->getPedestal() << endl;
    cerr << "Cmrang=" << it->getCmRange() << " Masked= " << it->getMasked() << " CmMasked= " <<  it->getCmMasked();
    cerr << " RMS= " <<  it->getRms() << " Gain=" << it->getGain() << " Zs=" << it->getZs() << endl;   
     //}
  }
  **/
  
  if ( econn ) {
    try {
      if ( dbg ) cout << "Inserting new Config Pedestals Data ..." << endl;
      if ( dataset.size() != 0 ) econn->insertConfigDataArraySet(dataset,runinfo);
                                         
      if ( dbg ) cout << "done." << endl;
      } catch (runtime_error &e) {
        cerr << e.what() << endl;
    }
  }
}

private:
  CondDBApp();  // hidden default constructor
  EcalCondDBInterface* econn;
  float ZSVal;
  float CMCVal;
  bool Isped;
};

int main (int argc, char* argv[])
{

 string host;
  string sid;
  string user;
  string pass;
  string selection;
  string comment;
  string tag;
  string Ped_sub;
  string zs;
  string cmc;
  string higLow;
  bool   ped;  
  string ZS_Val;
  string CMC_Range;
  string cmcT;
  string zsT;
  string sub_PedT;
  string higLowT;
  string tagok;
  string Magnet;
  string MagOnOff;

  if (argc != 5) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " <host> <SID> <user> <pass> " << endl;
    exit(1);
  }

  char comments[90];
  dbg=true;
  while(1)
  {
    zs="ON";
    cmc="ON";
    Ped_sub="ON";
    Magnet="ON";

    sub_PedT="PS";
    zsT="ZS";
    cmcT="CMC";
    higLowT="HG";
    MagOnOff="BON";

    ZS_Val="4.3";
    CMC_Range="3.0";

    while(1)
    {    
      cout << "PED_SUB ON/OFF ? ---> ON/OFF --> " ;
      cin >> Ped_sub;
      if((Ped_sub=="ON")||(Ped_sub=="on")||(Ped_sub=="OFF")||(Ped_sub=="off"))
        break;
      else
        cout << "PED_SUB: Enter on/ON or off/OFF --> " << endl;
    }

    while(1)
    {
      cout << "ZS ON/OFF ? ---> ON/OFF --> ";
      cin >> zs;
      if((zs=="ON")||(zs=="on")||(zs=="OFF")||(zs=="off"))
        break;
      else
        cout << "ZS: Enter on/ON or off/OFF --> " << endl;
    }
    if ((zs=="OFF")||(zs=="off"))
    {
      cout << "Set ZS=1.0 ";
      ZS_Val="1.0";
      zsT="NZS";
    }
    else
    {
      cout << "ZS_Val(4.3): Enter new Value= ";
      cin >> ZS_Val;
    }

    while(1)
    {
      cout << "CMC ON/OFF ? ---> ON/OFF --> ";
      cin >> cmc;
      if((cmc=="ON")||(cmc=="on")||(cmc=="OFF")||(cmc=="off"))
        break;
      else
        cout << "CMC: Enter on/ON or off/OFF --> " << endl;
    }
    if((cmc=="OFF")||(cmc=="off"))
    {

      CMC_Range="0.0";
      cmcT="NCMC";
    }
    else
    {
      cout << "CMC_Range(3): Enter new Value= ";
      cin >> CMC_Range;
    }

    while(1)
    {
      cout << "High Gain/Low Gin ? ---> H/h(igh) or L/l(ow)";
      cin >> higLow;
      if((higLow=="h")||(higLow=="H")||(higLow=="l")||(higLow=="L"))
        break;
      else
        cout << "High Gain/Low Gin ? ---> H/h(igh) or L/l(ow) " << endl;
    }
    if((higLow=="L")||(higLow=="l"))
      higLowT="LG";
 
    while(1)
    {
      cout << "MAGNET ON/OFF ? ---> ON/OFF --> ";
      cin >> Magnet;
      if((Magnet=="ON")||(Magnet=="on")||(Magnet=="OFF")||(Magnet=="off"))
        break;
      else
        cout << "MAGNET: Enter on/ON or off/OFF --> " << endl;
    }

    ped=true;
    if((Ped_sub =="OFF")||(Ped_sub=="off"))
    {
      ped=false;
      sub_PedT="NPS";
    }

    if((Magnet=="OFF")||(Magnet=="off"))
      MagOnOff="BOFF";

    tag=sub_PedT+"_"+zsT+"_"+cmcT+"_"+higLowT+"_"+MagOnOff;

    tagok="y";
    cout << "Is your tag name ok: ---> (Y)es/(N)o ? " << tag << endl;
    cin >> tagok;
    if ((tagok=="n")||(tagok=="N"))
      tag="Test";
   
    cin.clear();
    cin.ignore(256,'\n');
    cout << "Do you have a comment to set? --> " ;
    cin.getline(comments,80);
    comment=comments;
    
    cout << endl;

    cout << "Tag Selection:" << endl << endl;
    cout << "  PED_SUB = " << Ped_sub << endl;
    cout << "  ZS      = " << zs << endl;
    cout << "  CMC     = " << cmc  << endl << endl ;
    cout << "  ZS Value (4.3)      = " << ZS_Val << endl;
    cout << "  CMC_Range Value (3) = " << CMC_Range << endl;
    if ((higLow=="H")||(higLow=="h"))
      cout << "  Selected Gain       = HIGH GAIN" << endl;
    else
      cout << "  Selected GAin       = LOW GAIN" << endl;
    cout << endl;
    
    cout << "  Tag Name       = " << tag << endl;
    cout << "  Comment        = " << comment << endl << endl;
    //cerr << tag << endl;
    
    while(1)
    {
      cout << "Do you want to proceed then type g(o)/G(O) or redo your selection (n)ew or (N)EW ? (a)bort/(A)BORT ---> "; 
      cin >> selection;
      if ((selection=="n")||(selection=="N")||(selection=="g")||(selection=="G")||(selection=="a")||(selection=="A"))
        break;
      else
        continue;
    }
    if((selection=="g")||(selection=="G"))
      break;
    if((selection=="a")||(selection=="A"))
    {
      cout << " Run Aborted ..." << endl;
      exit(2);
    }
  }

  cout << endl;
  host = argv[1];
  sid = argv[2];
  user = argv[3];
  pass = argv[4];

  try {
    CondDBApp app(host, sid, user, pass);

    app.testWrite(ped,atof(ZS_Val.c_str()),atof(CMC_Range.c_str()),tag,higLow,comment);

  } catch (exception &e) {
    cout << "ERROR:  " << e.what() << endl;
  } catch (...) {
    cout << "Unknown error caught" << endl;
  }

  cout << "All Done." << endl;

  return 0;
}
