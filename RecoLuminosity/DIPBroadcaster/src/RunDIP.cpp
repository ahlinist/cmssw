#include "RecoLuminosity/DIPBroadcaster/interface/DIPBroadcaster.h"
#include <signal.h>

bool shouldContinue= true;

void CtrlC(int aSigNum) {
  std::cout << "stopping" << std::endl;
  shouldContinue= false;
  exit(1);
}

int main(){

  signal(SIGINT,CtrlC);

  std::string lumiPubName = "dip/CMS/HF/LumiData";
  std::string lumiLHPubName = "dip/CMS/HF/LumiDataLH";

  DIPBroadcaster dipBroad(51007, "localhost", 5, 0);

  bool lumiPubExists = true;
  bool lumiLHPubExists = true;

  while(lumiPubExists == true || lumiLHPubExists == true){
    lumiPubExists = dipBroad.doesPublicationAlreadyExist(lumiPubName);
    lumiLHPubExists = dipBroad.doesPublicationAlreadyExist(lumiLHPubName);
    if(lumiPubExists == true || lumiLHPubExists == true){
      std::cout<<"WARNING: The lumi publication "<<lumiPubName<<" and/or "<<lumiLHPubName<<" already exists."<<std::endl;
      std::cout<<"Can't create these publications before the other ones are destroyed!"<<std::endl<<std::endl;
      sleep(5);
    }
  }

  dipBroad.beginJob(lumiPubName, lumiLHPubName);
  
  while(shouldContinue == true){
    try{
      dipBroad.analyze();
      }catch(...){ std::cout<<"Exception: Analyze failed"<<std::endl;}
  }

  dipBroad.endJob();
  
  return 0;
}
