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

//	TFile *hfile1= new TFile("PFGNZSRAW-run193116.root", "READ");        
//	TFile *hfile1= new TFile("PFGNZSRAW-run194151.root", "READ");        
//	TFile *hfile1= new TFile("PFGNZSRAW-run199751.root", "READ");        
	TFile *hfile1= new TFile("testNZS.root", "READ");        

	//    getchar();
	//
//          TPostScript psfile ("zerrors193116.ps", 111);
//          TPostScript psfile ("zerrors194151.ps", 111);
//          TPostScript psfile ("zerrors199751.ps", 111);
          TPostScript psfile ("testNZS.ps", 111);

    //======================================================================





    //======================================================================
   c1 = new TCanvas("c1","Hcal4test",200,10,700,900);


    //==================================================================================================== 1
    //======================================================================
    //======================================================================
    //================
    //======================================================================
            
      c1->Clear();
      c1->Divide(1,2);
      
      c1->cd(1);
	TH1F *mp3d= (TH1F*)hfile1->Get("h_amplFine_HB");
	gPad->SetLogy();
	//	mp3d->Scale(mscale);
	mp3d->SetMarkerStyle(21);
	mp3d->SetMarkerSize(0.4);
	mp3d->GetXaxis()->SetTitle("Energy (noError&Error) HB");
	mp3d->GetYaxis()->SetLabelSize(0.04);
	mp3d->SetMarkerColor(2);
	mp3d->SetLineColor(2);
	//       mp3d->SetFillColor(5);
	//   mp3d->SetMinimum(0.8);
	mp3d->Draw("Error");
	TH1F *mp3d= (TH1F*)hfile1->Get("h_amplError_HB");
	gPad->SetLogy();
	//	mp3d->Scale(mscale);
	mp3d->SetMarkerStyle(21);
	mp3d->SetMarkerSize(0.4);
	//	mp3d->GetXaxis()->SetTitle("Energy (AnyError HB)");
	mp3d->GetYaxis()->SetLabelSize(0.04);
	mp3d->SetMarkerColor(1);
	mp3d->SetLineColor(1);
	//       mp3d->SetFillColor(5);
	//   mp3d->SetMinimum(0.8);
	mp3d->Draw("ErrorSame");
	
      c1->cd(2);
	TH1F *mp3d= (TH1F*)hfile1->Get("h_amplFine_HE");
	gPad->SetLogy();
	//	mp3d->Scale(mscale);
	mp3d->SetMarkerStyle(21);
	mp3d->SetMarkerSize(0.4);
	mp3d->GetXaxis()->SetTitle("Energy (noError&Error) HE");
	mp3d->GetYaxis()->SetLabelSize(0.04);
	mp3d->SetMarkerColor(2);
	mp3d->SetLineColor(2);
	//       mp3d->SetFillColor(5);
	//   mp3d->SetMinimum(0.8);
	mp3d->Draw("Error");
	TH1F *mp3d= (TH1F*)hfile1->Get("h_amplError_HE");
	gPad->SetLogy();
	//	mp3d->Scale(mscale);
	mp3d->SetMarkerStyle(21);
	mp3d->SetMarkerSize(0.4);
	//	mp3d->GetXaxis()->SetTitle("Energy (AnyError HE)");
	mp3d->GetYaxis()->SetLabelSize(0.04);
	mp3d->SetMarkerColor(1);
	mp3d->SetLineColor(1);
	//       mp3d->SetFillColor(5);
	//   mp3d->SetMinimum(0.8);
	mp3d->Draw("ErrorSame");
	
	
	
	c1->Update();
	
	
    //==================================================================================================== 2
    //======================================================================
    //======================================================================
    //================
    //======================================================================
	double neventsHB = 0.;
	c1->Clear();
	c1->Divide(1,1);
      
      c1->cd(1);
      TH1F *mp3d= (TH1F*)hfile1->Get("h_errorGeneral_HB");
      int nx = mp3d->GetXaxis()->GetNbins();
      cout << "********* h_errorGeneral_HB   nx= "  << nx <<   endl;
      double ccc1 =mp3d->GetBinContent(1);
      double ccc2 =mp3d->GetBinContent(2);
      double ccc3 =mp3d->GetBinContent(3);
      double ccc4 =mp3d->GetBinContent(4);
      double ccc5 =mp3d->GetBinContent(5);
      double ccc6 = ccc2/(ccc1+ccc2+ccc3+ccc4+ccc5);
      neventsHB = ccc1+ccc2+ccc3+ccc4+ccc5;
      cout << "********* h_errorGeneral_HB   #Ev =  "  << ccc1+ccc2+ccc3+ccc4+ccc5 <<   endl;
      cout << "********* h_errorGeneral_HB   ccc2 = "  << ccc6  <<   endl;
      ccc6 = ccc3/(ccc1+ccc2+ccc3+ccc4+ccc5);
      cout << "********* h_errorGeneral_HB   ccc3 = "  << ccc6  <<   endl;
      ccc6 = ccc4/(ccc1+ccc2+ccc3+ccc4+ccc5);
      cout << "********* h_errorGeneral_HB   ccc4 = "  << ccc6  <<   endl;
      ccc6 = ccc5/(ccc1+ccc2+ccc3+ccc4+ccc5);
      cout << "********* h_errorGeneral_HB   ccc5 = "  << ccc6  <<   endl;
      //  Purity->Sumw2();
      gPad->SetLogy();
      mp3d->SetMarkerStyle(21);
      mp3d->SetMarkerSize(0.4);
      mp3d->GetXaxis()->SetTitle("capid 1 2 3 4_HB");
      mp3d->GetYaxis()->SetLabelSize(0.04);
      mp3d->SetMarkerColor(1);
      mp3d->SetLineColor(1);
      //       mp3d->SetFillColor(5);
      //   mp3d->SetMinimum(0.8);
      mp3d->Draw("Error");
      
	
	c1->Update();
	
	
    //==================================================================================================== 3
    //======================================================================
    //======================================================================
    //================
    //======================================================================
	double neventsHE = 0.;
            
      c1->Clear();
      c1->Divide(1,1);
      
      c1->cd(1);
	TH1F *mp3d= (TH1F*)hfile1->Get("h_errorGeneral_HE");
      int nx = mp3d->GetXaxis()->GetNbins();
      cout << "********* h_errorGeneral_HE   nx= "  << nx <<   endl;
      double ccc1 =mp3d->GetBinContent(1);
      double ccc2 =mp3d->GetBinContent(2);
      double ccc3 =mp3d->GetBinContent(3);
      double ccc4 =mp3d->GetBinContent(4);
      double ccc5 =mp3d->GetBinContent(5);
      double ccc6 = ccc2/(ccc1+ccc2+ccc3+ccc4+ccc5);
      neventsHE = ccc1+ccc2+ccc3+ccc4+ccc5;
      cout << "********* h_errorGeneral_HE   #Ev =  "  << ccc1+ccc2+ccc3+ccc4+ccc5 <<   endl;
      cout << "********* h_errorGeneral_HE   ccc2 = "  << ccc6  <<   endl;
      ccc6 = ccc3/(ccc1+ccc2+ccc3+ccc4+ccc5);
      cout << "********* h_errorGeneral_HE   ccc3 = "  << ccc6  <<   endl;
      ccc6 = ccc4/(ccc1+ccc2+ccc3+ccc4+ccc5);
      cout << "********* h_errorGeneral_HE   ccc4 = "  << ccc6  <<   endl;
      ccc6 = ccc5/(ccc1+ccc2+ccc3+ccc4+ccc5);
      cout << "********* h_errorGeneral_HE   ccc5 = "  << ccc6  <<   endl;
      //  Purity->Sumw2();
	gPad->SetLogy();
	mp3d->SetMarkerStyle(21);
	mp3d->SetMarkerSize(0.4);
	mp3d->GetXaxis()->SetTitle("capid 1 2 3 4_HE");
	mp3d->GetYaxis()->SetLabelSize(0.04);
	mp3d->SetMarkerColor(1);
	mp3d->SetLineColor(1);
	//       mp3d->SetFillColor(5);
	//   mp3d->SetMinimum(0.8);
	mp3d->Draw("Error");

	c1->Update();
	
            
    //==================================================================================================== 4
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(1,2);
      
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1Error_HB");
       gPad->SetGridy();
       gPad->SetGridx();
      gPad->SetLogz();
      twod1->SetMarkerStyle(20);
      twod1->SetMarkerSize(0.4);
      twod1->GetYaxis()->SetLabelSize(0.04);
