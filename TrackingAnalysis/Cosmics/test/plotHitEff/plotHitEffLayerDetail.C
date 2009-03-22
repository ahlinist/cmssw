////////////////////////////
// Script to be used to plot sensor efficiency for one layer with 
// granularity at the ROD/string/ring/petal level
// The input file is the output from TrackingAnalysis/Cosmics/src/HitRes.cc
// Usage in root: root> .L plotHitEffLayerDetail.C+
//                root>  plotHitEffLayerDetail(layer) 
// Original Author:  Keith Ulmer  University of Colorado 
//                   keith.ulmer@colorado.edu
////////////////////////////

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2F.h>
#include <TLeaf.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TProfile.h>

#include <map>
#include <vector>

#include <iostream>

using namespace std;

void plotHitEffLayerDetail(Long_t layer){
  //  TString file = TString("hitEff_CRAFT_run69912_layer") + layer;
  //  TFile* f = TFile::Open(file+"_220_ReReco_redotracking/hiteff_CRAFT.root");
  //TFile* f = TFile::Open("hiteff_CRAFT_layer19_run67818.root");
  TFile* f = TFile::Open("hitEff_CRAFT_run69912_layer17_220_ReReco_redotracking/hiteff_CRAFT.root");
  f->cd("anEff");

  TCanvas*  c1 = new TCanvas("c1","hit efficiency by run",600,600);
  TTree* tree = (TTree*)(gDirectory->Get("traj")) ;
  TLeaf* isBadLf = tree->GetLeaf("ModIsBad");
  TLeaf* isQualBadLf = tree->GetLeaf("SiStripQualBad");
  TLeaf* idLf = tree->GetLeaf("Id");
  int nevents = tree->GetEntries();
  float threshhold = 0.9;
  bool isTIB = false;
  bool isTID = false;
  bool isTOB = false;
  bool isTEC = false;
  if ( layer < 5 ){
    cout << "TIB layer, plotting vs string" << endl;
    isTIB = true;
  }
  if ( layer > 4 && layer < 11) {
    cout << "TOB layer, plotting vs rod" << endl;
    isTOB = true;
  }
  if ( layer > 10 && layer < 14) {
    cout << "TID layer, plotting vs ring" << endl;
    isTID = true;
  } 
  if ( layer > 13 ) {
    cout << "TEC layer, plotting vs petal" << endl;
    isTEC = true;
  }
  
  map< unsigned int, pair< unsigned int, unsigned int> > modCounterPlus;
  map< unsigned int, pair< unsigned int, unsigned int> > modCounterMinus;
  map< unsigned int, pair< unsigned int, unsigned int> > modCounterPlusOutside;
  map< unsigned int, pair< unsigned int, unsigned int> > modCounterMinusOutside;
  int plusminus, plusminustid, plusminustec, plusminustib, plusminustob;  
  int internaltib;

  for(int i=0; i<nevents; ++i) {
    tree->GetEvent(i);
    unsigned int isBad = isBadLf->GetValue();
    unsigned int id = idLf->GetValue();
    unsigned int isQualBad = isQualBadLf->GetValue();

    int var;    
    int ringtid = ((id>>9) & 0x3); //only meaningful for TID
    ((id>>13) & 0x3) == 1 ? plusminustid = -1 : plusminustid = 1;
    
    int petaltec = ((id>>8) & 0xF); //only meaningful for TEC
    ((id>>18) & 0x3) == 1 ? plusminustec = -1 : plusminustec = 1;

    int stringtib = ((id>>4) & 0x3F); //only meaningful for TIB
    ((id>>12) & 0x3) == 1 ? plusminustib = -1 : plusminustib = 1;
    ((id>>10) & 0x3) == 1 ? internaltib = 1 : internaltib = -1;

    int rodtob = ((id>>5) & 0x7F); //only meaningful for TOB
    ((id>>12) & 0x3) == 1 ? plusminustob = -1 : plusminustob = 1;

    if (isTID) {
      var = ringtid; 
      plusminus = plusminustid;
      internaltib = 0;
    } else if (isTIB) {
      var = stringtib;
      plusminus = plusminustib;
    } else if (isTOB) {
      var = rodtob;
      plusminus = plusminustob;
      internaltib = 0;
    } else if (isTEC) {
      var = petaltec;
      plusminus = plusminustec;
      internaltib = 0;
    }
    

    if (plusminus==1 && internaltib>-0.5 && isQualBad==0) {
      map< unsigned int, pair<unsigned int, unsigned int> >::iterator it = modCounterPlus.find(var);
      if ( it==modCounterPlus.end() ) {
	pair<unsigned int, unsigned int> newgoodpair (1,1);
	pair<unsigned int, unsigned int> newbadpair (1,0);
	if ( isBad == 1) modCounterPlus[var] = newbadpair;
	else modCounterPlus[var] = newgoodpair;
      } else {
	++(((*it).second).first);
	if ( isBad == 0)	++(((*it).second).second);
      }
    }

    if (plusminus==-1 && internaltib>-0.5 && isQualBad==0) {
      map< unsigned int, pair<unsigned int, unsigned int> >::iterator it = modCounterMinus.find(var);
      if ( it==modCounterMinus.end() ) {
	pair<unsigned int, unsigned int> newgoodpair (1,1);
	pair<unsigned int, unsigned int> newbadpair (1,0);
	if ( isBad == 1) modCounterMinus[var] = newbadpair;
	else modCounterMinus[var] = newgoodpair;
      } else {
	++(((*it).second).first);
	if ( isBad == 0)	++(((*it).second).second);
      }
    }

    //make outside string plots for TIB only
    if (plusminus==1 && internaltib==-1 && isQualBad==0) {
      map< unsigned int, pair<unsigned int, unsigned int> >::iterator it = modCounterPlusOutside.find(var);
      if ( it==modCounterPlusOutside.end() ) {
	pair<unsigned int, unsigned int> newgoodpair (1,1);
	pair<unsigned int, unsigned int> newbadpair (1,0);
	if ( isBad == 1) modCounterPlusOutside[var] = newbadpair;
	else modCounterPlusOutside[var] = newgoodpair;
      } else {
	++(((*it).second).first);
	if ( isBad == 0)	++(((*it).second).second);
      }
    }

    if (plusminus==-1 && internaltib==-1 && isQualBad==0) {
      map< unsigned int, pair<unsigned int, unsigned int> >::iterator it = modCounterMinusOutside.find(var);
      if ( it==modCounterMinusOutside.end() ) {
	pair<unsigned int, unsigned int> newgoodpair (1,1);
	pair<unsigned int, unsigned int> newbadpair (1,0);
	if ( isBad == 1) modCounterMinusOutside[var] = newbadpair;
	else modCounterMinusOutside[var] = newgoodpair;
      } else {
	++(((*it).second).first);
	if ( isBad == 0)	++(((*it).second).second);
      }
    }
  }


  //make the plots

  if (isTID || isTEC ) {
    int size = (modCounterMinus.size()+modCounterPlus.size());
    Double_t var[size];
    Double_t eff[size];
    int iTG = 0;
    for ( map<unsigned int, pair<unsigned int, unsigned int> >::const_iterator ih=modCounterMinus.begin(); ih!=modCounterMinus.end(); ++ih ) {
      if ( (((*ih).second).first) > 0 ) {
	double efficiency = (double)(((*ih).second).second)/(((*ih).second).first);
	var[iTG] = (*ih).first;
	eff[iTG] = efficiency;
	if (efficiency < threshhold) cout << "Layer " << layer << " Ring/Petal Minus " << iTG+1 << " has efficiency = " << efficiency << endl;
	iTG++;
      }
    }
    for ( map<unsigned int, pair<unsigned int, unsigned int> >::const_iterator ih=modCounterPlus.begin(); ih!=modCounterPlus.end(); ++ih ) {
      if ( (((*ih).second).first) > 0 ) {
	double efficiency = (double)(((*ih).second).second)/(((*ih).second).first);
	var[iTG] = ((*ih).first)+modCounterMinus.size();
	eff[iTG] = efficiency;
	if (efficiency < threshhold) cout << "Layer " << layer << " Ring/Petal Plus " << iTG-2 << " has efficiency = " << efficiency << endl;
	iTG++;
      }
    }
    
    TGraphErrors *graph = new TGraphErrors(size,var,eff);
    if (isTID) graph->GetXaxis()->SetTitle("TID Ring");
    if (isTEC) graph->GetXaxis()->SetTitle("TEC Petal");
    graph->SetTitle(TString(" Efficiency CRAFT layer ")+layer);
    graph->GetYaxis()->SetTitle("Efficiency");
    graph->SetMarkerColor(2);
    graph->SetMarkerSize(1.2);
    graph->SetLineColor(2);
    graph->SetLineWidth(4);
    graph->SetMarkerStyle(20);
    graph->SetMinimum(0.0);
    graph->SetMaximum(1.005);
    
    for ( Long_t i=0; i<(size/2); i++) {
      TString minuslabel = TString("-") + (i+1);
      TString pluslabel = TString("+") + (i+1);
      graph->GetXaxis()->SetBinLabel( (i+0.5)*(100/size) , minuslabel);
      graph->GetXaxis()->SetBinLabel((i+0.5+size/2)*(100/size), pluslabel);
    }
    graph->Draw("AP");
    TString outputfile;
    if (isTID) outputfile = TString("hitEffByRing_layer") + layer;
    if (isTEC) outputfile = TString("hitEffByPetal_layer") + layer;
    c1->Print(outputfile+".eps");
    c1->Print(outputfile+".png");
  }

  if (isTIB) {
    c1->Divide(2,2);
    
    int sizeMinusInside = modCounterMinus.size();
    Double_t stringMinusInside[sizeMinusInside];
    Double_t effMinusInside[sizeMinusInside];
    int iTG = 0;  
  
    for ( map<unsigned int, pair<unsigned int, unsigned int> >::const_iterator ih=modCounterMinus.begin(); ih!=modCounterMinus.end(); ++ih ) {
      if ( (((*ih).second).first) > 0 ) {
	double efficiency = (double)(((*ih).second).second)/(((*ih).second).first);
	stringMinusInside[iTG] = (*ih).first;
	effMinusInside[iTG] = efficiency;
	if (efficiency < threshhold) cout << "Layer " << layer << " String Inside Minus " << iTG+1 << " has efficiency = " << efficiency << endl;
	iTG++;
      }
    }
    
    TGraphErrors *graphMinusInside = new TGraphErrors(sizeMinusInside,stringMinusInside,effMinusInside);
    graphMinusInside->SetTitle(TString(" Efficiency CRAFT layer ")+layer);
    graphMinusInside->GetYaxis()->SetTitle("Efficiency");
    graphMinusInside->GetXaxis()->SetTitle("String Minus Inside");
    graphMinusInside->SetMarkerColor(2);
    graphMinusInside->SetMarkerSize(1.2);
    graphMinusInside->SetLineColor(2);
    graphMinusInside->SetLineWidth(4);
    graphMinusInside->SetMarkerStyle(20);
    graphMinusInside->SetMinimum(0.0);
    graphMinusInside->SetMaximum(1.005);
    c1->cd(1);
    graphMinusInside->Draw("AP");
    
    
    int sizeMinusOutside = modCounterMinusOutside.size();
    Double_t stringMinusOutside[sizeMinusOutside];
    Double_t effMinusOutside[sizeMinusOutside];
    iTG = 0;  
    
    for ( map<unsigned int, pair<unsigned int, unsigned int> >::const_iterator ih=modCounterMinusOutside.begin(); ih!=modCounterMinusOutside.end(); ++ih ) {
      if ( (((*ih).second).first) > 0 ) {
	double efficiency = (double)(((*ih).second).second)/(((*ih).second).first);
	stringMinusOutside[iTG] = (*ih).first;
	effMinusOutside[iTG] = efficiency;
	if (efficiency < threshhold) cout << "Layer " << layer << " String Outside Minus " << iTG+1 << " has efficiency = " << efficiency << endl;
	iTG++;
      }
    }
    
    TGraphErrors *graphMinusOutside = new TGraphErrors(sizeMinusOutside,stringMinusOutside,effMinusOutside);
    graphMinusOutside->SetTitle(TString(" Efficiency CRAFT layer ")+layer);
    graphMinusOutside->GetYaxis()->SetTitle("Efficiency");
    graphMinusOutside->GetXaxis()->SetTitle("String Minus Outside");
    graphMinusOutside->SetMarkerColor(2);
    graphMinusOutside->SetMarkerSize(1.2);
    graphMinusOutside->SetLineColor(2);
    graphMinusOutside->SetLineWidth(4);
    graphMinusOutside->SetMarkerStyle(20);
    graphMinusOutside->SetMinimum(0.0);
    graphMinusOutside->SetMaximum(1.005);
    c1->cd(2);
    graphMinusOutside->Draw("AP");
    
    //make plot for plus side
    
    int sizePlusInside = modCounterPlus.size();
    Double_t stringPlusInside[sizePlusInside];
    Double_t effPlusInside[sizePlusInside];
    iTG = 0;  
    
    for ( map<unsigned int, pair<unsigned int, unsigned int> >::const_iterator ih=modCounterPlus.begin(); ih!=modCounterPlus.end(); ++ih ) {
      if ( (((*ih).second).first) > 0 ) {
	double efficiency = (double)(((*ih).second).second)/(((*ih).second).first);
	stringPlusInside[iTG] = (*ih).first;
	effPlusInside[iTG] = efficiency;
	if (efficiency < threshhold) cout << "Layer " << layer << " String Inside Plus " << iTG+1 << " has efficiency = " << efficiency << endl;
	iTG++;
      }
    }
    
    TGraphErrors *graphPlusInside = new TGraphErrors(sizePlusInside,stringPlusInside,effPlusInside);
    graphPlusInside->SetTitle(TString(" Efficiency CRAFT layer ")+layer);
    graphPlusInside->GetYaxis()->SetTitle("Efficiency");
    graphPlusInside->GetXaxis()->SetTitle("String Plus Inside");
    graphPlusInside->SetMarkerColor(2);
    graphPlusInside->SetMarkerSize(1.2);
    graphPlusInside->SetLineColor(2);
    graphPlusInside->SetLineWidth(4);
    graphPlusInside->SetMarkerStyle(20);
    graphPlusInside->SetMinimum(0.0);
    graphPlusInside->SetMaximum(1.005);
    c1->cd(3);
    graphPlusInside->Draw("AP");
    
    int sizePlusOutside = modCounterPlusOutside.size();
    Double_t stringPlusOutside[sizePlusOutside];
    Double_t effPlusOutside[sizePlusOutside];
    iTG = 0;  
    
    for ( map<unsigned int, pair<unsigned int, unsigned int> >::const_iterator ih=modCounterPlusOutside.begin(); ih!=modCounterPlusOutside.end(); ++ih ) {
      if ( (((*ih).second).first) > 0 ) {
	double efficiency = (double)(((*ih).second).second)/(((*ih).second).first);
	stringPlusOutside[iTG] = (*ih).first;
	effPlusOutside[iTG] = efficiency;
	if (efficiency < threshhold) cout << "Layer " << layer << " String Outside Plus " << iTG+1 << " has efficiency = " << efficiency << endl;
	iTG++;
      }
    }

    TGraphErrors *graphPlusOutside = new TGraphErrors(sizePlusOutside,stringPlusOutside,effPlusOutside);
    graphPlusOutside->SetTitle(TString(" Efficiency CRAFT layer ")+layer);
    graphPlusOutside->GetYaxis()->SetTitle("Efficiency");
    graphPlusOutside->GetXaxis()->SetTitle("String Plus Outside");
    graphPlusOutside->SetMarkerColor(2);
    graphPlusOutside->SetMarkerSize(1.2);
    graphPlusOutside->SetLineColor(2);
    graphPlusOutside->SetLineWidth(4);
    graphPlusOutside->SetMarkerStyle(20);
    graphPlusOutside->SetMinimum(0.0);
    graphPlusOutside->SetMaximum(1.005);
    c1->cd(4);
    graphPlusOutside->Draw("AP");
    
    TString outputfile = TString("hitEffByString_layer") + layer;
    c1->Print(outputfile+".eps");
    c1->Print(outputfile+".png");
    
  }
  

  if (isTOB) {
    c1->Divide(1,2);
    int sizeMinus = modCounterMinus.size();
    Double_t rodMinus[sizeMinus];
    Double_t effMinus[sizeMinus];
    int iTG = 0;  
    
    for ( map<unsigned int, pair<unsigned int, unsigned int> >::const_iterator ih=modCounterMinus.begin(); ih!=modCounterMinus.end(); ++ih ) {
      if ( (((*ih).second).first) > 0 ) {
	double efficiency = (double)(((*ih).second).second)/(((*ih).second).first);
	rodMinus[iTG] = (*ih).first;
	effMinus[iTG] = efficiency;
	if (efficiency < threshhold) cout << "Layer " << layer << " Rod Minus " << iTG+1 << " has efficiency = " << efficiency << endl;
	iTG++;
      }
    }
    
    TGraphErrors *graphMinus = new TGraphErrors(sizeMinus,rodMinus,effMinus);
    graphMinus->SetTitle(TString(" Efficiency CRAFT layer ")+layer);
    graphMinus->GetYaxis()->SetTitle("Efficiency");
    graphMinus->GetXaxis()->SetTitle("Rod Minus");
    graphMinus->SetMarkerColor(2);
    graphMinus->SetMarkerSize(1.2);
    graphMinus->SetLineColor(2);
    graphMinus->SetLineWidth(4);
    graphMinus->SetMarkerStyle(20);
    graphMinus->SetMinimum(0.0);
    graphMinus->SetMaximum(1.005);
    c1->cd(1);
    graphMinus->Draw("AP");

    int sizePlus = modCounterPlus.size();
    Double_t rodPlus[sizePlus];
    Double_t effPlus[sizePlus];
    iTG = 0;  
    
    for ( map<unsigned int, pair<unsigned int, unsigned int> >::const_iterator ih=modCounterPlus.begin(); ih!=modCounterPlus.end(); ++ih ) {
      if ( (((*ih).second).first) > 0 ) {
	double efficiency = (double)(((*ih).second).second)/(((*ih).second).first);
	rodPlus[iTG] = (*ih).first;
	effPlus[iTG] = efficiency;
	if (efficiency < threshhold) cout << "Layer " << layer << " Rod Plus " << iTG+1 << " has efficiency = " << efficiency << endl;
	iTG++;
      }
    }
    
    TGraphErrors *graphPlus = new TGraphErrors(sizePlus,rodPlus,effPlus);
    graphPlus->SetTitle(TString(" Efficiency CRAFT layer ")+layer);
    graphPlus->GetYaxis()->SetTitle("Efficiency");
    graphPlus->GetXaxis()->SetTitle("Rod Plus");
    graphPlus->SetMarkerColor(2);
    graphPlus->SetMarkerSize(1.2);
    graphPlus->SetLineColor(2);
    graphPlus->SetLineWidth(4);
    graphPlus->SetMarkerStyle(20);
    graphPlus->SetMinimum(0.0);
    graphPlus->SetMaximum(1.005);
    c1->cd(2);
    graphPlus->Draw("AP");
    
    TString outputfile = TString("hitEffByRod_layer") + layer;
    c1->Print(outputfile+"run67818_fixMap.eps");
    c1->Print(outputfile+"run67818_fixMap.png");
  }
  
}
