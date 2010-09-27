void Run_ZgSelectorMC(){
  int   eleID_index(8);//relIso80
  int   phoID_index(6); //0-7:95,90,85,80,75,65,EG_Loose,EG_Tight
  float DelRCut(0.7);
  Run_ZgSelectorMC(eleID_index,phoID_index,DelRCut);
}

void Run_ZgSelectorMC(Int_t eleID, Int_t phoID,Float_t DelRCut){

   gROOT->LoadMacro("Zg_SelectorMC.C+");

   FILE *Table = fopen("Input_table_Sig", "r");
   char filename[200];
   char tmp[100];
   int  flag = 1;
   int  nfile(0);
   double EvtWeight[40];
   double nevt[40];
   double luminosity(1.0);
   TString ProcessTag[40];

   //Load Input table 
   cout<<"<<< File Table >>>"<<endl;
   while (flag != -1){
      flag=fscanf(Table, "%s", tmp);
      if (flag == -1) continue;
      ProcessTag[nfile] = tmp;
      flag=fscanf(Table, "%s", filename);
      flag=fscanf(Table, "%s", tmp);
      double cross=atof(tmp);
      flag=fscanf(Table, "%s", tmp);
      double evt=atof(tmp);

      EvtWeight[nfile] = 0;
      EvtWeight[nfile] = cross/evt*luminosity;
      if (nfile == 0) nevt[nfile] = evt;
      else nevt[nfile] = evt + nevt[nfile-1];
      cout<<">>> "<<nfile<<" : "<<filename<<endl;
      cout<<"    "<<ProcessTag[nfile]<<"  "<<cross<<"  "<<evt<<"  "<<nevt[nfile]<<"  "<<EvtWeight[nfile]<<endl;
      nfile += 1;
   }
   fclose(Table);

   cout<<"  "<<endl;
   TString TSaveFileName = Form("Test_eID%d_pID%d_dR07",eleID,phoID);
   TH1D *hInfo  = new TH1D("hInfo","Event Information",15,0,15);
   hInfo->SetBinContent(1,nfile);
   hInfo->SetBinContent(2,eleID);
   hInfo->SetBinContent(3,phoID);

   TFile *f1 = new TFile(TSaveFileName+".root","RECREATE");
   f1->cd();
   hInfo->Write();
   f1->Close();

   for (Int_t iSample=0;iSample<nfile;iSample++) {
     cout<<"Now Running....sample:"<<iSample<<", Weight:"<<EvtWeight[iSample]<<endl;
     Zg_SelectorMC t(iSample);
     t.Loop(iSample,phoID,eleID,EvtWeight[iSample],ProcessTag[iSample],TSaveFileName,DelRCut);
   }
   cout<<"Done!"<<endl;
}
