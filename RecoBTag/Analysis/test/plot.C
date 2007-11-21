
TObject* get(TFile & file, const string & name) {
  TObject* object = file.Get( name.c_str() );
  if (object == 0) {
    string message = "ERROR: cannot find object " + name;
    cerr << message << endl;
    throw( message );
  } 
  return object;
}

vector<string> get_list(TFile& file) {
  TPRegexp pcreJetTag("JetTag_(.*)BJetTags_GLOBAL");

  vector<string> list;
  for (unsigned int i = 0; i < file.GetListOfKeys()->GetSize(); ++i) {
    TString name( file.GetListOfKeys()->At(i)->GetName() );
    if (pcreJetTag.MatchB( name ))
      list.push_back( string( ((TObjString *) pcreJetTag.MatchS( name )->At(1))->GetString() ) );
  }

  return list;
}

void list(TFile & file) {
  vector<string> list = get_list(file);

  cout << "BJetTags" << endl;
  for (unsigned int i = 0; i < list.size(); i++)
      cout << '\t' << list[i] << endl;
  cout << endl;
}

enum zone_list {
	GLOBAL  = 0,
	BARREL  = 1,
	ENDCAP  = 2,
	FORWARD = 2
};

const char* zone_name[] = { "GLOBAL", "ETA_0-1.4", "ETA_1.4-2.4" };
const char* zone_description[] = { "", " (barrel)", " (forward)" };

enum flavour_color {
	b_color = 2,
	c_color = 4,
	x_color = 3,
	g_color = 1
};

