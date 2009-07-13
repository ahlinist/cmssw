
class Comparator {

public:

  enum Mode {
    NORMAL,
    SCALE,
    EFF
  };


  Comparator() : rebin_(-1), xMin_(0), xMax_(0), resetAxis_(false), 
		 s0_(0), s1_(0), legend_(0,0,1,1) {}

  Comparator( const char* file0,
	      const char* dir0,
	      const char* file1,
	      const char* dir1 ) : 
    rebin_(-1), xMin_(0), xMax_(0), resetAxis_(false), 
    s0_(0), s1_(0), legend_(0,0,1,1) {
    
    SetDirs( file0, dir0, file1, dir1);
  }
  
  void SetDirs( const char* file0,
		const char* dir0,
		const char* file1,
		const char* dir1  ) {

    file0_ = new TFile( file0 );
    if( file0_->IsZombie() ) exit(1);
    dir0_ = file0_->GetDirectory( dir0 );
    if(! dir0_ ) exit(1);
    
    file1_ = new TFile( file1 );
    if( file1_->IsZombie() ) exit(1);
    dir1_ = file1_->GetDirectory( dir1 );
    if(! dir1_ ) exit(1);
  }

  // set the rebinning factor and the range
  void SetAxis( int rebin,
		float xmin, 
		float xmax) {
    rebin_ = rebin;
    xMin_ = xmin;
    xMax_ = xmax;
    resetAxis_ = true;
  }
  
  // set the rebinning factor, unset the range
  void SetAxis( int rebin ) {
    rebin_ = rebin;
    resetAxis_ = false;
  }
  
  // draws a Y projection of a slice along X
  void DrawSlice( const char* key, 
		  int binxmin, int binxmax, 
		  Mode mode ) {
    
    static int num = 0;
    
    ostrstream out0;
    out0<<"RECO2d_"<<num;
    ostrstream out1;
    out1<<"PAT2d_"<<num;
    num++;

    string name0 = out0.str();
    string name1 = out1.str();
      

    TH1* h0 = Histo( key, 0);
    TH1* h1 = Histo( key, 1);

    TH2* RECO2d = dynamic_cast< TH2* >(h0);
    TH2* PAT2d = dynamic_cast< TH2* >(h1);
    
    if(RECO2d->GetNbinsY() == 1 || 
       PAT2d->GetNbinsY() == 1 ) {
      cerr<<key<<" is not 2D"<<endl;
      return;
    }
    
    TH1::AddDirectory( false );

    TH1D* RECOslice = RECO2d->ProjectionY(name0.c_str(),
					binxmin, binxmax, "");
    TH1D* PATslice = PAT2d->ProjectionY(name1.c_str(),
					binxmin, binxmax, "");
    TH1::AddDirectory( true );
    Draw( RECOslice, PATslice, mode);        
  }


  void Draw( const char* key, Mode mode) {

    TH1::AddDirectory( false );
    TH1* h0 = Histo( key, 0);
    TH1* h1 = Histo( key, 1)->Clone("h1");
    int RECOentries = h0->GetEntries();
    int PATentries = h1->GetEntries();
    cout << " RECOentries =" << RECOentries << " PATentries =" <<  PATentries << endl;
//    gStyle->SetStatColor( (RECOentries == PATentries) ? 3 : 2); 
 
 //   double pv = h1->KolmogorovTest(h0,"OU");
 //   gStyle->SetStatColor(pv > 0.1 ? 3 : 2);
    TH1::AddDirectory( true );
    Draw( h0, h1, mode);
    gStyle->SetStatColor( (RECOentries == PATentries) ? 3 : 2);  
    
  }

  
  void Draw( const char* key0, const char* key1, Mode mode) {
    TH1* h0 = Histo( key0, 0);
    TH1* h1 = Histo( key1, 1);
    
    Draw( h0, h1, mode);
  }

  // cd to a give path
  void cd(const char* path ) {
    path_ = path;
  }
  
  // return the two temporary 1d histograms, that have just
  // been plotted
  TH1* h0() {return RECO;}
  TH1* h1() {return PAT;}

  const TLegend& Legend() {return legend_;}
  
  // set the styles for further plots
  void SetStyles( Style* s0, Style* s1,
		  const char* leg0,
		  const char* leg1) { 
    s0_ = s0; 
    s1_ = s1;
    
    legend_.Clear();
    legend_.AddEntry( s0_, leg0, "mlf");
    legend_.AddEntry( s1_, leg1, "mlf");
  }
  
private:

  // retrieve an histogram in one of the two directories
  TH1* Histo( const char* key, unsigned dirIndex) {
    if(dirIndex<0 || dirIndex>1) { 
      cerr<<"bad dir index: "<<dirIndex<<endl;
      return 0;
    }
    TDirectory* dir;
    if(dirIndex == 0) dir = dir0_;
    if(dirIndex == 1) dir = dir1_;
    
    dir->cd();

    TH1* h = (TH1*) dir->Get(key);
    if(!h)  
      cerr<<"no key "<<key<<" in directory "<<dir->GetName()<<endl;
    return h;
  }

  // draw 2 1D histograms.
  // the histograms can be normalized to the same number of entries, 
  // or plotted as a ratio.
  void Draw( TH1* h0, TH1* h1, Mode mode ) {
    if( !(h0 && h1) ) { 
      cerr<<"invalid histo"<<endl;
      return;
    }
    
    TH1::AddDirectory( false );
    RECO = (TH1*) h0->Clone( "RECO");
    PAT = (TH1*) h1->Clone( "PAT");
    TH1::AddDirectory( true );
    
    // unsetting the title, since the title of projections
    // is still the title of the 2d histo
    // and this is better anyway
    RECO->SetTitle("");
    PAT->SetTitle("");    

    RECO->SetStats(1);
    PAT->SetStats(1);

    if(rebin_>1) {
      RECO->Rebin( rebin_);
      PAT->Rebin( rebin_);
    }
    if(resetAxis_) {
      RECO->GetXaxis()->SetRangeUser( xMin_, xMax_);
      PAT->GetXaxis()->SetRangeUser( xMin_, xMax_);
    }

    switch(mode) {
    case SCALE:
      PAT->Scale( RECO->GetEntries()/PAT->GetEntries() );
    case NORMAL:
      if(s0_)
	FormatHisto( RECO , s0_);
      if(s1_)
 	FormatHisto( PAT , s1_);

      gStyle->SetStatColor(2);
      RECO->Draw();
      gPad->Update();
      TPaveStats *st = (TPaveStats*)RECO ->FindObject("stats");
      st->SetY1NDC(0.68);
      st->SetY2NDC(0.84);
      gStyle->SetStatColor(4);
      PAT->Draw("sames");
      break;
    case EFF:
      PAT->Divide( RECO );
      if(s1_)
 	FormatHisto( PAT , s0_);
      PAT->Draw();
    default:
      break;
    }
  }

  int rebin_;
  float xMin_;
  float xMax_;
  bool resetAxis_;

  TFile*      file0_;
  TDirectory* dir0_;
  TFile*      file1_;
  TDirectory* dir1_;
  
  TH1* RECO;
  TH1* PAT;
  
  Style* s0_;
  Style* s1_;
  
  TLegend legend_;

  string path_;
};