//      twod1->SetTitle("any Error, HB Depth1 \n");
      twod1->SetXTitle("#eta");
      twod1->SetYTitle("#phi");
      twod1->SetZTitle("HB Depth1");
      twod1->SetMarkerColor(2);
      twod1->SetLineColor(2);
      twod1->Draw("COLZ");
      
      c1->cd(2);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2Error_HB");
       gPad->SetGridy();
       gPad->SetGridx();
      gPad->SetLogz();
      twod1->SetMarkerStyle(20);
      twod1->SetMarkerSize(0.4);
      twod1->GetYaxis()->SetLabelSize(0.04);
//      twod1->SetTitle("any Error, HB Depth2 \n");
      twod1->SetXTitle("#eta");
      twod1->SetYTitle("#phi");
      twod1->SetZTitle("HB Depth2");
      twod1->SetMarkerColor(2);
      twod1->SetLineColor(2);
      twod1->Draw("COLZ");
      
      
      c1->Update();
	
	
            
    //==================================================================================================== 5
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      c1->cd(1);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth1Error_HE");
       gPad->SetGridy();
       gPad->SetGridx();
      gPad->SetLogz();
      twod1->SetMarkerStyle(20);
      twod1->SetMarkerSize(0.4);
      twod1->GetYaxis()->SetLabelSize(0.04);
