//
{
  // Dorian Kcira 2007.05.22: macro to read event tree produced by the interface and create a root file with an ntuple
  // original version from http://root.cern.ch/root/html/examples/basic.C.html
  gROOT->Reset();
  // open event tree
#include "Riostream.h"
  ifstream in;
  TString etree="/nfs/cms/mass8/USERS/d/dorian/2007_05_22_Johan_Simon/Zmumujets_xqcut20_qcut30_50K.tree";
  std::cout<<" using input file "<<etree<<std::endl;
  in.open(etree);
  // open root file and book ntuple
  TFile *f = new TFile("eventtree.root","RECREATE");
  TString allnames="Npart:Qjet1:Qjet2:Qjet3:Qjet4:Ptcjet1:Ptcjet2:Ptcjet3:Ptcjet4:Etacjet1:Etacjet2:Etacjet3:Etacjet4:Phicjet1:Phicjet2:Phicjet3:Phicjet4:Ptjet1:Ptjet2:Ptjet3:Ptjet4:Etajet1:Etajet2:Etajet3:Etajet4:Phijet1:Phijet2:Phijet3:Phijet4:Idres1:Ptres1:Etares1:Phires1:Idres2:Ptres2:Etares2:Phires2:Ptlep1:Etmiss:Htjets:Ptb:Etab:Ptbbar:Etabbar:Ptbj:Etabj:Qpar1:Qpar2:Qpar3:Qpar4:Ptpar1:Ptpar2:Ptpar3:Ptpar4:Ncjets:Njets:Nfile";
  TNtuple *ntuple = new TNtuple("eventtree","Event tree from MadGraph interface",allnames);
  // first read the 2 comment lines - they cause otherwise problems in the implicit loop below
  Int_t nlines = 0;
  float all[56];
  char icomment[512]; // get first 2 comment lines
  in.getline(icomment,512); std::cout<<icomment<<std::endl;
  in.getline(icomment,512); std::cout<<icomment<<std::endl;
  while (1) { // loop over all other lines and fill ntuple
  in>>all[0]>>all[1]>>all[2]>>all[3]>>all[4]>>all[5]>>all[6]>>all[7]>>all[8]>>all[9]>>all[10]>>all[11]>>all[12]>>all[13]>>all[14]>>all[15]>>all[16]>>all[17]>>all[18]>>all[19]>>all[20]>>all[21]>>all[22]>>all[23]>>all[24]>>all[25]>>all[26]>>all[27]>>all[28]>>all[29]>>all[30]>>all[31]>>all[32]>>all[33]>>all[34]>>all[35]>>all[36]>>all[37]>>all[38]>>all[39]>>all[40]>>all[41]>>all[42]>>all[43]>>all[44]>>all[45]>>all[46]>>all[47]>>all[48]>>all[49]>>all[50]>>all[51]>>all[52]>>all[53]>>all[54]>>all[55]>>all[55];
  if (!in.good()) break;
    ntuple->Fill(all); // std::cout<<"line "<<nlines<<std::endl;
    nlines++;
  }
  std::cout<<"The event tree had "<<nlines<<" lines"<<std::endl;
  in.close();

  // fill some histograms
  std::cout<<"Filling some histograms"<<std::endl;
  TH1F *hq2_all = new TH1F("hq2_all","1->2 jet rate sum",100,-1.,400.);
  TH1F *hq2_0   = new TH1F("hq2_0",  "1->2 jet rate 0",100,-1.,400.);
  TH1F *hq2_1   = new TH1F("hq2_1",  "1->2 jet rate 1",100,-1.,400.);
  TH1F *hq2_2   = new TH1F("hq2_2",  "1->2 jet rate 2",100,-1.,400.);
  TH1F *hq2_3   = new TH1F("hq2_3",  "1->2 jet rate 3",100,-1.,400.);
  TH1F *hq2_4   = new TH1F("hq2_4",  "1->2 jet rate 4",100,-1.,400.);
  eventtree->Draw("Qpar2>>hq2_all","","goff");          // goff = no graphics is generated
  eventtree->Draw("Qpar2>>hq2_0","Npart==0","goff");
  eventtree->Draw("Qpar2>>hq2_1","Npart==1","goff");
  eventtree->Draw("Qpar2>>hq2_2","Npart==2","goff");
  eventtree->Draw("Qpar2>>hq2_3","Npart==3","goff");
  eventtree->Draw("Qpar2>>hq2_4","Npart==4","goff");

  // write ntuple and histos to root file
  f->Write();
}
