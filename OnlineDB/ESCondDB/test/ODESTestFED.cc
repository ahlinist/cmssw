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
  cout << " Display Size= " << p->size() << endl << endl;
  for(unsigned i=0; i< 35 ; ++i)
  {
    l=(*p)[i].getPedestal();
    cout << "ID= " << (*p)[i].getId() << endl;
    cout <<" ped=" << l << " Strip= " << (*p)[i].getStripId() << " Fiber= " << (*p)[i].getFiberId() <<" Kchip= " << (*p)[i].getKchipId() << endl;
    cout << " Rms= " << (*p)[i].getRms() <<  endl;
  }
}


void testWrite(const int nbP,const int nbM,const string tag,const int version,const string comment )
{ 
  
  ODESFEPedestalOffsetInfo run_info;

  run_info.setConfigTag(tag);
  run_info.setVersion(version);
  run_info.setIov_pl(nbP);
  run_info.setIov_mi(nbM);
  run_info.setUser_comment(comment);
  
  cout << "Inserting in DB the ODRunConfigInfo..." << flush;
  econn->insertConfigSet(&run_info);
  cout << "Done." << endl;
  econn->fetchLastConfigSet(&run_info);
  //this->printInfo(run_info);

   vector<ODESFEPedestalOffsetsDat> dataset;

   econn->fetchConfigDataSet(&dataset,&run_info);
   if( dataset.size()==137216)
     this->printResult(&dataset);
   else
   {
     cout << "Strips not Completed" << endl;
     return;
   }
   this->writeDb(dataset,&run_info);
}


void writeDb(std::vector< ODESFEPedestalOffsetsDat > dataset,ODESFEPedestalOffsetInfo * runinfo) {
 
  for (unsigned int i=0; i< dataset.size(); ++i) 
  {
     int rms=dataset[i].getRms();
     if((rms>20) || (rms<3))
       dataset[i].setMasked(1);
     else
       dataset[i].setMasked(0);         
     dataset[i].setCmRange(4.3*rms);
     if(rms>8) 
	dataset[i].setCmMasked(1);
     else 
	dataset[i].setCmMasked(0);
     dataset[i].setGain(1);
     dataset[i].setZs(3*rms);
  }

  /***
  std::vector< ODESFEPedestalOffsetsDat >::iterator it;
  int i=0;
  for(it=dataset.begin(); it< dataset.end() ; ++it)
  {
    if(( it->getMasked()==1)||( it->getCmMasked()==1)) {
     cout << "Ped=" << it->getPedestal() << " Kchip= " << it->getKchipId() << " Fiber= " <<  it->getFiberId() << endl;
     cout << "Cmrang=" << it->getCmRange() << " Masked= " << it->getMasked() << " CmMasked= " <<  it->getCmMasked() << endl;
     cout << " RMS= " <<  it->getRms() << " Gain=" << it->getGain() << " Zs=" << it->getZs() << endl;
     i++;
    }
  } 
  ***/

  if ( econn ) {
    try {
      if ( dbg ) cout << "Inserting Config Pedestals Data ..." << endl;
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
};

int main (int argc, char* argv[])
{
  string host;
  string sid;
  string user;
  string pass;
  string selection,comment;
  string tag;
  int version;
  int rnbPlus;
  int rnbMinus;
 

  if (argc != 7) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " <host> <SID> <user> <pass> <RunNumberEndcap+> <RunNubmerEncap->" << endl;
    exit(-1);
  }

  dbg=true;
  while(1)
  {
    cout << "Enter Your Tag -->" ;
    cin >> tag;
    cout << endl;
    cout << "Enter Your Version -->" ;
    cin >> version;
    cout << endl;
    cout << "Do you have a comment to set? -->" ;
    cin >> comment;
    cout << endl;
    cout << " Your Selection: Tag=" << tag << " Version=" << version << " Comment=" << comment << endl;
    cout << "Do you want to quit (q) or (Q) or make a new selection (n) ?" << endl;
    cin >> selection;
    if ((selection=="q")||(selection=="Q"))
      break;
    else
      continue;
  }

  host = argv[1];
  sid = argv[2];
  user = argv[3];
  pass = argv[4];
  rnbPlus=atoi(argv[5]);
  rnbMinus=atoi(argv[6]);

  try {
    CondDBApp app(host, sid, user, pass);

    app.testWrite(rnbPlus,rnbMinus,tag,version,comment);

  } catch (exception &e) {
    cout << "ERROR:  " << e.what() << endl;
  } catch (...) {
    cout << "Unknown error caught" << endl;
  }

  cout << "All Done." << endl;

  return 0;
}
