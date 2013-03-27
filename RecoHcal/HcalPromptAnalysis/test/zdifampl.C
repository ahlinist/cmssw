{
   //======================================================================
      printf("reco: gROOT Reset \n");
        gROOT->Reset();
        gROOT->SetStyle("Plain");
				gStyle->SetOptStat(0);   //  no statistics _or_
	//		   gStyle->SetOptStat(1100);// mean and rms only !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

//	TFile *hfile1= new TFile("analysisUSC211146.root", "READ");        
//	TFile *hfile2= new TFile("analysisUSC211154.root", "READ");        

//	TFile *hfile1= new TFile("BadRBXrun211146.root", "READ");        
//	TFile *hfile2= new TFile("BadRBXrun211154.root", "READ");        

//	TFile *hfile1= new TFile("BadRBXrun187927.root", "READ");        
//	TFile *hfile1= new TFile("BadRBXrun207167.root", "READ");        
//	TFile *hfile2= new TFile("BadRBXrun211146.root", "READ");        
//
//	TFile *hfile1= new TFile("test191392.root", "READ");        
//	TFile *hfile2= new TFile("test193001.root", "READ");        

// short runs:
////	TFile *hfile1= new TFile("test191392.root", "READ");        
////	TFile *hfile2= new TFile("test193001.root", "READ");        

// long runs:
// pair 1
//	TFile *hfile1= new TFile("test195682.root", "READ");        
//	TFile *hfile2= new TFile("test196870.root", "READ");        

// pair 2
	TFile *hfile1= new TFile("test201305.root", "READ");        
	TFile *hfile2= new TFile("test202341.root", "READ");

// pair 3
//	TFile *hfile1= new TFile("test202341.root", "READ");        
//	TFile *hfile2= new TFile("test203339.root", "READ");

// pair 4
//	TFile *hfile1= new TFile("test203339.root", "READ");        
//	TFile *hfile2= new TFile("test204172.root", "READ");

// pair 5
//	TFile *hfile1= new TFile("test204172.root", "READ");        
//	TFile *hfile2= new TFile("test205355.root", "READ");

// pair 6
//	TFile *hfile1= new TFile("test205355.root", "READ");        
//	TFile *hfile2= new TFile("test206456.root", "READ");



	//    getchar();
	//
          TPostScript psfile ("zdifampl.ps", 111);
//

   c1 = new TCanvas("c1","Hcal4test",200,10,700,900);


    //============================================================================================= 1
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1AmplE34_HB");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth1_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Ceff->SetMarkerStyle(20);
      Ceff->SetMarkerSize(0.4);
      Ceff->GetYaxis()->SetLabelSize(0.04);
      //    Ceff->SetTitle("any Error, HB Depth1 \n");
      Ceff->SetXTitle("#eta");
      Ceff->SetYTitle("#phi");
      Ceff->SetZTitle("Signal for 1st Run - HB Depth1");
      Ceff->SetMarkerColor(2);
      Ceff->SetLineColor(2);
      Ceff->Draw("COLZ");
      
      c1->cd(2);
      TH2F *tqod1= (TH2F*)hfile2->Get("h_mapDepth1AmplE34_HB");
      TH2F *tqod0= (TH2F*)hfile2->Get("h_mapDepth1_HB");
      tqod1->Sumw2();
      tqod0->Sumw2();
      if(tqod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Czeff = (TH2F*)tqod1->Clone("Czeff");
	Czeff->Divide(tqod1,tqod0, 1, 1, "B");
	Czeff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Czeff->SetMarkerStyle(20);
      Czeff->SetMarkerSize(0.4);
      Czeff->GetYaxis()->SetLabelSize(0.04);
      //    Czeff->SetTitle("any Error, HB Depth1 \n");
      Czeff->SetXTitle("#eta");
      Czeff->SetYTitle("#phi");
      Czeff->SetZTitle("Signal for 2nd Run - HB Depth1");
      Czeff->SetMarkerColor(2);
      Czeff->SetLineColor(2);
      Czeff->Draw("COLZ");
      
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth1_HB = (TH2F*)Ceff->Clone("Diffe_Depth1_HB");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	Diffe_Depth1_HB->SetBinContent(i,j,0.);
	if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	  ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	  if(fabs(ccc1) > 2.)  Diffe_Depth1_HB->SetBinContent(i,j,fabs(ccc1));
	}
      }
    }
    gPad->SetGridy();
    gPad->SetGridx();
    gPad->SetLogz();
    Diffe_Depth1_HB->SetMarkerStyle(20);
    Diffe_Depth1_HB->SetMarkerSize(0.4);
    Diffe_Depth1_HB->GetYaxis()->SetLabelSize(0.04);
    //    Diffe_Depth1_HB->SetTitle("any Error, HB Depth1 \n");
    Diffe_Depth1_HB->SetXTitle("#eta");
    Diffe_Depth1_HB->SetYTitle("#phi");
    Diffe_Depth1_HB->SetZTitle("Relative Difference in % - Depth1_HB");
    Diffe_Depth1_HB->SetMarkerColor(2);
    Diffe_Depth1_HB->SetLineColor(2);
    Diffe_Depth1_HB->Draw("COLZ");
    
    
      c1->cd(4);
      TH1F* diffAmpl_Depth1_HB = new TH1F("diffAmpl_Depth1_HB","", 100, -10., 10.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	    ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	    diffAmpl_Depth1_HB->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmpl_Depth1_HB->SetMarkerStyle(20);
      diffAmpl_Depth1_HB->SetMarkerSize(0.4);
      diffAmpl_Depth1_HB->GetYaxis()->SetLabelSize(0.04);
      diffAmpl_Depth1_HB->SetTitle("AmplRunsDifference Depth1_HB \n");
      diffAmpl_Depth1_HB->SetXTitle("Relative Difference, % - Depth1_HB");
      diffAmpl_Depth1_HB->SetMarkerColor(2);
      diffAmpl_Depth1_HB->SetLineColor(2);
      diffAmpl_Depth1_HB->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 2
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2AmplE34_HB");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth2_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Ceff->SetMarkerStyle(20);
      Ceff->SetMarkerSize(0.4);
      Ceff->GetYaxis()->SetLabelSize(0.04);
      //    Ceff->SetTitle("any Error, HB Depth2 \n");
      Ceff->SetXTitle("#eta");
      Ceff->SetYTitle("#phi");
      Ceff->SetZTitle("Signal for 1st Run - HB Depth2");
      Ceff->SetMarkerColor(2);
      Ceff->SetLineColor(2);
      Ceff->Draw("COLZ");
      
      c1->cd(2);
      TH2F *tqod1= (TH2F*)hfile2->Get("h_mapDepth2AmplE34_HB");
      TH2F *tqod0= (TH2F*)hfile2->Get("h_mapDepth2_HB");
      tqod1->Sumw2();
      tqod0->Sumw2();
      if(tqod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Czeff = (TH2F*)tqod1->Clone("Czeff");
	Czeff->Divide(tqod1,tqod0, 1, 1, "B");
	Czeff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Czeff->SetMarkerStyle(20);
      Czeff->SetMarkerSize(0.4);
      Czeff->GetYaxis()->SetLabelSize(0.04);
      //    Czeff->SetTitle("any Error, HB Depth2 \n");
      Czeff->SetXTitle("#eta");
      Czeff->SetYTitle("#phi");
      Czeff->SetZTitle("Signal for 2nd Run - HB Depth2");
      Czeff->SetMarkerColor(2);
      Czeff->SetLineColor(2);
      Czeff->Draw("COLZ");
      
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth2_HB = (TH2F*)Ceff->Clone("Diffe_Depth2_HB");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	Diffe_Depth2_HB->SetBinContent(i,j,0.);
	if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	  ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	  if(fabs(ccc1) > 2. )  Diffe_Depth2_HB->SetBinContent(i,j,fabs(ccc1));
	}
      }
    }
    gPad->SetGridy();
    gPad->SetGridx();
    gPad->SetLogz();
    Diffe_Depth2_HB->SetMarkerStyle(20);
    Diffe_Depth2_HB->SetMarkerSize(0.4);
    Diffe_Depth2_HB->GetYaxis()->SetLabelSize(0.04);
    //    Diffe_Depth2_HB->SetTitle("any Error, HB Depth2 \n");
    Diffe_Depth2_HB->SetXTitle("#eta");
    Diffe_Depth2_HB->SetYTitle("#phi");
    Diffe_Depth2_HB->SetZTitle("Relative Difference in % - Depth2_HB");
    Diffe_Depth2_HB->SetMarkerColor(2);
    Diffe_Depth2_HB->SetLineColor(2);
    Diffe_Depth2_HB->Draw("COLZ");
    

      c1->cd(4);
      TH1F* diffAmpl_Depth2_HB = new TH1F("diffAmpl_Depth2_HB","", 100, -10., 10.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	    ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	    diffAmpl_Depth2_HB->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmpl_Depth2_HB->SetMarkerStyle(20);
      diffAmpl_Depth2_HB->SetMarkerSize(0.4);
      diffAmpl_Depth2_HB->GetYaxis()->SetLabelSize(0.04);
      diffAmpl_Depth2_HB->SetTitle("AmplRunsDifference Depth2_HB \n");
      diffAmpl_Depth2_HB->SetXTitle("Relative Difference, % - Depth2_HB");
      diffAmpl_Depth2_HB->SetMarkerColor(2);
      diffAmpl_Depth2_HB->SetLineColor(2);
      diffAmpl_Depth2_HB->Draw("");
      
      c1->Update();
      
      
            
    //==================================================================================================== 3
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1AmplE34_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth1_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Ceff->SetMarkerStyle(20);
      Ceff->SetMarkerSize(0.4);
      Ceff->GetYaxis()->SetLabelSize(0.04);
      //    Ceff->SetTitle("any Error, HE Depth1 \n");
      Ceff->SetXTitle("#eta");
      Ceff->SetYTitle("#phi");
      Ceff->SetZTitle("Signal for 1st Run - HE Depth1");
      Ceff->SetMarkerColor(2);
      Ceff->SetLineColor(2);
      Ceff->Draw("COLZ");
      
      c1->cd(2);
      TH2F *tqod1= (TH2F*)hfile2->Get("h_mapDepth1AmplE34_HE");
      TH2F *tqod0= (TH2F*)hfile2->Get("h_mapDepth1_HE");
      tqod1->Sumw2();
      tqod0->Sumw2();
      if(tqod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Czeff = (TH2F*)tqod1->Clone("Czeff");
	Czeff->Divide(tqod1,tqod0, 1, 1, "B");
	Czeff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Czeff->SetMarkerStyle(20);
      Czeff->SetMarkerSize(0.4);
      Czeff->GetYaxis()->SetLabelSize(0.04);
      //    Czeff->SetTitle("any Error, HE Depth1 \n");
      Czeff->SetXTitle("#eta");
      Czeff->SetYTitle("#phi");
      Czeff->SetZTitle("Signal for 2nd Run - HE Depth1");
      Czeff->SetMarkerColor(2);
      Czeff->SetLineColor(2);
      Czeff->Draw("COLZ");
      
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth1_HE = (TH2F*)Ceff->Clone("Diffe_Depth1_HE");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	Diffe_Depth1_HE->SetBinContent(i,j,0.);
	if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	  ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	  if(fabs(ccc1) > 2. )  Diffe_Depth1_HE->SetBinContent(i,j,fabs(ccc1));
	}
      }
    }
    gPad->SetGridy();
    gPad->SetGridx();
    gPad->SetLogz();
    Diffe_Depth1_HE->SetMarkerStyle(20);
    Diffe_Depth1_HE->SetMarkerSize(0.4);
    Diffe_Depth1_HE->GetYaxis()->SetLabelSize(0.04);
    //    Diffe_Depth1_HE->SetTitle("any Error, HE Depth1 \n");
    Diffe_Depth1_HE->SetXTitle("#eta");
    Diffe_Depth1_HE->SetYTitle("#phi");
    Diffe_Depth1_HE->SetZTitle("Relative Difference in % - Depth1_HE");
    Diffe_Depth1_HE->SetMarkerColor(2);
    Diffe_Depth1_HE->SetLineColor(2);
    Diffe_Depth1_HE->Draw("COLZ");
    
    
      c1->cd(4);
      TH1F* diffAmpl_Depth1_HE = new TH1F("diffAmpl_Depth1_HE","", 100, -10., 10.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	    ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	    diffAmpl_Depth1_HE->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmpl_Depth1_HE->SetMarkerStyle(20);
      diffAmpl_Depth1_HE->SetMarkerSize(0.4);
      diffAmpl_Depth1_HE->GetYaxis()->SetLabelSize(0.04);
      diffAmpl_Depth1_HE->SetTitle("AmplRunsDifference Depth1_HE \n");
      diffAmpl_Depth1_HE->SetXTitle("Relative Difference, % - Depth1_HE");
      diffAmpl_Depth1_HE->SetMarkerColor(2);
      diffAmpl_Depth1_HE->SetLineColor(2);
      diffAmpl_Depth1_HE->Draw("");
      
      c1->Update();
      
      
            
    //==================================================================================================== 4
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2AmplE34_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth2_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Ceff->SetMarkerStyle(20);
      Ceff->SetMarkerSize(0.4);
      Ceff->GetYaxis()->SetLabelSize(0.04);
      //    Ceff->SetTitle("any Error, HE Depth2 \n");
      Ceff->SetXTitle("#eta");
      Ceff->SetYTitle("#phi");
      Ceff->SetZTitle("Signal for 1st Run - HE Depth2");
      Ceff->SetMarkerColor(2);
      Ceff->SetLineColor(2);
      Ceff->Draw("COLZ");
      
      c1->cd(2);
      TH2F *tqod1= (TH2F*)hfile2->Get("h_mapDepth2AmplE34_HE");
      TH2F *tqod0= (TH2F*)hfile2->Get("h_mapDepth2_HE");
      tqod1->Sumw2();
      tqod0->Sumw2();
      if(tqod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Czeff = (TH2F*)tqod1->Clone("Czeff");
	Czeff->Divide(tqod1,tqod0, 1, 1, "B");
	Czeff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Czeff->SetMarkerStyle(20);
      Czeff->SetMarkerSize(0.4);
      Czeff->GetYaxis()->SetLabelSize(0.04);
      //    Czeff->SetTitle("any Error, HE Depth2 \n");
      Czeff->SetXTitle("#eta");
      Czeff->SetYTitle("#phi");
      Czeff->SetZTitle("Signal for 2nd Run - HE Depth2");
      Czeff->SetMarkerColor(2);
      Czeff->SetLineColor(2);
      Czeff->Draw("COLZ");
      
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth2_HE = (TH2F*)Ceff->Clone("Diffe_Depth2_HE");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	Diffe_Depth2_HE->SetBinContent(i,j,0.);
	if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	  ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	  if(fabs(ccc1) > 2. )  Diffe_Depth2_HE->SetBinContent(i,j,fabs(ccc1));
	}
      }
    }
    gPad->SetGridy();
    gPad->SetGridx();
    gPad->SetLogz();
    Diffe_Depth2_HE->SetMarkerStyle(20);
    Diffe_Depth2_HE->SetMarkerSize(0.4);
    Diffe_Depth2_HE->GetYaxis()->SetLabelSize(0.04);
    //    Diffe_Depth2_HE->SetTitle("any Error, HE Depth2 \n");
    Diffe_Depth2_HE->SetXTitle("#eta");
    Diffe_Depth2_HE->SetYTitle("#phi");
    Diffe_Depth2_HE->SetZTitle("Relative Difference in % - Depth2_HE");
    Diffe_Depth2_HE->SetMarkerColor(2);
    Diffe_Depth2_HE->SetLineColor(2);
    Diffe_Depth2_HE->Draw("COLZ");
    

      c1->cd(4);
      TH1F* diffAmpl_Depth2_HE = new TH1F("diffAmpl_Depth2_HE","", 100, -10., 10.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	    ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	    diffAmpl_Depth2_HE->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmpl_Depth2_HE->SetMarkerStyle(20);
      diffAmpl_Depth2_HE->SetMarkerSize(0.4);
      diffAmpl_Depth2_HE->GetYaxis()->SetLabelSize(0.04);
      diffAmpl_Depth2_HE->SetTitle("AmplRunsDifference Depth2_HE \n");
      diffAmpl_Depth2_HE->SetXTitle("Relative Difference, % - Depth2_HE");
      diffAmpl_Depth2_HE->SetMarkerColor(2);
      diffAmpl_Depth2_HE->SetLineColor(2);
      diffAmpl_Depth2_HE->Draw("");
      
      c1->Update();
      
      
            
    //==================================================================================================== 5
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth3AmplE34_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth3_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Ceff->SetMarkerStyle(20);
      Ceff->SetMarkerSize(0.4);
      Ceff->GetYaxis()->SetLabelSize(0.04);
      //    Ceff->SetTitle("any Error, HE Depth3 \n");
      Ceff->SetXTitle("#eta");
      Ceff->SetYTitle("#phi");
      Ceff->SetZTitle("Signal for 1st Run - HE Depth3");
      Ceff->SetMarkerColor(2);
      Ceff->SetLineColor(2);
      Ceff->Draw("COLZ");
      
      c1->cd(2);
      TH2F *tqod1= (TH2F*)hfile2->Get("h_mapDepth3AmplE34_HE");
      TH2F *tqod0= (TH2F*)hfile2->Get("h_mapDepth3_HE");
      tqod1->Sumw2();
      tqod0->Sumw2();
      if(tqod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Czeff = (TH2F*)tqod1->Clone("Czeff");
	Czeff->Divide(tqod1,tqod0, 1, 1, "B");
	Czeff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Czeff->SetMarkerStyle(20);
      Czeff->SetMarkerSize(0.4);
      Czeff->GetYaxis()->SetLabelSize(0.04);
      //    Czeff->SetTitle("any Error, HE Depth3 \n");
      Czeff->SetXTitle("#eta");
      Czeff->SetYTitle("#phi");
      Czeff->SetZTitle("Signal for 2nd Run - HE Depth3");
      Czeff->SetMarkerColor(2);
      Czeff->SetLineColor(2);
      Czeff->Draw("COLZ");
      
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth3_HE = (TH2F*)Ceff->Clone("Diffe_Depth3_HE");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	Diffe_Depth3_HE->SetBinContent(i,j,0.);
	if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	  ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	  if(fabs(ccc1) > 2. )  Diffe_Depth3_HE->SetBinContent(i,j,fabs(ccc1));
	}
      }
    }
    gPad->SetGridy();
    gPad->SetGridx();
    gPad->SetLogz();
    Diffe_Depth3_HE->SetMarkerStyle(20);
    Diffe_Depth3_HE->SetMarkerSize(0.4);
    Diffe_Depth3_HE->GetYaxis()->SetLabelSize(0.04);
    //    Diffe_Depth3_HE->SetTitle("any Error, HE Depth3 \n");
    Diffe_Depth3_HE->SetXTitle("#eta");
    Diffe_Depth3_HE->SetYTitle("#phi");
    Diffe_Depth3_HE->SetZTitle("Relative Difference in % - Depth3_HE");
    Diffe_Depth3_HE->SetMarkerColor(2);
    Diffe_Depth3_HE->SetLineColor(2);
    Diffe_Depth3_HE->Draw("COLZ");
    

      c1->cd(4);
      TH1F* diffAmpl_Depth3_HE = new TH1F("diffAmpl_Depth3_HE","", 100, -10., 10.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	    ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	    diffAmpl_Depth3_HE->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmpl_Depth3_HE->SetMarkerStyle(20);
      diffAmpl_Depth3_HE->SetMarkerSize(0.4);
      diffAmpl_Depth3_HE->GetYaxis()->SetLabelSize(0.04);
      diffAmpl_Depth3_HE->SetTitle("AmplRunsDifference Depth3_HE \n");
      diffAmpl_Depth3_HE->SetXTitle("Relative Difference, % - Depth3_HE");
      diffAmpl_Depth3_HE->SetMarkerColor(2);
      diffAmpl_Depth3_HE->SetLineColor(2);
      diffAmpl_Depth3_HE->Draw("");
      
      c1->Update();
      
      
    //=============================================================================================== 6
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1AmplE34_HF");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth1_HF");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Ceff->SetMarkerStyle(20);
      Ceff->SetMarkerSize(0.4);
      Ceff->GetYaxis()->SetLabelSize(0.04);
      //    Ceff->SetTitle("any Error, HF Depth1 \n");
      Ceff->SetXTitle("#eta");
      Ceff->SetYTitle("#phi");
      Ceff->SetZTitle("Signal for 1st Run - HF Depth1");
      Ceff->SetMarkerColor(2);
      Ceff->SetLineColor(2);
      Ceff->Draw("COLZ");
      
      c1->cd(2);
      TH2F *tqod1= (TH2F*)hfile2->Get("h_mapDepth1AmplE34_HF");
      TH2F *tqod0= (TH2F*)hfile2->Get("h_mapDepth1_HF");
      tqod1->Sumw2();
      tqod0->Sumw2();
      if(tqod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Czeff = (TH2F*)tqod1->Clone("Czeff");
	Czeff->Divide(tqod1,tqod0, 1, 1, "B");
	Czeff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Czeff->SetMarkerStyle(20);
      Czeff->SetMarkerSize(0.4);
      Czeff->GetYaxis()->SetLabelSize(0.04);
      //    Czeff->SetTitle("any Error, HF Depth1 \n");
      Czeff->SetXTitle("#eta");
      Czeff->SetYTitle("#phi");
      Czeff->SetZTitle("Signal for 2nd Run - HF Depth1");
      Czeff->SetMarkerColor(2);
      Czeff->SetLineColor(2);
      Czeff->Draw("COLZ");
      
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth1_HF = (TH2F*)Ceff->Clone("Diffe_Depth1_HF");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	Diffe_Depth1_HF->SetBinContent(i,j,0.);
	if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	  ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	  if(fabs(ccc1) > 2. )  Diffe_Depth1_HF->SetBinContent(i,j,fabs(ccc1));
	}
      }
    }
    gPad->SetGridy();
    gPad->SetGridx();
    gPad->SetLogz();
    Diffe_Depth1_HF->SetMarkerStyle(20);
    Diffe_Depth1_HF->SetMarkerSize(0.4);
    Diffe_Depth1_HF->GetYaxis()->SetLabelSize(0.04);
    //    Diffe_Depth1_HF->SetTitle("any Error, HF Depth1 \n");
    Diffe_Depth1_HF->SetXTitle("#eta");
    Diffe_Depth1_HF->SetYTitle("#phi");
    Diffe_Depth1_HF->SetZTitle("Relative Difference in % - Depth1_HF");
    Diffe_Depth1_HF->SetMarkerColor(2);
    Diffe_Depth1_HF->SetLineColor(2);
    Diffe_Depth1_HF->Draw("COLZ");
    
    
      c1->cd(4);
      TH1F* diffAmpl_Depth1_HF = new TH1F("diffAmpl_Depth1_HF","", 100, -10., 10.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	    ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	    diffAmpl_Depth1_HF->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmpl_Depth1_HF->SetMarkerStyle(20);
      diffAmpl_Depth1_HF->SetMarkerSize(0.4);
      diffAmpl_Depth1_HF->GetYaxis()->SetLabelSize(0.04);
      diffAmpl_Depth1_HF->SetTitle("AmplRunsDifference Depth1_HF \n");
      diffAmpl_Depth1_HF->SetXTitle("Relative Difference, % - Depth1_HF");
      diffAmpl_Depth1_HF->SetMarkerColor(2);
      diffAmpl_Depth1_HF->SetLineColor(2);
      diffAmpl_Depth1_HF->Draw("");
      
      c1->Update();
      
      
            
    //============================================================================================== 7
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2AmplE34_HF");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth2_HF");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Ceff->SetMarkerStyle(20);
      Ceff->SetMarkerSize(0.4);
      Ceff->GetYaxis()->SetLabelSize(0.04);
      //    Ceff->SetTitle("any Error, HF Depth2 \n");
      Ceff->SetXTitle("#eta");
      Ceff->SetYTitle("#phi");
      Ceff->SetZTitle("Signal for 1st Run - HF Depth2");
      Ceff->SetMarkerColor(2);
      Ceff->SetLineColor(2);
      Ceff->Draw("COLZ");
      
      c1->cd(2);
      TH2F *tqod1= (TH2F*)hfile2->Get("h_mapDepth2AmplE34_HF");
      TH2F *tqod0= (TH2F*)hfile2->Get("h_mapDepth2_HF");
      tqod1->Sumw2();
      tqod0->Sumw2();
      if(tqod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Czeff = (TH2F*)tqod1->Clone("Czeff");
	Czeff->Divide(tqod1,tqod0, 1, 1, "B");
	Czeff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Czeff->SetMarkerStyle(20);
      Czeff->SetMarkerSize(0.4);
      Czeff->GetYaxis()->SetLabelSize(0.04);
      //    Czeff->SetTitle("any Error, HF Depth2 \n");
      Czeff->SetXTitle("#eta");
      Czeff->SetYTitle("#phi");
      Czeff->SetZTitle("Signal for 2nd Run - HF Depth2");
      Czeff->SetMarkerColor(2);
      Czeff->SetLineColor(2);
      Czeff->Draw("COLZ");
      
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth2_HF = (TH2F*)Ceff->Clone("Diffe_Depth2_HF");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	Diffe_Depth2_HF->SetBinContent(i,j,0.);
	if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	  ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	  if(fabs(ccc1) > 2. )  Diffe_Depth2_HF->SetBinContent(i,j,fabs(ccc1));
	}
      }
    }
    gPad->SetGridy();
    gPad->SetGridx();
    gPad->SetLogz();
    Diffe_Depth2_HF->SetMarkerStyle(20);
    Diffe_Depth2_HF->SetMarkerSize(0.4);
    Diffe_Depth2_HF->GetYaxis()->SetLabelSize(0.04);
    //    Diffe_Depth2_HF->SetTitle("any Error, HF Depth2 \n");
    Diffe_Depth2_HF->SetXTitle("#eta");
    Diffe_Depth2_HF->SetYTitle("#phi");
    Diffe_Depth2_HF->SetZTitle("Relative Difference in % - Depth2_HF");
    Diffe_Depth2_HF->SetMarkerColor(2);
    Diffe_Depth2_HF->SetLineColor(2);
    Diffe_Depth2_HF->Draw("COLZ");
    

      c1->cd(4);
      TH1F* diffAmpl_Depth2_HF = new TH1F("diffAmpl_Depth2_HF","", 100, -10., 10.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	    ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	    diffAmpl_Depth2_HF->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmpl_Depth2_HF->SetMarkerStyle(20);
      diffAmpl_Depth2_HF->SetMarkerSize(0.4);
      diffAmpl_Depth2_HF->GetYaxis()->SetLabelSize(0.04);
      diffAmpl_Depth2_HF->SetTitle("AmplRunsDifference Depth2_HF \n");
      diffAmpl_Depth2_HF->SetXTitle("Relative Difference, % - Depth2_HF");
      diffAmpl_Depth2_HF->SetMarkerColor(2);
      diffAmpl_Depth2_HF->SetLineColor(2);
      diffAmpl_Depth2_HF->Draw("");
      
      c1->Update();
      
      
            
    //============================================================================================== 8
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth4AmplE34_HO");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth4_HO");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Ceff->SetMarkerStyle(20);
      Ceff->SetMarkerSize(0.4);
      Ceff->GetYaxis()->SetLabelSize(0.04);
      //    Ceff->SetTitle("any Error, HO Depth4 \n");
      Ceff->SetXTitle("#eta");
      Ceff->SetYTitle("#phi");
      Ceff->SetZTitle("Signal for 1st Run - HO Depth4");
      Ceff->SetMarkerColor(2);
      Ceff->SetLineColor(2);
      Ceff->Draw("COLZ");
      
      c1->cd(2);
      TH2F *tqod1= (TH2F*)hfile2->Get("h_mapDepth4AmplE34_HO");
      TH2F *tqod0= (TH2F*)hfile2->Get("h_mapDepth4_HO");
      tqod1->Sumw2();
      tqod0->Sumw2();
      if(tqod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Czeff = (TH2F*)tqod1->Clone("Czeff");
	Czeff->Divide(tqod1,tqod0, 1, 1, "B");
	Czeff->Sumw2();
      }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Czeff->SetMarkerStyle(20);
      Czeff->SetMarkerSize(0.4);
      Czeff->GetYaxis()->SetLabelSize(0.04);
      //    Czeff->SetTitle("any Error, HO Depth4 \n");
      Czeff->SetXTitle("#eta");
      Czeff->SetYTitle("#phi");
      Czeff->SetZTitle("Signal for 2nd Run - HO Depth4");
      Czeff->SetMarkerColor(2);
      Czeff->SetLineColor(2);
      Czeff->Draw("COLZ");
      
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth4_HO = (TH2F*)Ceff->Clone("Diffe_Depth4_HO");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	Diffe_Depth4_HO->SetBinContent(i,j,0.);
	if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	  ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	  if(fabs(ccc1) > 2. )  Diffe_Depth4_HO->SetBinContent(i,j,fabs(ccc1));
	}
      }
    }
    gPad->SetGridy();
    gPad->SetGridx();
    gPad->SetLogz();
    Diffe_Depth4_HO->SetMarkerStyle(20);
    Diffe_Depth4_HO->SetMarkerSize(0.4);
    Diffe_Depth4_HO->GetYaxis()->SetLabelSize(0.04);
    //    Diffe_Depth4_HO->SetTitle("any Error, HO Depth4 \n");
    Diffe_Depth4_HO->SetXTitle("#eta");
    Diffe_Depth4_HO->SetYTitle("#phi");
    Diffe_Depth4_HO->SetZTitle("Relative Difference in % - Depth4_HO");
    Diffe_Depth4_HO->SetMarkerColor(2);
    Diffe_Depth4_HO->SetLineColor(2);
    Diffe_Depth4_HO->Draw("COLZ");
    
    
      c1->cd(4);
      TH1F* diffAmpl_Depth4_HO = new TH1F("diffAmpl_Depth4_HO","", 100, -10., 10.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 && Czeff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j)   -   Czeff->GetBinContent(i,j)  ;
	    ccc1 = 100.*ccc1/Ceff->GetBinContent(i,j);
	    diffAmpl_Depth4_HO->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmpl_Depth4_HO->SetMarkerStyle(20);
      diffAmpl_Depth4_HO->SetMarkerSize(0.4);
      diffAmpl_Depth4_HO->GetYaxis()->SetLabelSize(0.04);
      diffAmpl_Depth4_HO->SetTitle("AmplRunsDifference Depth4_HO \n");
      diffAmpl_Depth4_HO->SetXTitle("Relative Difference, % - Depth4_HO");
      diffAmpl_Depth4_HO->SetMarkerColor(2);
      diffAmpl_Depth4_HO->SetLineColor(2);
      diffAmpl_Depth4_HO->Draw("");
      
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
