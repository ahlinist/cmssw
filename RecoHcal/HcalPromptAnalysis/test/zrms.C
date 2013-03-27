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

//	TFile *hfile1= new TFile("BadRBXrun211154.root", "READ");        
	TFile *hfile1= new TFile("test.root", "READ");        
//	TFile *hfile1= new TFile("test8runs.root", "READ");        
//	TFile *hfile1= new TFile("test203056.root", "READ");        

	//    getchar();
	//
          TPostScript psfile ("zrms.ps", 111);
//

   c1 = new TCanvas("c1","Hcal4test",200,10,700,900);


    //=============================================================================================== 1
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1Amplitude_HB");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth1_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1Amplitude225_HB");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth1_HB");
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
      Cefz225->SetZTitle("Rate for TS RMS in each event & cell > 2.25 - HB Depth1 \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_Amplitude_HB");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("TS RMS in each event & cell HB \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth1_HB = (TH2F*)Ceff->Clone("Diffe_Depth1_HB");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_Depth1_HB->SetBinContent(i,j,0.);
	  if(ccc1 < 1.0 || ccc1 > 2.3 )  Diffe_Depth1_HB->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_Depth1_HB->SetMarkerStyle(20);
      Diffe_Depth1_HB->SetMarkerSize(0.4);
      Diffe_Depth1_HB->GetZaxis()->SetLabelSize(0.08);
      //    Diffe_Depth1_HB->SetTitle("any Error, HB Depth1 \n");
      Diffe_Depth1_HB->SetXTitle("#eta \b");
      Diffe_Depth1_HB->SetYTitle("#phi \b");
      Diffe_Depth1_HB->SetZTitle("<TS RMS> out 1.0-2.3 - HB Depth1 \b");
      Diffe_Depth1_HB->SetMarkerColor(2);
      Diffe_Depth1_HB->SetLineColor(2);
      Diffe_Depth1_HB->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_Depth1_HB = new TH1F("diffAmplitude_Depth1_HB","", 100, 0., 5.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_Depth1_HB->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_Depth1_HB->SetMarkerStyle(20);
      diffAmplitude_Depth1_HB->SetMarkerSize(0.4);
      diffAmplitude_Depth1_HB->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_Depth1_HB->SetXTitle("<TS RMS> in each cell \b");
      diffAmplitude_Depth1_HB->SetMarkerColor(2);
      diffAmplitude_Depth1_HB->SetLineColor(2);
      diffAmplitude_Depth1_HB->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 2
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2Amplitude_HB");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth2_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2Amplitude225_HB");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth2_HB");
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
      Cefz225->GetZaxis()->SetLabelSize(0.08);
      Cefz225->SetXTitle("#eta \b");
      Cefz225->SetYTitle("#phi \b");
      Cefz225->SetZTitle("Rate for TS RMS in each event & cell > 2.25 - HB Depth2 \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_Amplitude_HB");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("TS RMS in each event & cell HB \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth2_HB = (TH2F*)Ceff->Clone("Diffe_Depth2_HB");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_Depth2_HB->SetBinContent(i,j,0.);
	  if(ccc1 < 1.0 || ccc1 > 2.3 )  Diffe_Depth2_HB->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_Depth2_HB->SetMarkerStyle(20);
      Diffe_Depth2_HB->SetMarkerSize(0.4);
      Diffe_Depth2_HB->GetZaxis()->SetLabelSize(0.08);
      //    Diffe_Depth2_HB->SetTitle("any Error, HB Depth2 \n");
      Diffe_Depth2_HB->SetXTitle("#eta \b");
      Diffe_Depth2_HB->SetYTitle("#phi \b");
      Diffe_Depth2_HB->SetZTitle("<TS RMS> out 1.0-2.3 - HB Depth2 \b");
      Diffe_Depth2_HB->SetMarkerColor(2);
      Diffe_Depth2_HB->SetLineColor(2);
      Diffe_Depth2_HB->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_Depth2_HB = new TH1F("diffAmplitude_Depth2_HB","", 100, 0., 5.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_Depth2_HB->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_Depth2_HB->SetMarkerStyle(20);
      diffAmplitude_Depth2_HB->SetMarkerSize(0.4);
      diffAmplitude_Depth2_HB->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_Depth2_HB->SetXTitle("<TS RMS> in each cell \b");
      diffAmplitude_Depth2_HB->SetMarkerColor(2);
      diffAmplitude_Depth2_HB->SetLineColor(2);
      diffAmplitude_Depth2_HB->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 3
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1Amplitude_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth1_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1Amplitude225_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth1_HE");
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
      Cefz225->GetZaxis()->SetLabelSize(0.08);
      Cefz225->SetXTitle("#eta \b");
      Cefz225->SetYTitle("#phi \b");
      Cefz225->SetZTitle("Rate for TS RMS in each event & cell > 2.25 - HE Depth1 \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_Amplitude_HE");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("TS RMS in each event & cell HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth1_HE = (TH2F*)Ceff->Clone("Diffe_Depth1_HE");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_Depth1_HE->SetBinContent(i,j,0.);
	  if(ccc1 < 1.0 || ccc1 > 2.3 )  Diffe_Depth1_HE->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_Depth1_HE->SetMarkerStyle(20);
      Diffe_Depth1_HE->SetMarkerSize(0.4);
      Diffe_Depth1_HE->GetZaxis()->SetLabelSize(0.08);
      //    Diffe_Depth1_HE->SetTitle("any Error, HE Depth1 \n");
      Diffe_Depth1_HE->SetXTitle("#eta \b");
      Diffe_Depth1_HE->SetYTitle("#phi \b");
      Diffe_Depth1_HE->SetZTitle("<TS RMS> out 1.0-2.3 - HE Depth1 \b");
      Diffe_Depth1_HE->SetMarkerColor(2);
      Diffe_Depth1_HE->SetLineColor(2);
      Diffe_Depth1_HE->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_Depth1_HE = new TH1F("diffAmplitude_Depth1_HE","", 100, 0., 5.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_Depth1_HE->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_Depth1_HE->SetMarkerStyle(20);
      diffAmplitude_Depth1_HE->SetMarkerSize(0.4);
      diffAmplitude_Depth1_HE->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_Depth1_HE->SetXTitle("<TS RMS> in each cell \b");
      diffAmplitude_Depth1_HE->SetMarkerColor(2);
      diffAmplitude_Depth1_HE->SetLineColor(2);
      diffAmplitude_Depth1_HE->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 4
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2Amplitude_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth2_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2Amplitude225_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth2_HE");
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
      Cefz225->GetZaxis()->SetLabelSize(0.08);
      Cefz225->SetXTitle("#eta \b");
      Cefz225->SetYTitle("#phi \b");
      Cefz225->SetZTitle("Rate for TS RMS in each event & cell > 2.25 - HE Depth2 \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_Amplitude_HE");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("TS RMS in each event & cell HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth2_HE = (TH2F*)Ceff->Clone("Diffe_Depth2_HE");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_Depth2_HE->SetBinContent(i,j,0.);
	  if(ccc1 < 1.0 || ccc1 > 2.3 )  Diffe_Depth2_HE->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_Depth2_HE->SetMarkerStyle(20);
      Diffe_Depth2_HE->SetMarkerSize(0.4);
      Diffe_Depth2_HE->GetZaxis()->SetLabelSize(0.08);
      //    Diffe_Depth2_HE->SetTitle("any Error, HE Depth2 \n");
      Diffe_Depth2_HE->SetXTitle("#eta \b");
      Diffe_Depth2_HE->SetYTitle("#phi \b");
      Diffe_Depth2_HE->SetZTitle("<TS RMS> out 1.0-2.3 - HE Depth2 \b");
      Diffe_Depth2_HE->SetMarkerColor(2);
      Diffe_Depth2_HE->SetLineColor(2);
      Diffe_Depth2_HE->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_Depth2_HE = new TH1F("diffAmplitude_Depth2_HE","", 100, 0., 5.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_Depth2_HE->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_Depth2_HE->SetMarkerStyle(20);
      diffAmplitude_Depth2_HE->SetMarkerSize(0.4);
      diffAmplitude_Depth2_HE->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_Depth2_HE->SetXTitle("<TS RMS> in each cell \b");
      diffAmplitude_Depth2_HE->SetMarkerColor(2);
      diffAmplitude_Depth2_HE->SetLineColor(2);
      diffAmplitude_Depth2_HE->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 5
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth3Amplitude_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth3_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth3Amplitude225_HE");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth3_HE");
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
      Cefz225->GetZaxis()->SetLabelSize(0.08);
      Cefz225->SetXTitle("#eta \b");
      Cefz225->SetYTitle("#phi \b");
      Cefz225->SetZTitle("Rate for TS RMS in each event & cell > 2.25 - HE Depth3 \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_Amplitude_HE");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("TS RMS in each event & cell HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth3_HE = (TH2F*)Ceff->Clone("Diffe_Depth3_HE");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_Depth3_HE->SetBinContent(i,j,0.);
	  if(ccc1 < 1.0 || ccc1 > 2.3 )  Diffe_Depth3_HE->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_Depth3_HE->SetMarkerStyle(20);
      Diffe_Depth3_HE->SetMarkerSize(0.4);
      Diffe_Depth3_HE->GetZaxis()->SetLabelSize(0.08);
      //    Diffe_Depth3_HE->SetTitle("any Error, HE Depth3 \n");
      Diffe_Depth3_HE->SetXTitle("#eta \b");
      Diffe_Depth3_HE->SetYTitle("#phi \b");
      Diffe_Depth3_HE->SetZTitle("<TS RMS> out 1.0-2.3 - HE Depth3 \b");
      Diffe_Depth3_HE->SetMarkerColor(2);
      Diffe_Depth3_HE->SetLineColor(2);
      Diffe_Depth3_HE->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_Depth3_HE = new TH1F("diffAmplitude_Depth3_HE","", 100, 0., 5.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_Depth3_HE->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_Depth3_HE->SetMarkerStyle(20);
      diffAmplitude_Depth3_HE->SetMarkerSize(0.4);
      diffAmplitude_Depth3_HE->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_Depth3_HE->SetXTitle("<TS RMS> in each cell \b");
      diffAmplitude_Depth3_HE->SetMarkerColor(2);
      diffAmplitude_Depth3_HE->SetLineColor(2);
      diffAmplitude_Depth3_HE->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 6
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1Amplitude_HF");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth1_HF");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1Amplitude225_HF");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth1_HF");
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
      Cefz225->SetZTitle("Rate for TS RMS in each event & cell > 2.25 - HF Depth1 \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_Amplitude_HF");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("TS RMS in each event & cell HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth1_HF = (TH2F*)Ceff->Clone("Diffe_Depth1_HF");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_Depth1_HF->SetBinContent(i,j,0.);
	  if(ccc1 < 0.5 || ccc1 > 2.0 )  Diffe_Depth1_HF->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_Depth1_HF->SetMarkerStyle(20);
      Diffe_Depth1_HF->SetMarkerSize(0.4);
      Diffe_Depth1_HF->GetZaxis()->SetLabelSize(0.08);
      //    Diffe_Depth1_HF->SetTitle("any Error, HF Depth1 \n");
      Diffe_Depth1_HF->SetXTitle("#eta \b");
      Diffe_Depth1_HF->SetYTitle("#phi \b");
      Diffe_Depth1_HF->SetZTitle("<TS RMS> out 0.5-2.0 - HF Depth1 \b");
      Diffe_Depth1_HF->SetMarkerColor(2);
      Diffe_Depth1_HF->SetLineColor(2);
      Diffe_Depth1_HF->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_Depth1_HF = new TH1F("diffAmplitude_Depth1_HF","", 100, 0., 5.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_Depth1_HF->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_Depth1_HF->SetMarkerStyle(20);
      diffAmplitude_Depth1_HF->SetMarkerSize(0.4);
      diffAmplitude_Depth1_HF->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_Depth1_HF->SetXTitle("<TS RMS> in each cell \b");
      diffAmplitude_Depth1_HF->SetMarkerColor(2);
      diffAmplitude_Depth1_HF->SetLineColor(2);
      diffAmplitude_Depth1_HF->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 7
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2Amplitude_HF");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth2_HF");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2Amplitude225_HF");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth2_HF");
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
      Cefz225->GetZaxis()->SetLabelSize(0.08);
      Cefz225->SetXTitle("#eta \b");
      Cefz225->SetYTitle("#phi \b");
      Cefz225->SetZTitle("Rate for TS RMS in each event & cell > 2.25 - HF Depth2 \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_Amplitude_HF");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("TS RMS in each event & cell HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth2_HF = (TH2F*)Ceff->Clone("Diffe_Depth2_HF");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_Depth2_HF->SetBinContent(i,j,0.);
	  if(ccc1 < 0.5 || ccc1 > 2.0 )  Diffe_Depth2_HF->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_Depth2_HF->SetMarkerStyle(20);
      Diffe_Depth2_HF->SetMarkerSize(0.4);
      Diffe_Depth2_HF->GetZaxis()->SetLabelSize(0.08);
      //    Diffe_Depth2_HF->SetTitle("any Error, HF Depth2 \n");
      Diffe_Depth2_HF->SetXTitle("#eta \b");
      Diffe_Depth2_HF->SetYTitle("#phi \b");
      Diffe_Depth2_HF->SetZTitle("<TS RMS> out 0.5-2.0 - HF Depth2 \b");
      Diffe_Depth2_HF->SetMarkerColor(2);
      Diffe_Depth2_HF->SetLineColor(2);
      Diffe_Depth2_HF->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_Depth2_HF = new TH1F("diffAmplitude_Depth2_HF","", 100, 0., 5.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_Depth2_HF->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_Depth2_HF->SetMarkerStyle(20);
      diffAmplitude_Depth2_HF->SetMarkerSize(0.4);
      diffAmplitude_Depth2_HF->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_Depth2_HF->SetXTitle("<TS RMS> in each cell \b");
      diffAmplitude_Depth2_HF->SetMarkerColor(2);
      diffAmplitude_Depth2_HF->SetLineColor(2);
      diffAmplitude_Depth2_HF->Draw("");
      
      c1->Update();
      
      
    //=============================================================================================== 8
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth4Amplitude_HO");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth4_HO");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH2F")){
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	Ceff->Sumw2();
      }
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth4Amplitude225_HO");
      TH2F *twod0= (TH2F*)hfile1->Get("h_mapDepth4_HO");
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
      Cefz225->GetZaxis()->SetLabelSize(0.08);
      Cefz225->SetXTitle("#eta \b");
      Cefz225->SetYTitle("#phi \b");
      Cefz225->SetZTitle("Rate for TS RMS in each event & cell > 2.25 - HO Depth4 \b");
      Cefz225->SetMarkerColor(2);
      Cefz225->SetLineColor(2);
      Cefz225->Draw("COLZ");
      
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_Amplitude_HO");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("TS RMS in each event & cell HO \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
    ///////////////////////////////////////
    TH2F* Diffe_Depth4_HO = (TH2F*)Ceff->Clone("Diffe_Depth4_HO");
    int nx = Ceff->GetXaxis()->GetNbins();
    int ny = Ceff->GetYaxis()->GetNbins();
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  double ccc1 =  Ceff->GetBinContent(i,j)   ;
	  Diffe_Depth4_HO->SetBinContent(i,j,0.);
	  if(ccc1 < 1.0 || ccc1 > 2.5 )  Diffe_Depth4_HO->SetBinContent(i,j,ccc1);
      }
    }
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogz();
      Diffe_Depth4_HO->SetMarkerStyle(20);
      Diffe_Depth4_HO->SetMarkerSize(0.4);
      Diffe_Depth4_HO->GetZaxis()->SetLabelSize(0.08);
      //    Diffe_Depth4_HO->SetTitle("any Error, HO Depth4 \n");
      Diffe_Depth4_HO->SetXTitle("#eta \b");
      Diffe_Depth4_HO->SetYTitle("#phi \b");
      Diffe_Depth4_HO->SetZTitle("<TS RMS> out 1.0-2.5 - HO Depth4 \b");
      Diffe_Depth4_HO->SetMarkerColor(2);
      Diffe_Depth4_HO->SetLineColor(2);
      Diffe_Depth4_HO->Draw("COLZ");
      

      c1->cd(4);
      TH1F* diffAmplitude_Depth4_HO = new TH1F("diffAmplitude_Depth4_HO","", 100, 0., 5.);
      int nx = Ceff->GetXaxis()->GetNbins();
      int ny = Ceff->GetYaxis()->GetNbins();
      for (int i=1;i<=nx;i++) {
	for (int j=1;j<=ny;j++) {
	  if(Ceff->GetBinContent(i,j) !=0 ) {
	    double ccc1 =  Ceff->GetBinContent(i,j) ;
	    diffAmplitude_Depth4_HO->Fill(ccc1);
	  }
	}
      }
      gPad->SetLogy();
      diffAmplitude_Depth4_HO->SetMarkerStyle(20);
      diffAmplitude_Depth4_HO->SetMarkerSize(0.4);
      diffAmplitude_Depth4_HO->GetYaxis()->SetLabelSize(0.04);
      diffAmplitude_Depth4_HO->SetXTitle("<TS RMS> in each cell \b");
      diffAmplitude_Depth4_HO->SetMarkerColor(2);
      diffAmplitude_Depth4_HO->SetLineColor(2);
      diffAmplitude_Depth4_HO->Draw("");
      
      c1->Update();
      
      
            
      
            
    //======================================================================
    //============================================================================================ end
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
