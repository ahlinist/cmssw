#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "TFile.h"
#include "TList.h"
#include "TKey.h"

#include "TH1D.h"
#include "TH2D.h"



int main(int argc, char ** argv) {

  if(argc<2){
      std::cout<<"Usage: " << argv[0] <<" Filelist" <<std::endl;
      return 1;
   }
  
  std::string fOut = "histos.root";

  std::map<std::string, TH1F *> histoTH1Map;
  std::map<std::string, TH2F *> histoTH2Map;
  std::vector<std::string> dirVec;
  dirVec.push_back("L1_RPC");
  //dirVec.push_back("L1_RPC/Resolution");
  dirVec.push_back("L1_RPC/Distribution");
  dirVec.push_back("L1_RPC/Efficiency");
  
  
  int iFile = 0;
  for(int i = 1; i < argc ; ++i) {
    ++iFile; // we could use i aswell
  
    TFile * file = new TFile(argv[i]);

    

    if (!file->IsOpen()){
      std::cout << "Problem with file: " << argv[i] << std::endl;
      continue;
    }

    if(file->GetDirectory("DQMData")==0){
    std::cout << "Problem with file: " << argv[i] << std::endl;
    continue;
    }

     std::vector<std::string>::iterator itDir = dirVec.begin();
     std::vector<std::string>::iterator itDirE =  dirVec.end();
     for (;itDir!=itDirE; ++itDir){
    if(file->GetDirectory("DQMData")->GetDirectory(itDir->c_str())==0){
    std::cout << "Problem with file: " << argv[i] << std::endl;
    continue;
    }

    TIter nextkey( file->GetDirectory("DQMData")->GetDirectory(itDir->c_str())->GetListOfKeys()  );
    TKey *key;
    while ( (key = (TKey*)nextkey())) {
      TObject *obj = key->ReadObj();
      TH1F * h1 = dynamic_cast<TH1F *>(obj);
      TH2F * h2 = dynamic_cast<TH2F *>(obj);
      if (h1!=0 ) {
        std::string name=obj->GetName();
        if (  histoTH1Map.find(name) == histoTH1Map.end() ){

            if (iFile > 1) {
              std::cout << " Warning: new histogram '"
                << name 
                <<"' appeared in file"  << argv[i]
                << std::endl;
                
            }
            std::string newName = name+"Sum";
            histoTH1Map[name]=(TH1F *)h1->Clone(newName.c_str());

        } else {
		std::cout << " OK "
                << name 
                <<"from file"  << argv[i]
                << std::endl;
            histoTH1Map[name]->Add(h1);
        }
        
      } else if(h2!=0){
	
	     std::string name=obj->GetName();
        if (  histoTH2Map.find(name) == histoTH2Map.end() ){

            if (iFile > 1) {
              std::cout << " Warning: new histogram '"
                << name 
                <<"' appeared in file"  << argv[i]
                << std::endl;
                
            }
            std::string newName = name+"Sum";
            histoTH2Map[name]=(TH2F *)h2->Clone(newName.c_str());

        } else {
            
	        std::cout << " OK "
                << name 
                <<"from file"  << argv[i]
                << std::endl;
               histoTH2Map[name]->Add(h2);
        }
        
    }else {
        std::cout << "Cannot cast: " << obj->GetName() << std::endl;
      }
  
  }
  } 
}// end file iteration
  std::cout << "Read " <<iFile << " files. " << std::endl;
  
  
  std::cout << " Saving to " << fOut << std::endl;

  TFile f(fOut.c_str(), "RECREATE");
  //f.Cd();
  std::map<std::string, TH1F *>::iterator itTH1 = histoTH1Map.begin();
  std::map<std::string, TH1F *>::iterator itTH1E = histoTH1Map.end();
  for (;itTH1!=itTH1E;++itTH1) 
  {
	if (  histoTH1Map.find(itTH1->first + "_Denom") == histoTH1Map.end() ){
    std::cout << itTH1->first << std::endl;
    itTH1->second->Write();
     }else {
          itTH1->second->Divide(histoTH1Map.find(itTH1->first + "_Denom")->second);
          std::cout << itTH1->first << " / "<< histoTH1Map.find(itTH1->first + "_Denom")-> first << std::endl;
          itTH1->second->Write();
     }
  }

  std::map<std::string, TH2F *>::iterator itTH2 = histoTH2Map.begin();
  std::map<std::string, TH2F *>::iterator itTH2E = histoTH2Map.end();
  for (;itTH2!=itTH2E;++itTH2) 
  {

	if (  histoTH2Map.find(itTH2->first + "_Denom") == histoTH2Map.end() ){
    std::cout << itTH2->first << std::endl;
    itTH2->second->Write();
     }else {
          itTH2->second->Divide(histoTH2Map.find(itTH2->first + "_Denom")->second);
          std::cout << itTH2->first << " / "<< histoTH2Map.find(itTH2->first + "_Denom")-> first << std::endl;
          itTH2->second->Write();
     }
  }


  return 0;
}