#include "TProfile2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include <cassert>
#include "TLatex.h"
#include "TLine.h"

#include "DQM/RenderPlugins/src/MuonRenderPlugin.h"
#include "DQM/RenderPlugins/src/utils.h"



MuonRenderPlugin::MuonRenderPlugin() {

  label_resTk = new TLatex(2.5,4.5,"NA");
  label_resSTA = new TLatex(3.5,4.5,"NA");
  label_muonIdSTA = new TLatex(3.5,5.5,"NA");
  label_glbHO = new TLatex(1.5,3.5,"NA");
  label_tkHO = new TLatex(2.5,3.5,"NA");
  label_staHO = new TLatex(3.5,3.5,"NA");

}


bool MuonRenderPlugin::applies( const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {
 
  if((o.name.find( "Muons/E" ) != std::string::npos) ||
     (o.name.find( "Muons/M" ) != std::string::npos) ||
     (o.name.find( "Muons/R" ) != std::string::npos) ||
     (o.name.find( "Muons/S" ) != std::string::npos) ||
     (o.name.find( "Muons/T" ) != std::string::npos) ||
     (o.name.find( "Muons/c" ) != std::string::npos) ||
     (o.name.find( "Muons/g" ) != std::string::npos) ||
     (o.name.find( "Muons/s" ) != std::string::npos)) {
    return true;
  } 

  return false;

}


void MuonRenderPlugin::preDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i, VisDQMRenderInfo &r ) {

#ifdef DEBUG
  std::cout << "MuonRenderPlugin:preDraw " << o.name << std::endl;
#endif

  c->cd();

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    preDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TProfile*>( o.object ) ) {
    preDrawTProfile( c, o );
  }
  else if( dynamic_cast<TH2*>( o.object ) ) {
    preDrawTH2( c, o );
  }
  else if( dynamic_cast<TH1*>( o.object ) ) {
    preDrawTH1( c, o );
  }

#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}

void MuonRenderPlugin::preDrawTProfile2D( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void MuonRenderPlugin::preDrawTProfile( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void MuonRenderPlugin::preDrawTH2( TCanvas *c, const DQMNet::CoreObject &o ) {

  TH2F* obj = dynamic_cast<TH2F*>( o.object );
  assert( obj );


  // This applies to all
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBorderSize(0);

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  obj->SetStats(kFALSE);
  obj->SetOption("colz");

  if(obj->GetEntries() != 0) c->SetLogz(0);

  obj->GetXaxis()->SetLabelSize(0.06);
  obj->GetYaxis()->SetLabelSize(0.06);

   // Summary map
  if( o.name.find( "reportSummaryMap" ) != std::string::npos ) {
    dqm::utils::reportSummaryMapPalette(obj);
    obj->GetXaxis()->SetNdivisions(4,true);
    obj->GetYaxis()->SetNdivisions(8,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    return;
  }


  // ------------------- summary plots -------------------
  if(o.name.find("energySummaryMap") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(4,true);
    obj->GetYaxis()->SetNdivisions(4,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    obj->GetXaxis()->SetTitleOffset(1.15);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(2.0);
    
    int colorErrorDI[2];
    colorErrorDI[0] = 416;// kGreen
    colorErrorDI[1] = 632;// kRed
    gStyle->SetPalette(2, colorErrorDI);
    return;
  }


  if(o.name.find("kinematicsSummaryMap") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(6,true);
    obj->GetYaxis()->SetNdivisions(4,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    obj->GetXaxis()->SetTitleOffset(1.15);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(2.0);
    
    int colorErrorDI[2];
    colorErrorDI[0] = 416;// kGreen
    colorErrorDI[1] = 632;// kRed
    gStyle->SetPalette(2, colorErrorDI);
    return;
  }


  if(o.name.find("muonIdSummaryMap") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(3,true);
    obj->GetYaxis()->SetNdivisions(4,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    obj->GetXaxis()->SetTitleOffset(1.15);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(2.0);
    
    int colorErrorDI[2];
    colorErrorDI[0] = 416;// kGreen
    colorErrorDI[1] = 632;// kRed
    gStyle->SetPalette(2, colorErrorDI);
    return;
  }

  if(o.name.find("residualsSummaryMap") != std::string::npos) {
    obj->GetXaxis()->SetNdivisions(4,true);
    obj->GetYaxis()->SetNdivisions(5,true);
    obj->GetXaxis()->CenterLabels();
    obj->GetYaxis()->CenterLabels();
    c->SetGrid(1,1);
    obj->GetXaxis()->SetTitleOffset(1.15);
    c->SetBottomMargin(0.1);
    c->SetLeftMargin(0.15);
    c->SetRightMargin(0.12);
    obj->SetMinimum(-0.00000001);
    obj->SetMaximum(2.0);
    
    int colorErrorDI[2];
    colorErrorDI[0] = 416;// kGreen
    colorErrorDI[1] = 632;// kRed
    gStyle->SetPalette(2, colorErrorDI);
    return;
  }

}


void MuonRenderPlugin::preDrawTH1( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}


void MuonRenderPlugin::postDraw( TCanvas *c, const DQMNet::CoreObject &o, const VisDQMImgInfo &i ) {

#ifdef DEBUG
  std::cout << "MuonRenderPlugin:postDraw " << o.name << std::endl;
#endif

  c->cd();

  if( dynamic_cast<TProfile2D*>( o.object ) ) {
    postDrawTProfile2D( c, o );
  }
  else if( dynamic_cast<TProfile*>( o.object ) ) {
    postDrawTProfile( c, o );
  }
  else if( dynamic_cast<TH2*>( o.object ) ) {
    postDrawTH2( c, o );
  }
  else if( dynamic_cast<TH1*>( o.object ) ) {
    postDrawTH1( c, o );
  }


#ifdef DEBUG
  std::cout << "done" << std::endl;
#endif

}


void MuonRenderPlugin::postDrawTProfile2D( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void MuonRenderPlugin::postDrawTProfile( TCanvas *c, const DQMNet::CoreObject &o ) {

  return;

}

void MuonRenderPlugin::postDrawTH2( TCanvas *c, const DQMNet::CoreObject &o ) {

  if( o.name.find( "reportSummaryMap" ) != std::string::npos ) {
    label_resTk->Draw("same");
    label_resSTA->Draw("same");
    label_muonIdSTA->Draw("same");
    return;
  }

  if(o.name.find("energySummaryMap") != std::string::npos) {
    label_glbHO->Draw("same");
    label_tkHO->Draw("same");
    label_staHO->Draw("same");
    return;
  }

return;

}

void MuonRenderPlugin::postDrawTH1( TCanvas *c, const DQMNet::CoreObject &o ) {

return;

}

