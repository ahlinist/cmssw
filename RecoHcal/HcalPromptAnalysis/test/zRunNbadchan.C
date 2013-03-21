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
					//	gStyle->SetOptStat(1110000);// und over, integral !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//gStyle->SetOptStat(101110);// entries, meam, rms, overflow !!!!!!!!!!!!!!!!!!!!!!!!
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
//	TFile *hfile1= new TFile("test_allruns.root", "READ");        
	TFile *hfile1= new TFile("test8runs.root", "READ");        

	//    getchar();
	//
          TPostScript psfile ("zRunNbadchan.ps", 111);
//

   c1 = new TCanvas("c1","Hcal4test",200,10,700,900);


    //=============================================================================================== 1
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(1,2);

      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_nbadchannels_depth1_HB");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("N bad channels over runs & events HB depth1 \b");
      aaaaaa1->SetYTitle("N events w/ bad channels \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->SetMinimum(0.8);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *twod1= (TH1F*)hfile1->Get("h_runnbadchannels_depth1_HB");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth1_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate1 = (TH1F*)twod1->Clone("Rate1");
	Rate1->Divide(twod1,twod0, 1, 1, "B");
	Rate1->Sumw2();
      }
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate1->SetMarkerStyle(20);
      Rate1->SetMarkerSize(0.4);
      Rate1->SetXTitle("iRun \b");
      Rate1->SetYTitle("< N bad channels > HB depth1 \b");
      Rate1->SetMarkerColor(2);
      Rate1->SetLineColor(2);
      Rate1->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 2
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(1,2);

      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_nbadchannels_depth2_HB");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("N bad channels over runs & events HB depth2 \b");
      aaaaaa1->SetYTitle("N events w/ bad channels \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->SetMinimum(0.8);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *twod1= (TH1F*)hfile1->Get("h_runnbadchannels_depth2_HB");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth2_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate2 = (TH1F*)twod1->Clone("Rate2");
	Rate2->Divide(twod1,twod0, 1, 1, "B");
	Rate2->Sumw2();
      }
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate2->SetMarkerStyle(20);
      Rate2->SetMarkerSize(0.4);
      Rate2->SetXTitle("iRun \b");
      Rate2->SetYTitle("< N bad channels > HB depth2 \b");
      Rate2->SetMarkerColor(2);
      Rate2->SetLineColor(2);
      Rate2->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 3
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(1,2);

      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_nbadchannels_depth1_HE");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("N bad channels over runs & events HE depth1 \b");
      aaaaaa1->SetYTitle("N events w/ bad channels \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->SetMinimum(0.8);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *twod1= (TH1F*)hfile1->Get("h_runnbadchannels_depth1_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth1_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate3 = (TH1F*)twod1->Clone("Rate3");
	Rate3->Divide(twod1,twod0, 1, 1, "B");
	Rate3->Sumw2();
      }
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate3->SetMarkerStyle(20);
      Rate3->SetMarkerSize(0.4);
      Rate3->SetXTitle("iRun \b");
      Rate3->SetYTitle("< N bad channels > HE depth1 \b");
      Rate3->SetMarkerColor(2);
      Rate3->SetLineColor(2);
      Rate3->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 4
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(1,2);

      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_nbadchannels_depth2_HE");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("N bad channels over runs & events HE depth2 \b");
      aaaaaa1->SetYTitle("N events w/ bad channels \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->SetMinimum(0.8);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *twod1= (TH1F*)hfile1->Get("h_runnbadchannels_depth2_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth2_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate4 = (TH1F*)twod1->Clone("Rate4");
	Rate4->Divide(twod1,twod0, 1, 1, "B");
	Rate4->Sumw2();
      }
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate4->SetMarkerStyle(20);
      Rate4->SetMarkerSize(0.4);
      Rate4->SetXTitle("iRun \b");
      Rate4->SetYTitle("< N bad channels > HE depth2 \b");
      Rate4->SetMarkerColor(2);
      Rate4->SetLineColor(2);
      Rate4->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 5
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(1,2);

      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_nbadchannels_depth3_HE");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("N bad channels over runs & events HE depth3 \b");
      aaaaaa1->SetYTitle("N events w/ bad channels \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->SetMinimum(0.8);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *twod1= (TH1F*)hfile1->Get("h_runnbadchannels_depth3_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth3_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate5 = (TH1F*)twod1->Clone("Rate5");
	Rate5->Divide(twod1,twod0, 1, 1, "B");
	Rate5->Sumw2();
      }
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate5->SetMarkerStyle(20);
      Rate5->SetMarkerSize(0.4);
      Rate5->SetXTitle("iRun \b");
      Rate5->SetYTitle("< N bad channels > HE depth3 \b");
      Rate5->SetMarkerColor(2);
      Rate5->SetLineColor(2);
      Rate5->Draw("");
      
      c1->Update();
      
      
            
    //=============================================================================================== 6
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(1,5);
      
      c1->cd(1);
      Rate1->SetMarkerStyle(20);
      Rate1->SetMarkerSize(0.8);
      Rate1->GetXaxis()->SetLabelSize(0.06);
      Rate1->GetYaxis()->SetLabelSize(0.06);
      Rate1->SetTitleSize(0.06,"XY" );
      Rate1->SetMarkerColor(2);
      Rate1->SetLineColor(2);
      Rate1->Draw("Error");
       
      c1->cd(2);
      Rate2->SetMarkerStyle(20);
      Rate2->SetMarkerSize(0.8);
      Rate2->GetXaxis()->SetLabelSize(0.06);
      Rate2->GetYaxis()->SetLabelSize(0.06);
      Rate2->SetTitleSize(0.06,"XY" );
      Rate2->SetMarkerColor(2);
      Rate2->SetLineColor(2);
      Rate2->Draw("Error");
       
      c1->cd(3);
      Rate3->SetMarkerStyle(20);
      Rate3->SetMarkerSize(0.8);
      Rate3->GetXaxis()->SetLabelSize(0.06);
      Rate3->GetYaxis()->SetLabelSize(0.06);
      Rate3->SetTitleSize(0.06,"XY" );
      Rate3->SetMarkerColor(2);
      Rate3->SetLineColor(2);
      Rate3->Draw("Error");
       
      c1->cd(4);
      Rate4->SetMarkerStyle(20);
      Rate4->SetMarkerSize(0.8);
      Rate4->GetXaxis()->SetLabelSize(0.06);
      Rate4->GetYaxis()->SetLabelSize(0.06);
      Rate4->SetTitleSize(0.06,"XY" );
      Rate4->SetMarkerColor(2);
      Rate4->SetLineColor(2);
      Rate4->Draw("Error");
       
      c1->cd(5);
      Rate5->SetMarkerStyle(20);
      Rate5->SetMarkerSize(0.8);
      Rate5->GetXaxis()->SetLabelSize(0.06);
      Rate5->GetYaxis()->SetLabelSize(0.06);
      Rate5->SetTitleSize(0.06,"XY" );
      Rate5->SetMarkerColor(2);
      Rate5->SetLineColor(2);
      Rate5->Draw("Error");
       
      c1->Update();
    //=============================================================================================== 7
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(1,3);
      
      c1->cd(1);
      Rate1->SetMarkerStyle(20);
      Rate1->SetMarkerSize(0.8);
      Rate1->GetXaxis()->SetLabelSize(0.06);
      Rate1->GetYaxis()->SetLabelSize(0.06);
      Rate1->SetTitleSize(0.06,"XY" );
      Rate1->SetMarkerColor(1);
      Rate1->SetLineColor(1);
      Rate1->Draw("Error");
       
      c1->cd(2);
      Rate2->SetMarkerStyle(20);
      Rate2->SetMarkerSize(0.8);
      Rate2->GetXaxis()->SetLabelSize(0.06);
      Rate2->GetYaxis()->SetLabelSize(0.06);
      Rate2->SetTitleSize(0.06,"XY" );
      Rate2->SetMarkerColor(1);
      Rate2->SetLineColor(1);
      Rate2->Draw("Error");
       
      c1->Update();
    //=============================================================================================== 8
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(1,3);
      
      c1->cd(1);
      Rate3->SetMarkerStyle(20);
      Rate3->SetMarkerSize(0.8);
      Rate3->GetXaxis()->SetLabelSize(0.06);
      Rate3->GetYaxis()->SetLabelSize(0.06);
      Rate3->SetTitleSize(0.06,"XY" );
      Rate3->SetMarkerColor(1);
      Rate3->SetLineColor(1);
      Rate3->Draw("Error");
       
      c1->cd(2);
      Rate4->SetMarkerStyle(20);
      Rate4->SetMarkerSize(0.8);
      Rate4->GetXaxis()->SetLabelSize(0.06);
      Rate4->GetYaxis()->SetLabelSize(0.06);
      Rate4->SetTitleSize(0.06,"XY" );
      Rate4->SetMarkerColor(1);
      Rate4->SetLineColor(1);
      Rate4->Draw("Error");
       
      c1->cd(3);
      Rate5->SetMarkerStyle(20);
      Rate5->SetMarkerSize(0.8);
      Rate5->GetXaxis()->SetLabelSize(0.06);
      Rate5->GetYaxis()->SetLabelSize(0.06);
      Rate5->SetTitleSize(0.06,"XY" );
      Rate5->SetMarkerColor(1);
      Rate5->SetLineColor(1);
      Rate5->Draw("Error");
       
      c1->Update();
    //======================================================================
    //============================================================================================= end
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
