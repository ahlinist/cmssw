
TObject* get(TFile & file, const string & name) {
  TObject* object = file.Get( name.c_str() );
  if (object == 0) {
    string message = "ERROR: cannot find object " + name;
    cerr << message << endl;
    throw( message );
  } 
  return object;
}

void list(TFile & file) {
  TPRegexp pcreJetTag("JetTag_(.*)BJetTags_GLOBAL");

  cout << "BJetTags" << endl;
  for (unsigned int i = 0; i < file.GetListOfKeys()->GetSize(); ++i) {
    TString name( file.GetListOfKeys()->At(i)->GetName() );
    if (pcreJetTag.MatchB( name ))
      cout << '\t' << ((TObjString *) pcreJetTag.MatchS( name )->At(1))->GetString() << endl;
  }
  cout << endl;
}

void plot(TFile & file, const string & name) {
  string folder = "JetTag_" + name + "BJetTags_GLOBAL";
  string name_mistag       = name + "_mistag";
  string name_discriminant = name + "_discriminant";
  string name_c_vs_b  = folder + "/" + "FlavEffVsBEff_C_discrFC_"   + name + "BJetTags_GLOBAL";
  string name_x_vs_b  = folder + "/" + "FlavEffVsBEff_DUS_discrFC_" + name + "BJetTags_GLOBAL";
  string name_g_vs_b  = folder + "/" + "FlavEffVsBEff_G_discrFC_"   + name + "BJetTags_GLOBAL";
  string name_b_discr = folder + "/" + "discr_" + name + "BJetTags_GLOBALB";
  string name_c_discr = folder + "/" + "discr_" + name + "BJetTags_GLOBALC";
  string name_x_discr = folder + "/" + "discr_" + name + "BJetTags_GLOBALDUS";
  string name_g_discr = folder + "/" + "discr_" + name + "BJetTags_GLOBALG";

  
  TCanvas* mistag = new TCanvas(name_mistag.c_str(), name.c_str(), 800, 600);
  mistag->SetLogy(true);
  
  TH1F* plot_c_vs_b = (TH1F*) get( file, name_c_vs_b );
  plot_c_vs_b->SetMarkerColor(4);
  plot_c_vs_b->SetMarkerSize(0.2);
  plot_c_vs_b->SetMarkerStyle(kFullDotMedium);
  
  TH1F* plot_x_vs_b = (TH1F*) get( file, name_x_vs_b );
  plot_x_vs_b->SetMarkerColor(3);
  plot_x_vs_b->SetMarkerSize(0.2);
  plot_x_vs_b->SetMarkerStyle(kFullDotMedium);
  
  TH1F* plot_g_vs_b = (TH1F*) get( file, name_g_vs_b );
  plot_g_vs_b->SetMarkerColor(1);
  plot_g_vs_b->SetMarkerSize(0.2);
  plot_g_vs_b->SetMarkerStyle(kFullDotMedium);
  
  plot_c_vs_b->SetMinimum(0.0001);
  plot_c_vs_b->SetMaximum(1.);
  plot_c_vs_b->Draw("");
  plot_x_vs_b->Draw("same");
  plot_g_vs_b->Draw("same");


  TCanvas* discriminant = new TCanvas(name_discriminant.c_str(), name.c_str(), 800, 600);
  float max = 0;

  TH1F* plot_b_discr = (TH1F*) get( file, name_b_discr );
  plot_b_discr->SetLineColor(2);
  plot_b_discr->SetLineWidth(2);
  if (plot_b_discr->GetMaximum() > max) max = plot_b_discr->GetMaximum();

  TH1F* plot_c_discr = (TH1F*) get( file, name_c_discr );
  plot_c_discr->SetLineColor(4);
  plot_c_discr->SetLineWidth(2);
  if (plot_c_discr->GetMaximum() > max) max = plot_c_discr->GetMaximum();

  TH1F* plot_x_discr = (TH1F*) get( file, name_x_discr );
  plot_x_discr->SetLineColor(3);
  plot_x_discr->SetLineWidth(2);
  if (plot_x_discr->GetMaximum() > max) max = plot_x_discr->GetMaximum();

  TH1F* plot_g_discr = (TH1F*) get( file, name_g_discr );
  plot_g_discr->SetLineColor(1);
  plot_g_discr->SetLineWidth(2);
  if (plot_g_discr->GetMaximum() > max) max = plot_g_discr->GetMaximum();

  plot_b_discr->SetMinimum(0.);
  plot_b_discr->SetMaximum(max * 1.1);
  plot_b_discr->Draw("");
  plot_c_discr->Draw("same");
  plot_x_discr->Draw("same");
  plot_g_discr->Draw("same");
}


// convenience functions with implicit file 

void list(void) {
  list( *_file0 );
}

void plot(const string & name) {
  plot( *_file0, name );
}

