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
	TFile *hfile1= new TFile("test.root", "READ");        
//	TFile *hfile1= new TFile("test_allruns.root", "READ");        

	//    getchar();
	//
          TPostScript psfile ("zRunRatio34.ps", 111);
//

   c1 = new TCanvas("c1","Hcal4test",200,10,700,900);


    //=========================================================================================== 1
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate_depth1_HB");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth1_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate10 = (TH1F*)twod1->Clone("Rate10");
	Rate10->Divide(twod1,twod0, 1, 1, "B");
	Rate10->Sumw2();
      }

      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate_depth2_HB");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth2_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate20 = (TH1F*)twod1->Clone("Rate20");
	Rate20->Divide(twod1,twod0, 1, 1, "B");
	Rate20->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate_depth1_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth1_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate30 = (TH1F*)twod1->Clone("Rate30");
	Rate30->Divide(twod1,twod0, 1, 1, "B");
	Rate30->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate_depth2_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth2_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate40 = (TH1F*)twod1->Clone("Rate40");
	Rate40->Divide(twod1,twod0, 1, 1, "B");
	Rate40->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate_depth3_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth3_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate50 = (TH1F*)twod1->Clone("Rate50");
	Rate50->Divide(twod1,twod0, 1, 1, "B");
	Rate50->Sumw2();
      }
      
      c1->Clear();
      c1->Divide(1,5);
      
      c1->cd(1);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate10->SetMarkerStyle(20);
      Rate10->SetMarkerSize(0.8);
      Rate10->GetXaxis()->SetLabelSize(0.06);
      Rate10->GetYaxis()->SetLabelSize(0.06);
      Rate10->SetTitleSize(0.06,"XY" );
      Rate10->SetXTitle("iRun \b");
      Rate10->SetYTitle("Rate if any badcell-HBdepth1\b");
      Rate10->SetMarkerColor(1);
      Rate10->SetLineColor(1);
      Rate10->Draw("Error");
       
      c1->cd(2);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate20->SetMarkerStyle(20);
      Rate20->SetMarkerSize(0.8);
      Rate20->GetXaxis()->SetLabelSize(0.06);
      Rate20->GetYaxis()->SetLabelSize(0.06);
      Rate20->SetTitleSize(0.06,"XY" );
      Rate20->SetXTitle("iRun \b");
      Rate20->SetYTitle("Rate if any badcell-HBdepth2\b");
      Rate20->SetMarkerColor(1);
      Rate20->SetLineColor(1);
      Rate20->Draw("Error");
       
      c1->cd(3);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate30->SetMarkerStyle(20);
      Rate30->SetMarkerSize(0.8);
      Rate30->GetXaxis()->SetLabelSize(0.06);
      Rate30->GetYaxis()->SetLabelSize(0.06);
      Rate30->SetTitleSize(0.06,"XY" );
      Rate30->SetXTitle("iRun \b");
      Rate30->SetYTitle("Rate if any badcell-HEdepth1\b");
      Rate30->SetMarkerColor(1);
      Rate30->SetLineColor(1);
      Rate30->Draw("Error");
       
      c1->cd(4);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate40->SetMarkerStyle(20);
      Rate40->SetMarkerSize(0.8);
      Rate40->GetXaxis()->SetLabelSize(0.06);
      Rate40->GetYaxis()->SetLabelSize(0.06);
      Rate40->SetTitleSize(0.06,"XY" );
      Rate40->SetXTitle("iRun \b");
      Rate40->SetYTitle("Rate if any badcell-HEdepth2\b");
      Rate40->SetMarkerColor(1);
      Rate40->SetLineColor(1);
      Rate40->Draw("Error");
       
      c1->cd(5);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate50->SetMarkerStyle(20);
      Rate50->SetMarkerSize(0.8);
      Rate50->GetXaxis()->SetLabelSize(0.06);
      Rate50->GetYaxis()->SetLabelSize(0.06);
      Rate50->SetTitleSize(0.06,"XY" );
      Rate50->SetXTitle("iRun \b");
      Rate50->SetYTitle("Rate if any badcell-HEdepth3\b");
      Rate50->SetMarkerColor(1);
      Rate50->SetLineColor(1);
      Rate50->Draw("Error");
       
      c1->Update();
    //=========================================================================================== 2
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate1_depth1_HB");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth1_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate11 = (TH1F*)twod1->Clone("Rate10");
	Rate11->Divide(twod1,twod0, 1, 1, "B");
	Rate11->Sumw2();
      }

      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate1_depth2_HB");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth2_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate21 = (TH1F*)twod1->Clone("Rate20");
	Rate21->Divide(twod1,twod0, 1, 1, "B");
	Rate21->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate1_depth1_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth1_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate31 = (TH1F*)twod1->Clone("Rate30");
	Rate31->Divide(twod1,twod0, 1, 1, "B");
	Rate31->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate1_depth2_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth2_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate41 = (TH1F*)twod1->Clone("Rate40");
	Rate41->Divide(twod1,twod0, 1, 1, "B");
	Rate41->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate1_depth3_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth3_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate51 = (TH1F*)twod1->Clone("Rate50");
	Rate51->Divide(twod1,twod0, 1, 1, "B");
	Rate51->Sumw2();
      }
      
      c1->Clear();
      c1->Divide(1,5);
      
      c1->cd(1);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate11->SetMarkerStyle(20);
      Rate11->SetMarkerSize(0.8);
      Rate11->GetXaxis()->SetLabelSize(0.06);
      Rate11->GetYaxis()->SetLabelSize(0.06);
      Rate11->SetTitleSize(0.06,"XY" );
      Rate11->SetXTitle("iRun \b");
      Rate11->SetYTitle("Rate if Nbadcells>cut1-HBdep1\b");
      Rate11->SetMarkerColor(2);
      Rate11->SetLineColor(2);
