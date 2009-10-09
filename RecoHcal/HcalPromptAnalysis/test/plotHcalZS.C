{

gROOT->Reset();

TFile f("HcalZS_111265_ZSMask0.root");
//TFile f("HcalZS_111266_111268_ZSMask110000111.root");
//TFile f("HcalZS_111272_ZSMask100000011.root");
//TFile f("HcalZS_test.root");
//TFile f("HcalZS_73751.root");
//TFile f("HcalZS_73749.root");

f.ls();
f.cd("hcalZS/Hcal");

TCanvas c1;
c1.Divide(2,3);

//h_hcal_hbhfd1
c1.cd(1);
c1_1.SetGridx();
c1_1.SetGridy();
//c1_1.SetLogz();

for(int ieta = 1 ; ieta <= h_hcal_hbhfd1->GetNbinsX() ; ieta++)
  for(int iphi = 1 ; iphi <= h_hcal_hbhfd1->GetNbinsY() ; iphi++)
{
  int thisNtotEvents = int(h_hcal_hbhfd1_all->GetBinContent(ieta,iphi));
  if( thisNtotEvents == 0 ) 
    h_hcal_hbhfd1->SetBinContent( ieta , iphi , 0. );
  else
    {
      float numDisagree = thisNtotEvents - h_hcal_hbhfd1->GetBinContent(ieta,iphi);
      h_hcal_hbhfd1->SetBinContent( ieta , iphi , numDisagree);

      if(numDisagree > 0)
	{
	  cout << "%%%%%%%%%" << endl;
	  int det_eta = ieta - 42 ; 
	  int det_phi = iphi; 
	  printf("hbhfd1: ieta=%i  iphi=%i  totEvents=%i  numDisagree=%i\n", det_eta, det_phi, int(thisNtotEvents), int(numDisagree));
	}

    }
}

h_hcal_hbhfd1->Draw("colz");
h_hcal_hbhfd1->SetStats(0);

//h_hcal_hbhfd2
c1.cd(2);
c1_2.SetGridx();
c1_2.SetGridy();
//c1_2.SetLogz();

for(int ieta = 1 ; ieta <= h_hcal_hbhfd2->GetNbinsX() ; ieta++)
  for(int iphi = 1 ; iphi <= h_hcal_hbhfd2->GetNbinsY() ; iphi++)
{
  int thisNtotEvents = int(h_hcal_hbhfd2_all->GetBinContent(ieta,iphi));
  if( thisNtotEvents == 0 ) 
    h_hcal_hbhfd2->SetBinContent( ieta , iphi , 0. );
  else
    {
      float numDisagree = thisNtotEvents - h_hcal_hbhfd2->GetBinContent(ieta,iphi);
      h_hcal_hbhfd2->SetBinContent( ieta , iphi , numDisagree);

      if(numDisagree > 0)
	{
	  cout << "%%%%%%%%%" << endl;
	  int det_eta = ieta - 42 ; 
	  int det_phi = iphi; 
	  printf("hbhfd2: ieta=%i  iphi=%i  totEvents=%i  numDisagree=%i\n", det_eta, det_phi, int(thisNtotEvents), int(numDisagree));
	}

    }
}

h_hcal_hbhfd2->Draw("colz");
h_hcal_hbhfd2->SetStats(0);

//h_hcal_hed1
c1.cd(3);
c1_3.SetGridx();
c1_3.SetGridy();
//c1_3.SetLogz();

for(int ieta = 1 ; ieta <= h_hcal_hed1->GetNbinsX() ; ieta++)
  for(int iphi = 1 ; iphi <= h_hcal_hed1->GetNbinsY() ; iphi++)
{
  int thisNtotEvents = int(h_hcal_hed1_all->GetBinContent(ieta,iphi));
  if( thisNtotEvents == 0 ) 
    h_hcal_hed1->SetBinContent( ieta , iphi , 0. );
  else
    {
      float numDisagree = thisNtotEvents - h_hcal_hed1->GetBinContent(ieta,iphi);
      h_hcal_hed1->SetBinContent( ieta , iphi , numDisagree);

      if(numDisagree > 0)
	{
	  cout << "%%%%%%%%%" << endl;
	  int det_eta = ieta - 42 ; 
	  int det_phi = iphi; 
	  printf("hed1: ieta=%i  iphi=%i  totEvents=%i  numDisagree=%i\n", det_eta, det_phi, int(thisNtotEvents), int(numDisagree));
	}

    }
}

h_hcal_hed1->Draw("colz");
h_hcal_hed1->SetStats(0);

//h_hcal_hed2
c1.cd(4);
c1_4.SetGridx();
c1_4.SetGridy();
//c1_4.SetLogz();

//h_hcal_hed2
for(int ieta = 1 ; ieta <= h_hcal_hed2->GetNbinsX() ; ieta++)
  for(int iphi = 1 ; iphi <= h_hcal_hed2->GetNbinsY() ; iphi++)
{
  int thisNtotEvents = int(h_hcal_hed2_all->GetBinContent(ieta,iphi));
  if( thisNtotEvents == 0 ) 
    h_hcal_hed2->SetBinContent( ieta , iphi , 0. );
  else
    {
      float numDisagree = thisNtotEvents - h_hcal_hed2->GetBinContent(ieta,iphi);
      h_hcal_hed2->SetBinContent( ieta , iphi , numDisagree);

      if(numDisagree > 0)
	{
	  cout << "%%%%%%%%%" << endl;
	  int det_eta = ieta - 42 ; 
	  int det_phi = iphi; 
	  printf("hed2: ieta=%i  iphi=%i  totEvents=%i  numDisagree=%i\n", det_eta, det_phi, int(thisNtotEvents), int(numDisagree));
	}

    }
}

h_hcal_hed2->Draw("colz");
h_hcal_hed2->SetStats(0);

//h_hcal_hed3
c1.cd(5);
c1_5.SetGridx();
c1_5.SetGridy();
//c1_5.SetLogz();

//h_hcal_hed3
for(int ieta = 1 ; ieta <= h_hcal_hed3->GetNbinsX() ; ieta++)
  for(int iphi = 1 ; iphi <= h_hcal_hed3->GetNbinsY() ; iphi++)
{
  int thisNtotEvents = int(h_hcal_hed3_all->GetBinContent(ieta,iphi));
  if( thisNtotEvents == 0 ) 
    h_hcal_hed3->SetBinContent( ieta , iphi , 0. );
  else
    {
      float numDisagree = thisNtotEvents - h_hcal_hed3->GetBinContent(ieta,iphi);
      h_hcal_hed3->SetBinContent( ieta , iphi , numDisagree);

      if(numDisagree > 0)
	{
	  cout << "%%%%%%%%%" << endl;
	  int det_eta = ieta - 42 ; 
	  int det_phi = iphi; 
	  printf("hed3: ieta=%i  iphi=%i  totEvents=%i  numDisagree=%i\n", det_eta, det_phi, int(thisNtotEvents), int(numDisagree));
	}

    }
}

h_hcal_hed3->Draw("colz");
h_hcal_hed3->SetStats(0);

//h_hcal_hod4
c1.cd(6);
c1_6.SetGridx();
c1_6.SetGridy();
//c1_6.SetLogz();

//h_hcal_hod4
for(int ieta = 1 ; ieta <= h_hcal_hod4->GetNbinsX() ; ieta++)
  for(int iphi = 1 ; iphi <= h_hcal_hod4->GetNbinsY() ; iphi++)
{
  int thisNtotEvents = int(h_hcal_hod4_all->GetBinContent(ieta,iphi));
  if( thisNtotEvents == 0 ) 
    h_hcal_hod4->SetBinContent( ieta , iphi , 0. );
  else
    {
      float numDisagree = thisNtotEvents - h_hcal_hod4->GetBinContent(ieta,iphi);
      h_hcal_hod4->SetBinContent( ieta , iphi , numDisagree);

      if(numDisagree > 0)
	{
	  cout << "%%%%%%%%%" << endl;
	  int det_eta = ieta - 42 ; 
	  int det_phi = iphi; 
	  printf("hod4: ieta=%i  iphi=%i  totEvents=%i  numDisagree=%i\n", det_eta, det_phi, int(thisNtotEvents), int(numDisagree));
	}

    }
}


h_hcal_hod4->Draw("colz");
h_hcal_hod4->SetStats(0);


TCanvas c2;
h_ZSagree->Draw();
h_ZSagree->SetTitle("Agreement between M&P bit and ZS emulation");


TCanvas c3;
c3.Divide(2,3);

c3.cd(1);
h_hcal_hbhfd1_all->Draw("colz");
h_hcal_hbhfd1_all->SetStats(0);

c3.cd(2);
h_hcal_hbhfd2_all->Draw("colz");
h_hcal_hbhfd2_all->SetStats(0);

c3.cd(3);
h_hcal_hed1_all->Draw("colz");
h_hcal_hed1_all->SetStats(0);

c3.cd(4);
h_hcal_hed2_all->Draw("colz");
h_hcal_hed2_all->SetStats(0);

c3.cd(5);
h_hcal_hed3_all->Draw("colz");
h_hcal_hed3_all->SetStats(0);

c3.cd(6);
h_hcal_hod4_all->Draw("colz");
h_hcal_hod4_all->SetStats(0);


}