//      twod1->SetTitle("any Error, HE Depth1 \n");
      twod1->SetXTitle("#eta");
      twod1->SetYTitle("#phi");
      twod1->SetZTitle("HE Depth1");
      twod1->SetMarkerColor(2);
      twod1->SetLineColor(2);
      twod1->Draw("COLZ");
      
      c1->cd(2);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth2Error_HE");
       gPad->SetGridy();
       gPad->SetGridx();
      gPad->SetLogz();
      twod1->SetMarkerStyle(20);
      twod1->SetMarkerSize(0.4);
      twod1->GetYaxis()->SetLabelSize(0.04);
//      twod1->SetTitle("any Error, HE Depth2 \n");
      twod1->SetXTitle("#eta");
      twod1->SetYTitle("#phi");
      twod1->SetZTitle("HE Depth2");
      twod1->SetMarkerColor(2);
      twod1->SetLineColor(2);
      twod1->Draw("COLZ");
      
      c1->cd(3);
      TH2F *twod1= (TH2F*)hfile1->Get("h_mapDepth3Error_HE");
       gPad->SetGridy();
       gPad->SetGridx();
      gPad->SetLogz();
      twod1->SetMarkerStyle(20);
      twod1->SetMarkerSize(0.4);
      twod1->GetYaxis()->SetLabelSize(0.04);
//      twod1->SetTitle("any Error, HE Depth3 \n");
      twod1->SetXTitle("#eta");
      twod1->SetYTitle("#phi");
      twod1->SetZTitle("HE Depth3");
      twod1->SetMarkerColor(2);
      twod1->SetLineColor(2);
      twod1->Draw("COLZ");
      
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
