{
   //======================================================================
      printf("reco: gROOT Reset \n");
        gROOT->Reset();
        gROOT->SetStyle("Plain");
				gStyle->SetOptStat(0);   //  no statistics _or_
	//	        	  gStyle->SetOptStat(11111111);
	//gStyle->SetOptStat(1101);// mame mean and rms 
	//	gStyle->SetOptStat(0101);// name and entries
	//	   gStyle->SetOptStat(1100);// mean and rms only !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//				gStyle->SetOptStat(1110000);// und over, integral !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//	gStyle->SetOptStat(101110);                                          // entries, meam, rms, overflow !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//
	//gStyle->SetOptFit(00010);// constant, mean and sigma only !!
		//	gStyle->SetOptFit(00001);// hi2/nu, constant, mean and sigma only !!
		gStyle->SetOptFit(0010);// constant, mean and sigma only !!
	//	gStyle->SetOptFit(00011);// constant, mean and sigma only !!
	// gStyle->SetOptFit(1101);
		//	   gStyle->SetOptFit(1011);
	//
	//gStyle->SetStatX(0.98);
	//gStyle->SetStatY(0.99);
	//gStyle->SetStatW(0.30);
	//gStyle->SetStatH(0.25);
	gStyle->SetStatX(0.91);
	gStyle->SetStatY(0.75);
	gStyle->SetStatW(0.20);
	gStyle->SetStatH(0.10);
	// gStyle->SetStatH(0.35);
        //

	//        Float_t LeftOffset = 0.12;
        //Float_t TopOffset = 0.12;
        Float_t LeftOffset = 0.12;
        Float_t TopOffset = 0.22;

        gStyle->SetLineWidth(1);
        gStyle->SetErrorX(0);

//---=[ Titles,Labels ]=-----------
        gStyle->SetOptTitle(0);             // title on/off
        //      gStyle->SetTitleColor(0);           // title color
        gStyle->SetTitleColor(1);           // title color
        //      gStyle->SetTitleX(0.35);            // title x-position
        gStyle->SetTitleX(0.15);            // title x-position
        gStyle->SetTitleH(0.15);             // title height
        //      gStyle->SetTitleW(0.53);            // title width
        gStyle->SetTitleW(0.60);            // title width
        gStyle->SetTitleFont(42);           // title font
        gStyle->SetTitleFontSize(0.07);     // title font size

	gStyle->SetPalette(1);
//---=[ Histogram style ]=----------
        //        gStyle->SetHistFillColor(38);
	//	        gStyle->SetFrameFillColor(41);// jelto-kori4nev-svetl
	//	gStyle->SetFrameFillColor(5);// jeltyi
	//	gStyle->SetFrameFillColor(17);// seryi
	//	gStyle->SetFrameFillColor(18);// svetlo seryi
	//	gStyle->SetFrameFillColor(20);// svetlo kori4nev
	//        gStyle->SetFrameFillColor(33);// sine-seryi
	//	gStyle->SetFrameFillColor(40);// fiolet-seryi
	//	gStyle->SetFrameFillColor(23);// sv.kor

//---=[ Pad style ]=----------------
        gStyle->SetPadTopMargin(TopOffset);
        gStyle->SetPadBottomMargin(LeftOffset);
        gStyle->SetPadRightMargin(TopOffset);
        gStyle->SetPadLeftMargin(LeftOffset);
//---=[ SetCanvasDef           ]=----------------
    //======================================================================
//
// Connect the input file and get the 2-d histogram in memory
    //======================================================================
//  TBrowser *b = new TBrowser

//	TFile *hfile1= new TFile("BadRBXrun211146.root", "READ");        
//	TFile *hfile1= new TFile("test.root", "READ");        
	TFile *hfile1= new TFile("test8runs.root", "READ");        

	//    getchar();
	//
          TPostScript psfile ("zcalib.ps", 111);
//

   c1 = new TCanvas("c1","Hcal4test",200,10,700,900);


    //=============================================================================================== 1
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapRatioCalib_HB");
      TH2F *twod0= (TH2F*)hfile1->Get("h_map_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapRatioCalib047_HB");
      TH2F *twod0= (TH2F*)hfile1->Get("h_map_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Cefz225= (TH2F*)twod1->Clone("Cefz225");
	Cefz225->Divide(twod1,twod0, 1, 1, "B");
	Cefz225->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Cefz225->SetMarkerStyle(20);
      Cefz225->SetMarkerSize(0.4);
      //    Cefz225->GetYaxis()->SetLabelSize(0.04);
      Cefz225->GetZaxis()->SetLabelSize(0.08);
      Cefz225->SetXTitle("#eta \b");
      Cefz225->SetYTitle("#phi \b");
          Cefz225->SetZTitle("Rate  HB \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_RatioCalib_HB");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("RatioCalib in each event & cell HB\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->SetMinimum(0.8);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_HB = (TH2F*)Ceff->Clone("Diffe_HB");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_HB->SetBinContent(i,j,0.);
	  if(ccc1 < 0.7 )  Diffe_HB->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_HB->SetMarkerStyle(20);
      Diffe_HB->SetMarkerSize(0.4);
      Diffe_HB->GetZaxis()->SetLabelSize(0.08);
      Diffe_HB->SetXTitle("#eta \b");
      Diffe_HB->SetYTitle("#phi \b");
      Diffe_HB->SetZTitle("<RatioCalib> smaller 0.7 HB \b");
      Diffe_HB->SetMarkerColor(2);
      Diffe_HB->SetLineColor(2);
      Diffe_HB->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_HB = new TH1F("diffAmplitude_HB","", 100, 0., 1.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_HB->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_HB->SetMarkerStyle(20);
      diffAmplitude_HB->SetMarkerSize(0.4);
      diffAmplitude_HB->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_HB->SetXTitle("<RatioCalib> in each cell HB\b");
      diffAmplitude_HB->SetMarkerColor(2);
      diffAmplitude_HB->SetLineColor(2);
      diffAmplitude_HB->SetMinimum(0.8);
      diffAmplitude_HB->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 2
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapRatioCalib_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_map_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapRatioCalib047_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_map_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Cefz225= (TH2F*)twod1->Clone("Cefz225");
	Cefz225->Divide(twod1,twod0, 1, 1, "B");
	Cefz225->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Cefz225->SetMarkerStyle(20);
      Cefz225->SetMarkerSize(0.4);
      //    Cefz225->GetYaxis()->SetLabelSize(0.04);
      Cefz225->GetZaxis()->SetLabelSize(0.08);
      Cefz225->SetXTitle("#eta \b");
      Cefz225->SetYTitle("#phi \b");
      Cefz225->SetZTitle("Rate HE \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_RatioCalib_HE");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("RatioCalib in each event & cell HE\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->SetMinimum(0.8);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_HE = (TH2F*)Ceff->Clone("Diffe_HE");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_HE->SetBinContent(i,j,0.);
	  if(ccc1 < 0.7 )  Diffe_HE->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_HE->SetMarkerStyle(20);
      Diffe_HE->SetMarkerSize(0.4);
      Diffe_HE->GetZaxis()->SetLabelSize(0.08);
      Diffe_HE->SetXTitle("#eta \b");
      Diffe_HE->SetYTitle("#phi \b");
      Diffe_HE->SetZTitle("<RatioCalib> smaller 0.7 HE \b");
      Diffe_HE->SetMarkerColor(2);
      Diffe_HE->SetLineColor(2);
      Diffe_HE->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_HE = new TH1F("diffAmplitude_HE","", 100, 0., 1.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_HE->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_HE->SetMarkerStyle(20);
      diffAmplitude_HE->SetMarkerSize(0.4);
      diffAmplitude_HE->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_HE->SetXTitle("<RatioCalib> in each cell HE\b");
      diffAmplitude_HE->SetMarkerColor(2);
      diffAmplitude_HE->SetLineColor(2);
      diffAmplitude_HE->SetMinimum(0.8);
      diffAmplitude_HE->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 3
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapRatioCalib_HO");
      TH2F *twod0= (TH2F*)hfile1->Get("h_map_HO");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapRatioCalib047_HO");
      TH2F *twod0= (TH2F*)hfile1->Get("h_map_HO");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Cefz225= (TH2F*)twod1->Clone("Cefz225");
	Cefz225->Divide(twod1,twod0, 1, 1, "B");
	Cefz225->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Cefz225->SetMarkerStyle(20);
      Cefz225->SetMarkerSize(0.4);
      //    Cefz225->GetYaxis()->SetLabelSize(0.04);
      Cefz225->GetZaxis()->SetLabelSize(0.08);
      Cefz225->SetXTitle("#eta \b");
      Cefz225->SetYTitle("#phi \b");
      Cefz225->SetZTitle("Rate HO \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_RatioCalib_HO");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("RatioCalib in each event & cell HO\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->SetMinimum(0.8);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_HO = (TH2F*)Ceff->Clone("Diffe_HO");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_HO->SetBinContent(i,j,0.);
	  if(ccc1 < 0.25)  Diffe_HO->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_HO->SetMarkerStyle(20);
      Diffe_HO->SetMarkerSize(0.4);
      Diffe_HO->GetZaxis()->SetLabelSize(0.08);
      Diffe_HO->SetXTitle("#eta \b");
      Diffe_HO->SetYTitle("#phi \b");
      Diffe_HO->SetZTitle("<RatioCalib> smaller 0.25 HO \b");
      Diffe_HO->SetMarkerColor(2);
      Diffe_HO->SetLineColor(2);
      Diffe_HO->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_HO = new TH1F("diffAmplitude_HO","", 100, 0., 1.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_HO->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_HO->SetMarkerStyle(20);
      diffAmplitude_HO->SetMarkerSize(0.4);
      diffAmplitude_HO->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_HO->SetXTitle("<RatioCalib> in each cell \b");
      diffAmplitude_HO->SetMarkerColor(2);
      diffAmplitude_HO->SetLineColor(2);
      diffAmplitude_HO->SetMinimum(0.8);
      diffAmplitude_HO->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 4
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapRatioCalib_HF");
      TH2F *twod0= (TH2F*)hfile1->Get("h_map_HF");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapRatioCalib047_HF");
      TH2F *twod0= (TH2F*)hfile1->Get("h_map_HF");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Cefz225= (TH2F*)twod1->Clone("Cefz225");
	Cefz225->Divide(twod1,twod0, 1, 1, "B");
	Cefz225->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Cefz225->SetMarkerStyle(20);
      Cefz225->SetMarkerSize(0.4);
      //    Cefz225->GetYaxis()->SetLabelSize(0.04);
      Cefz225->GetZaxis()->SetLabelSize(0.08);
      Cefz225->SetXTitle("#eta \b");
      Cefz225->SetYTitle("#phi \b");
      Cefz225->SetZTitle("Rate HF \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_RatioCalib_HF");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("RatioCalib in each event & cell HF\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->SetMinimum(0.8);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_HF = (TH2F*)Ceff->Clone("Diffe_HF");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_HF->SetBinContent(i,j,0.);
	  if(ccc1 < 0.40)  Diffe_HF->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_HF->SetMarkerStyle(20);
      Diffe_HF->SetMarkerSize(0.4);
      Diffe_HF->GetZaxis()->SetLabelSize(0.08);
      Diffe_HF->SetXTitle("#eta \b");
      Diffe_HF->SetYTitle("#phi \b");
      Diffe_HF->SetZTitle("<RatioCalib> smaller 0.40 HF \b");
      Diffe_HF->SetMarkerColor(2);
      Diffe_HF->SetLineColor(2);
      Diffe_HF->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_HF = new TH1F("diffAmplitude_HF","", 100, 0., 1.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_HF->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_HF->SetMarkerStyle(20);
      diffAmplitude_HF->SetMarkerSize(0.4);
      diffAmplitude_HF->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_HF->SetXTitle("<RatioCalib> in each cell \b");
      diffAmplitude_HF->SetMarkerColor(2);
      diffAmplitude_HF->SetLineColor(2);
      diffAmplitude_HF->SetMinimum(0.8);
      diffAmplitude_HF->Draw("");
      
      c1->Update();
      
      
            
    //======================================================================
    //==================================================================================================== end
    //======================================================================
    //======================================================================
    // close and delete all possible things:

    psfile->Close();

//    hfile0->Close();
//    hfile0->Delete();
    hfile1->Close();
    hfile1->Delete();

        //  Exit Root
        gSystem->Exit(0);
    //======================================================================

}