//      Rate11->Draw("Error");
       
      c1->cd(2);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate21->SetMarkerStyle(20);
      Rate21->SetMarkerSize(0.8);
      Rate21->GetXaxis()->SetLabelSize(0.06);
      Rate21->GetYaxis()->SetLabelSize(0.06);
      Rate21->SetTitleSize(0.06,"XY" );
      Rate21->SetXTitle("iRun \b");
      Rate21->SetYTitle("Rate if Nbadcells>cut1-HBdep2\b");
      Rate21->SetMarkerColor(2);
      Rate21->SetLineColor(2);
//      Rate21->Draw("Error");
       
      c1->cd(3);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate31->SetMarkerStyle(20);
      Rate31->SetMarkerSize(0.8);
      Rate31->GetXaxis()->SetLabelSize(0.06);
      Rate31->GetYaxis()->SetLabelSize(0.06);
      Rate31->SetTitleSize(0.06,"XY" );
      Rate31->SetXTitle("iRun \b");
      Rate31->SetYTitle("Rate if Nbadcells>cut1-HEdep1\b");
      Rate31->SetMarkerColor(2);
      Rate31->SetLineColor(2);
//      Rate31->Draw("Error");
       
      c1->cd(4);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate41->SetMarkerStyle(20);
      Rate41->SetMarkerSize(0.8);
      Rate41->GetXaxis()->SetLabelSize(0.06);
      Rate41->GetYaxis()->SetLabelSize(0.06);
      Rate41->SetTitleSize(0.06,"XY" );
      Rate41->SetXTitle("iRun \b");
      Rate41->SetYTitle("Rate if Nbadcells>cut1-HEdep2\b");
      Rate41->SetMarkerColor(2);
      Rate41->SetLineColor(2);
//      Rate41->Draw("Error");
       
      c1->cd(5);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate51->SetMarkerStyle(20);
      Rate51->SetMarkerSize(0.8);
      Rate51->GetXaxis()->SetLabelSize(0.06);
      Rate51->GetYaxis()->SetLabelSize(0.06);
      Rate51->SetTitleSize(0.06,"XY" );
      Rate51->SetXTitle("iRun \b");
      Rate51->SetYTitle("Rate if Nbadcells>cut1-HEdep3\b");
      Rate51->SetMarkerColor(2);
      Rate51->SetLineColor(2);
//      Rate51->Draw("Error");
       
      c1->Update();
    //=========================================================================================== 3
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate2_depth1_HB");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth1_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate12 = (TH1F*)twod1->Clone("Rate10");
	Rate12->Divide(twod1,twod0, 1, 1, "B");
	Rate12->Sumw2();
      }

      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate2_depth2_HB");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth2_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate22 = (TH1F*)twod1->Clone("Rate20");
	Rate22->Divide(twod1,twod0, 1, 1, "B");
	Rate22->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate2_depth1_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth1_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate32 = (TH1F*)twod1->Clone("Rate30");
	Rate32->Divide(twod1,twod0, 1, 1, "B");
	Rate32->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate2_depth2_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth2_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate42 = (TH1F*)twod1->Clone("Rate40");
	Rate42->Divide(twod1,twod0, 1, 1, "B");
	Rate42->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate2_depth3_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth3_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate52 = (TH1F*)twod1->Clone("Rate50");
	Rate52->Divide(twod1,twod0, 1, 1, "B");
	Rate52->Sumw2();
      }
      
      c1->Clear();
      c1->Divide(1,5);
      
      c1->cd(1);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate12->SetMarkerStyle(20);
      Rate12->SetMarkerSize(0.8);
      Rate12->GetXaxis()->SetLabelSize(0.06);
      Rate12->GetYaxis()->SetLabelSize(0.06);
      Rate12->SetTitleSize(0.06,"XY" );
      Rate12->SetXTitle("iRun \b");
      Rate12->SetYTitle("Rate if Nbadcells>cut2-HBdep1 \b");
      Rate12->SetMarkerColor(3);
      Rate12->SetLineColor(3);