void plot(TFile & file, const string & name, zone_list zone = GLOBAL, bool keep = false) {
  string folder = "JetTag_" + name + "BJetTags_" + zone_name[zone];
  string name_discriminant  = name + "_discriminant";
  string name_efficiency    = name + "_efficiency";
  string name_mistag        = name + "_mistag";
  string title_discriminant = name + ": discriminant by flavour";
  string title_efficiency   = name + ": efficiency vs. discriminator cut";
  string title_mistag       = name + ": mistag vs. b tag efficiency";
  string name_b_discr = folder + "/" + "discr_"                     + name + "BJetTags_" + zone_name[zone] + "B";
  string name_c_discr = folder + "/" + "discr_"                     + name + "BJetTags_" + zone_name[zone] + "C";
  string name_x_discr = folder + "/" + "discr_"                     + name + "BJetTags_" + zone_name[zone] + "DUS";
  string name_g_discr = folder + "/" + "discr_"                     + name + "BJetTags_" + zone_name[zone] + "G";
  string name_b_eff   = folder + "/" + "effVsDiscrCut_discrFC_"     + name + "BJetTags_" + zone_name[zone] + "B";
  string name_c_eff   = folder + "/" + "effVsDiscrCut_discrFC_"     + name + "BJetTags_" + zone_name[zone] + "C";
  string name_x_eff   = folder + "/" + "effVsDiscrCut_discrFC_"     + name + "BJetTags_" + zone_name[zone] + "DUS";
  string name_g_eff   = folder + "/" + "effVsDiscrCut_discrFC_"     + name + "BJetTags_" + zone_name[zone] + "G";
  string name_c_vs_b  = folder + "/" + "FlavEffVsBEff_C_discrFC_"   + name + "BJetTags_" + zone_name[zone];
  string name_x_vs_b  = folder + "/" + "FlavEffVsBEff_DUS_discrFC_" + name + "BJetTags_" + zone_name[zone];
  string name_g_vs_b  = folder + "/" + "FlavEffVsBEff_G_discrFC_"   + name + "BJetTags_" + zone_name[zone];
  
  // discriminant distribution, by flavour
  TCanvas* discriminant = new TCanvas(name_discriminant.c_str(), title_discriminant.c_str());
  float max = 0;

  TH1F* plot_b_discr = (TH1F*) get( file, name_b_discr );
  plot_b_discr->SetLineColor(b_color);
  plot_b_discr->SetLineWidth(2);
  if (plot_b_discr->GetMaximum() > max) max = plot_b_discr->GetMaximum();

  TH1F* plot_c_discr = (TH1F*) get( file, name_c_discr );
  plot_c_discr->SetLineColor(c_color);
  plot_c_discr->SetLineWidth(2);
  if (plot_c_discr->GetMaximum() > max) max = plot_c_discr->GetMaximum();

  TH1F* plot_x_discr = (TH1F*) get( file, name_x_discr );
  plot_x_discr->SetLineColor(x_color);
  plot_x_discr->SetLineWidth(2);
  if (plot_x_discr->GetMaximum() > max) max = plot_x_discr->GetMaximum();

  TH1F* plot_g_discr = (TH1F*) get( file, name_g_discr );
  plot_g_discr->SetLineColor(g_color);
  plot_g_discr->SetLineWidth(2);
  if (plot_g_discr->GetMaximum() > max) max = plot_g_discr->GetMaximum();

  plot_b_discr->SetMinimum(0.);
  plot_b_discr->SetMaximum(max * 1.1);
  plot_b_discr->SetTitle(title_discriminant.c_str());
  plot_b_discr->Draw("");
  plot_c_discr->Draw("same");
  plot_x_discr->Draw("same");
  plot_g_discr->Draw("same");
  discriminant->SetFillColor( kWhite );
  discriminant->SetGridx( true );
  discriminant->SetGridy( true );
  discriminant->SetFrameBorderMode( 0 );
  discriminant->SetFrameLineWidth( 2 );
  discriminant->SetCanvasSize(1280, 960);
  discriminant->SaveAs((name_discriminant + ".png").c_str());
  discriminant->SetCanvasSize(480, 320);
  discriminant->SaveAs((name_discriminant + "_small.png").c_str());

  // efficiency vs. discriminator cut, by flavour
  TCanvas* efficiency = new TCanvas(name_efficiency.c_str(), title_efficiency.c_str());

  TH1F* plot_b_eff = (TH1F*) get( file, name_b_eff );
  plot_b_eff->SetMarkerColor(b_color);
  plot_b_eff->SetMarkerSize(0.2);
  plot_b_eff->SetMarkerStyle(kFullDotMedium);
  if (plot_b_eff->GetMaximum() > max) max = plot_b_eff->GetMaximum();

  TH1F* plot_c_eff = (TH1F*) get( file, name_c_eff );
  plot_c_eff->SetMarkerColor(c_color);
  plot_c_eff->SetMarkerSize(0.2);
  plot_c_eff->SetMarkerStyle(kFullDotMedium);
  if (plot_c_eff->GetMaximum() > max) max = plot_c_eff->GetMaximum();

  TH1F* plot_x_eff = (TH1F*) get( file, name_x_eff );
  plot_x_eff->SetMarkerColor(x_color);
  plot_x_eff->SetMarkerSize(0.2);
  plot_x_eff->SetMarkerStyle(kFullDotMedium);
  if (plot_x_eff->GetMaximum() > max) max = plot_x_eff->GetMaximum();

  TH1F* plot_g_eff = (TH1F*) get( file, name_g_eff );
  plot_g_eff->SetMarkerColor(g_color);
  plot_g_eff->SetMarkerSize(0.2);
  plot_g_eff->SetMarkerStyle(kFullDotMedium);
  if (plot_g_eff->GetMaximum() > max) max = plot_g_eff->GetMaximum();

  plot_b_eff->SetMinimum(0.);
  if (plot_b_eff->GetMaximum() < 0.2) {
    // soft lepton
	plot_b_eff->SetMaximum(0.2);
  } else {
    // other algorithms
    plot_b_eff->SetMaximum(1.);
  }
  plot_b_eff->SetTitle(title_efficiency.c_str());
  plot_b_eff->Draw("");
  plot_c_eff->Draw("same");
  plot_x_eff->Draw("same");
  plot_g_eff->Draw("same");
  efficiency->SetFillColor( kWhite );
  efficiency->SetGridx( true );
  efficiency->SetGridy( true );
  efficiency->SetFrameBorderMode( 0 );
  efficiency->SetFrameLineWidth( 2 );
  efficiency->SetCanvasSize(1280, 960);
  efficiency->SaveAs((name_efficiency + ".png").c_str());
  efficiency->SetCanvasSize(480, 320);
  efficiency->SaveAs((name_efficiency + "_small.png").c_str());

  // mistag vs. efficency plot
  TCanvas* mistag = new TCanvas(name_mistag.c_str(), title_mistag.c_str());
  mistag->SetLogy(true);

  TH1F* plot_c_vs_b = (TH1F*) get( file, name_c_vs_b );
  plot_c_vs_b->SetMarkerColor(c_color);
  plot_c_vs_b->SetMarkerSize(0.2);
  plot_c_vs_b->SetMarkerStyle(kFullDotMedium);
  
  TH1F* plot_x_vs_b = (TH1F*) get( file, name_x_vs_b );
  plot_x_vs_b->SetMarkerColor(x_color);
  plot_x_vs_b->SetMarkerSize(0.2);
  plot_x_vs_b->SetMarkerStyle(kFullDotMedium);
  
  TH1F* plot_g_vs_b = (TH1F*) get( file, name_g_vs_b );
  plot_g_vs_b->SetMarkerColor(g_color);
  plot_g_vs_b->SetMarkerSize(0.2);
  plot_g_vs_b->SetMarkerStyle(kFullDotMedium);
  
  plot_c_vs_b->SetMinimum(0.0001);
  plot_c_vs_b->SetMaximum(1.);
  plot_c_vs_b->SetTitle(title_mistag.c_str());
  plot_c_vs_b->Draw("");
  plot_x_vs_b->Draw("same");
  plot_g_vs_b->Draw("same");
  
  mistag->SetFillColor( kWhite );
  mistag->SetGridx( true );
  mistag->SetGridy( true );
  mistag->SetFrameBorderMode( 0 );
  mistag->SetFrameLineWidth( 2 );
  mistag->SetCanvasSize(1280, 960);
  mistag->SaveAs((name_mistag + ".png").c_str());
  mistag->SetCanvasSize(480, 320);
  mistag->SaveAs((name_mistag + "_small.png").c_str());

  if (! keep) {
    delete discriminant;
	delete efficiency;
	delete mistag;
  }
}

// convenience functions with implicit file 

void list(void) {
  list( *_file0 );
}

void plot(const string & name, zone_list zone = GLOBAL, bool keep = false) {
  plot( *_file0, name, zone, keep );
}

// convenience macro for CMSSW 1.7.x
make_all_plots() {
  plot( "trackCountingHighEff" );
  plot( "trackCountingHighPur" );
  plot( "jetProbability" );
  plot( "jetBProbability" );
  plot( "impactParameterMVA" );
  plot( "softMuon" );
  plot( "softMuonNoIP" );
  plot( "softElectron", BARREL );
  plot( "combinedSecondaryVertex" );
  plot( "combinedSecondaryVertexMVA" );
  plot( "simpleSecondaryVertex" );
}
