#ifndef AH_DEBUGUTILS
#define AH_DEBUGUTILS

#include <iostream>
#include <stdio.h>
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1.h"

using std::endl;
using std::cout;
using std::cerr;

void print_list_of_functions( TH1& hist )
{
  cout<<hist.GetName();
  TList* pl = hist.GetListOfFunctions();
  if( !pl ) {
    cout<<" has no attached \"functions\""<<endl;
    return;
  }
  cout<<" : "<<endl;
  for( int io = 0; io < pl->GetSize(); ++io ) {
    TObject* po = pl->At( io );
    cout<<"---------------------------------------\n"
	<<io<<") "<<po->GetName()<<" : "<<endl;
    po->Print();
  }
  cout<<"'--------------------------------------------"<<endl;
  return;
}

void debugUtils () {
  TCanvas* cnv = new TCanvas;
  TH1F* hh = new TH1F ("hh", "hhh", 3, 4, 5);
  hh->Fill(4.4, 3);
  hh->Fill(4.1, 3);
  hh->Fill(4.8, 4);
  cout<<"Before:"<<endl;
  print_list_of_functions( *hh );
  hh->Fit("pol1");
  cnv->Update();
  cout<<"After:"<<endl;
  print_list_of_functions( *hh );
  return;
}

#endif
