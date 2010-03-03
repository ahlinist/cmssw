#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>

#include "TFile.h"
#include "TList.h"
#include "TKey.h"

#include "TH1D.h"

#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TStyle.h"
//#include <>

/*
Note on usage:

AddDQMHistos L1RPC dqm.root // this file needs to be saved directly, not by dqm fw 
PlotHistos histos.root // this file is produced by AddDQMHistos


--- 
  TMF 2010

*/

struct TStackAndLegend {
  TStackAndLegend(THStack * s, TLegend * l ): _stack(s), _leg(l), _all(0), _rec(0), _recCosm(0) {};
  TStackAndLegend(): _stack(0), _leg(0), _all(0), _rec(0), _recCosm(0) {};
  THStack * _stack;
  TLegend * _leg; 
  float _all, _rec, _recCosm;  
};

int main(int argc, char ** argv)
{


  gStyle->SetCanvasBorderMode (0);
  gStyle->SetPadBorderMode (0);
  gStyle->SetPadColor (0);
  gStyle->SetCanvasColor (0);
  gStyle->SetTitleBorderSize (0);
  gStyle->SetStatColor (0);
  gStyle->SetOptDate (0);
  gStyle->SetPalette (1);
  gStyle->SetOptFit (0111);
  gStyle->SetOptStat (1000000);
                                    
                    
  
    if(argc!=2)
    {
        std::cout<<"Usage: " << argv[0] <<" rootfile" <<std::endl;
        return 1;
    }

    TFile * file = new TFile(argv[1]);
    if (!file->IsOpen())
    {
        std::cout << "Problem with file: " << argv[1] << std::endl;
    }

    std::map<std::string, TStackAndLegend> stackMap;

    std::vector<TKey * > keys;
    
    TIter nextkey( file->GetListOfKeys()  );
    TKey *key;
    while ( (key = (TKey*)nextkey()))
    {
       keys.push_back(key);
    }
    
    for (std::vector<TKey * >::reverse_iterator it = keys.rbegin(); 
         it != keys.rend();
         ++it  ) 
    {
        TObject *obj = (*it)->ReadObj();
        TH1F * h = dynamic_cast<TH1F *>(obj);
        std::string name = obj->GetName();
        if (h==0 )
        {
            std::cout << "Cannot cast: " << obj->GetName() << std::endl;
            continue;
        }

        if ( name.find("ghost")==std::string::npos )
        {
            unsigned int pos = name.find("quality");
            if ( pos != std::string::npos)
            {
                std::stringstream ss;
                ss << name.substr(pos+8,1);
                int qualVal = -1;
                ss >> qualVal;

                if ( qualVal != 4)
                {
                    std::string stackName = name.substr(0,pos) + "Stack" ;
                    std::cout   << name << " " << stackName << " " << qualVal << std::endl;
                    if ( stackMap.find(stackName) == stackMap.end() )
                    {
                        stackMap[stackName] = TStackAndLegend(  
                                  new THStack(stackName.c_str(), h->GetTitle()),
                                  new TLegend(0.2,0.6,0.3,0.9)
                                );
                    }
                    h->SetFillColor(qualVal+1);
                    stackMap[stackName]._stack->Add(h);
                    std::string leg = "q"+ss.str();
                    stackMap[stackName]._leg->AddEntry(h, leg.c_str(), "f");
                    stackMap[stackName]._all += h->Integral(1,32); 
                    stackMap[stackName]._recCosm += h->Integral(2,32); 
                    stackMap[stackName]._rec +=  h->Integral(3,32); 
                    // , _all(0), _rec(0), _recCosm(0)


                }
            }
        }

    }

    TFile plots("plots.root", "recreate");
    std::map<std::string, TStackAndLegend >::iterator it = stackMap.begin();
    std::map<std::string, TStackAndLegend >::iterator itE = stackMap.end();
    for (;it!=itE;++it)
    {
       std::string canName = "canvas" + it->first;
       float eff           = float(it->second._rec)/it->second._all;
       float effWithCosmic = float(it->second._recCosm)/it->second._all;
       //canTitle << it->first << " eff=" << eff << " effCosm="<< effWithCosmic; 
       
       TCanvas * c = new TCanvas(canName.c_str(), canName.c_str(), 800, 600);
       c->cd();
       c->SetGridx();
       c->SetGridy();
                     
       
       std::stringstream ss;
       ss.setf(std::ios::fixed, std::ios::floatfield);
       ss.setf(std::ios::showpoint);
       ss.precision(2);
             
       ss << it->second._stack->GetTitle() << " eff=" << eff << " effCosm="<< effWithCosmic;
       
       it->second._stack->SetTitle(ss.str().c_str());
       it->second._stack->Draw();
       it->second._stack->Write();
       
       it->second._leg->Draw();
       it->second._leg->Write();

       c->Write();
       c->Print( (canName + ".png").c_str() );       
    }

    return 0;
}
