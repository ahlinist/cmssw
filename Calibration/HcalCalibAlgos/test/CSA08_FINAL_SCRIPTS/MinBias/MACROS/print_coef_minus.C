{
gROOT->Reset();
gROOT->SetStyle("Plain");

gStyle->SetOptStat(1111);
gStyle->SetOptFit(111);
     

TH1F  *h12mlnetacoefmin1 = new TH1F("h12mlnetacoefmin1", "h12mlnetacoefmin1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin2 = new TH1F("h12mlnetacoefmin2", "h12mlnetacoefmin2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin3 = new TH1F("h12mlnetacoefmin3", "h12mlnetacoefmin3", 100, 0., 2.);
TH1F  *h12mlnetacoefmin4 = new TH1F("h12mlnetacoefmin4", "h12mlnetacoefmin4", 100, 0., 2.);
TH1F  *h12mlnetacoefmin5 = new TH1F("h12mlnetacoefmin5", "h12mlnetacoefmin5", 100, 0., 2.);
TH1F  *h12mlnetacoefmin6 = new TH1F("h12mlnetacoefmin6", "h12mlnetacoefmin6", 100, 0., 2.);
TH1F  *h12mlnetacoefmin7 = new TH1F("h12mlnetacoefmin7", "h12mlnetacoefmin7", 100, 0., 2.);
TH1F  *h12mlnetacoefmin8 = new TH1F("h12mlnetacoefmin8", "h12mlnetacoefmin8", 100, 0., 2.);
TH1F  *h12mlnetacoefmin9 = new TH1F("h12mlnetacoefmin9", "h12mlnetacoefmin9", 100, 0., 2.);
TH1F  *h12mlnetacoefmin10 = new TH1F("h12mlnetacoefmin10", "h12mlnetacoefmin10", 100, 0., 2.);
TH1F  *h12mlnetacoefmin11 = new TH1F("h12mlnetacoefmin11", "h12mlnetacoefmin11", 100, 0., 2.);
TH1F  *h12mlnetacoefmin12 = new TH1F("h12mlnetacoefmin12", "h12mlnetacoefmin12", 100, 0., 2.);
TH1F  *h12mlnetacoefmin13 = new TH1F("h12mlnetacoefmin13", "h12mlnetacoefmin13", 100, 0., 2.);
TH1F  *h12mlnetacoefmin14 = new TH1F("h12mlnetacoefmin14", "h12mlnetacoefmin14", 100, 0., 2.);
TH1F  *h12mlnetacoefmin15 = new TH1F("h12mlnetacoefmin15", "h12mlnetacoefmin15", 100, 0., 2.);
TH1F  *h12mlnetacoefmin16_1 = new TH1F("h12mlnetacoefmin16_1", "h12mlnetacoefmin16_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin16_2 = new TH1F("h12mlnetacoefmin16_2", "h12mlnetacoefmin16_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin16_3 = new TH1F("h12mlnetacoefmin16_3", "h12mlnetacoefmin16_3", 100, 0., 2.);

TH1F  *h12mlnetacoefmin17_1 = new TH1F("h12mlnetacoefmin17_1", "h12mlnetacoefmin17_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin18_1 = new TH1F("h12mlnetacoefmin18_1", "h12mlnetacoefmin18_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin19_1 = new TH1F("h12mlnetacoefmin19_1", "h12mlnetacoefmin19_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin20_1 = new TH1F("h12mlnetacoefmin20_1", "h12mlnetacoefmin20_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin21_1 = new TH1F("h12mlnetacoefmin21_1", "h12mlnetacoefmin21_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin22_1 = new TH1F("h12mlnetacoefmin22_1", "h12mlnetacoefmin22_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin23_1 = new TH1F("h12mlnetacoefmin23_1", "h12mlnetacoefmin23_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin24_1 = new TH1F("h12mlnetacoefmin24_1", "h12mlnetacoefmin24_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin25_1 = new TH1F("h12mlnetacoefmin25_1", "h12mlnetacoefmin25_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin26_1 = new TH1F("h12mlnetacoefmin26_1", "h12mlnetacoefmin26_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin27_1 = new TH1F("h12mlnetacoefmin27_1", "h12mlnetacoefmin27_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin28_1 = new TH1F("h12mlnetacoefmin28_1", "h12mlnetacoefmin28_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin29_1 = new TH1F("h12mlnetacoefmin29_1", "h12mlnetacoefmin29_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin30_1 = new TH1F("h12mlnetacoefmin30_1", "h12mlnetacoefmin30_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin31_1 = new TH1F("h12mlnetacoefmin31_1", "h12mlnetacoefmin31_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin32_1 = new TH1F("h12mlnetacoefmin32_1", "h12mlnetacoefmin32_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin33_1 = new TH1F("h12mlnetacoefmin33_1", "h12mlnetacoefmin33_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin34_1 = new TH1F("h12mlnetacoefmin34_1", "h12mlnetacoefmin34_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin35_1 = new TH1F("h12mlnetacoefmin35_1", "h12mlnetacoefmin35_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin36_1 = new TH1F("h12mlnetacoefmin36_1", "h12mlnetacoefmin36_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin37_1 = new TH1F("h12mlnetacoefmin37_1", "h12mlnetacoefmin37_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin38_1 = new TH1F("h12mlnetacoefmin38_1", "h12mlnetacoefmin38_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin39_1 = new TH1F("h12mlnetacoefmin39_1", "h12mlnetacoefmin39_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin40_1 = new TH1F("h12mlnetacoefmin40_1", "h12mlnetacoefmin40_1", 100, 0., 2.);
TH1F  *h12mlnetacoefmin41_1 = new TH1F("h12mlnetacoefmin41_1", "h12mlnetacoefmin41_1", 100, 0., 2.);

TH1F  *h12mlnetacoefmin17_2 = new TH1F("h12mlnetacoefmin17_2", "h12mlnetacoefmin17_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin18_2 = new TH1F("h12mlnetacoefmin18_2", "h12mlnetacoefmin18_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin19_2 = new TH1F("h12mlnetacoefmin19_2", "h12mlnetacoefmin19_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin20_2 = new TH1F("h12mlnetacoefmin20_2", "h12mlnetacoefmin20_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin21_2 = new TH1F("h12mlnetacoefmin21_2", "h12mlnetacoefmin21_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin22_2 = new TH1F("h12mlnetacoefmin22_2", "h12mlnetacoefmin22_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin23_2 = new TH1F("h12mlnetacoefmin23_2", "h12mlnetacoefmin23_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin24_2 = new TH1F("h12mlnetacoefmin24_2", "h12mlnetacoefmin24_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin25_2 = new TH1F("h12mlnetacoefmin25_2", "h12mlnetacoefmin25_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin26_2 = new TH1F("h12mlnetacoefmin26_2", "h12mlnetacoefmin26_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin27_2 = new TH1F("h12mlnetacoefmin27_2", "h12mlnetacoefmin27_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin28_2 = new TH1F("h12mlnetacoefmin28_2", "h12mlnetacoefmin28_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin29_2 = new TH1F("h12mlnetacoefmin29_2", "h12mlnetacoefmin29_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin30_2 = new TH1F("h12mlnetacoefmin30_2", "h12mlnetacoefmin30_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin31_2 = new TH1F("h12mlnetacoefmin31_2", "h12mlnetacoefmin31_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin32_2 = new TH1F("h12mlnetacoefmin32_2", "h12mlnetacoefmin32_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin33_2 = new TH1F("h12mlnetacoefmin33_2", "h12mlnetacoefmin33_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin34_2 = new TH1F("h12mlnetacoefmin34_2", "h12mlnetacoefmin34_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin35_2 = new TH1F("h12mlnetacoefmin35_2", "h12mlnetacoefmin35_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin36_2 = new TH1F("h12mlnetacoefmin36_2", "h12mlnetacoefmin36_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin37_2 = new TH1F("h12mlnetacoefmin37_2", "h12mlnetacoefmin37_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin38_2 = new TH1F("h12mlnetacoefmin38_2", "h12mlnetacoefmin38_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin39_2 = new TH1F("h12mlnetacoefmin39_2", "h12mlnetacoefmin39_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin40_2 = new TH1F("h12mlnetacoefmin40_2", "h12mlnetacoefmin40_2", 100, 0., 2.);
TH1F  *h12mlnetacoefmin41_2 = new TH1F("h12mlnetacoefmin41_2", "h12mlnetacoefmin41_2", 100, 0., 2.);


std::string line;
std::ifstream in20( "coef_without_noise_10mln_check.txt" );
while( std::getline( in20, line)){
int subd,eta,phi,dep;
Float_t var,err;
istringstream linestream(line);
linestream>>subd>>dep>>eta>>phi>>var>>err;

  if(subd==1) {

    
    if(dep == 1 && eta == -1) h12mlnetacoefmin1->Fill(var);
    if(dep == 1 && eta == -2) h12mlnetacoefmin2->Fill(var);
    if(dep == 1 && eta == -3) h12mlnetacoefmin3->Fill(var);
    if(dep == 1 && eta == -4) h12mlnetacoefmin4->Fill(var); 
    if(dep == 1 && eta == -5) h12mlnetacoefmin5->Fill(var);
    if(dep == 1 && eta == -6) h12mlnetacoefmin6->Fill(var);
    if(dep == 1 && eta == -7) h12mlnetacoefmin7->Fill(var);
    if(dep == 1 && eta == -8) h12mlnetacoefmin8->Fill(var); 
    if(dep == 1 && eta == -9) h12mlnetacoefmin9->Fill(var); 
    if(dep == 1 && eta == -10) h12mlnetacoefmin10->Fill(var);       
    if(dep == 1 && eta == -11) h12mlnetacoefmin11->Fill(var);
    if(dep == 1 && eta == -12) h12mlnetacoefmin12->Fill(var); 
    if(dep == 1 && eta == -13) h12mlnetacoefmin13->Fill(var); 
    if(dep == 1 && eta == -14) h12mlnetacoefmin14->Fill(var);
    if(dep == 1 && eta == -15) h12mlnetacoefmin15->Fill(var);
    if(dep == 1 && eta == -16) h12mlnetacoefmin16_1->Fill(var);
    if(dep == 2 && eta == -16) h12mlnetacoefmin16_2->Fill(var);
    
  } // subd = 1
  if( subd == 2) {
    
    if(dep == 3 && eta == -16) h12mlnetacoefmin16_3->Fill(var);   
    if(dep == 1 && eta == -17) h12mlnetacoefmin17_1->Fill(var);
    if(dep == 1 && eta == -18) h12mlnetacoefmin18_1->Fill(var); 
    if(dep == 1 && eta == -19) h12mlnetacoefmin19_1->Fill(var);
    if(dep == 1 && eta == -20) h12mlnetacoefmin20_1->Fill(var);
    if(dep == 1 && eta == -21) h12mlnetacoefmin21_1->Fill(var);
    if(dep == 1 && eta == -22) h12mlnetacoefmin22_1->Fill(var); 
    if(dep == 1 && eta == -23) h12mlnetacoefmin23_1->Fill(var); 
    if(dep == 1 && eta == -24) h12mlnetacoefmin24_1->Fill(var);       
    if(dep == 1 && eta == -25) h12mlnetacoefmin25_1->Fill(var);
    if(dep == 1 && eta == -26) h12mlnetacoefmin26_1->Fill(var); 
    if(dep == 1 && eta == -27) h12mlnetacoefmin27_1->Fill(var); 
    if(dep == 1 && eta == -28) h12mlnetacoefmin28_1->Fill(var);
    if(dep == 1 && eta == -29) h12mlnetacoefmin29_1->Fill(var); 
    
    if(dep == 2 && eta == -17) h12mlnetacoefmin17_2->Fill(var);
    if(dep == 2 && eta == -18) h12mlnetacoefmin18_2->Fill(var); 
    if(dep == 2 && eta == -19) h12mlnetacoefmin19_2->Fill(var);
    if(dep == 2 && eta == -20) h12mlnetacoefmin20_2->Fill(var);
    if(dep == 2 && eta == -21) h12mlnetacoefmin21_2->Fill(var);
    if(dep == 2 && eta == -22) h12mlnetacoefmin22_2->Fill(var); 
    if(dep == 2 && eta == -23) h12mlnetacoefmin23_2->Fill(var); 
    if(dep == 2 && eta == -24) h12mlnetacoefmin24_2->Fill(var);       
    if(dep == 2 && eta == -25) h12mlnetacoefmin25_2->Fill(var);
    if(dep == 2 && eta == -26) h12mlnetacoefmin26_2->Fill(var); 
    if(dep == 2 && eta == -27) h12mlnetacoefmin27_2->Fill(var); 
    if(dep == 2 && eta == -28) h12mlnetacoefmin28_2->Fill(var);
    if(dep == 2 && eta == -29) h12mlnetacoefmin29_2->Fill(var); 
   
   } // subd = 2
 if( subd == 4) {  
      
    if(dep == 1 && eta == -30) h12mlnetacoefmin30_1->Fill(var); 
    if(dep == 1 && eta == -31) h12mlnetacoefmin31_1->Fill(var);
    if(dep == 1 && eta == -32) h12mlnetacoefmin32_1->Fill(var);
    if(dep == 1 && eta == -33) h12mlnetacoefmin33_1->Fill(var); 
    if(dep == 1 && eta == -34) h12mlnetacoefmin34_1->Fill(var);
    if(dep == 1 && eta == -35) h12mlnetacoefmin35_1->Fill(var);
    if(dep == 1 && eta == -36) h12mlnetacoefmin36_1->Fill(var); 
    if(dep == 1 && eta == -37) h12mlnetacoefmin37_1->Fill(var);
    if(dep == 1 && eta == -38) h12mlnetacoefmin38_1->Fill(var);
    if(dep == 1 && eta == -39) h12mlnetacoefmin39_1->Fill(var);
    if(dep == 1 && eta == -40) h12mlnetacoefmin40_1->Fill(var);
    if(dep == 1 && eta == -41) h12mlnetacoefmin41_1->Fill(var);   
    if(dep == 2 && eta == -30) h12mlnetacoefmin30_2->Fill(var); 
    if(dep == 2 && eta == -31) h12mlnetacoefmin31_2->Fill(var);
    if(dep == 2 && eta == -32) h12mlnetacoefmin32_2->Fill(var);
    if(dep == 2 && eta == -33) h12mlnetacoefmin33_2->Fill(var); 
    if(dep == 2 && eta == -34) h12mlnetacoefmin34_2->Fill(var);
    if(dep == 2 && eta == -35) h12mlnetacoefmin35_2->Fill(var);
    if(dep == 2 && eta == -36) h12mlnetacoefmin36_2->Fill(var); 
    if(dep == 2 && eta == -37) h12mlnetacoefmin37_2->Fill(var);
    if(dep == 2 && eta == -38) h12mlnetacoefmin38_2->Fill(var);
    if(dep == 2 && eta == -39) h12mlnetacoefmin39_2->Fill(var);
    if(dep == 2 && eta == -40) h12mlnetacoefmin40_2->Fill(var);
    if(dep == 2 && eta == -41) h12mlnetacoefmin41_2->Fill(var);   
    
  } // subd = 2 
}

h12mlnetacoefmin1->Draw();
TFile efile("coefficients_minus_10mln_check.root","recreate");

h12mlnetacoefmin1->Write();
h12mlnetacoefmin2->Write();
h12mlnetacoefmin3->Write();
h12mlnetacoefmin4->Write();
h12mlnetacoefmin5->Write();
h12mlnetacoefmin6->Write();
h12mlnetacoefmin7->Write();
h12mlnetacoefmin8->Write();
h12mlnetacoefmin9->Write();
h12mlnetacoefmin10->Write();
h12mlnetacoefmin11->Write();
h12mlnetacoefmin12->Write();
h12mlnetacoefmin13->Write();
h12mlnetacoefmin14->Write();
h12mlnetacoefmin15->Write();
h12mlnetacoefmin16_1->Write();
h12mlnetacoefmin16_2->Write();
h12mlnetacoefmin16_3->Write();

h12mlnetacoefmin17_1->Write();
h12mlnetacoefmin18_1->Write();
h12mlnetacoefmin19_1->Write();
h12mlnetacoefmin20_1->Write();
h12mlnetacoefmin21_1->Write();
h12mlnetacoefmin22_1->Write();
h12mlnetacoefmin23_1->Write();
h12mlnetacoefmin24_1->Write();
h12mlnetacoefmin25_1->Write();
h12mlnetacoefmin26_1->Write();
h12mlnetacoefmin27_1->Write();
h12mlnetacoefmin28_1->Write();
h12mlnetacoefmin29_1->Write();
h12mlnetacoefmin30_1->Write();
h12mlnetacoefmin31_1->Write();
h12mlnetacoefmin32_1->Write();
h12mlnetacoefmin33_1->Write();
h12mlnetacoefmin34_1->Write();
h12mlnetacoefmin35_1->Write();
h12mlnetacoefmin36_1->Write();
h12mlnetacoefmin37_1->Write();
h12mlnetacoefmin38_1->Write(); 
h12mlnetacoefmin39_1->Write();
h12mlnetacoefmin40_1->Write();
h12mlnetacoefmin41_1->Write();

h12mlnetacoefmin17_2->Write();
h12mlnetacoefmin18_2->Write();
h12mlnetacoefmin19_2->Write();
h12mlnetacoefmin20_2->Write();
h12mlnetacoefmin21_2->Write();
h12mlnetacoefmin22_2->Write();
h12mlnetacoefmin23_2->Write();
h12mlnetacoefmin24_2->Write();
h12mlnetacoefmin25_2->Write();
h12mlnetacoefmin26_2->Write();
h12mlnetacoefmin27_2->Write();
h12mlnetacoefmin28_2->Write();
h12mlnetacoefmin29_2->Write();
h12mlnetacoefmin30_2->Write();
h12mlnetacoefmin31_2->Write();
h12mlnetacoefmin32_2->Write();
h12mlnetacoefmin33_2->Write();
h12mlnetacoefmin34_2->Write();
h12mlnetacoefmin35_2->Write();
h12mlnetacoefmin36_2->Write();
h12mlnetacoefmin37_2->Write();
h12mlnetacoefmin38_2->Write();
h12mlnetacoefmin39_2->Write();
h12mlnetacoefmin40_2->Write();
h12mlnetacoefmin41_2->Write();

}
