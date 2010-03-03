#include <iostream>
#include <string>
#include <map>

#include "TFile.h"
#include "TList.h"
#include "TKey.h"

#include "TH1D.h"

/*
Usage:

 L1RPCAddDQMHistos L1_RPC dqm.root // dqm.root is file saved by L1RPC validator, not by dqm fw
 L1RPCPlotHistos histos.root // generated at prev step

---

  TMF 2010

*/



int main(int argc, char ** argv) {

  if(argc<3){
      std::cout<<"Usage: " << argv[0] <<" DirectoryName Filelist" <<std::endl;
      return 1;
   }
  


  std::map<std::string, TH1F *> histoMap;
  
  std::string DQMDir = argv[1];
  
  int iFile = 0;
  for(int i = 2; i < argc ; ++i) {
    ++iFile; // we could use i aswell
  
    TFile * file = new TFile(argv[i]);
    if (!file->IsOpen()){
      std::cout << "Problem with file: " << argv[i] << std::endl;
      continue;
    }

    
    TIter nextkey( file->GetDirectory("DQMData")->GetDirectory(argv[1])->GetListOfKeys()  );
    TKey *key;
    while ( (key = (TKey*)nextkey())) {
      TObject *obj = key->ReadObj();
      TH1F * h = dynamic_cast<TH1F *>(obj);
      if (h==0 ) {
        std::cout << "Cannot cast: " << obj->GetName() << std::endl;
      } else {
        std::string name=obj->GetName();
        if (  histoMap.find(name) == histoMap.end() ){
            if (iFile > 1) {
              std::cout << " Warning: new histogram '"
                << name 
                <<"' appeared in file"  << argv[i]
                << std::endl;
                
            }
            std::string newName = name+"Sum";
            histoMap[name]=(TH1F *)h->Clone(newName.c_str());
        } else {
            histoMap[name]->Add(h);
        }
      }
        
    }
  
  
  } // end file iteration
  std::cout << "Read " <<iFile << " files. " << std::endl;
  
  std::string fOut = "histos.root";
  std::cout << " Saving to " << fOut << std::endl;

  TFile f(fOut.c_str(), "new");
  //f.Cd();
  std::map<std::string, TH1F *>::iterator it = histoMap.begin();
  std::map<std::string, TH1F *>::iterator itE = histoMap.end();
  for (;it!=itE;++it) 
  {
    std::cout << it->first << std::endl;
    it->second->Write();
  }


  return 0;
}