//      Rate12->Draw("Error");
       
      c1->cd(2);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate22->SetMarkerStyle(20);
      Rate22->SetMarkerSize(0.8);
      Rate22->GetXaxis()->SetLabelSize(0.06);
      Rate22->GetYaxis()->SetLabelSize(0.06);
      Rate22->SetTitleSize(0.06,"XY" );
      Rate22->SetXTitle("iRun \b");
      Rate22->SetYTitle("Rate if Nbadcells>cut2-HBdep2 \b");
      Rate22->SetMarkerColor(3);
      Rate22->SetLineColor(3);
//      Rate22->Draw("Error");
       
      c1->cd(3);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate32->SetMarkerStyle(20);
      Rate32->SetMarkerSize(0.8);
      Rate32->GetXaxis()->SetLabelSize(0.06);
      Rate32->GetYaxis()->SetLabelSize(0.06);
      Rate32->SetTitleSize(0.06,"XY" );
      Rate32->SetXTitle("iRun \b");
      Rate32->SetYTitle("Rate if Nbadcells>cut2-HEdep1 \b");
      Rate32->SetMarkerColor(3);
      Rate32->SetLineColor(3);
//      Rate32->Draw("Error");
       
      c1->cd(4);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate42->SetMarkerStyle(20);
      Rate42->SetMarkerSize(0.8);
      Rate42->GetXaxis()->SetLabelSize(0.06);
      Rate42->GetYaxis()->SetLabelSize(0.06);
      Rate42->SetTitleSize(0.06,"XY" );
      Rate42->SetXTitle("iRun \b");
      Rate42->SetYTitle("Rate if Nbadcells>cut2-HEdep2 \b");
      Rate42->SetMarkerColor(3);
      Rate42->SetLineColor(3);
//      Rate42->Draw("Error");
       
      c1->cd(5);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate52->SetMarkerStyle(20);
      Rate52->SetMarkerSize(0.8);
      Rate52->GetXaxis()->SetLabelSize(0.06);
      Rate52->GetYaxis()->SetLabelSize(0.06);
      Rate52->SetTitleSize(0.06,"XY" );
      Rate52->SetXTitle("iRun \b");
      Rate52->SetYTitle("Rate if Nbadcells>cut2-HEdep3 \b");
      Rate52->SetMarkerColor(3);
      Rate52->SetLineColor(3);
//      Rate52->Draw("Error");
       
      c1->Update();
    //=========================================================================================== 4
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate3_depth1_HB");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth1_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate13 = (TH1F*)twod1->Clone("Rate10");
	Rate13->Divide(twod1,twod0, 1, 1, "B");
	Rate13->Sumw2();
      }

      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate3_depth2_HB");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth2_HB");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate23 = (TH1F*)twod1->Clone("Rate20");
	Rate23->Divide(twod1,twod0, 1, 1, "B");
	Rate23->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate3_depth1_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth1_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate33 = (TH1F*)twod1->Clone("Rate30");
	Rate33->Divide(twod1,twod0, 1, 1, "B");
	Rate33->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate3_depth2_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth2_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate43 = (TH1F*)twod1->Clone("Rate40");
	Rate43->Divide(twod1,twod0, 1, 1, "B");
	Rate43->Sumw2();
      }
      
      TH1F *twod1= (TH1F*)hfile1->Get("h_runbadrate3_depth3_HE");
      TH1F *twod0= (TH1F*)hfile1->Get("h_runbadrate0_depth3_HE");
      twod1->Sumw2();
      twod0->Sumw2();
      if(twod0->IsA()->InheritsFrom("TH1F")){
	TH1F* Rate53 = (TH1F*)twod1->Clone("Rate50");
	Rate53->Divide(twod1,twod0, 1, 1, "B");
	Rate53->Sumw2();
      }
      
      c1->Clear();
      c1->Divide(1,5);
      
      c1->cd(1);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate13->SetMarkerStyle(20);
      Rate13->SetMarkerSize(0.8);
      Rate13->GetXaxis()->SetLabelSize(0.06);
      Rate13->GetYaxis()->SetLabelSize(0.06);
      Rate13->SetTitleSize(0.06,"XY" );
      Rate13->SetXTitle("iRun \b");
      Rate13->SetYTitle("Rate if Nbadcells>cut3-HBdep1 \b");
      Rate13->SetMarkerColor(4);
      Rate13->SetLineColor(4);
//      Rate13->Draw("Error");
       
      c1->cd(2);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate23->SetMarkerStyle(20);
      Rate23->SetMarkerSize(0.8);
      Rate23->GetXaxis()->SetLabelSize(0.06);
      Rate23->GetYaxis()->SetLabelSize(0.06);
      Rate23->SetTitleSize(0.06,"XY" );
      Rate23->SetXTitle("iRun \b");
      Rate23->SetYTitle("Rate if Nbadcells>cut3-HBdep2 \b");
      Rate23->SetMarkerColor(4);
      Rate23->SetLineColor(4);
//      Rate23->Draw("Error");
       
      c1->cd(3);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate33->SetMarkerStyle(20);
      Rate33->SetMarkerSize(0.8);
      Rate33->GetXaxis()->SetLabelSize(0.06);
      Rate33->GetYaxis()->SetLabelSize(0.06);
      Rate33->SetTitleSize(0.06,"XY" );
      Rate33->SetXTitle("iRun \b");
      Rate33->SetYTitle("Rate if Nbadcells>cut3-HEdep1 \b");
      Rate33->SetMarkerColor(4);
      Rate33->SetLineColor(4);
//      Rate33->Draw("Error");
       
      c1->cd(4);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate43->SetMarkerStyle(20);
      Rate43->SetMarkerSize(0.8);
      Rate43->GetXaxis()->SetLabelSize(0.06);
      Rate43->GetYaxis()->SetLabelSize(0.06);
      Rate43->SetTitleSize(0.06,"XY" );
      Rate43->SetXTitle("iRun \b");
      Rate43->SetYTitle("Rate if Nbadcells>cut3-HEdep2 \b");
      Rate43->SetMarkerColor(4);
      Rate43->SetLineColor(4);
//      Rate43->Draw("Error");
       
      c1->cd(5);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate53->SetMarkerStyle(20);
      Rate53->SetMarkerSize(0.8);
      Rate53->GetXaxis()->SetLabelSize(0.06);
      Rate53->GetYaxis()->SetLabelSize(0.06);
      Rate53->SetTitleSize(0.06,"XY" );
      Rate53->SetXTitle("iRun \b");
      Rate53->SetYTitle("Rate if Nbadcells>cut3-HEdep3 \b");
      Rate53->SetMarkerColor(4);
      Rate53->SetLineColor(4);
//      Rate53->Draw("Error");
       
      c1->Update();
    //=========================================================================================== 5
    //======================================================================
    //======================================================================
    //================
      c1->Clear();
      c1->Divide(1,3);
      

      c1->cd(1);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate10->Draw("Error");
      Rate11->Draw("ErrorSame");
      Rate12->Draw("ErrorSame");
      Rate13->Draw("ErrorSame");
       
      c1->cd(2);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate20->Draw("Error");
      Rate21->Draw("ErrorSame");
      Rate22->Draw("ErrorSame");
      Rate23->Draw("ErrorSame");
       
      
      c1->Update();

    //=========================================================================================== 6
    //======================================================================
    //======================================================================
    //================
      c1->Clear();
      c1->Divide(1,3);
      

      c1->cd(1);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate30->Draw("Error");
      Rate31->Draw("ErrorSame");
      Rate32->Draw("ErrorSame");
      Rate33->Draw("ErrorSame");
       
      c1->cd(2);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate40->Draw("Error");
      Rate41->Draw("ErrorSame");
      Rate42->Draw("ErrorSame");
      Rate43->Draw("ErrorSame");
       
      c1->cd(3);
      //    gPad->SetGridy();
      //    gPad->SetGridx();
      Rate50->Draw("Error");
      Rate51->Draw("ErrorSame");
      Rate52->Draw("ErrorSame");
      Rate53->Draw("ErrorSame");
       
      
      c1->Update();

    //======================================================================
    //======================================================================
    //========================================================================================== end
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
