// an Object for Jet (ID efficiencies) Certification

// Basic design: 
// - the "on" object is used for default parameters, and named paramaters.
// - the jet_studies is used to organize data.
//   But it is not used to encapsulate data, since this is to be used interactively and the
//   the data is only hidden from the CINT user.
//   - So most data members are public. Only those that hide hard coping should be private.
//   - utility functions (no globals, one output object) are kept outside the object (e.g. all of jc_util)
// - main method creates base histograms by number (what), eta region (iEta), and pT range (iPt)
// - subsequent building blocks start from previously created histograms/graphs
// - the building blocks (functions) will not recreate its objects if they already exist (unless forced by what<0)
// - building block functions should return a pointer (0 if failed) or an allOK bool (false if failed)
// - all objects (histograms, graphs) and output plots follow a similar naming convention which
//   basically reveals the commands used to create them (suffix describes input files)
// - additional outputs are stored in (member) maps, with a similar naming convention for the indices
// - function arguments are in the same order for all building block functions
// - BUT this is not important, as the named argument idion is used
// - each (level of) function uses debug values of 0-9 taken from the leats digit, and calls functions with iDebug/10 
// - regions (of eta, or pT):
//   - (most of) this design is implemented in rangeUtils.c and the classes therein
//   - indices are used directly in TObject names as "..E%dP%d".
//   - to make that useful, are always positive (but of type int)
//   - translation are available from regions indexes (eta,pT) are to short strings used in file names and long strings
//     used in legends. Also possible to search for range corresponding to desired user range (for interactive use).
//     - e.g. E101 or 0.087<#eta<0.174, A101 or 0.087<|#eta|<0.174, ? --> pt20_30 or 20<p_{T}<30, pt200 or 200<p_{T} 
//   - special names can happen. E.g. EB or 1.0<|#eta|<2.25
//   - 0 always means no cut
//   - any sort of cut may be used inside ,e.g., some eta ranges cut on |eta| instead.
//   - to keep the indices small, they are separate for eta and pT.

//
// Known problems:
// - code to select the best fit should be factorized and generalized for N fits
// - naming of graphs ignores negative range indices, which is sometimes needed, but messes
//   up the bookkeeping making it impossible to overwrite old values at times --> wrong plots outputed :-(
// - plotting of inefficiency graph is very time-wasting, as the events are transversed several times per point
//   - old solution of using 2D plots was much faster, but messed up nEntries, and caused small inaccuracies
//   - can speed up tremendously by introducing an event loop. Which is a completely different design.
//
// - trees produced before note freeze (<2009/5) have a bug in "tj" flag. Workaround: also require "ntj>0".
//

//
// Obviously helpful rewrites:
// - add "cut" and "plot" objects (or is it plot_options?)
//   - allow choosing a plot / cut by name.
//     - add some reverse parsing a-la rangeUtils' name2index
//   - allow using multiple cuts at once.
//   - all the draw options should be in the "plot" object
// - add the sample type and/or default analysis cuts to the jet_studies_input

//
// Wish list:
// - add a "batch" mode, where plots are registered as "wanted", and then all done at once, a-la Burt's CMSSW tool
//   - have inefficiency graphs use this
//   - have routines that produce all the subfigures in the big figures
//

//For alcic:
#include <iostream>
#include <stdio.h>
#include "TMath.h"
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TRegexp.h"
#include "TPRegexp.h"
#include "TLegend.h"
#include "TLine.h"
#include "TKey.h"
#include "TCut.h"
#include "TLatex.h"
#include "TArrow.h"
#include "TPaveStats.h"
#include "TPaletteAxis.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
//#include "TMultiGraph.h"
//#include "THStack.h"
#include "TProfile.h"
#include "TStyle.h"
//#include "TPostScript.h"
#include "TNtuple.h"
#include "TRandom3.h"

#include <limits>
//#include "stdlib.h"
#include <string>
#include <sstream>
#include <map>
#include <iterator>

#include "myMathUtils.c"
//#include "mystyle.c"
#include "histUtils.c"
#include "plotUtils.c"
#include "rangeUtils.c"
#include "debugUtils.c"

#include "jc_util.c"

using std::cout;
using std::endl;
using std::cerr;
using std::vector;

class jet_studies;

// chain names serve as titles, but for single trees, need the filename
const char* get_title( const TTree& tree )
{
  if( tree.InheritsFrom( "TChain" ) ) {
    return tree.GetTitle();
  } else {
    TString ss( tree.GetCurrentFile()->GetName() );
    TString basename ( ss( ss.Last('/')+1, ss.Length() ) );
    return basename.Data();
  }
}

const char* get_title( const TTree* tree )
{
  if( tree == 0 ) return Form( "NULL" );
  return get_title( *tree );
}

class jet_studies_input
{
 public:
  jet_studies_input( const TString& suffix, const TString& file_name );
  jet_studies_input( const TString& suffix, const TString& tree_path, const TString& file_mask );
  bool add( const TString& file_mask );
  //bool add( const char* chars ) {return add( TString( chars ) );}
  bool Add( const TString& file_mask ) {return add( file_mask );}

  static TTree* find_tree( TFile& file );
  TTree* tree();

  ~jet_studies_input();

  TString _suffix;
 private:
  TChain* _chain;
  TFile* _file;
};

jet_studies_input::jet_studies_input( const TString& suffix, const TString& file_name )
{
  _suffix = suffix;
  _chain = 0;
  _file = new TFile( file_name ); 
}

jet_studies_input::jet_studies_input( const TString& suffix, const TString& tree_path, const TString& file_mask )
{
  _suffix = suffix;
  _file = 0;
  _chain = new TChain( tree_path, suffix );
  add( file_mask );
}

jet_studies_input::~jet_studies_input()
{
  delete _file;
  delete _chain;
}

bool jet_studies_input::add( const TString& file_mask )
{
  if(! _chain ) {cerr<<"ERROR. Can't add a file to an input source that is a file, not a chain"<<endl; return false;}
  return _chain->Add( file_mask );
}

TTree* jet_studies_input::find_tree( TFile& file )
{
  TTree* tree = (TTree *) file.Get( "t" );
  if( tree == 0 ) {
    tree = (TTree *) file.Get( "jetid/t" );
    if( tree == 0 ) {
      tree = (TTree *) file.Get( "ak5JetID/t" );
      if( tree == 0 ) {
	cerr<<"Can't find the jet ID tree (in file: "<<file.GetPath()<<")!"<<endl;
      }
    }
  }
  return tree;
}

TTree* jet_studies_input::tree()
{
  if( _chain ) {
    return _chain;
  } else {
    if( 0 == _file ) {
      cerr<<"Can't figure out the tree underlying input \""<<_suffix<<"\""<<endl;
      return 0;
    } else {
      return find_tree( * _file );
    }
  }
}

// this class holds the arguments for the jet_studies methods.
// Conceptually: "ON what sample should we plot this?", 
// though almost all inputs are included, e.g., "what" should we plot
// Since typing "on" is much shorter than "jet_studies_arguments"...
class on {
 public:
  on( TFile& file );
  on( TTree& tree );
  on( jet_studies_input& input );
  on( int what, TFile& file );
  on( int what, TTree& tree );
  on( int what, jet_studies_input& input );
  void set_to_defaults ();
  void initialize_tree_from_file( TFile& file );

  bool valid() const;
  on& what( int what );
  on& abs_what( int abs_what );
  on& init( bool init );
  on& init() {return init( true );}
  on& var2( int var2 );
  on& oneD () {return var2( 0 );}
  on& cut( int cut );
  on& eta( int iEta );
  on& eta( const TString& name );
  on& pT( int iPt );
  on& pT( const TString& name );
  on& pt( int iPt ) {return pT( iPt );}
  on& pt( const TString& name ) {return pT( name );}
  on& suffix( const TString& suffix );
  on& how_much( int how_much );
  on& howMuch( int howMuch ) {return how_much( howMuch );}
  on& iDebug( int debug_level );
  on& debug( int debug_level ) {return iDebug( debug_level );}
  on& dbg( int debug_level ) {return iDebug( debug_level );}
  on& less_debug () {return iDebug( _debug / 10 );}
  on& fin( float fin );
  on& write_level( int write_level );
  on& iWrite( int iWrite ) {return write_level( iWrite );}
  on& read_only() {return write_level( 0 );}
  on& slice( int slice );
  on& min_entries( int min_entries );
  on& aux_probe( bool aux_probe );
  on& no_aux( bool no_aux );
  on& func( int func );
  on& iFunc( int iFunc ) {return func( iFunc );}
  on& min_pT( float min_pT ) ;
  on& minPt( float minPt ) {return min_pT( minPt );}
  on& remap( bool remap ) ;
  on& cut_value( float cut_value ) ;
  on& pass_above( bool pass_above ) ;
  on& pass_below() {return pass_above( false );}
  on& iList( int iList ) ;
  on& cut_list( int cut_list ) {return iList( cut_list );}
  on& cumulative( int cumulative ); // 0 = no, negative = from left, positive = from right
  on& sum( int direction ) {return cumulative( direction );}
  on& rejection( bool rejection );
  on& eff() {return rejection( false );}

  friend std::ostream& operator<< (std::ostream& outStream, const on& args);

  TTree* tree() {return _tree;} // for CINT debugging

 private:
  friend class jet_studies;
  int _what;
  bool _init;
  int _var2;
  int _cut;
  int _iEta;
  TString _eta_name;
  int _iPt;
  TString _pT_name;
  TTree* _tree;
  TString _suffix;
  int _how_much;
  int _debug;
  float _fin;
  int _write_level;
  int _slice;
  int _min_entries;
  bool _aux_probe;
  bool _no_aux;
  int _func;
  float _min_pT;
  bool _remap;
  float _cut_value;
  bool _pass_above;
  int _iList;
  int _cumulative;
  bool _rejection;
};

std::ostream& operator<< (std::ostream& outStream, const on& a)
{
  return outStream<<"what: "<<a._what<<", var2: "<<a._var2<<", cut: "<<a._cut
		  <<"; eta: (i "<<a._iEta<<" name "<<a._eta_name<<")"
		  <<"; pT: (i "<<a._iPt<<" name "<<a._pT_name<<")"
		  <<"; tree's title: "<<get_title( a._tree )<<", suffix: "<<a._suffix
		  <<"; how much: "<<a._how_much<<", debug: "<<a._debug<<", fin: "<<a._fin
		  <<"; write level: "<<a._write_level
		  <<"; slice: "<<a._slice<<", min_entries: "<<a._min_entries
		  <<"; aux probe? "<<a._aux_probe<<", no aux? "<<a._no_aux
		  <<"; func: "<<a._func<<", init? "<<a._init
		  <<"; min pT: "<<a._min_pT<<", remap? "<<a._remap
		  <<"; iList: "<<a._iList<<", cumulative: "<<a._cumulative
		  <<"; cut value: "<<a._cut_value<<", pass_above? "<<a._pass_above<<", rejection? "<<a._rejection;
}

on::on( TTree& tree )
{
  set_to_defaults();
  _tree = & tree;
}

on::on( TFile& file )
{
  set_to_defaults();
  initialize_tree_from_file( file );
}

on::on( jet_studies_input& input )
{
  set_to_defaults();
  _tree = input.tree();
  _suffix = input._suffix;
}

on::on( int what, TTree& tree )
{
  set_to_defaults();
  _tree = & tree;
  this->what( what );
}

on::on( int what, TFile& file )
{
  set_to_defaults();
  initialize_tree_from_file( file );
  this->what( what );
}

on::on( int what, jet_studies_input& input )
{
  set_to_defaults();
  _tree = input.tree();
  this->what( what );
  _suffix = input._suffix;
}

void on::set_to_defaults ()
{
  _what = 0;
  _init = false;
  _var2 = 0;
  _cut = 0;
  _iEta = 0;
  _eta_name = "";
  _iPt = 0;
  _pT_name = "";
  _tree = 0;
  _suffix = "";
  _how_much = 3;
  _debug = 0;
  _fin = 0;
  _write_level = 10;
  _slice = 0;
  _min_entries = 100;
  _aux_probe = false;
  _no_aux = false;
  _func = 3;
  _min_pT = 50;
  _remap = true;
  _cut_value = 0.5;
  _pass_above = true;
  _iList = 0;
  _cumulative = 0;
  _rejection = true;
}

void on::initialize_tree_from_file( TFile& file )
{
  _tree = jet_studies_input::find_tree( file );
}

bool on::valid () const
{
  if( _iEta != 0 && ! _eta_name.IsNull() ) return false;
  if( _iPt  != 0 && ! _pT_name.IsNull()  ) return false;
  if( _what < 0 ) return false;
  return _tree;
}



inline on& on::what( int what )
{
  _what = TMath::Abs( what );
  _init = what < 0;
  return *this;
}

inline on& on::abs_what( int abs_what )
{
  _what = TMath::Abs( abs_what );
  return *this;
}

inline on& on::var2( int var2 )
{
  _var2 = var2;
  return *this;
}

inline on& on::cut( int cut )
{
  _cut = cut;
  return *this;
}

inline on& on::eta( int iEta )
{
  _iEta = iEta;
  return *this;
}

inline on& on::eta( const TString& name )
{
  _eta_name = name;
  return *this;
}

inline on& on::pT( int iPt )
{
  _iPt = iPt;
  return *this;
}

inline on& on::pT( const TString& name )
{
  _pT_name = name;
  return *this;
}

inline on& on::suffix( const TString& suffix )
{
  _suffix = suffix;
  return *this;
}

inline on& on::how_much( int how_much )
{
  _how_much = how_much;
  return *this;
}

inline on& on::iDebug( int debug_level )
{
  _debug = debug_level;
  return *this;
}

inline on& on::fin( float fin )
{
  _fin = fin;
  return *this;
}

inline on& on::write_level( int write_level )
{
  _write_level = write_level;
  return *this;
}

inline on& on::slice( int slice )
{
  _slice = slice;
  return *this;
}

inline on& on::min_entries( int min_entries )
{
  _min_entries = min_entries;
  return *this;
}

inline on& on::aux_probe( bool aux_probe )
{
  _aux_probe = aux_probe;
  return *this;
}

inline on& on::no_aux( bool no_aux )
{
  _no_aux = no_aux;
  return *this;
}

inline on& on::func( int func )
{
  _func = func;
  return *this;
}

inline on& on::init( bool init )
{
  _init = init;
  return *this;
}

inline on& on::min_pT( float min_pT )
{
  _min_pT = min_pT;
  return *this;
}

inline on& on::remap( bool remap )
{
  _remap = remap;
  return *this;
}

inline on& on::cut_value( float cut_value )
{
  _cut_value = cut_value;
  return *this;
}

inline on& on::pass_above( bool pass_above )
{
  _pass_above = pass_above;
  return *this;
}

inline on& on::iList( int iList )
{
  _iList = iList;
  return *this;
}

inline on& on::cumulative( int cumulative )
{
  _cumulative = cumulative;
  return *this;
}

inline on& on::rejection( bool rejection )
{
  _rejection = rejection;
  return *this;
}

// ==================================================================================================

class jet_studies {

 public:  
  // construction / destruction
  void set_defaults ();
  void init_eta_ranges ();
  void init_pT_ranges ();
  jet_studies ();

  // public methods

  //----------------------------------------------------------------------------------------------
  // Methods to be called from CINT prompt:
  // All methods accepting args are assumed to use _tree, _suffix, and _debug
  
  // Uses: what, var2, cut, eta, pT, how_much, fin, write_level
  // how_much: 0 = if historam already exists and !init, do nothing. If not, as in case "1"
  //           1 = prepare (as much as possible) without plotting
  //           2 = plot but don't save to disk
  //           3 = everything, including saving to disk
  // write_level > 10 forces how_much = 3
  // 
  // To force a re-projection (loop over nTuple), use what<0. Without, calling howMuch==1
  // for an existing histogram just sets histogram options.
  TH1* plot( const on& args );
  TH1* operator() ( const on& args ) { return plot( args ); }

  // Makes an eta phi map
  // Uses: cut, eta, pT, how_much, fin
  TH2* eta_phi( const on& args );
  // Makes an online DQM summary plot (phys vs. L1P)
  // Uses: cut, eta, pt, how_much, fin
  TH2* online( const on& args );

  // Wrappers for "plot" that meant to add a kind of a ROOT-ish safe cast - Note changed "howMuch" default!
  TH1F* getTH1F( const on& args );
  TH1D* getTH1D( const on& args );
  TH2F* getTH2F( const on& args );

  // Optional 2nd stage processing of histograms - messes up nEntries, so trying to avoid it...
  // Uses: what, var2, cut, eta, pT, write_level
  bool slice( const on& args );
  // Uses: what, var2, cut, eta, pT, slice
  TH1D* get_slice( const on& args );
  // Uses: what, var2, cut, eta, pT, write_level, fin
  bool slices( const on& args );
  // Uses: what, var2, cut, eta, pT, min_entries
  void get_slice_hists( vector<TH1*> &hists, const on& args );


  // Uses: what, var2 (!?), cut, eta, pT, min_entries, fin, aux_probe, no_aux
  //
  // Gets histograms (from main function, but conceptually slices) underlying an inefficiency plot.
  // Also returns the corresponding auxiliary histograms, of the x-variable within the bin (to be remapped)
  //
  // - either iEta or iPt must be negative, that index will be looped over, starting from -input
  //   - e.g. normal case is to scan in iPt starting from 30 (tagpt) --> input -30.
  // - args._what is the base variable being plotted. E.g. giving jtpt makes remapping histograms, 
  // - unless auxprobe is true, which means that instead of plotting tag (pt) in auxiliary, we plot probe jet (pt)
  //
  // Design notes:
  // - this code is in its own function since it's used by both prepRemapGraph & MakeInEff
  // - title & captions are extracted here, since this is where the indexing is known (and all callers will need it)
  void get_hists( vector<TH1*> &hists, vector<TH1*> &xHists, TString& title, vector<TString> &captions,
		  const on& args );


  // Uses: what, var2 (!?), cut, eta, pT, func
  //
  // - iFunc included to avoid memory management problems when refitting existing TGraph
  // - either iEta or iPt must be negative, see get_hists
  // - args._what is "what are we remapping?", normally jtpt
  TGraphErrors* get_remap_graph( const on& args );
  
  // Uses: what, var2 (!?), cut, eta, pT, func
  TF1* fit_remapping( const on& args );

  // get_remap_graph and fit_remapping are to some extend two halfs of the same function
  // Note odd memory management coupling (historically a workaround for ROOT problems)
  // and the debugging control here
  //


  // Note: may have to switch to remapping functions at some point --> new functions with different interface...
  // Uses: what, var2 (!?), cut, eta, pT, func
  bool plot_remap( const on& args );
  
  // Scans on either iEta or iPt, see getHists above
  // Uses: what, cut, eta, pT, cut_value, pass_above, remap, min_pT, min_entries, fin, aux_probe
  TGraphAsymmErrors* get_ineffs( const on& args );
  
  // Uses: what, cut, eta, pT, cut_value, pass_above, remap, min_pT, min_entries, fin, aux_probe
  bool plot_ineff( const on& args );
  		   
  // interface to sets of cuts
  // Uses: what, cut, eta, pT, remap, min_pT, min_entries, fin
  bool plot_set_ineff( const on& args )
  {
    return plot_ineff( on( args ).aux_probe( false ).cut_value( 0.5 ).pass_above( true ) );
  }
  
  bool plot_track_ineff( const on& args )
  {
    return plot_ineff( on( args ).aux_probe( true ).remap( false ).pass_above( true ).pt( -210 ) );
  }

  // interface for low pT, no remapping, inefficiencies
  // Uses: what, cut, eta, pT, cut_value, pass_above, min_entries, fin
  bool plot_rough_ineff( const on& args )
  {
    return plot_ineff( on( args ).remap( false ).min_pT( 0 ).aux_probe( true ) ); 
  }

  // compare x-sections by vetodeltaPhi dist. with and without the default veto
  // Uses: eta, pT, write_level, fin
  bool xsecs( const on& args );

  // Uses: write_level
  bool plotXsecs( const on& args );
  
  
  // compare deltaPhi dist. (but not normalization) with various vetoes
  // Uses: eta, pT, fin, write_level
  bool dphis( const on& args, int rebin = 4 );
 
  // Uses: fin, write_level
  bool plot_dphis( const on& args );

  // compare deltaPhi dist. & normalization with and without the default veto
  // Uses: eta, pT, write_level, fin
  bool dvj( const on& args, int rebin = 1 );
  
  // Uses: what, cut, eta, pT, write_level, fin
  bool bin_bias( const on& args );

  // Uses: what, eta, pT, write_level, fin
  bool trigger_bias( const on& args );

  // Uses: eta, pT, howMuch, fin
  bool trigger_pT_bias( const on& args );

  // Uses: eta, pT, write_level, fin
  bool old_trigger_bias( const on& args );
  
  // Indicate an eta scan with a negative eta, others the default pT (0) indicates a pT scan
  // Uses: what, var2 (?!), cut, eta, pT, write_level, min_entries, fin (?)
  bool scan_and_overlay( const on& args, int rebin = 1 );

  bool overlay_hists( vector<TH1*> hists, vector<TString> captions, const TString& title,
		      const on& args, const TString& file_prefix,
		      bool contrast = false,
		      float leg_min_x = 0.15, float leg_min_y = 0.6, // legend lower left corner
		      float total_legs_width = 0.85, // total width allowed for all legends
		      float leg_line_height = 0.1,   // height of each entry in the legends
		      float max_leg_width = -1, //positive maximum enforced. Width of individual legened.
		      int rebin = 0, // <=1 = don't rebin
		      int user_logy = 0 // negative = off, positive = on, 0 = whatever's in the gPad
		      );
  bool overlay_hists( TH1& h1, TH2& h2, const on& args,
		      bool contrast = false,
		      float leg_min_x = 0.15, float leg_min_y = 0.6, // legend lower left corner
		      float total_legs_width = 0.85, // total width allowed for all legends
		      float leg_line_height = 0.1,   // height of each entry in the legends
		      float max_leg_width = -1, //positive maximum enforced. Width of individual legened.
		      int rebin = 0, // <=1 = don't rebin
		      int user_logy = 0 // negative = off, positive = on, 0 = whatever's in the gPad
		      ) {
    vector<TH1*> hists;
    vector<TString> captions;
    hists.push_back( &h1 );
    hists.push_back( &h2 );
    captions.push_back( "debug h1" );
    captions.push_back( "debug h2" );
    return overlay_hists( hists, captions, "", args, "debug", contrast, 
			  leg_min_x, leg_min_y, total_legs_width, leg_line_height,
			  max_leg_width, rebin, user_logy );
  }

  bool plot_two( const on& args, int what2, bool subtract = false, bool logVal = false,
		 int colorScheme = 0, const TString statOpt = "nemr" );

  
  bool old_plot_two( const on& args, int what2, bool logVal = false, int colorScheme = 0 );

  vector<int> cut_list_at( int index = 0 );
  // Uses: iList, what, var2 (?!), cut, eta, pT, write_level, min_entries, fin (?)
  bool xsec_by_cuts( const on& args, int units = -666 );
  // Uses: what, var2 (?!), cut, eta, pT, write_level, min_entries, fin (?)
  bool xsec_by_cuts( const on& args, vector<int> cuts, int units = -666 );
  // Uses: iList, what, var2 (?!), cut, pT, write_level, min_entries, fin (?)
  bool xsec_by_cuts_all_regions( const on& args, int units = -666 );
  // Uses: iList, what, var2 (?!), cut, pT, write_level, min_entries, fin (?)
  bool xsec_by_cuts_all_regions( const on& args, vector<int> cuts, int units = -666 );

// -----------------------------------------------------------------------------------------------

  void default_veto_normal ()    { _default_dj_bits = 0xFF; }
  void default_veto_corrected () { _default_dj_bits = 0x15; }
  void default_veto_original ()  { _default_dj_bits = 0x05; }


 
 
//-------------------------------------------------------------------------------------------------
// Utilities:

  int iEta( const on& args );
  int iPt ( const on& args );

  range eta_range_at( int index ) {return _eta_ranges.rangeAt( index );}
  range pT_range_at( int index )  {return _pT_ranges.rangeAt( index );}

  // negative return = failure
  int find_eta_range( float min, float max, bool abs = true );
  int find_pT_range( float min, float max );

  unsigned int n_colors () {return TMath::Min( _gradual_colors.size(), _widths.size() ); }
  unsigned int n_contrasting_colors () {return TMath::Min( _contrasting_colors.size(), _widths.size() ); }

  // this is essentially a wrapper around TString::Form + a look up of my ranges, of course
  // prefixes are used when it'll be useful to sort resulting filenames accordings
  // Notice: illegal indices all result in same empty name. If this is problematic, fix is have written below...
  // BUT: for now this is used to avoid indices in InEfficiency graph names :-)
  // todo: split into two functions accordingly?
  char* build_name( int what, int var2, int cut, int iEta, int iPt, 
		    const TString& suffix = "", const TString& prefix = "" );
  char* build_name( int what, int iEta, int iPt, 
		    const TString& suffix = "", const TString& prefix = "" );
  char* build_name( int what, const TString& suffix = "", const TString& prefix = "");

  // -----------------------------------------------------------------------------------------------
  // public data members
  std::string _name;
  int _default_dj_bits;
  std::map<std::string, double> _doubles;
  rangeMap _pT_ranges, _eta_ranges;
  Int_t _fit_color, _default_hist_color, _hist_fill_color, _default_canvas_width, _default_canvas_height;
  Int_t _fit_width;
  
 private: // typedefs
 private: // members
  int _n_pT_bins; // these get compared to signed ints later, so they're not defined as unsigned
  int _n_pT_bins_2TeV; // these get compared to signed ints later, so they're not defined as unsigned
  int _n_eta_bins;
  int _n_coarse_pT_bins;
  int _n_coarse_eta_bins;
  int _n_mid_eta_bins;
  double _highest_eta;

  std::vector< Int_t > _contrasting_colors, _gradual_colors;
  std::vector< Int_t > _widths;

}; // end of jet_studies class definition

jet_studies::jet_studies () 
{
  set_defaults();
  init_eta_ranges();
  init_pT_ranges();
}

void jet_studies::set_defaults ()
{
  _name = "oj";
  _default_dj_bits = 0xFF; // 0xFF is normal. 0x5 are the old loose cuts. 0x15 are the corrected loose cuts.
  _highest_eta = 5.2;
  Int_t ca[] = {kBlack, kRed+1,  kBlue,         kSpring-6, kCyan+2, kViolet,
		kOrange-3, kPink+7, kYellow+3,    kAzure-3, kMagenta, kTeal-6};
  _contrasting_colors = std::vector< Int_t > ( ca, ca+12 );
  Int_t ga[] = {kBlack, kRed-1,  kRed+2,  kRed,        kPink+7, kMagenta, kViolet+2, kBlue+1,
		kAzure-3, kCyan, kTeal-6, kSpring-6,   kYellow+3, kOrange-3, kOrange+10};
  _gradual_colors = std::vector< Int_t > ( ga, ga+15 );
  Int_t wa[] = {1, 2, 2, 2,    2, 2, 2, 2,    2, 2, 2, 2,    2, 2, 2};
  _widths = std::vector< Int_t > (wa, wa+15);
  _fit_color = 7;
  _fit_width = 2;
  _default_hist_color = kMagenta+4; // very dark, but being non-black helps figure editing (e.g. manually for .eps)
  _hist_fill_color = kYellow;
  _default_canvas_width = 550;
  _default_canvas_height = 400;
}

// must be kept syncronyzed with get_hists
void jet_studies::init_eta_ranges ()
{
  static const float big_float = std::numeric_limits<float>::max();
  rangeMap& map = _eta_ranges;
  map.clear();
  map[0] = range( "", 0, big_float, "", "" );

  const int nAreas = 5;
  float areaEdges [1+nAreas] = {0, 1.0, 1.75, 2.6, 3.25, big_float}; // was 2.55 in CRAFT08 studies
  TString areaNames [nAreas] = {"HB","BE","HE","EF","HF"};
  _n_coarse_eta_bins = nAreas;
  // Note: CaloJetIDAnalyzer calls the jet.eta() - jtdeta (jteta has PVZ0 correction)
  map.initWithBins(  1, "abs(jtdeta)", "?", "E", nAreas, areaEdges, 0, 0, 2, areaNames );
  map.initWithBins( 11,     "jtdeta" , "?", "E", nAreas, areaEdges, 1, 0, 2, areaNames, "+" );
  map.initWithBins( 21,     "jtdeta" , "?", "E", nAreas, areaEdges, 1, 1, 2, areaNames, "-" );

  map[30] = range( "abs(jtdeta)<2.5", 0, 2.5, "HBHE", "|#eta|<2.5" );
  map[31] = range( "abs(jtdeta)>=2.5", 2.5, big_float, "EDGE", "|#eta|#geq2.5" );

  map[33] = range( "abs(jtdeta)<1.75", 0, 2.5, "E33", "|#eta|<1.75" );
  map[34] = range( "0.3<=abs(jtdeta) && abs(jtdeta)<0.5", 0.3, 0.5, "E34", "0.5>|#eta|#geq0.3" );
  map[35] = range( "1.6<=abs(jtdeta) && abs(jtdeta)<1.8", 1.6, 1.8, "E35", "1.8>|#eta|#geq1.6" );
  map[36] = range( "1.0<=abs(jtdeta) && abs(jtdeta)<1.8", 1.0, 1.8, "E36", "1.8>|#eta|#geq1.0" );
  map[37] = range( "1.3<=abs(jtdeta) && abs(jtdeta)<3.0", 1.3, 3.0, "E37", "3.0>|#eta|#geq1.3" ); // HE related
  map[38] = range( "1.0<=abs(jtdeta) && abs(jtdeta)<1.3", 1.0, 1.3, "E38", "1.3>|#eta|#geq1.0" ); // HB edge (for RBX noise)
  map[39] = range( "1.5<=abs(jtdeta) && abs(jtdeta)<1.8", 1.5, 1.8, "E39", "1.5>|#eta|#geq1.8" );
  map[40] = range( "4.0<=abs(jtdeta)", 4.0, big_float, "E40", "|#eta|#geq4.0" );
  map[41] = range( "4.2<=abs(jtdeta)", 4.2, big_float, "E41", "|#eta|#geq4.2" );

  const int nAreas4 = 10;
  float area4Edges [1+nAreas4] = {0, 0.5, 1.0, 1.4, 1.8, 2.2, 2.6, 2.9, 3.2, 3.6, big_float};
  TString area4Names [nAreas4] = {"HB1","HB2", "HBE","HEB", "HE1","HE2", "HEF","HFE", "HF1","HF2"};
  _n_mid_eta_bins = nAreas4;
  map.initWithBins( 51, "abs(jtdeta)", "?", "E", nAreas4, area4Edges, 0, 0, 2, area4Names );
  map.initWithBins( 61,     "jtdeta" , "?", "E", nAreas4, area4Edges, 1, 0, 2, area4Names, "+" );
  map.initWithBins( 71,     "jtdeta" , "?", "E", nAreas4, area4Edges, 1, 1, 2, area4Names, "-" );

  map[86] = range( "2.6<=abs(jtdeta) && abs(jtdeta)<2.98-4*jtetaeta", 2.6, 3.0, "HE2*", "HE2*" );
  map[93] = range( "1.75<=abs(jtdeta) && abs(jtdeta)<2.98-4*jtetaeta", 1.75, 2.98, "HE*", "HE*" );
  map[94] = range( "2.98-4*jtetaeta<=abs(jtdeta) && abs(jtdeta)<3.02+4*jtetaeta", 2.98, 3.02, "EF*", "EF*" );
  map[95] = range( "3.02+4*jtetaeta<=abs(jtdeta)", 3.02, big_float, "HF*", "HF*" );
  //  TCut on_EF_crack( "abs(abs(jtdeta)-3)<(0.02+4*jtetaeta)" );

  const int nBins = 41;
  float binEdges [1+nBins] = {0, 
     0.087, 0.174, 0.261, 0.348, 0.435,   0.522, 0.609, 0.696, 0.783, 0.879,   
     0.957, 1.044, 1.131, 1.218, 1.305,   1.392, 1.479, 1.566, 1.653, 1.740, 
     1.830, 1.930, 2.043, 2.172, 2.322,   2.500, 2.650, 
     2.853, // note: iEta 28 is HE and covers up to 3.0, but we'll assign jets at ~2.9 to iEta 29 (with HF)
     2.964, 3.139,
     3.314, 3.489, 3.664, 3.839, 4.013,   4.191, 4.363, 4.538, 4.716, 4.889, 
     5.191};
  _n_eta_bins = nBins;
  map.initWithBins( 100, "abs(jtdeta)", "|#eta|", "E", nBins, binEdges, 0, 0, 3 );
  map.initWithBins( 200,     "jtdeta" ,  "#eta" , "E", nBins, binEdges, 1, 0, 3 );
  map.initWithBins( 300,     "jtdeta" ,  "#eta" , "E", nBins, binEdges, 1, 1, 3 );

  float areaEdgesNew [1+nAreas] = {0, 1.0, 1.75, 2.7, 3.25, big_float}; // Move HE end to 2.7 to lie beyond HPD peak
  TString areaNamesNew [nAreas] = {"HB'","BE'","HE'","EF'","HF'"};
  // Note: CaloJetIDAnalyzer calls the jet.eta() - jtdeta (jteta has PVZ0 correction)
  map.initWithBins( 401, "abs(jtdeta)", "?", "E", nAreas, areaEdgesNew, 0, 0, 2, areaNamesNew );
  map.initWithBins( 411,     "jtdeta" , "?", "E", nAreas, areaEdgesNew, 1, 0, 2, areaNamesNew, "+" );
  map.initWithBins( 421,     "jtdeta" , "?", "E", nAreas, areaEdgesNew, 1, 1, 2, areaNamesNew, "-" );
}    

// must be kept syncronyzed with get_hists
void jet_studies::init_pT_ranges ()
{
  static const float big_float = std::numeric_limits<float>::max();
  rangeMap& map = _pT_ranges;
  map.clear();
  map[0] = range( "", 0, big_float, "", "" );
  const int nCoarse = 3;
  float coarseEdges [1+nCoarse] = {0, 50, 100, big_float};
  _n_coarse_pT_bins = nCoarse;
  map.initWithBins( 1, "tagpt", "p'_{T}", "P", nCoarse, coarseEdges );
  map.initWithBins( 6, "jtpt",  "p_{T}",  "P", nCoarse, coarseEdges );
  // why do it with the biases jet pt???
  //map[1] = range (            "jtpt<50" ,   0,        50, "P1",     "p_{T}<50");
  //map[2] = range ("50<=jtpt && jtpt<100",  50,       100, "P2",  "50<p_{T}<100");
  //map[3] = range ("100<=jtpt"           , 100, big_float, "P3", "100<p_{T}");

  map[4] = range( "jtpt<35", 0, 35, "P4", "p_{T}<35" );
  map[5] = range( "35<=jtpt && jtpt<120", 35, 120, "P5", "35#leqp_{T}<120" );
  map[9] = range( "jtpt<25", 0, 25, "P9", "p_{T}<25" );

  const int nBins = 13;
  float binEdges [1+nBins] = {0, 10, 20,  30, 50, 80,  130, 210, 340,  550, 900, 1500,  3000, big_float}; 
  _n_pT_bins = nBins;
  map.initWithBins( 10, "jtpt", "p_{T}", "P", nBins, binEdges );
  map.initWithBins( 30, "tagpt", "p'_{T}", "P", nBins, binEdges );
  map.initWithBins( 50, "(tagpt+jtpt)/2", "(p_{T}+p'_{T})/2", "P", nBins, binEdges );
  const int nBins2Tev = 10;
  float binEdges2Tev [1+nBins] = {0, 5, 10, 20,  35, 60, 100,  160, 280, 550, big_float}; 
  _n_pT_bins_2TeV = nBins2Tev;
  map.initWithBins( 210, "jtpt", "p_{T}", "P", nBins2Tev, binEdges2Tev );
  map.initWithBins( 230, "tagpt", "p'_{T}", "P", nBins2Tev, binEdges2Tev );
  map.initWithBins( 250, "(tagpt+jtpt)/2", "(p_{T}+p'_{T})/2", "P", nBins2Tev, binEdges2Tev );
 
  // soft @ CRAFT08 studies
  map[70] = range( "jtpt<25 && jtrawpt>1", 1, 25, "P70", "1<p_{T}^{raw}, p_{T}<25" );
  map[71] = range( "jtpt<25 && jtrawpt>5", 5, 25, "P71", "5<p_{T}^{raw}, p_{T}<25" );
  // HF @ CRAFT08 studies
  map[77] = range( "jtrawpt<35",                 0, 35,        "P77", "p_{T}^{raw}<35" );
  map[78] = range( "35<=jtrawpt && jtrawpt<70", 35, 70,        "P78", "35#leqp_{T}^{raw}<70" );
  map[79] = range( "70<=jtrawpt",               70, big_float, "P79", "70#leqp_{T}^{raw}" );
  map[80] = range( "jtrawpt<10",                 0, 10,        "P80", "p_{T}^{raw}<10" );
  map[81] = range( "10<=jtrawpt && jtrawpt<30", 10, 30,        "P81", "10#leqp_{T}^{raw}<30" );
  map[82] = range( "35<jtrawpt && jtrawpt<45",  35, 45,        "P82", "35<p_{T}^{raw}<45" );
  map[83] = range( "35<jtrawpt",                35, big_float, "P83", "35<p_{T}^{raw}" );
  // HBHE @ CRAFT08 studies
  map[85] = range( "50<=jtpt && jtpt<200", 50, 200, "P85", "50#leqp_{T}<200" ); // HB RBX 20+
  map[86] = range( "40<=jtpt && jtpt<500", 40, 500, "P86", "40#leqp_{T}<500" );
  map[87] = range( "50<=jtpt && jtpt<1000", 50, 1000, "P87", "50#leqp_{T}<1000" );
  map[88] = range( "20<=jtpt && jtpt<55", 20, 55, "P88", "20#leqp_{T}<55" );
  map[89] = range( "80<=jtpt && jtpt<600", 80, 600, "P89", "600#leqp_{T}<800" );
  map[90] = range( "jtrawpt>5", 5, big_float, "P90", "p_{T}^{raw}>5GeV" );
  // HE @ CRAFT09 studies
  map[91] = range( "jtrawpt<5",                    0,  5,        "P91", "p_{T}^{raw}<5" );
  map[92] = range( "7.5<=jtrawpt && jtrawpt<25", 7.5, 25,        "P92", "7.5#leqp_{T}^{raw}<25" );
  map[93] = range( "40<=jtrawpt",                 40, big_float, "P93", "40#leqp_{T}^{raw}" );
  map[94] = range( "jtrawpt>10", 10, big_float, "P94", "p_{T}^{raw}>10GeV" );
  // Beam splash studies
  const int n_beam_splash_HB = 4;
  float beam_splash_HB [1+n_beam_splash_HB] = {0, 100, 500, 1900, big_float};
  map.initWithBins( 110, "jtrawpt",  "p_{T}^{raw}",  "P", n_beam_splash_HB, beam_splash_HB );
  map[115] = range( "jtrawpt<200",    0,       200, "P115", "p_{T}^{raw}<200" );
  map[116] = range( "200<=jtrawpt", 200, big_float, "P116", "200#leqp_{T}^{raw}" );
  map[117] = range( "jtrawpt<650",    0,       650, "P117", "p_{T}^{raw}<650" );
  map[118] = range( "650<=jtrawpt", 650, big_float, "P118", "650#leqp_{T}^{raw}" );

 
  map[120] = range( "tagpt>10", 10, big_float, "P120", "p'_{T}>10" );

  map[170] = range( "5<=jtrawpt&&jtrawpt<50",  5, 50       , "P170", "5#leqp_{T}^{raw}<50" );
  map[171] = range( "50<=jtrawpt",            50, big_float, "P171", "50#leqp_{T}^{raw}" );
  map.initWithBins( 180, "jtrawpt", "p_{T}^{raw}", "P", nBins2Tev, binEdges2Tev );
  // todo: add a check that the titles are unique... (I've had too many typos before)
}

// negative return = failure
int jet_studies::find_eta_range( float min, float max, bool abs )
{
  if( (min < 0 || max < 0) && abs ) {
    cout<<"Warning: assuming you really meant to look for non-abs bins!"<<endl; 
    abs=false;
  }
  cout<<"Using coarse bins:";
  int out = _eta_ranges.findRanges( min, max, abs, 1, 99 );
  out = TMath::Min( out, _eta_ranges.findRanges( min, max, abs, 200, 299 ) );
  cout<<"Using fine bins:";
  out = TMath::Min( out, _eta_ranges.findRanges( min, max, abs, 100, 199 ) );
  out = TMath::Min( out, _eta_ranges.findRanges( min, max, abs, 300, 399 ) );
  return out;
}

int jet_studies::find_pT_range( float min, float max )
{
  cout<<"Using coarse bins:";
  int out1 = _pT_ranges.findRanges (min, max, false, 1, 99);
  cout<<"Using fine bins:";
  int out2 = _pT_ranges.findRanges (min, max, false, 100, 400);
  return TMath::Min( out1, out2 );
}


TString join_text( const TString& s1, const TString& s2, const TString& sep=", " )
{
  TString out( s1 );
  if( ( !s1.IsWhitespace() ) && ( !s2.IsWhitespace() ) ) out += sep;
  out += s2;
  return out;
}
  
TString joint_caption( const range& etaRange, const range& pTRange )
{
  return join_text( etaRange.caption, pTRange.caption, "," );
}

void add_to_text( TString& org, const TString& addition, const TString& sep=", " )
{
  org = join_text( org.Strip(), addition, sep);
}

char* jet_studies::build_name( int what, int var2, int cut, int iEta, int iPt, 
			       const TString& suffix, const TString& prefix )
{
  TString sVar2, sCut;
  if( var2 ) sVar2 = Form( "S%d", var2 );
  if( cut ) sCut = Form("C%d", cut );
  return Form( "j%s%d%s%s%s%s%s", prefix.Data(), what, sVar2.Data(), sCut.Data(), 
	       eta_range_at( iEta ).cname(), pT_range_at( iPt ).cname(), suffix.Data() );
}

char* jet_studies::build_name( int what, int iEta, int iPt, 
			       const TString& suffix, const TString& prefix )
{
  return Form( "j%s%d%s%s%s", prefix.Data(), what, eta_range_at( iEta ).cname(), 
	                      pT_range_at( iPt ).cname(), suffix.Data() );
}

char* jet_studies::build_name( int what, const TString& suffix, const TString& prefix )
{
  return build_name( what, 0, 0, suffix, prefix );
}

int get_tree_format (TTree& tree)
{
  // so far, only needed to keep track of format changes that effect "WeightCut"
  TLeaf* plr = tree.GetLeaf( "jtrawpt" );
  TLeaf* plr2 = tree.GetLeaf( "dj" );
  if( plr && plr2 ) return 4;
  if( plr ) return 3;
  return 2; // the 1st format is no longer around (I hope)
}

// works around a little interface inconsistency in ROOT 5-20-00
TFile* get_file( const TTree& tree )
{
  if( tree.InheritsFrom( "TChain" ) ) {
    TChain *pc = (TChain *) &tree;
    return pc->GetFile();
  } else {
    return tree.GetCurrentFile();
  }
}

TH1* accumulation_of( const TH1& org, bool from_left = true )
{
  TString clone_name = org.GetName();
  clone_name += ( from_left ? "L" : "R" );
  TH1* clone = (TH1*) org.Clone( clone_name );
  accumulate( clone, from_left );
  clone->SetMaximum();
  clone->GetYaxis()->SetTitle( join_text( clone->GetYaxis()->GetTitle(), 
					  from_left ? "below" : "above",
					  " " ) );
  clone->GetXaxis()->SetTitle( join_text( clone->GetXaxis()->GetTitle(), "threshold", " " ) );
  return clone;
}


int jet_studies::iEta( const on& args )
{
  if( ! args._eta_name.IsNull() ) {
    if( args._iEta != 0 ) {
      cerr<<"Error! both the eta bin's index and caption were specified"<<endl;
      return INT_MIN;
    }
    int index = _eta_ranges.name2index( args._eta_name );
    if( index == INT_MIN ) {
      index = _eta_ranges.caption2index( args._eta_name );
      if( index == INT_MIN ) {
	cerr<<"Error! Given iEta name/caption ("<<args._eta_name<<") wasn't found"<<endl;
      }
    }
    return index;
  }
  return args._iEta;
}

int jet_studies::iPt( const on& args )
{
  if( ! args._pT_name.IsNull() ) {
    if( args._iPt != 0 ) {
      cerr<<"Error! both the pT bin's index and caption were specified"<<endl;
      return INT_MIN;
    }
    int index = _pT_ranges.name2index( args._pT_name );
    if( index == INT_MIN ) {
      index = _pT_ranges.caption2index( args._pT_name );
      if( index == INT_MIN ) {
	cerr<<"Error! Given iPt name/caption ("<<args._pT_name<<") wasn't found"<<endl;
      }
    }
    return index;
  }
  return args._iPt;
}

// ==========================================================================================
TH1* jet_studies::plot( const on& args )
{
  if( ! args.valid() ) {cerr<<"jet_studies::plot called with invalid args ("<<args<<")."<<endl; return 0;}
  const int what = args._what;
  const int var2 = args._var2;
  const int iCut = args._cut;
  const int eta = iEta( args );
  if( eta < 0 ) {cerr<<"Warning: unknown eta range in "<<_name<<": "<<eta<<endl; return 0;}
  const int pT = iPt( args );
  TTree& tree = *args._tree;
  const int how_much = ( args._write_level > 10 ) ? TMath::Max( 3, args._how_much ) : args._how_much;
  const int iDebug = args._debug;
  const float fin = args._fin;
  
  TString hist_name( build_name( args._what, args._var2, args._cut, eta, pT, args._suffix ) );
  TH1* hBase = (TH1*) gDirectory->Get( hist_name );
  if( how_much == 0 && hBase != 0 && ! args._init) return hBase;

  bool init = args._init;
  // do we need an automatic init?
  if( hBase == 0 ) init = 1;
  cout<<"Making basic plot #"<<what;
  if( var2 != 0 ) cout<<", var2: "<<var2;
  if( iCut != 0 ) cout<<", iCut: "<<iCut;
  cout<<" for eta range #"<<eta<<" & pT range #"<<pT;
  if( fin != 0 ) cout<<", fin: "<<fin;
  cout<<", init? "<<init<<endl;

  TString tree_title( get_title( tree ) );
  TString tree_title_lowercase( tree_title ); tree_title_lowercase.ToLower();
  bool weighted_tree = tree_title_lowercase.Contains ("w") || tree_title_lowercase.Contains ("mc");
  if( tree_title_lowercase.Contains( "flat" ) ) weighted_tree = false;
  bool craft_08_data = tree_title.Contains ("hf");
  bool craft_data = craft_08_data || tree_title_lowercase.Contains( "cr" ) || args._suffix.Contains( "cr" );
  bool min_bias = tree_title_lowercase.Contains ("mb");
  bool beam_splash = tree_title_lowercase.Contains ("bs");
  bool first_data = tree_title_lowercase.BeginsWith( "fd" );
  bool early_data = tree_title_lowercase.BeginsWith( "ed" );
  bool year_09_data = tree_title.BeginsWith( "n" ) && TString(tree_title(1,4)).IsDigit();
  int tree_title_number = -1;
  TString title_end( tree_title );
  if( tree_title_lowercase.BeginsWith("r") && !tree_title_lowercase.Contains( "mc" ) ) title_end.Remove( 0, 1 );
  if( tree_title_lowercase.BeginsWith("run") ) title_end.Remove( 0, 3 );
  if( tree_title_lowercase.BeginsWith("cr") ) title_end.Remove( 0, 2 );
  if( tree_title_lowercase.BeginsWith("ed") ) title_end.Remove( 0, 2 );
  if( title_end != tree_title ) tree_title_number = title_end.Atoi();
  bool commissioning_run = ( tree_title_number > 0 && tree_title_number < 124000 );
  bool LHC_run = tree_title_number > 0;
  bool no_hard_collisions = craft_data || min_bias || beam_splash || first_data || commissioning_run;
  if( tree_title.IsDigit() && tree_title.Atoi() < 120000 ) craft_data = true;
  if( iDebug > 10 ) cerr<<"tree_title: "<<tree_title<<" -> craft_data? "<<craft_data
			<<", run#"<<tree_title_number<<endl;
  if( get_file( tree ) ) {
    TString file_path( get_file( tree )->GetPath() );
    if( file_path.Contains( "rt" ) ) craft_data = true;
    if( iDebug > 10 ) cerr<<"file_path: "<<file_path<<" -> craft_data? "<<craft_data<<endl;
  }
  int format_version = get_tree_format (tree);
  
  // Get a canvas using Rene Brun's stop gag measure circa 2000. Anything better available?
  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) {c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );}

  bool gridx = 1, gridy = 1, logx = 0, logy = 0, logz = 0, hide_stat = false;
  int optTitle = 1;
  TString opt_stat( "nemr" );
  float stat_x_low = 0.65, stat_y_low = 0.7, stat_x_high = -1, stat_y_high = -1;
  float rotate_phi = -666,  marker_size = -1, top_fraction = -1;
  bool use_MC_weight = weighted_tree;
  bool norm_it = false, pack_the_flows = true, equalize_x_max = true;
  
  range eta_range = eta_range_at( eta );
  if( eta > 0 && eta_range.isEmpty() ) {cerr<<"Warning: unknown eta range in "<<_name<<": "<<eta<<endl; return 0;}
  range pT_range = pT_range_at( pT );
  if( pT > 0 && pT_range.isEmpty() ) {cerr<<"Warning: unknown pT range in "<<_name<<": "<<pT<<endl; return 0;}

  // The follow bug was there until late in the ARC review, but had little effect 
  //TCut oldDeltaCut ("abs(tagdelta)<0.1 || (abs(tagdelta)<0.2 && tagpt>80) || (abs(tagdelta)<0.3 && tagpt>210)");
  // delta cut for the loose veto (either original or current unbiassing version):
  TCut loose_delta_cut ("abs(tagdelta)<0.1 || (abs(tagdelta)<0.2 && tagpt<210) || (abs(tagdelta)<0.3 && tagpt<80)");
  TCut tight_delta_cut ("abs(tagdelta)<0.1 || (abs(tagdelta)<0.2 && tagpt<80) || (abs(tagdelta)<0.3 && tagpt<50)");
  TCut cal_veto ("vjet3pt<min(50,0.5*min(jtpt,tagpt))");
  TCut dj_cut = loose_delta_cut && cal_veto;
  TCut corr_cal_veto ("vjet3pt<min(50,0.5*tagpt)");
  TCut corr_dj_cut = loose_delta_cut && corr_cal_veto;
  TCut tight_cal_veto ("vjet3pt<0.1*tagpt");
  TCut tight_dj_cut = tight_delta_cut && tight_cal_veto;
  //if( iDebug ) cerr<<"DBG: format_version: "<<format_version<<" -> dj_cut: "<<dj_cut.GetTitle()<<endl;

  TCut minimal08( "abs(jtdeta)>2.6 || jtemf>0.01" );
  TCut loose08( minimal08 && "jtf1hpd<0.98 && jthn90>1.5"
		        " && (abs(jtdeta)<2.6 || (jtemf>-0.9 && (jtpt<80 || jtemf<1) ) )" );
  TCut tight08( minimal08  &&
		"jtf1hpd<0.98 && (jtrawpt<25 || jtf1hpd<0.95) && jthn90>1.5 && (jtpt<80 || abs(jtdeta)<1 || jtemf<1) "
		" && (abs(jtdeta)<2.6 || abs(jtdeta)>3.25 || "
		      "(jtemf>-0.3 && (jtpt<50 || jtemf>-0.2) && (jtpt<80 || jtemf>-0.1) && (jtpt<340 || jtemf<0.95)) )"
		" && (abs(jtdeta)<3.25 || (jtemf>-0.3 && jtemf<0.9 && (jtpt<50 || (jtemf>-0.2 && jtemf<0.8))"
		                                                 " && (jtpt<130 || (jtemf>-0.1 && jtemf<0.7))))");
  TCut tight08_EF_EMF_cut( "jtemf>-0.3 && (jtpt<50 || jtemf>-0.2) && (jtpt<80 || jtemf>-0.1) && (jtpt<340 || jtemf<0.95)" );
  TCut outside_HF( "abs(jtdeta)<3.25" );
  TCut HF_tower_cut_neto( "jtntwrs>1+1.1111*(log(jtrawpt)-1.6)" );
  TCut HF_tight_tower_cut_neto( "jtntwrs>1+1.2*(log(jtrawpt)-1)" );
  TCut HF_tower_cut( outside_HF || HF_tower_cut_neto );
  TCut HF_tight_tower_cut( outside_HF || HF_tight_tower_cut_neto );
  TCut HF_EMF_cut_neto( "jtemf<1-0.15385*(log(jtrawpt)-2.2) && jtemf>min(-0.3,-1+0.4375*(log(jtrawpt)-0.6))" );
  TCut HF_tight_max_EMF_cut_neto( "jtemf<1-0.15385*(log(jtrawpt)-0.9)" );
  TCut HF_tight_min_EMF_cut_neto( "((log(jtrawpt)<=2.)&&(jtemf>-0.6)&&(jtemf>-0.6+(log(jtrawpt)-0.8)/3.))"
				  " || ((log(jtrawpt)>2.)&&(jtemf>-0.2+(log(jtrawpt)-2.0)/15.))");
  TCut HF_tight_EMF_cut_neto( HF_tight_max_EMF_cut_neto && HF_tight_min_EMF_cut_neto );
  // && jtemf>min(-0.3,-1+0.4375*(log(jtrawpt)-0.6))" );
  TCut HF_cuts_neto( HF_tower_cut_neto && HF_EMF_cut_neto );
  TCut HF_tight_cuts_neto( HF_tight_tower_cut_neto && HF_tight_EMF_cut_neto );
  TCut HF_cuts( outside_HF || HF_cuts_neto );
  TCut HF_tight_cuts( outside_HF || HF_tight_cuts_neto );
  TCut outside_EF( "abs(jtdeta)<2.6 || abs(jtdeta)>3.25" );
  TCut EF_tower_cut_neto( "jtntwrs>log(jtrawpt)-0.4" );
  TCut EF_tower_cut( outside_EF || EF_tower_cut_neto );
  TCut on_EF_crack( "abs(abs(jtdeta)-3)<(0.02+4*jtetaeta)" );
  TCut loose085( minimal08 && "jtf1hpd<0.98 && jthn90>1.5"
		 " && (abs(jtdeta)<2.6 || abs(jtdeta)>3.25 && (jtemf>-0.9 && (jtpt<80 || jtemf<1) ) )" );
  loose085 = loose085 && ( outside_EF || EF_tower_cut_neto ) && HF_cuts;
  TCut tight085( minimal08 && 
		 "jtf1hpd<0.98 && (jtrawpt<25 || jtf1hpd<0.95) && jthn90>1.5 && (jtpt<80 || abs(jtdeta)<1 || jtemf<1) " );
  tight085 = tight085 && ( outside_EF ||( EF_tower_cut_neto && tight08_EF_EMF_cut ) );
  tight085 = tight085 && HF_tight_cuts;

  TCut fLS_tight( "(jtfls<0.7 && jtfls<0.7-(log(jtrawpt)-1.5)/4) || (log(jtrawpt)>3.9 && jtfls<0.1)" );
  TCut fLS_loose( "(jtfls<0.8 && jtfls<0.8-(log(jtrawpt)-1.5)/4) || (log(jtrawpt)>3.9 && jtfls<0.2)" );
  TCut tight09v1( minimal08 && "jtf1hpd<0.98 && (jtrawpt<25 || jtf1hpd<0.95) && jthn90>1.5" );
  tight09v1 = tight09v1 && ( outside_HF || (HF_tight_tower_cut_neto && fLS_tight));
  TCut loose09v1( minimal08 && "jtf1hpd<0.98 && jthn90>1.5" );
  loose09v1 = loose09v1 && ( outside_HF || (HF_tower_cut_neto && fLS_loose));

  TCut cur_cut; // add regions later, to ease parsing
  TCut craft_hot_cell_cut ( "jtphi<-2.38 || jtphi>-2.33 || jtdeta<-3.55 || jtdeta>-3.2" ); 
  craft_hot_cell_cut += "jtphi<1.055 || jtphi >1.135 || jtdeta<1.3 || jtdeta>1.4 || jtpt<21 || jtpt>27"; 
  craft_hot_cell_cut += "jtphi<0.65 || jtphi>0.77 || jtdeta<-1.16 || jtdeta>-1.14";
  craft_hot_cell_cut += "jtphi<-0.43634 || jtphi>-0.43633 || jtdeta<3.6 || jtdeta>4.3";
  TCut first_data_hot_cell_cut( "jtphi<-2.95 || jtphi>-2.7 || jtdeta<-2.31 || jtdeta>-2.1" );
  TCut year_09_900GeV_hot_cell_cut( "jtphi<1.63 || jtphi>1.7 || jtdeta<1.523 || jtdeta>1.528" );
  TCut year_09_900GeV_EF_hot_cell_cut( "jtphi<-1.365 || jtphi>-1.325 || jtdeta<2.625 || jtdeta>2.83" );
  year_09_900GeV_hot_cell_cut += year_09_900GeV_EF_hot_cell_cut;
  bool pT_weighted = false;
  int dj_bits = _default_dj_bits;
  int hot_cell_removal = 0;
  if( craft_08_data ) hot_cell_removal = 1;
  if( first_data ) hot_cell_removal = 2;
  if( year_09_data ) hot_cell_removal = 3;
  if( no_hard_collisions ) dj_bits = 0;
  if( year_09_data ) dj_bits = 0; // not enough statistics there
  TCut good_PV( "pvntrk[0]>0 && abs(pvz[0])<20" ); // there's always at least one PV from RECO (no need for npv>0)
  
  int leading_jet = 0;

  TH1F *h1 = 0;
  TH2F *h2 = 0;
  TString field;
    
  double min1= -0.5, max1 = 10.5, ymin  = -666;
  double scale  = 1; // e.g. if everything happens to be plotted twice, 0.5 will fix that
  double min2= -0.5, max2 = 10.5;
  int nBins = -1, nBins2 = 100;

  TString field2 (""); // as long as this is empty, plot only one!
  TString fieldText (""); // as long as this is empty, use the field itself
  TString field2Text (""); // as long as this is empty, use the field itself
  TString units (""); // not used when empty
  TString fitFunc = "", fitOpt = "", drawOpt = "hist", simpleCutText = ""; // empty string means no fitting!

  // decide on 1st variable to plot
  if (what == 1) {field = "jtpt" ; fieldText = "p_{T}"; nBins = 400; min1 = 0; max1 = 4000; logy = true;
                   top_fraction = 0.03;} 
  if (what == 2) {field = "tagpt"; fieldText = "p'_{T}"; nBins = 400; min1 = 0; max1 = 4000; logy = true;
                   top_fraction = 0.03;}
  if (what == 3) {field = "jtntrk"; max1 = 20.5;}
  if (what == 4) {field = "(tagpt-jtpt)/jtpt"; cur_cut += "jtpt>0"; simpleCutText = " "; top_fraction = 0.4;
                   nBins = 240; min1 = -1; max1 = 5; marker_size = 0.5; drawOpt = "E"; logy = 1;}
  if (what >= 5 && what <= 9) {
    drawOpt = "E"; marker_size = 0.5;
    field = "pthat"; nBins = 250; min1 = 0; max1 = 5000; logy = true;
    if (what == 5 || what == 8) {use_MC_weight = false; simpleCutText = "unweighted";}
    if (what >= 8) max1 = 500;
    if (what == 8) {stat_x_low = 0.3; stat_x_high=0.8; stat_y_low=0.2; stat_y_high=0.5;}
  }
  if (what == 10) {field = "jtemf"; fieldText = "EM fraction"; nBins = 100; min1=0; max1 = 1;}
  if (what == 11) {field = "jtpt" ; fieldText = "p_{T}"; nBins = 100; min1 = 0; max1 = 200; 
                   logy = true; top_fraction = 0.03;}
  if (what == 12) {field = "tagpt"; fieldText = "p'_{T}"; nBins = 100; min1 = 0; max1 = 200; 
                   logy = true; top_fraction = 0.03;}
  if (what == 13) {field = "jtdeta"; fieldText = "#eta"; max1 = _highest_eta; 
                    min1 = -max1; nBins = int(0.5+(max1-min1)/0.1);}
  if (what == 14) {field = "jtphi"; fieldText = "#phi"; nBins = 72; min1=-TMath::Pi(); max1 = -min1; equalize_x_max = false;}
  if (what == 15) {field = "jtn90"; max1 = 20.5;}
  if (what == 16) {field = "set"; nBins = 50; min1 = 0; max1 = 200;}
  if (what == 17) {field = "met"; nBins = 50; min1 = 0; max1 = 200;}
  if (what == 18) {field = "mett"; nBins = 50; min1 = 0; max1 = 200;}
  if (what == 19) {field = "TMath::Sqrt(met*met-mett*mett)"; fieldText="metl"; nBins = 50; min1 = 0; max1 = 200;}
  if (what == 20) {field = "vjet3pt"; fieldText="p_{T}^{3rd}"; nBins = 50; min1 = 0; max1 = 200;}
  if (what == 21) {field = "abs(tagdelta)"; fieldText="#pi-#Delta#phi"; nBins = 120; min1 = 0; max1 = 0.6;}
  if (what == 22) {field = "jtposf"; nBins = 100; min1 = 0; max1 = 1;}
  if (what == 23) {field = "jtchf"; nBins = 500; min1 = 0; max1 = 5;}
  if (what == 24) {field = "(tagpt-jtpt)/(tagpt+jtpt)"; fieldText="(p'_{T}-p_{T})/(p'_{T}+p_{T})"; 
                    nBins = 200; min1 = -1; max1 = 1; drawOpt = "E"; marker_size = 1.0; top_fraction = 0.35;}
  if (what == 25) {field = "jthn50"; max1 = 49.5;}
  if (what == 26) {field = "jthn90"; max1 = 249.5;}
  if (what == 27) {field = "jtf1hpd"; nBins = 100; min1=0; max1 = 1; logy = true; norm_it = true;}
  if (what == 28) {field = "ontrk"; max1 = 79.5;}
  if (what == 29) {field = "otrkptsum"; max1 = 79.5;}
  if (what == 30) {field = "omaxtrkpt"; max1 = 79.5;}
  if (what == 31) {field = "jtdeta"; max1 = _highest_eta; min1 = -max1; nBins = int(0.5+(max1-min1)/0.1);}
  // .---- many bins versions, for remapping (to be used in get_hists)
  if (what == 32) {field = "jtdeta"     ; fieldText =  "#eta" ; max1 = _highest_eta; min1 = -max1;}
  if (what == 33) {field = "abs(jtdeta)"; fieldText = "|#eta|"; max1 = _highest_eta; min1 = 0;}
  if (what >= 32 && what <= 33) {nBins = int(0.5+(max1-min1)/0.01);}
  if (what >= 34 && what <= 36) {nBins = 4000; min1 = 0; max1 = 4000; logy = true;}
  if (what == 34) {field = "jtpt"; fieldText = "p_{T}";}
  if (what == 35) {field = "tagpt"; fieldText = "p'_{T}";}
  if (what == 36) {field = "(jtpt+tagpt)/2"; fieldText = "(p_{T}+p'_{T})/2";}
  // `---------------------------------------------------------------
  if (what == 37) {field = "jtpt" ; fieldText = "p_{T}"; nBins = 400; min1 = 0; max1 = 4000; 
                    logy = true; top_fraction = 0.05; drawOpt="E";}
  if (what == 38) {field = "jtn90"; max1 = 20.5; norm_it = true;}
  if (what == 39) {field = "njt"; max1 = 20.5;}
  if (what == 40) {field = "jtf1rbx"; nBins = 100; min1=0; max1 = 1; logy = true; norm_it = true;}
  if (what == 41) {field = "imet"; nBins = 50; min1 = 0; max1 = 200;}
  if (what == 42) {field = "iset"; nBins = 50; min1 = 0; max1 = 200;}
  if (what == 43) {field = "jthn90"; max1 = 60.5;}
  if (what == 44) {field = "jtntwrs"; fieldText = "N_{towers}"; max1 = 120.5; norm_it = true;}
  if (what == 45) {field = "jtntwrs"; fieldText = "N_{towers}"; max1 = 40.5; norm_it = true;}
  if (what == 46) {field = "jtemf"; fieldText = "EM fraction"; nBins = 100; min1=0; max1 = 1; logy = true;}
  if (what == 47) {field = "jtf1rbx/(1-jtemf)"; fieldText = "hottest RBX / Had fractions"; equalize_x_max = false;
                    cur_cut += "jtemf<1"; // stat_x_low=0.55; stat_x_high=0.85;
                    nBins = 100; min1=0; max1 = 1; logy = norm_it = true; simpleCutText = " ";}
  if (what == 48) {field = "jtf1hpd/(1-jtemf)"; fieldText = "hottest HPD / Had fractions"; equalize_x_max = false;
                    cur_cut += "jtemf<1"; //stat_x_low=0.55; stat_x_high=0.85;
                    nBins = 100; min1=0; max1 = 1; logy = norm_it = true; simpleCutText = " ";}
  if (what == 49) {field = "jtetaeta"; fieldText = "#eta#eta width"; 
                    nBins = 35; min1=0; max1 = 0.35; logy = true;}
  if (what == 50) {field = "jtphiphi"; fieldText = "#phi#phi width"; 
                    nBins = 35; min1=0; max1 = 0.35; logy = true;}
  if (what == 51) {field = "jtdeta"; fieldText = "#eta"; max1=2.5;
                    min1 = -max1; nBins = int(0.5+(max1-min1)/0.1);}
  if (what == 52) {field = "jthn50"; max1 = 20.5; norm_it = true;}
  if (what == 53) {field = "jtf1h"; nBins = 100; max1 = 1; min1 = 0; norm_it = true; logy = true;}
  if (what == 54) {field = "jte"; nBins = 200; min1 = 0; max1 = 5000;}
  if (what == 55) {field = "jte"; nBins = 200; min1 = 0; max1 = 400;} 
  if (what == 56) {field = "imet"; nBins = 100; min1 = 0; max1 = 500;}
  if (what == 57) {field = "iset"; nBins = 100; min1 = 40; max1 = 540;}
  if (what == 58) {field = "jtrawpt"; nBins = 100; min1 = 0; max1 = 500;}
  if (what == 59) {field = "jtpt" ; fieldText = "p_{T}"; nBins = 100; min1 = 0; max1 = 1000; 
                    logy = true; top_fraction = 0.03;}
  if (what == 60) {field = "jtphiphi"; fieldText = "#phi#phi width"; 
                    nBins = 100; min1=0; max1 = 0.02; logy = true;}
  if (what == 61) {field = "jtn90"; max1 = 60.5;}
  if (what == 62) {field = "jthn50"; max1 = 199.5; nBins=50;}
  if (what == 63) {field = "jtrawpt"; nBins = 100; min1 = 0; max1 = 50;}
  if (what == 64) {field = "sqrt(jtetaeta*jtetaeta+jtphiphi*jtphiphi)"; fieldText = "Width"; 
                    nBins = 35; min1=0; max1 = 0.35; logy = true;}
  if (what == 65) {field = "jtnhits"; min1=0.5; max1 = 40.5;}
  if (what == 66) {field = "jtemf"; fieldText = "EM fraction"; nBins = 100; min1=0; max1=0.1; 
                    logy = true; norm_it = true; pack_the_flows = false;}
  if (what == 67) {field = "jtnhh"; fieldText = "N^{had}_{hit}"; max1 = 100.5; logy = true;}
  if (what == 68) {field = "jtnhh"; fieldText = "N^{had}_{hit}"; max1 = 10.5; logy = true;}
  if (what == 69) {field = "log(1+jtneh)"; fieldText = "ln(1+N^{EM}_{hit})"; min1=0; max1 = 7; nBins=140; logy = true;}
  if (what == 70) {field = "jtneh"; fieldText = "N^{EM}_{hit}"; max1 = 20.5; logy = true;}
  if (what == 71) {field = "jtnet+jtnht"; fieldText = "N_{half}"; max1 = 30.5; logy = true;}
  if (what == 72) {field = "jtneh"; fieldText = "N^{EM}_{hits}"; max1 = 30.5; logy = true;}
  if (what == 73) {field = "jtpt" ; fieldText = "p_{T}"; nBins = 80; min1 = 0; max1 = 40;
                    logy = true; top_fraction = 0.03;}
  if (what == 74) {field = "jtnhits"; min1=0.5; max1 = 200.5;}
  if (what == 75) {field = "jtemf"; fieldText = "EM fraction"; nBins = 100; min1=-1; max1 = 1; logy = true;}
  if (what == 76) {field = "tageta"; fieldText = "#eta'";
                    max1 = _highest_eta; min1 = -max1; nBins = int(0.5+(max1-min1)/0.1);}
  if (what == 77) {field = "jtemf"; fieldText = "EM fraction"; nBins = 100; min1=0.9001; max1=1.001; 
                    logy = true; norm_it = true; pack_the_flows = false;}
  if (what == 78) {field = "jthn90"; max1 = 20.5;}
  if (what == 79) {field = "jtntwrs"; fieldText = "N_{towers}"; max1 = 40.5; norm_it = true; logy = true;}
  if (what == 80) {field = "jtrawpt"; nBins = 60; min1 = 0; max1 = 30; norm_it = true;
                    logy = true; top_fraction = 0.03;}
  if (what == 81) {field = "jtnet"; fieldText = "N^{EM}_{towers}"; max1 = 20.5; logy = true; norm_it = true;}
  if (what == 82) {field = "jtnht"; fieldText = "N^{had}_{towers}"; max1 = 20.5; logy = true; norm_it = true;}
  if (what == 83) {field = "jtneh"; fieldText = "N^{EM}_{hits}"; max1 = 40.5; logy = true;}
  if (what == 84) {field = "jtnhh"; fieldText = "N^{had}_{hit}"; max1 = 20.5; logy = true;}
  if (what == 85) {field = "jtrawpt"; nBins = 50; min1 = 0; max1 = 2000; norm_it = true;
                    logy = true; top_fraction = 0.03;}
  if (what == 87) {field = "jtemf"; fieldText = "EM fraction"; nBins = 51; min1=-1.001; max1=-0.950; 
                    logy = true; norm_it = true; pack_the_flows = false;}
  if (what == 88) {field = "jtrawpt"; nBins = 100; min1 = 0; max1 = 250; logy = true; top_fraction = 0.03;}
  if (what == 89) {field = "jthn90"; nBins = 20; min1 = 0.5; max1 = 20.5; logy = norm_it = true; top_fraction = 0.05;}

  // prospective jet IDs (each is a set of cuts)
  if (what == 90) {field = "jtntrk>0 && jthn90>1 && jtf1hpd<0.85 && jtf1rbx<0.99 && jtemf<0.99"; fieldText = "ID1";
                    max1 = 1.5; drawOpt = "E";}
  if (what == 91) {field = "jtf1hpd<0.98 && jthn90>1.5 && (jtrawpt>25 || abs(jtdeta)>2.55 || jtemf>0.01) && (abs(jtdeta)<2.55 || (jtemf>-0.9 && (jtpt<80 || jtemf<1) ) )";
                    fieldText = "loose ID v0"; max1 = 1.5; drawOpt = "E";}
  if (what == 92) {field = "jtf1hpd<0.98 && (jtrawpt<25 || jtf1hpd<0.95) && jthn90>1.5 && (jtrawpt>25 || abs(jtdeta)>2.55 || jtemf>0.01) && (jtpt<80 || abs(jtdeta)<1 || jtemf<1) && (abs(jtdeta)<2.55 || abs(jtdeta)>3.25 || (jtemf>-0.3 && (jtpt<50 || jtemf>-0.2) && (jtpt<80 || jtemf>-0.1) && (jtpt<340 || jtemf<0.95)) ) && (abs(jtdeta)<3.25 || (jtemf>-0.3 && jtemf<0.9 && (jtpt<50 || (jtemf>-0.2 && jtemf<0.8)) && (jtpt<130 || (jtemf>-0.1 && jtemf<0.7))))"; 
                    fieldText = "tight ID v0"; max1 = 1.5; drawOpt = "E";}
  if (what == 93) {field = "jtf1hpd<0.98 && jthn90>1.5 && (abs(jtdeta)>2.55 || jtemf>0.01) && (abs(jtdeta)<2.55 || (jtemf>-0.9 && (jtpt<80 || jtemf<1) ) )";
                    fieldText = "loose ID v1"; max1 = 1.5; drawOpt = "E";}
  if (what == 94) {field = "jtf1hpd<0.98 && (jtrawpt<25 || jtf1hpd<0.95) && jthn90>1.5 && (abs(jtdeta)>2.55 || jtemf>0.01) && (jtpt<80 || abs(jtdeta)<1 || jtemf<1) && (abs(jtdeta)<2.55 || abs(jtdeta)>3.25 || (jtemf>-0.3 && (jtpt<50 || jtemf>-0.2) && (jtpt<80 || jtemf>-0.1) && (jtpt<340 || jtemf<0.95)) ) && (abs(jtdeta)<3.25 || (jtemf>-0.3 && jtemf<0.9 && (jtpt<50 || (jtemf>-0.2 && jtemf<0.8)) && (jtpt<130 || (jtemf>-0.1 && jtemf<0.7))))"; 
                    fieldText = "tight ID v1"; max1 = 1.5; drawOpt = "E";}

  if (what == 95) {field = "jtemf"; fieldText = "f_{EM}"; nBins = 42; min1=-0.0249999; max1 = 1.0249999; 
                    norm_it = logy = true;}
  if (what == 96) {field = "jtpt" ; fieldText = "p_{T}"; nBins = 90; min1 = 0; max1 = 300; 
                    logy = true; top_fraction = 0.03;}
  if (what == 97) {field = "jtntwrs"; fieldText = "N_{towers}"; max1 = 120.5;} // to allow subtraction
  if (what == 98) {field = "ntj"; max1 = 10.5; norm_it = logy = true;}
  if (what == 99) {field = "jtemf"; fieldText = "EM fraction"; nBins = 100; min1=-1; max1 = 1; norm_it = logy = true;}
  if( what == 100 ) {field = "jtemf"; fieldText = "f_{EM}"; nBins = 22; min1=-1.099999; max1 = 1.099999; 
                      norm_it = logy = true;}
  if( what == 101 ) {field = "jtpt" ; fieldText = "p_{T}"; nBins = 47; min1 = 0; max1 = 2350; 
                      logy = true; top_fraction = 0.03;}
  if( what == 102 ) {field = "jtpt" ; fieldText = "p_{T}"; nBins = 46; min1 = 40; max1 = 500; 
                      logy = true; top_fraction = 0.03;}
  if( what == 103 ) {field = "jtf1rbx"; fieldText = "f_{RBX}"; 
                      nBins = 51; min1=0.95; max1 = 1.001; logy = true; norm_it = true; }//pack_the_flows = false;}
  if( what == 104 ) {field = "jtf1hpd"; fieldText = "f_{HPD}"; nBins = 42; min1=-0.0249999; max1 = 1.0249999; 
                      norm_it = logy = true;}
  if( what == 105 ) {field = "jthn90"; fieldText = "hn90"; max1 = 249.5; logy = true; norm_it = true; }
  if( what == 106 ) {field = "abs(tagdelta)"; fieldText="#pi-#Delta#phi"; nBins = 120; min1 = 0; max1 = 0.6;
                      norm_it = true;}
  if( what == 107 ) {field = "jtpt" ; fieldText = "p_{T} [GeV]"; nBins = 80; min1 = 0; max1 = 800; 
                      logy = true; pack_the_flows = false; top_fraction = 0.03;}
  if( what == 108 ) {field = "jtdeta"; fieldText = "#eta"; max1 = _highest_eta; logy = true;
                    min1 = -max1; nBins = int(0.5+(max1-min1)/0.1);}
  if( what == 109 ) {field = "jtremf"; fieldText = "f_{EM}^{r}"; nBins = 42; min1=-0.0249999; max1 = 1.0249999; 
                    norm_it = logy = true;}
  if( what == 111 ) {field = "abs(jtdeta)>2.6 || jtemf>0.01";
                      fieldText = "minimal ID"; max1 = 1.5; drawOpt = "E";}
  if( what == 112 ) {field = "(abs(jtdeta)>2.6 || jtemf>0.01) && jtemf<1";
                      fieldText = "minimal ID + EMF<1"; max1 = 1.5; drawOpt = "E";}
  if( what == 113 ) {field = "(abs(jtdeta)>2.6 || jtemf>0.01) && jtemf<0.99";
                      fieldText = "minimal ID + EMF<0.99"; max1 = 1.5; drawOpt = "E";}
  if( what == 114 ) {field = "njt"; max1 = 107.5; nBins = 36;}
  if( what == 115 ) {field = "jtetaeta"; fieldText = "#eta#eta width"; 
                     nBins = 20; min1=0; max1 = 0.1; logy = true;}

  if( what == 116 ) {field = "jtgf"; fieldText = "f_{g}"; min1=0; max1=2; nBins=40; logy=true;}
  if( what == 117 ) {field = "jtnhits"; min1=0.5; max1 = 60.5;}
  if( what == 118 ) {field = "phys"; min1=-0.5; max1 = 1.5; nBins = 2;}
  if( what == 119 ) {field = "vjet3pt/tagpt"; fieldText="p_{T}^{3rd}/p_'{T}"; nBins = 50; min1 = 0; max1 = 1;}

  if( what == 120 ) {field = "jtf1hpd<0.98 && jthn90>1.5 && (abs(jtdeta)>2.6 || jtemf>0.01) && (abs(jtdeta)<2.6 || (jtemf>-0.9 && (jtpt<80 || jtemf<1) ) )";
                    fieldText = "loose ID v2"; max1 = 1.5; drawOpt = "E";}
  if( what == 130 ) {field = "jtf1hpd<0.98 && (jtrawpt<25 || jtf1hpd<0.95) && jthn90>1.5 && (abs(jtdeta)>2.6 || jtemf>0.01) && (jtpt<80 || abs(jtdeta)<1 || jtemf<1) && (abs(jtdeta)<2.6 || abs(jtdeta)>3.25 || (jtemf>-0.3 && (jtpt<50 || jtemf>-0.2) && (jtpt<80 || jtemf>-0.1) && (jtpt<340 || jtemf<0.95)) ) && (abs(jtdeta)<3.25 || (jtemf>-0.3 && jtemf<0.9 && (jtpt<50 || (jtemf>-0.2 && jtemf<0.8)) && (jtpt<130 || (jtemf>-0.1 && jtemf<0.7))))"; 
                    fieldText = "tight ID v2"; max1 = 1.5; drawOpt = "E";}
  if( what == 131 ) {field = loose085.GetTitle();
                    fieldText = "loose08+"; max1 = 1.5; drawOpt = "E";}
  if( what == 132 ) {field = tight085.GetTitle();
                    fieldText = "tight08+"; max1 = 1.5; drawOpt = "E";}
  if( what == 133 ) {dj_bits = 0; field = loose085.GetTitle();
                    fieldText = "loose08+"; max1 = 1.5; drawOpt = "E";}
  if( what == 134 ) {dj_bits = 0; field = tight085.GetTitle();
                    fieldText = "tight08+"; max1 = 1.5; drawOpt = "E";}
  if( what == 135 ) {field = loose09v1.GetTitle();
                    fieldText = "loose09v1"; max1 = 1.5; drawOpt = "E";}
  if( what == 136 ) {field = tight09v1.GetTitle();
                    fieldText = "tight09v1"; max1 = 1.5; drawOpt = "E";}

  if( what == 150 ) {field = "jtntrk"; max1 = 20.5;}
  if( what == 151 ) {field = "jtnhtrk"; max1 = 20.5;}
  if( what == 152 ) {field = "jtnqtrk"; max1 = 20.5;}
  if( what == 153 ) {field = "jtnhqtrk"; max1 = 20.5;}
  if( what == 154 ) {field = "clntrk"; max1 = 20.5;}
  if( what == 155 ) {field = "clnhtrk"; max1 = 20.5;}
  if( what == 156 ) {field = "clnqtrk"; max1 = 20.5;}
  if( what == 157 ) {field = "clnhqtrk"; max1 = 20.5;}
  if( what == 160 ) {field = "jtntrk"; max1 = 20.5; logy = true;}
  if( what == 161 ) {field = "jtnhtrk"; max1 = 20.5; logy = true;}
  if( what == 162 ) {field = "jtnqtrk"; max1 = 20.5; logy = true;}
  if( what == 163 ) {field = "jtnhqtrk"; max1 = 20.5; logy = true;}
  if( what == 164 ) {field = "clntrk"; max1 = 20.5; logy = true;}
  if( what == 165 ) {field = "clnhtrk"; max1 = 20.5; logy = true;}
  if( what == 166 ) {field = "clnqtrk"; max1 = 20.5; logy = true;}
  if( what == 167 ) {field = "clnhqtrk"; max1 = 20.5; logy = true;}

  if( what == 200 ) {field = "L1P"; max1 = 1.5;}
  if( what == 201 ) {field = "jtgf"; fieldText = "f_{g}"; min1=0; max1=4; nBins=40; logy=true;}
  if( what == 202 ) {field = "jtpt/jtrawpt"; fieldText = "JES"; cur_cut += "jtrawpt>0"; simpleCutText = " "; 
                     min1=1; max1=3.5; nBins=70; logy=true;}
  if( what == 203 ) {field = "jtpt/jtrawpt"; fieldText = "JES"; cur_cut += "jtrawpt>0"; simpleCutText = " "; 
                     min1=0.8; max1=2.8; nBins=40; logy=true;}
  if( what == 204 ) {field = "jtpt/jtrawpt"; fieldText = "JES"; cur_cut += "jtrawpt>0"; simpleCutText = " "; 
                     min1=0.; max1=4; nBins=40; logy=true;}
  if( what == 205 ) {field = "log(jtrawpt)"; fieldText = "ln(p_{T}^{raw})"; min1=0; max1=3.5; nBins=70; logy=true;}
  if( what == 206 ) {field = "log(jte*jtrawpt/jtpt)"; fieldText = "ln(E^{raw})"; cur_cut += "jtpt>0"; simpleCutText = " "; 
                     min1=2.3; max1=7.3; nBins=50; logy=true;} // optimized for HF
  if( what == 207 ) {field = "log(jte*jtrawpt/jtpt)"; fieldText = "ln(E^{raw})"; cur_cut += "jtpt>0"; simpleCutText = " "; 
                     min1=1.2; max1=6.2; nBins=60; logy=true;} // optimized for EF
  if( what == 208 ) {field = "log(jtrawpt)"; fieldText = "ln(p_{T}^{raw})"; min1=0; max1=7.; nBins=70; logy=true;}
  if( what == 209 ) {field = "jthef"; fieldText = "f_{HE}"; nBins = 42; min1=-0.0249999; max1 = 1.0249999; 
                     norm_it = logy = true;}
  if( what == 210 ) {field = "jthef/(1-jtemf)"; fieldText = "f_{HE}/(1-f_{EM})"; nBins = 42; min1=-0.0249999; max1 = 1.0249999; 
                     norm_it = logy = true; cur_cut += "jtemf<1"; simpleCutText = " ";}
  if (what == 211)  {field = "jtpt" ; fieldText = "p_{T}"; nBins = 40; min1 = 0; max1 = 80; 
                     logy = true; top_fraction = 0.03;}

  if( what >= 300 && what < 304 ) {
    drawOpt = "E"; marker_size = 0.5; opt_stat = "ne"; top_fraction = 0.01; stat_y_low=0.85;
    field = "log(jtpt)/log(10)"; nBins = 175; min1 = 0.1; max1 = 3.6; logy = true;
    if( what % 2 ) { use_MC_weight = false; simpleCutText = "unweighted"; }
    if( what >= 302) { field = "jtpt"; min1=1; max1 = 176; }
  }
  if( what == 310 ) {field = "imet"; nBins = 120; min1 = 0; max1 = 3000; logy = true; }
  if( what == 311 ) {field = "iset"; nBins = 120; min1 = 0; max1 = 3000; logy = true; }
  if( what == 312 ) {field = "imet"; nBins = 100; min1 = 0; max1 = 500; logy = true; }
  if( what == 313 ) {field = "iset"; nBins = 100; min1 = 0; max1 = 500; logy = true; }
  if( what == 314 ) {field = "imet"; nBins = 75; min1 = 0; max1 = 150; logy = true; }

  if( what == 315 ) {field = "jtnls"; fieldText = "N_{LS}"; max1 = 120.5; norm_it = logy = true; }
  if( what == 316 ) {field = "jtnls"; fieldText = "N_{LS}"; max1 = 10.5; norm_it = logy = true; }
  if( what == 317 ) {field = "jtnq-jtnls"; fieldText = "N_{Q}-N_{LS}"; max1 = 10.5; norm_it = logy = true; }
  if( what == 318 ) {field = "jtfls"; fieldText = "f_{LS}"; nBins = 42; min1=-0.0249999; max1 = 1.0249999; 
                      norm_it = logy = true;}
  if( what == 319 ) {field = "jtfq-jtfls"; fieldText = "f_{Q}-f_{LS}"; nBins = 42; min1=-0.0249999; max1 = 1.0249999; 
                      norm_it = logy = true;}
  if( what == 320 ) {field = "hcal"; max1 = 7.5; logy=true; }
  if( what == 321 ) {field = "jtnoot"; fieldText = "N_{OOT}"; max1 = 120.5; norm_it = logy=true; }
  if( what == 322 ) {field = "jtnoot"; fieldText = "N_{OOT}"; max1 = 10.5; norm_it = logy=true; }
  if( what == 323 ) {field = "jtnsdoot"; fieldText = "SD_{OOT}"; max1 = 3.5; norm_it = logy = true;}
  if( what == 324 ) {field = "jtfoot"; fieldText = "f_{OOT}"; nBins = 42; min1=-0.0249999; max1 = 1.0249999; 
                      norm_it = logy = true;}
  if( what == 325 ) {field = "jtnq"; fieldText = "N_{Q}"; max1 = 120.5; norm_it = logy=true; }
  if( what == 326 ) {field = "jtnq"; fieldText = "N_{Q}"; max1 = 10.5; norm_it = logy=true; }
  if( what == 327 ) {field = "jtnsdq"; fieldText = "SD_{Q}"; max1 = 3.5; norm_it = logy = true;}
  if( what == 328 ) {field = "jtfq"; fieldText = "f_{Q}"; nBins = 42; min1=-0.0249999; max1 = 1.0249999; 
                      norm_it = logy = true;}
  if( what == 331 ) {field = "jtnsat"; fieldText = "N_{sat}"; max1 = 120.5; norm_it = logy=true; }
  if( what == 332 ) {field = "jtnsat"; fieldText = "N_{sat}"; max1 = 10.5; norm_it = logy=true; }
  if( what == 333 ) {field = "jtnsdsat"; fieldText = "SD_{sat}"; max1 = 3.5; norm_it = logy = true;}
  if( what == 334 ) {field = "jtfsat"; fieldText = "f_{sat}"; nBins = 42; min1=-0.0249999; max1 = 1.0249999; 
                      norm_it = logy = true;}
 
  if( what == 335 ) {field = "abs(jtdeta)"; fieldText = "|#eta|"; max1 = _highest_eta; min1 = 2.0;
                      nBins = int(0.5+(max1-min1)/0.01); logy = true; stat_y_low = 0.8; opt_stat = "ne";}
  if( what == 336 ) {field = "abs(jtdeta)"; fieldText = "|#eta|"; max1 = 3.4; min1 = 2.0; pack_the_flows = false;
                      nBins = int(0.5+(max1-min1)/0.01); logy = true; stat_y_low = 0.8; opt_stat = "ne";}
  if( what == 337 ) {field = "abs(jtdeta)"; fieldText = "|#eta|"; max1 = _highest_eta; min1 = 0;
                      nBins = int(0.5+(max1-min1)/0.01); logy = true; stat_y_low = 0.8; opt_stat = "ne";}
  if( what == 338 ) {field = "abs(jtdeta)"; fieldText = "|#eta|"; max1 = _highest_eta; min1 = 0;
                      nBins = int(0.5+(max1-min1)/0.1); logy = true; stat_y_low = 0.8; opt_stat = "ne";}
  if( what == 340 ) {field = "jtsd1"; fieldText = "f_{sd1}"; max1 = 5; min1=-0.05; nBins=101; norm_it = logy = true; }
  if( what >= 341 && what <= 344 ) {norm_it = logy = true; nBins = 42; min1=-0.0249999; max1 = 1.0249999;}
  if( what == 341 ) {field = "jtsd1"; fieldText = "f_{sd1}";}
  if( what == 342 ) {field = "jtsd2"; fieldText = "f_{sd2}";}
  if( what == 343 ) {field = "jtsd3"; fieldText = "f_{sd3}";}
  if( what == 344 ) {field = "jtsd4"; fieldText = "f_{sd4}";}
  if( what == 345 ) {field = "jtsd1"; fieldText = "f_{sd1}"; min1 = 0.8; max1=1.005; nBins=41; 
                     pack_the_flows = false; norm_it = logy = true; }
  if( what == 346 ) {field = "1-jtsd1-jtsd2"; fieldText = "f_{sd3+}"; min1 = -0.009999; max1 = 0.339999; nBins = 35;
                     pack_the_flows = false; norm_it = logy = true; }
  if( what == 930 ) {field = "(abs(jtdeta)>2.55 || jtemf>0.01) && "
                      "(abs(jtdeta)<2.55 || (jtemf>-0.9 && (jtpt<80 || jtemf<1) ) )";
                      fieldText = "loose EMF cuts"; max1 = 1.5; drawOpt = "E";}
  if( what == 931 ) {field = "jtf1hpd<0.98 && (abs(jtdeta)>2.55 || jtemf>0.01) && "
                      "(abs(jtdeta)<2.55 || (jtemf>-0.9 && (jtpt<80 || jtemf<1) ) )";
                      fieldText = "loose EMF+HPD cuts"; max1 = 1.5; drawOpt = "E";}
  if( what == 932 ) {field = "(abs(jtdeta)>2.55 || jtemf>0.01) && (abs(jtdeta)<2.55 || jtemf>-0.9)";
                      fieldText = "loose min EMF cuts"; max1 = 1.5; drawOpt = "E";}
  if( what == 940 ) {field = "(abs(jtdeta)>2.55 || jtemf>0.01) && "
                              "(abs(jtdeta)<2.55 || abs(jtdeta)>3.25 || "
                              "(jtemf>-0.3 && (jtpt<50 || jtemf>-0.2) && (jtpt<80 || jtemf>-0.1))) && "
                              "(abs(jtdeta)<3.25 || (jtemf>-0.3 && (jtpt<50 || jtemf>-0.2) && (jtpt<130 || jtemf>-0.1)))"; 
                      fieldText = "tight min EMF cuts"; max1 = 1.5; drawOpt = "E";}
  if( what == 941 ) {field = "(abs(jtdeta)>2.55 || jtemf>0.01) && "
                              "(jtpt<80 || abs(jtdeta)<1 || jtemf<1) && (abs(jtdeta)<2.55 || abs(jtdeta)>3.25 || "
                              "(jtemf>-0.3 && (jtpt<50 || jtemf>-0.2) && (jtpt<80 || jtemf>-0.1) && "
                              "(jtpt<340 || jtemf<0.95)) ) && "
                              "(abs(jtdeta)<3.25 || (jtemf>-0.3 && jtemf<0.9 && (jtpt<50 || (jtemf>-0.2 && jtemf<0.8)) "
                              "&& (jtpt<130 || (jtemf>-0.1 && jtemf<0.7))))"; 
                      fieldText = "tight EMF cuts"; max1 = 1.5; drawOpt = "E";}
  if( what == 942 ) {field = "jtf1hpd<0.98 && (jtrawpt<25 || jtf1hpd<0.95) && "
                              "(abs(jtdeta)>2.55 || jtemf>0.01) && (jtpt<80 || abs(jtdeta)<1 || jtemf<1) && "
                              "(abs(jtdeta)<2.55 || abs(jtdeta)>3.25 || (jtemf>-0.3 && (jtpt<50 || jtemf>-0.2) && "
                              "(jtpt<80 || jtemf>-0.1) && (jtpt<340 || jtemf<0.95)) ) && "
                              "(abs(jtdeta)<3.25 || (jtemf>-0.3 && jtemf<0.9 && (jtpt<50 || (jtemf>-0.2 && jtemf<0.8)) "
                              "&& (jtpt<130 || (jtemf>-0.1 && jtemf<0.7))))"; 
                      fieldText = "tight EMF+HPD cuts"; max1 = 1.5; drawOpt = "E";}

  // decide on 2nd variable to plot. Typical resultion is fine, to enable later slices (except for eta and pT)
  //  var2 == 0 --> simple 1D plots
  if( var2 > 0 ) {logz = logy; logy = false;}
  if( var2 == 1 ) {field2 = "jtpt" ; nBins2 = 400; min2 = 0; max2 = 4000; logz = true; drawOpt="HCOLZ"; equalize_x_max = false;}
  if( var2 == 2 ) {field2 = "tagpt"; nBins2 = 400; min2 = 0; max2 = 4000; logz = true; drawOpt="HCOLZ";}
  if( var2 == 3 ) {field2 = "jtntrk"; max2 = 20.5;}
  if( var2 == 4 ) {field2 = "(tagpt+jtpt)/2"; nBins2 = 800; min2 = 0; max2 = 4000; logz = true; 
                   drawOpt="HCOLZ"; equalize_x_max = false;}
  if( var2 == 5 ) {field2 = "jtdeta"; field2Text = "#eta"; drawOpt="HCOLZ"; hide_stat = true; 
                   max2 = _highest_eta; min2 = -max2; nBins2 = int(0.5+(max2-min2)/0.1);}
  if( var2 == 6 ) {field2 = "abs(jtdeta)"; field2Text = "|#eta|"; 
                   max2 = _highest_eta; min2 = 0;     nBins2 = int(0.5+max2/0.1);}
  if( var2 == 7 ) {field2 = "abs(jtpt-tagpt)"; nBins2 = 100; min2 = 0; max2 = 2500; logz = true; drawOpt="HCOLZ";}
  if( var2 == 8 ) {field2 = "jtdeta"; field2Text = "#eta"; drawOpt="HCOLZ"; hide_stat = true; 
                    max2=2.5; min2 = -max2; nBins2 = int(0.5+(max2-min2)/0.1);}
  if( var2 == 9 ) {field2 = "imet"; field2Text = "Missing E_{T}"; drawOpt="HCOLZ"; hide_stat = true;
                    max2=200; min2 = 0; nBins2 = 50;}
  if( var2 == 10 ) {field2 = "jtntwrs"; field2Text = "N_{towers}"; min2 = 0.5 ; max2 = 40.5; nBins2 = 40; 
                    drawOpt="HCOLZ"; hide_stat = true; }
  if( var2 == 11 ) {field2 = "jthn90"; min2=0.5; max2 = 60.5; nBins2 = 60; drawOpt="HCOLZ"; hide_stat = true;}
  if( var2 == 12 ) {field2 = "jtn90"; min2=0.5; max2 = 25.5; nBins2 = 25; drawOpt="HCOLZ"; hide_stat = true;}
  if( var2 == 13 ) {field2 = "jtntwrs"; field2Text = "N_{towers}"; min2 = 0.5; max2 = 16.5; nBins2 = 16;
                     drawOpt="HCOLZ"; hide_stat = true;}
  if( var2 == 14 ) {field2 = "jtnet"; field2Text = "N^{EM}_{towers}"; min2=-0.5; max2 = 10.5; nBins2 = 11;
                     drawOpt="HCOLZ"; hide_stat = true;}
  if( var2 == 15 ) {field2 = "jtpt" ; nBins2 = 20; min2 = 0; max2 = 200; logz = true; drawOpt="HCOLZ"; equalize_x_max = false;}
  if( var2 == 16 ) {field2 = "jtdeta"; field2Text = "#eta"; drawOpt="HCOLZ"; hide_stat = true;
                    max2=3.; min2 = -max2; nBins2 = int(0.5+(max2-min2)/0.1);}
  if( var2 == 17 ) {field2 = "log(tagpt)"; nBins2 = 21; min2 = 2.5; max2 = 6; logz = true; drawOpt="HCOLZ"; 
                    equalize_x_max = false;}
  if( var2 == 18 ) {field2 = "jtemf"; nBins2 = 42; min2=-0.0249999; max2 = 1.0249999; 
                    logz = true; drawOpt="HCOLZ";}
  if( var2 == 19 ) {field2 = "jtphiphi"; nBins2 = 20; min2=0; max2 = 0.1; 
                    logz = true; drawOpt="HCOLZ";}
  if( var2 == 20 ) {field2 = "phys"; min2=-0.5; max2 = 1.5; nBins2 = 2; drawOpt="text"; hide_stat = true; marker_size = 5;}
  if( var2 == 21 ) {field2 = "hcal"; min2=0.5; max2 = 7.5; nBins2 = 7; drawOpt="HCOLZ"; hide_stat = true;}
  if( var2 == 22 ) {field2 = "abs(jtdeta)"; field2Text = "|#eta|"; drawOpt="HCOLZ"; hide_stat = true;
                    max2 = _highest_eta; min2 = 0;     nBins2 = int(0.5+max2/0.1);}
  if( var2 == 23 ) {field2 = "jtgf"; field2Text = "f_{g}"; drawOpt="HCOLZ"; hide_stat = true;
                    min2=0; max2=4; nBins=40; logz=true;}
  if( var2 == 24 ) {field2 = "jtgf*jtpt"; field2Text = "p_{T}^{g}"; drawOpt="HCOLZ"; hide_stat = true;
                    min2=0; max2=40; nBins=40; logz=true;}
  if( var2 == 25 ) {field2 = "jtemf"; field2Text = "f_{EM}"; drawOpt="HCOLZ"; hide_stat = true;
                    min2 = -1.049999; max2 = 1.049999; nBins2 = 42; logz = true;}
  if( var2 == 26 ) {field2 = "jtntwrs"; field2Text = "N_{towers}"; min2 = 0.5; max2 = 16.5; nBins2 = 16;
                    drawOpt="HCOLZ"; hide_stat = true; logz = true;}
  if( var2 == 27 ) {field2 = "jtsd1"; field2Text = "f_{sd1}"; min2 = -0.0249999; max2 = 1.0249999; nBins2 = 42;
                    drawOpt="HCOLZ"; hide_stat = true; norm_it = true; logz = true;}
  if( var2 == 28 ) {field2 = "1-jtsd1-jtsd2"; field2Text = "f_{sd3+}"; min2 = -0.009999; max2 = 0.339999; nBins2 = 35;
                    drawOpt="HCOLZ"; hide_stat = true; norm_it = true; logz = true;}
  if( var2 == 29 ) {field2 = "abs(jtdeta)"; field2Text = "|#eta|"; drawOpt="HCOLZ"; hide_stat = true;
                    max2 = 3.4; min2 = 2.5; nBins2 = 45;}
  if( var2 == 30 ) {field2 = "jtfq"; field2Text = "f_{Q}"; min2 = -0.0249999; max2 = 1.0249999; nBins2 = 42;
                    drawOpt="HCOLZ"; hide_stat = true; norm_it = true; logz = true;}
  if( var2 == 31 ) {field2 = "jthef"; field2Text = "f_{HE}"; min2 = -0.0249999; max2 = 1.0249999; nBins2 = 42;
                    drawOpt="HCOLZ"; hide_stat = true; norm_it = true; logz = true;}
  if( var2 == 32 ) {field2 = "jthef/(1-jtemf)"; field2Text = "f_{HE}/(1-f_{EM})"; cur_cut += "jtemf<1"; simpleCutText = " ";
                    min2 = -0.0249999; max2 = 1.0249999; nBins2 = 42;
                    drawOpt="HCOLZ"; hide_stat = true; norm_it = true; logz = true; }
  if( var2 == 33 ) {field2 = "jtfls"; field2Text = "f_{LS}"; min2 = -0.0249999; max2 = 1.0249999; nBins2 = 42;
                    drawOpt="HCOLZ"; hide_stat = true; norm_it = true; logz = true;}


  // Additional (explicit) cuts
  // iCut == 0 and 99 are reserved for no special cut
  TCut cutBefore (cur_cut);
  TString textBefore (simpleCutText); 
  if (iCut == 1) {cur_cut += "vjet3pt<50";}
  if (iCut == 2) {cur_cut += "vjet3pt<50 && ptHat>2600";}
  if (iCut == 3) {cur_cut += "vjet3pt<50 && ptHat<2600";}
  // doesn't work since additional jets get booked now
  // if (iCut == 4) {cur_cut += "njt==1"; add_to_text( simpleCutText, "1 tag");}
  if (iCut == 4) {cur_cut += "jtntrk<0.5 || jttrg<1.5"; add_to_text( simpleCutText, "!tag");}
  if (iCut == 5) {dj_bits = 0;}
  if (iCut == 6) {cur_cut += "jtntrk>0.5 && jttrg<1.5"; 
                   add_to_text( simpleCutText, "track & !trigger");}
  if (iCut == 7) {dj_bits = 0; pT_weighted = true; logz = true; drawOpt = "HCOLZ";
                   add_to_text( simpleCutText, "#Sigmap_{T}");}
  if (iCut == 8) {dj_bits = 0; cur_cut += "jtntwrs==1";}
  // add additional pT and/or |eta| bins here, if REALLY needed (otherwise use slicing)
  // e.g. {cur_cut += "0.4<abs(jtdeta) && abs(jtdeta)<0.5"; simpleCutText += "0.4<|#eta|<0.5";}
  if (iCut == 9) {dj_bits = 0; cur_cut += "jtf1h>0.95"; 
                   add_to_text( simpleCutText, "f^{1}_{had}>0.95");}
  if (iCut == 10) {dj_bits = 0; cur_cut += "0.95>=jtf1h && jtf1h>0.4"; 
                    add_to_text( simpleCutText, "0.95#geqf^{1}_{had}>0.4");}
  if (iCut == 11) {dj_bits = 0; cur_cut += "0.4>=jtf1h"; 
                    add_to_text( simpleCutText, "0.4#geqf^{1}_{had}");}
  if (iCut == 12) {cur_cut += "jttrg<1.5"; add_to_text( simpleCutText, "!trigger");}

  if (iCut == 13) {dj_bits = 0; cur_cut += "jtemf>0.5"; drawOpt="hist"; logy=false;
                    add_to_text( simpleCutText, "EMF>0.5");}
  if (iCut == 14) {dj_bits = 0; cur_cut += "jtemf>0.5 && jtntwrs>20"; drawOpt="hist"; logy=false;
                    add_to_text( simpleCutText, "EMF>0.5 && N_{towers}>20");}
  if (iCut == 15) {dj_bits = 0; hot_cell_removal = 0;}

  if (iCut == 16) {dj_bits = 0; cur_cut += "jtemf<0.08";
                    add_to_text( simpleCutText, "EMF<0.08");}
  if (iCut == 17) {dj_bits = 0; cur_cut += "jtemf<0.08 && jtntwrs>=40";
                    add_to_text( simpleCutText, "EMF<0.08 & N_{towers}#geq40");}
  if (iCut == 18) {dj_bits = 0; cur_cut += "jtemf<0.08 && 30>jtntwrs && jtntwrs>=20";
                    add_to_text( simpleCutText, "EMF<0.08 & 30>N_{towers}#geq20");}
  if (iCut == 19) {dj_bits = 0; cur_cut += "jtemf<0.08 && jtntwrs==10";
                    add_to_text( simpleCutText, "EMF<0.08 & N_{towers}=10");}
  if (iCut == 20) {dj_bits = 0; cur_cut += "jtemf<0.08 && 30>jtntwrs && jtntwrs>=20 && jtpt<1500";
                    add_to_text( simpleCutText, "EMF<0.08 & 30>N_{towers}#geq20 & soft");}
  if (iCut == 21) {dj_bits = 0; cur_cut += "jtemf<0.08 && 30>jtntwrs && jtntwrs>=20 && jtpt>1500";
                    add_to_text( simpleCutText, "EMF<0.08 & 30>N_{towers}#geq20 & hard");}
  if (iCut == 22) {dj_bits = 0; cur_cut += "jtemf<0.08 && jtntwrs==10 && jtphiphi<0.004";
                    add_to_text( simpleCutText, "EMF<0.08 & N_{towers}=10 & #sigma_{#phi#phi}<0.004");}
  if (iCut == 23) {dj_bits = 0; cur_cut += "jtemf<0.08 && jtntwrs==10 && jtphiphi>=0.004";
                    add_to_text( simpleCutText, "EMF<0.08 & N_{towers}=10 & #sigma_{#phi#phi}#geq0.004");}
  if( iCut == 24 ) {dj_bits = 1;} // booked as di-jets
  if( iCut == 25 ) {dj_bits = 0; cur_cut += "jtemf>0.01";}
  if( iCut == 26 ) {cur_cut += "jtemf>0.01";}
  if( iCut == 27 ) {dj_bits = 3;}// original, loose, somewhat biased calorimetry veto (no dphi cut)
  if (iCut == 28)  {dj_bits = 0; cur_cut += "jtemf<0.08 && jtntwrs>=10 && jtntwrs<=11 && jtphiphi<0.004";
                    add_to_text( simpleCutText, "EMF<0.08 & 10#leqN_{towers}#leq11 & #sigma_{#phi#phi}<0.004");}
  if (iCut == 29)  {dj_bits = 0; cur_cut += "jtemf<0.08 && jtntwrs>=10 && jtntwrs<=11";
                    add_to_text( simpleCutText, "EMF<0.08 & 10#leqN_{towers}#leq11");}
  if (iCut == 30) {cur_cut += "jtemf<0.08 && 30>jtntwrs && jtntwrs>=20 && jtpt<1500";
                    add_to_text( simpleCutText, "EMF<0.08 & 30>N_{towers}#geq20 & soft");}
  if (iCut == 31) {cur_cut += "jtemf<0.08 && 30>jtntwrs && jtntwrs>=20 && jtpt>1500";
                    add_to_text( simpleCutText, "EMF<0.08 & 30>N_{towers}#geq20 & hard");}
  if (iCut == 32) {cur_cut += "jtemf<0.08 && jtntwrs==10 && jtphiphi<0.004";
                    add_to_text( simpleCutText, "EMF<0.08 & N_{towers}=10 & #sigma_{#phi#phi}<0.004");}
  if (iCut == 33) {cur_cut += "jtemf<0.08 && jtntwrs==10 && jtphiphi>=0.004";
                    add_to_text( simpleCutText, "EMF<0.08 & N_{towers}=10 & #sigma_{#phi#phi}#geq0.004");}
  if( iCut == 34 ) {dj_bits = 5;} // original, loose, somewhat biased veto
  if( iCut == 35 ) {dj_bits = 9;} // tight calorimetry veto (no dphi cut)

  if (iCut == 36) {cur_cut += "jtemf<0.08";
                    add_to_text( simpleCutText, "EMF<0.08");}
  if (iCut == 37) {cur_cut += "jtemf<0.08 && jtntwrs>=40";
                    add_to_text( simpleCutText, "EMF<0.08 & N_{towers}#geq40");}
  if (iCut == 38) {cur_cut += "jtemf<0.08 && 30>jtntwrs && jtntwrs>=20";
                    add_to_text( simpleCutText, "EMF<0.08 & 30>N_{towers}#geq20");}
  if (iCut == 39) {cur_cut += "jtemf<0.08 && jtntwrs==10";
                    add_to_text( simpleCutText, "EMF<0.08 & N_{towers}=10");}
  // HF studies
  if( iCut == 40 ) {dj_bits = 0; cur_cut += "-0.6<jtemf && jtemf<0.5";
                     add_to_text( simpleCutText, "-0.6<f_{EM}<0.5");}
  if( iCut == 41 ) {dj_bits = 0; cur_cut += "jtemf<-0.9";
                     add_to_text( simpleCutText, "f_{EM}<-0.9");}
  if( iCut == 42 ) {dj_bits = 0x13;} // corrected loose calorimetry veto (no dphi cut)
  if( iCut == 43 ) {dj_bits = 0x15;} // corrected loose selection
  if( iCut == 44 ) {cur_cut += "jtemf<0.01";
                     add_to_text( simpleCutText, "EMF<0.01");}
  if( iCut == 45 ) {cur_cut += "jtemf<0";
                     add_to_text( simpleCutText, "EMF<0");}
  if( iCut == 46 ) {dj_bits = 0; cur_cut += "jtemf<0.01";
                     add_to_text( simpleCutText, "EMF<0.01");}
  if( iCut == 47 ) {dj_bits = 0; cur_cut += "jtemf<0";
                     add_to_text( simpleCutText, "EMF<0");}
  if( iCut == 48 ) { cur_cut += "jtntwrs>1"; add_to_text( simpleCutText, "n^{tower}>1" );}
  if( iCut == 49 ) { cur_cut += "jtntwrs==1"; add_to_text( simpleCutText, "n^{tower}=1" );}
  // CRAFT09 - Run 108290
  if( iCut == 50 ) {cur_cut += "imet>3000";}
  if( iCut == 51 ) {cur_cut += "imet>500 && imet<3000"; 
                    simpleCutText = join_text( simpleCutText.Strip(), "500<iMET<3000" );}
  if( iCut == 52 ) {cur_cut += "imet>100 && imet<500";
                    simpleCutText = join_text( simpleCutText.Strip(), "100<iMET<500" );}
  if( iCut == 53 ) {cur_cut += "imet<100";
                    simpleCutText = join_text( simpleCutText.Strip(), "iMET<100" );}

  if( iCut == 55 ) {cur_cut += "hcal==7";
                    simpleCutText = join_text( simpleCutText.Strip(), "hcal-tight" );}
  if( iCut == 56 ) {cur_cut += "(hcal&2)>0";
                    simpleCutText = join_text( simpleCutText.Strip(), "hcal-loose" );}
  if( iCut == 57 ) {cur_cut += "hcal==1";
                    simpleCutText = join_text( simpleCutText.Strip(), "!hcal-loose" );}
  // per jet cuts
  if( iCut == 58 ) {cur_cut += "(hcal&2)>0"; cur_cut += minimal08;
                    simpleCutText = join_text( simpleCutText.Strip(), "hcal-loose & minimal08" );}
  if( iCut == 59 ) {cur_cut += "(hcal&2)>0"; cur_cut += tight08;
                    simpleCutText = join_text( simpleCutText.Strip(), "hcal-loose & tight08" );}
  if( iCut == 60 ) {cur_cut += minimal08;
                    simpleCutText = join_text( simpleCutText.Strip(), "minimal08" );}
  if( iCut == 61 ) {cur_cut += loose08;
                    simpleCutText = join_text( simpleCutText.Strip(), "loose08" );}
  if( iCut == 62 ) {cur_cut += tight08;
                    simpleCutText = join_text( simpleCutText.Strip(), "tight08" );}
  if( iCut == 63 ) {cur_cut += "hcal==7"; cur_cut += minimal08;
                    simpleCutText = join_text( simpleCutText.Strip(), "hcal-tight & minimal08" );}
  if( iCut == 64 ) {cur_cut += "hcal==7"; cur_cut += tight08;
                    simpleCutText = join_text( simpleCutText.Strip(), "hcal-tight & tight08" );}
  if( iCut == 65 ) {cur_cut += "(hcal&2)>0"; cur_cut += loose08;
                    simpleCutText = join_text( simpleCutText.Strip(), "hcal-loose & loose08" );}
               
  if( iCut == 66 ) {cur_cut += "njt<10";}
  if( iCut == 67 ) {cur_cut += tight085 + "hcal==7"; add_to_text( simpleCutText, "hcal-tight & tight08+" );}

  // leading jet cuts
  if( iCut == 68 ) {leading_jet = 3;}
  if( iCut == 69 ) {leading_jet = 1;}
  if( iCut == 70 ) {leading_jet = 2;}
  if( iCut == 71 ) {leading_jet = 2; cur_cut += minimal08; add_to_text( simpleCutText, "minimal08" );}
  if( iCut == 72 ) {leading_jet = 2; cur_cut += loose08; add_to_text( simpleCutText, "loose08" );}
  if( iCut == 73 ) {leading_jet = 2; cur_cut += tight08; add_to_text( simpleCutText, "tight08" );}
  if( iCut == 74 ) {leading_jet = 2; cur_cut += minimal08 + "(hcal&2)>0"; 
                                        add_to_text( simpleCutText, "hcal-loose & minimal08" );}
  if( iCut == 75 ) {leading_jet = 2; cur_cut += loose08 + "(hcal&2)>0"; 
                                        add_to_text( simpleCutText, "hcal-loose & loose08" );}
  if( iCut == 76 ) {leading_jet = 2; cur_cut += tight08 + "(hcal&2)>0"; 
                                        add_to_text( simpleCutText, "hcal-loose & tight08" );}
  if( iCut == 77 ) {leading_jet = 2; cur_cut += minimal08 + "hcal==7"; 
                                        add_to_text( simpleCutText, "hcal-tight & minimal08" );}
  if( iCut == 78 ) {leading_jet = 2; cur_cut += loose08 + "hcal==7"; 
                                        add_to_text( simpleCutText, "hcal-tight & loose08" );}
  if( iCut == 79 ) {leading_jet = 2; cur_cut += tight08 + "hcal==7"; 
                                        add_to_text( simpleCutText, "hcal-tight & tight08" );}
  if( iCut == 80 ) {leading_jet = 2; cur_cut += "(hcal&2)>0"; 
                                        add_to_text( simpleCutText, "hcal-loose" );}
  if( iCut == 81 ) {leading_jet = 2; cur_cut += "hcal==7"; 
                                        add_to_text( simpleCutText, "hcal-tight" );}
  if( iCut == 82 ) {leading_jet = 2; cur_cut += tight085 + "hcal==7"; 
                                        add_to_text( simpleCutText, "hcal-tight & tight08+" );}
  if( iCut == 83 ) {leading_jet = 2; cur_cut += loose085 + "hcal==7"; 
                                        add_to_text( simpleCutText, "hcal-tight & loose08+" );}

  if( iCut == 90 ) { cur_cut += "jtemf<=0.5"; add_to_text( simpleCutText, "f_{EM}#leq0.5");}
  if( iCut == 91 ) { cur_cut += "jtemf>0.5"; add_to_text( simpleCutText, "f_{EM}>0.5");}
  if( iCut == 92 ) { cur_cut += "jtemf<=0.5 && jthn90>12"; add_to_text( simpleCutText, "f_{EM}#leq0.5, n^{hits}_{90}>12");}
  if( iCut == 93 ) { cur_cut += "jtemf<=0.5 && 6<=jthn90 && jthn90<=10"; 
                     add_to_text( simpleCutText, "f_{EM}#leq0.5, 6#leqn^{hits}_{90}#leq1");}
  if( iCut == 94 ) { cur_cut += "jtemf<=0.5 && jthn90<=2"; add_to_text( simpleCutText, "f_{EM}#leq0.5, n^{hits}_{90}#leq2");}


  //iCut=99 reserved for no cut
  if( iCut == 100 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; add_to_text( simpleCutText, "f_{g}>0.6" );}
  if( iCut == 101 ) { dj_bits = 0; cur_cut += "jtgf<0.2"; add_to_text( simpleCutText, "f_{g}<0.2" );}
  if( iCut == 102 ) { dj_bits = 0; cur_cut += "jtgf>0.8 && jtgf<2"; add_to_text( simpleCutText, "2>f_{g}>0.8" );}
  if( iCut == 103 ) { dj_bits = 0; cur_cut += "jtgf<0.1"; add_to_text( simpleCutText, "f_{g}<0.1" );}
  if( iCut == 108 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; cur_cut += ! on_EF_crack; 
                      add_to_text( simpleCutText, "f_{g}>0.6, !crack" );}
  if( iCut == 109 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; cur_cut += on_EF_crack; 
                      add_to_text( simpleCutText, "f_{g}>0.6, crack" );}
  if( iCut == 110 ) { cur_cut += "jtgf>0.6"; add_to_text( simpleCutText, "f_{g}>0.6" );}
  if( iCut == 111 ) { cur_cut += "jtgf<0.2"; add_to_text( simpleCutText, "f_{g}<0.2" );}
  if( iCut == 112 ) { cur_cut += "jtgf>0.6 && jtgf<2"; add_to_text( simpleCutText, "2>f_{g}>0.8" );}
  if( iCut == 113 ) { cur_cut += "jtgf<0.1"; add_to_text( simpleCutText, "f_{g}<0.1" );}

  if( iCut == 119 ) { dj_bits = 0; cur_cut += "jtntwrs>2"; add_to_text( simpleCutText, "n^{tower}>2" );}
  if( iCut == 120 ) { cur_cut += "jtntwrs>2"; add_to_text( simpleCutText, "n^{tower}>2" );}
  if( iCut == 121 ) { cur_cut += "jtntwrs<=2"; add_to_text( simpleCutText, "n^{tower}#leq2" );}
  if( iCut == 122 ) { dj_bits = 0; cur_cut += "jtntwrs>2&&jtgf>0.8&&jtgf<2"; 
                      add_to_text( simpleCutText, "2>f_{g}>0.8, n^{tower}>2" );}
  if( iCut == 123 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; cur_cut += HF_tower_cut; 
                     add_to_text( simpleCutText, "f_{g}>0.6, HF-N" );}
  if( iCut == 124 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; cur_cut += EF_tower_cut; 
                      add_to_text( simpleCutText, "f_{g}>0.6, EF-N" );}
  if( iCut == 125 ) { dj_bits = 0; cur_cut += "jtgf>0.6&&(jtsd1+jtsd2)<.99"; cur_cut += EF_tower_cut; 
                      add_to_text( simpleCutText, "f_{g}>0.6, EF-N, f_{sd3+}>1%" );}
  if( iCut == 126 ) { dj_bits = 0; cur_cut += "jtgf>0.6&&(jtsd1+jtsd2)>=.99"; cur_cut += EF_tower_cut; 
                      add_to_text( simpleCutText, "f_{g}>0.6, EF-N, f_{sd3+}#leq1%" );}
  if( iCut == 127 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; cur_cut += EF_tower_cut && on_EF_crack; 
                      add_to_text( simpleCutText, "f_{g}>0.6, EF-N, crack" );}
  if( iCut == 128 ) { dj_bits = 0; cur_cut += "jtntwrs>1&&jtgf>0.6"; 
                      add_to_text( simpleCutText, "f_{g}>0.6, n^{tower}>1" );}
  if( iCut == 129 ) { dj_bits = 0; cur_cut += "jtntwrs>2&&jtgf>0.6"; 
                      add_to_text( simpleCutText, "f_{g}>0.6, n^{tower}>2" );}

  if( iCut == 130 ) { cur_cut += HF_cuts; add_to_text( simpleCutText, "HFcuts" );}
  if( iCut == 131 ) { cur_cut += tight085; add_to_text( simpleCutText, "tight08+" );}
  if( iCut == 132 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; cur_cut += HF_cuts;add_to_text( simpleCutText, "f_{g}>0.6, HFcuts" );}
  if( iCut == 133 ) { cur_cut += loose085 + "hcal==7"; add_to_text( simpleCutText, "hcal-tight & loose08+" );}
  if( iCut == 134 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; cur_cut += loose08; 
                      add_to_text( simpleCutText, "f_{g}>0.6, loose08" );}
  if( iCut == 135 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; cur_cut += tight08; 
                      add_to_text( simpleCutText, "f_{g}>0.6, tight08" );}
  if( iCut == 136 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; cur_cut += loose085; 
                      add_to_text( simpleCutText, "f_{g}>0.6, loose08+" );}
  if( iCut == 137 ) { dj_bits = 0; cur_cut += "jtgf>0.6"; cur_cut += tight085; 
                      add_to_text( simpleCutText, "f_{g}>0.6, tight08+" );}
  if( iCut == 140 ) { dj_bits = 0; cur_cut += loose09v1; add_to_text( simpleCutText, "loose09v1" ); }
  if( iCut == 141 ) { dj_bits = 0; cur_cut += tight09v1; add_to_text( simpleCutText, "tight09v1" ); }

  if( iCut == 150 ) { cur_cut += good_PV; add_to_text( simpleCutText, "good PV" );}

  if( iCut == 200 ) { cur_cut += "phys";}
  if( iCut == 201 ) { cur_cut += "phys"; cur_cut += minimal08; add_to_text( simpleCutText, "phys, minimal08" );}
  if( iCut == 202 ) { cur_cut += "phys"; cur_cut += loose08; add_to_text( simpleCutText, "phys, loose08" );}
  if( iCut == 203 ) { cur_cut += "phys"; cur_cut += tight08; add_to_text( simpleCutText, "phys, tight08" );}
  if( iCut == 210 ) { cur_cut += "L1P";}
  if( iCut == 213 ) { cur_cut += "L1P"; cur_cut += tight08; add_to_text( simpleCutText, "L1P, tight08" );}
  if( iCut == 220 ) { cur_cut += "GB";}
  if( iCut == 222 ) { cur_cut += "GB"; cur_cut += loose08; add_to_text( simpleCutText, "GB, loose08" );}
  if( iCut == 223 ) { cur_cut += "GB"; cur_cut += tight08; add_to_text( simpleCutText, "GB, tight08" );}
  if( iCut == 225 ) { cur_cut += "GB&&jtnhits>7"; add_to_text( simpleCutText, "GB, n^{hits}>7" );}
  if( iCut == 226 ) { cur_cut += "GB&&hcal>1"; add_to_text( simpleCutText, "GB, H_{loose}");}
  if( iCut == 227 ) { cur_cut += "GB&&hcal==7"; add_to_text( simpleCutText, "GB, H_{tight}");}
  if( iCut == 228 ) { cur_cut += "GB&&hcal==7"; cur_cut += tight08; add_to_text( simpleCutText, "GB, H_{tight}, tight08"); }
  if( iCut == 229 ) { cur_cut += "GB&&hcal==1"; add_to_text( simpleCutText, "GB, !H_{loose}");}
  if( iCut == 230 ) { cur_cut += "(!L1HS)&&(!L1BH)"; add_to_text( simpleCutText, "quiet" );}
  if( iCut == 231 ) { cur_cut += "(!L1HS)&&(!L1BH)"; add_to_text( simpleCutText, "quiet" );
                      cur_cut += "jtemf<=0.5"; add_to_text( simpleCutText, "f_{EM}#leq0.5");}
  if( iCut == 232 ) { cur_cut += "(!L1HS)&&(!L1BH)"; add_to_text( simpleCutText, "quiet" );
                      cur_cut += "jtemf>0.5"; add_to_text( simpleCutText, "f_{EM}>0.5");}
  if( iCut == 233 ) { cur_cut += "(!L1HS)&&(!L1BH)"; add_to_text( simpleCutText, "quiet" );
                      cur_cut += "jtemf<0.05"; add_to_text( simpleCutText, "f_{EM}<.05");
                      cur_cut += "hcal==1"; add_to_text( simpleCutText, "!H_{loose}");}
  if( iCut == 234 ) { cur_cut += "(!L1HS)&&(!L1BH)"; cur_cut += tight08; 
                      add_to_text( simpleCutText, "quiet, tight08" );}
  if( iCut == 235 ) { cur_cut += "(!L1HS)&&(!L1BH)&&jtnhits>7"; add_to_text( simpleCutText, "quiet, n^{hits}>7" );}
  if( iCut == 236 ) { cur_cut += "(!L1HS)&&(!L1BH)&&hcal==1&&jtnhits>5"; 
                      add_to_text( simpleCutText, "quiet, !H_{loose}, n^{hits}>5" );}
  if( iCut == 237 ) { cur_cut += "(!L1HS)&&(!L1BH)&&hcal==1&&jtnhits>5&&jtemf<0.05&&phys"; 
                      add_to_text( simpleCutText, "phys, quiet, !H_{loose}, N^{hit}>7, f_{EM}<.05" );}
  if( iCut == 238 ) { cur_cut += "hcal==1&&jtnhits>5&&jtemf<0.05"; 
                      add_to_text( simpleCutText, "!H_{loose}, N^{hit}>7, f_{EM}<.05" );}
  if( iCut == 239 ) { cur_cut += "jtnhits>5&&jtemf<0.05"; 
                      add_to_text( simpleCutText, "N^{hit}>7, f_{EM}<.05" );}
  if( iCut == 240 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; add_to_text( simpleCutText, "QP" );}
  if( iCut == 241 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys && jtntwrs>1"; 
                      add_to_text( simpleCutText, "QP, n^{tower}>1" );}
  if( iCut == 242 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys && jtntwrs==1"; 
                      add_to_text( simpleCutText, "QP, n^{tower}=1" );}
  if( iCut == 243 ) { cur_cut += "(!L1HS)&&(!L1BH)&&phys&&jtntwrs>2"; 
                      add_to_text( simpleCutText, "QP, n^{tower}>2" );}
  if( iCut == 244 ) { cur_cut += "(!L1HS)&&(!L1BH)&&phys&&jtntwrs<=2"; 
                      add_to_text( simpleCutText, "QP, n^{tower}#leq2" );}
  if( iCut == 245 ) { cur_cut += "(!L1HS)&&(!L1BH)&&phys&&jtntwrs>8"; add_to_text( simpleCutText, "QP, n^{tower}>8" );}
  if( iCut == 246 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; add_to_text( simpleCutText, "QP" );
                      cur_cut += HF_tower_cut; add_to_text( simpleCutText, "HF-N" );}
  if( iCut == 247 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; add_to_text( simpleCutText, "QP" );
                      cur_cut += EF_tower_cut; add_to_text( simpleCutText, "EF-N" );}
  if( iCut == 248 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; add_to_text( simpleCutText, "QP" );
                      cur_cut += EF_tower_cut; add_to_text( simpleCutText, "EF-N, f_{HPD}<.95" );
		      cur_cut += "jtf1hpd<0.95";}
  if( iCut == 249 ) { cur_cut += "jtntwrs==10&&(!L1HS)&&(!L1BH) && phys"; add_to_text( simpleCutText, "QP, N_{towers}=10" );}
  if( iCut == 250 ) { cur_cut += "jtntwrs<6&&(!L1HS)&&(!L1BH) && phys"; add_to_text( simpleCutText, "QP, N_{towers}#leq5" );
                      cur_cut += EF_tower_cut; add_to_text( simpleCutText, "EF-N" );}
  if( iCut == 251 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys && (jtsd1+jtsd2)>=.99"; cur_cut += EF_tower_cut; 
                      add_to_text( simpleCutText, "QP, EF-N, f_{sd3+}#leq1%" );}
  if( iCut == 252 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; cur_cut += EF_tower_cut && on_EF_crack; 
                      add_to_text( simpleCutText, "QP, EF-N, crack" );}
  if( iCut == 253 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; cur_cut += loose08; add_to_text( simpleCutText, "QP, loose08" );}
  if( iCut == 254 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; cur_cut += tight08; add_to_text( simpleCutText, "QP, tight08" );}
  if( iCut == 255 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; cur_cut += loose085; add_to_text( simpleCutText, "QP, loose08+" );}
  if( iCut == 256 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; cur_cut += tight085; add_to_text( simpleCutText, "QP, tight08+" );}
  if( iCut == 258 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; cur_cut += ! on_EF_crack; 
                      add_to_text( simpleCutText, "QP, !crack" );}
  if( iCut == 259 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; cur_cut += on_EF_crack; 
                      add_to_text( simpleCutText, "QP, crack" );}

  //  more "GB" cuts
  if( iCut == 260 ) { cur_cut += "GB&&hcal==7"; cur_cut += tight085; add_to_text( simpleCutText, "GB, H_{tight}, tight08+"); }
  if( iCut == 261 ) { cur_cut += "GB && jtntwrs>1"; add_to_text( simpleCutText, "GB, n^{tower}>1" );}
  if( iCut == 262 ) { cur_cut += "GB && jtntwrs==1"; add_to_text( simpleCutText, "GB, n^{tower}=1" );}
  if( iCut == 263 ) { cur_cut += "GB && jtntwrs>2"; add_to_text( simpleCutText, "GB, n^{tower}>2" );}
  if( iCut == 264 ) { cur_cut += "GB && jtntwrs<=2"; add_to_text( simpleCutText, "GB, n^{tower}#leq2" );}
  if( iCut == 265 ) { cur_cut += "GB&&hcal==7"; cur_cut += loose085; add_to_text( simpleCutText, "GB, H_{tight}, loose08+"); }
  if( iCut == 266 ) { cur_cut += "GB"; cur_cut += loose085; add_to_text( simpleCutText, "GB, loose08+"); }
  if( iCut == 267 ) { cur_cut += "GB"; cur_cut += tight085; add_to_text( simpleCutText, "GB, tight08+"); }
  
  //  more "QP" cuts
  if( iCut == 280 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; cur_cut += loose09v1; 
                      add_to_text( simpleCutText, "QP, loose09v1" ); }
  if( iCut == 281 ) { cur_cut += "(!L1HS)&&(!L1BH) && phys"; cur_cut += tight09v1; 
                      add_to_text( simpleCutText, "QP, tight09v1" ); }

  if( iCut == 305 ) { cur_cut += "jtnhits>7"; add_to_text( simpleCutText, "N^{hit}>7" );}


  
  if (iDebug > 1) {
    cout<<"After selecting what - field: "<<field<<", field2: "<<field2<<", cur_cut: "<<cur_cut.GetTitle()
	<<"\n   dj_bits: "<<dj_bits<<", hotCell: "<<hot_cell_removal
	                  <<", craft data? "<<craft_data<<", pT_weighted?"<<pT_weighted<<", simpleCutText: "<<simpleCutText
	<<"\n   use_MC_weight? "<<use_MC_weight<<", drawOpt: "<<drawOpt<<", pack_the_flows? "<<pack_the_flows
	                        <<", min_bias? "<<min_bias<<", beam_splash? "<<beam_splash
	<<"\n   fieldText: "<<fieldText<<", nBins: "<<nBins<<", min1: "<<min1<<", max1: "<<max1<<endl;
    if (field2.Length() > 0) cout<<"   nBins2: "<<nBins2<<", min2: "<<min2<<", max2: "<<max2<<", text: "<<field2Text<<endl;
  }
    
  bool is_2D = field2.Length() > 0;
  if( is_2D && (!init) && ! TString( hBase->ClassName() ).BeginsWith("TH2") ) {
    cerr<<"ERROR! existing histogram "<<hBase->GetName()<<" should be 2D but is a: "<<hBase->ClassName()<<endl;
    return 0;
  }

  // override as needed for special combinations
  if( leading_jet ) {
    cur_cut = "jtrank==0" + cur_cut;
    if( field.BeginsWith( "jt" ) ) add_to_text( simpleCutText, "leading" );
    if( leading_jet == 2 ) {
      cur_cut += "jtpt>10";
      if(! field.BeginsWith( "jt" ) ) add_to_text( simpleCutText, "leading" );
      add_to_text( simpleCutText, ">10GeV", "" );
    }
    if( leading_jet == 3 ) {
      cur_cut += "jtpt<=10";
      if(! field.BeginsWith( "jt" ) ) add_to_text( simpleCutText, "leading" );
      add_to_text( simpleCutText, "#leq10GeV", "" );
    }
  }    

  // complete missing details
  if( field == field2 ) {cerr<<"field == field2 == "<<field<<" ?! "<<endl; return 0;}
  if( nBins < 0 ) nBins = int( 0.5 + max1 - min1 );
  TString fieldName( fieldText.Length() > 0 ? fieldText : field );
  if( fieldName.Contains( TRegexp( "^(.*)$" ) ) && ! fieldName.Contains( TRegexp( "^(.*).*(.*)$") ) ) {
    fieldName.Remove( fieldName.Length()-1 ); 
    fieldName.Remove( 0, 1 );
  }
  if( top_fraction < 0 ) {
    if( fin > 0 && fin < 1 ) {
      top_fraction = fin;  
    } else {
      top_fraction = 0.15;
      if( (! hide_stat ) && top_fraction < 1 - stat_y_low ) top_fraction = 1 - stat_y_low;
    }
  }

  if( hide_stat ) opt_stat = "";

  TString field2Name;

  if( init ) {
    // figure out how to describe the cut
    TString cutText;
    if( simpleCutText.Length() == 0 ) {
      cutText = cur_cut.GetTitle();
    } else {
      if( simpleCutText.IsWhitespace() ) { // means: don't show this cut!
	cutText = "";
      } else {
	cutText = simpleCutText;
      }
    }
    if( iDebug>20 ) cout<<"adding to cutText '"<<joint_caption (eta_range, pT_range)
			<<"' = '"<<eta_range<<"' + '"<<pT_range<<"'"<<endl;
    cutText = join_text (cutText, joint_caption (eta_range, pT_range));
    cur_cut += eta_range.cut + pT_range.cut;
    if( hot_cell_removal ) {
      if( hot_cell_removal == 1 ) cur_cut += craft_hot_cell_cut;
      if( hot_cell_removal == 2 ) cur_cut += first_data_hot_cell_cut;
      if( hot_cell_removal == 3 ) cur_cut += year_09_900GeV_hot_cell_cut;
      if( !cutText.IsWhitespace() ) cutText += ", ";
      cutText += "!hotCells";
    }
    if( iDebug>15 ) cout<<"temporarily cutText: "<<cutText<<", cur_cut: "<<cur_cut<<endl;
    if( !cutText.IsWhitespace() ) cutText = Form( " {%s}", cutText.Data() );
    if( 0 == dj_bits ) cutText += ", **";
    if( (dj_bits & 0x1) > 0 && format_version >= 4 ) cur_cut += "dj"; // take only jets booked into dijet sample
    if( (dj_bits & 0x8) > 0 ) { // tight calorimetry veto
      if( (dj_bits & 0x4) > 0 ) {cur_cut += tight_dj_cut; cutText += ", TDJ";}
      else {cur_cut += tight_cal_veto; cutText += ", TCV";}
    } else {
      if( (dj_bits & 0x10) > 0 ) { // corrected calorimetry veto
	if( (dj_bits & 0x4) > 0 ) {cur_cut += corr_dj_cut; cutText += ", DJ";}
	else if( (dj_bits & 0x2) > 0 ) {cur_cut += corr_cal_veto; cutText += ", CV";}
      } else { // original, loose, and somewhat biasing veto
	if( (dj_bits & 0x4) > 0 ) {cur_cut += dj_cut; cutText += ", BDJ";}
	else if( (dj_bits & 0x2) > 0 ) {cur_cut += cal_veto; cutText += ", BCV";}
      }
    }
    /*    if( (trigger_bits & 0x1) > 0 ) {
      cur_cut += "GB";
    } else {
      if( LHC_run ) {add_to_text( cutText, "(noL1)" );}
      }*/
    
    delete hBase; // C++ standard says delete does nothing if hBase is 0
    if( is_2D ) { 
      field2Name = (field2Text.Length() > 0) ? field2Text : field2;
      h1 = 0;
      if( iDebug>5 ) cout<<"x binning ("<<nBins<<","<<min1<<","<<max1
			 <<"), y binning ("<<nBins2<<","<<min2<<","<<max2<<")"<<endl;
      h2 = new TH2F( hist_name, Form( "%s:%s%s", field2Name.Data(), fieldName.Data(), cutText.Data() ),
		     nBins, min1, max1, nBins2, min2, max2);
      if( h2->GetSumw2N() == 0 ) h2->Sumw2();
      if( iDebug )
	cout<<tree.GetTitle()<<"->Draw(\""<<Form( "%s:%s", field2.Data(), field.Data())
	    <<"\", \""<<weightCut( cur_cut, use_MC_weight, format_version, pT_weighted )<<"\")"<<endl;
      int nProj = tree.Project( hist_name, Form ("%s:%s", field2.Data(), field.Data() ), 
				weightCut( cur_cut, use_MC_weight, format_version, pT_weighted ) );
      if( iDebug>9 ) cout<<"nProj: "<<nProj<<endl;
      hBase = h2;
    } else { // 1D case
      h2 = 0;
      h1 = new TH1F( hist_name, Form( "%s%s", fieldName.Data(), cutText.Data() ), 
		     nBins, min1, max1 );
      if( h1->GetSumw2N() == 0 ) h1->Sumw2();
      if( iDebug ) cout<<tree.GetTitle()<<"->Draw(\""<<field.Data()<<"\", \""
		       <<weightCut( cur_cut, use_MC_weight, format_version, pT_weighted )<<"\")"<<endl;
      tree.Project( hist_name, field, weightCut( cur_cut, use_MC_weight, format_version, pT_weighted ) );
      if( norm_it ) normalize( *h1, 1. );
      if( pack_the_flows ) packFlows( h1 );
      hBase = h1;
    }
    if( scale != 1 ) {hBase->Scale( scale ); hBase->SetEntries( scale*hBase->GetEntries() );}
  } // if init

  if( hBase == 0 ) {cerr<<"Bug! hBase is NULL after init block"<<endl; return 0;}
  if( iDebug > 1 ) cout<<hBase->GetName()<<" has nEntries: "<<hBase->GetEntries()
		       <<", effec: "<<hBase->GetEffectiveEntries()<<" [ "
		       <<getEffectiveEntriesWithOverflows( hBase )<<" ] "<<endl;

  hBase->GetXaxis()->SetTitle( fieldName );
  hBase->SetLineColor  ( _default_hist_color );
  hBase->SetMarkerColor( _default_hist_color );
  // prepare y-axis title (adjusts units if appropriate)
  if( is_2D ) {
    TString tmp = field2Name;
    if( tmp.BeginsWith( "(" ) && tmp.EndsWith( ")" ) ) {tmp.Remove( tmp.Length()-1 ); tmp.Remove( 0, 1 );}
    if( units.Length() > 0 ) {tmp += " "; tmp += units;}
    hBase->GetYaxis()->SetTitle( tmp );
  } else { // 1D
    if( norm_it ) {
      hBase->GetYaxis()->SetTitle( "Fraction" );
      if( hBase->GetMaximum() > 1 ) hBase->SetMaximum( 1. ); //
      if( logy && ymin == -666 ) {
	ymin = 0.8 * hBase->GetMinimum();
	if( ymin <= 0 ) ymin = 1E-3 * hBase->GetMaximum();
      }
    } else {
      if( use_MC_weight ) {
	// do NOT relabel an existing histogram - that will lead, all too easily
	// to dangerously mislabeled plots...
	if( init ) hBase->GetYaxis()->SetTitle( "#sigma [mb]" );
	int rc = adjustUnits( *hBase ); // readjust if needed (e.g., after xsecs)
	if( rc == -666 ) {cout<<"Oops - failed to adjustUnits"<<endl;}
      } else {
	hBase->GetYaxis()->SetTitle( "Events" );
      }
    }
  }

  if( 0 != args._cumulative ) {
    if( is_2D ) {
      cerr<<"Warning: request for cumulative histogram ignored since this is a 2D histogram."
				  <<"\n    args: "<<args<<endl;
    } else {
      h1 = (TH1F*) accumulation_of( *hBase, args._cumulative < 0 );
      if( h1 == 0 ) {cerr<<"Bug! h1 is NULL after accumulation"<<endl; return 0;}
      hBase = h1;
    }
  }

  if( nBins == 2 && ! is_2D ) {
    Double_t ineff, dineff;
    bool OK = passingFraction( *hBase, hBase->GetBinLowEdge( 2 ), ineff, dineff );
    if( OK ) {
      cout<<"Fraction in higher bin: "<<formValErr( ineff, dineff )<<" = "<<ineff<<" +/- "<<dineff<<endl;
      if( opt_stat.Contains( "m" ) ) opt_stat += "M";
      if( opt_stat.Contains( "r" ) ) opt_stat += "R"; // might as well
      if( stat_x_high == -1 ){
	stat_x_low = 0.55; stat_y_low = 0.7;
      }
    } else {
      cerr<<"passingFraction failed !?"<<endl;
    }
  }
  
  // Prepare global and pad options for draw / fit
  // ---------------------------------------------
  if( how_much >= 2 ) {
    gPad->GetCanvas()->cd(); // get us up to the canvas
    gPad->Clear();
    gPad->SetLogx(logx);
    gPad->SetLogy(logy);
    gPad->SetLogz(logz);
    gPad->SetGridx(gridx);
    gPad->SetGridy(gridy);
    gStyle->SetPalette (1);
    gStyle->SetOptTitle(optTitle);
    gStyle->SetOptStat(opt_stat); 
  }
  if( h1 && fitFunc.Length() ) {
    if( fitFunc.BeginsWith ("pol") ) fitOpt += "F"; // want a chi^2 for polinomial fits.
    if( how_much <= 1 ) fitOpt += "0"; // do not draw.
    h1->Fit( fitFunc, fitOpt );
    TF1* func = h1->GetFunction( fitFunc );
    if( 0 == func ) {cerr<<"Can't find fitted function!"<<endl; return 0;}
    func->SetLineColor( _fit_color );
    func->SetLineWidth( _fit_width );
    if( how_much >= 2 ) gStyle->SetOptFit ();
  } // if fitting

  // Prepare histogram options for draw
  // ----------------------------------
  //    if (yTitleOffset > 0) h1->SetTitleOffset (yTitleOffset, "Y");
  if (ymin != -666) {
    hBase->SetMinimum( ymin );
  } else if( logy==0 && hBase->GetMinimum() > 0 && hBase->GetMinimum() < 0.4 *hBase->GetMaximum() ) {
    hBase->SetMinimum(0);
  }
  if( rotate_phi != -666 ) gPad->SetPhi( rotate_phi );
  ymin = hBase->GetMinimum();
  double ymax = hBase->GetMaximum();

  // leave enough space on the right side of the pad for HCOLZ's color legend
  {
    TString draw_opt_in_lower_case( drawOpt ); draw_opt_in_lower_case.ToLower();
    TVirtualPad* pvp = c1->GetPad(0);
    if( pvp ) {
      Float_t right_margin = pvp->GetRightMargin();
      if( draw_opt_in_lower_case.Contains("hcolz") ) { 
	if( right_margin < 0.15 ) pvp->SetRightMargin( 0.15 );
	if( stat_x_high > 0.85 ) stat_x_high = 0.85; // so it doesn't cover the z-axis labels
      } else {
	pvp->SetRightMargin(0.08);
      }
    }
  }

  // allow logy to work with 0s
  if( (! is_2D ) && logy && hBase->GetMaximum() <= 0) 
    hBase->SetMinimum( TMath::Min( 0.6, 1E-3 * hBase->GetMaximum() ) ); 

  // make sure we have y labels in logy 
  if( (! is_2D ) && logy && ymin > 0 && ymax / ymin < 20 ) hBase->GetYaxis()->SetMoreLogLabels( true );

  // leave enough space on top for text and such
  set_top_fraction( *hBase, top_fraction, logy, iDebug-5 );

  // used to have a first draw here. Hopefully this was just for resizeStatBox and is no longer needed
  if( marker_size > 0 ) hBase->SetMarkerSize( marker_size );
  // if this won't mess up log view
  if( how_much >= 2 ) {
    //if (h1) 
    {
      hBase->Draw(); 
      resizeStatBox( c1, hBase, stat_x_low, stat_y_low, stat_x_high, stat_y_high );
    }
    if( iDebug > 2 ) cout<<"drawOpt: "<<drawOpt<<", statx: "<<stat_x_low<<" - "<<stat_x_high
			 <<" * y: "<<stat_y_low<<" - "<<stat_y_high<<endl;

    if( h1 ) { h1->SetFillColor( _hist_fill_color ); }
    hBase->Draw( drawOpt );
    resizeStatBox( c1, hBase, stat_x_low, stat_y_low, stat_x_high, stat_y_high );
  }
  
  // after all the options
  gPad->GetCanvas()->cd();
  gPad->Update();
  if (how_much >= 3) {
    gPad->SaveAs( Form( "figs/%s.eps", hist_name.Data() ) );
    gPad->SaveAs( Form( "iwin/%s.png", hist_name.Data() ) );
    gPad->SaveAs( Form( "pads/%s.root", hist_name.Data() ) );
  }
  
  
  if( is_2D && equalize_x_max && nBins>2 && ! drawOpt.Contains( "text" ) ) {
    TH2* eq_hist = equalize_max_of_x_slices( * (TH2*) hBase );
    if( eq_hist == 0 ) {
      cerr<<"ERROR! normalize_x_slices falied for: "<<hBase->GetName()<<endl;
    } else {
      eq_hist->Draw( drawOpt );
      gPad->GetCanvas()->cd();
      gPad->Update();
      if (how_much >= 3) {
	gPad->SaveAs( Form( "figs/%s.eps", eq_hist->GetName() ) );
	gPad->SaveAs( Form( "iwin/%s.png", eq_hist->GetName() ) );
	gPad->SaveAs( Form( "pads/%s.root", eq_hist->GetName() ) );
      }
    }
  }
  return hBase;
}


//-----------------------------------------------------------------------------------------

TH2* jet_studies::eta_phi( const on& args )
{
  int eta = iEta( args );
  bool onlyHBHE = true;
  range rr = eta_range_at( eta );
  if( eta > 0 && rr.isEmpty() ) {cerr<<"Warning: unknown eta range: "<<eta<<endl; return 0;}
  
  if( (! rr.isEmpty()) && rr.to > 2.5 ) onlyHBHE = false;
  
  TH2* ph = (TH2*) plot( on(args).abs_what( 14 ).var2( onlyHBHE ? 8 : 5 ) );

  gPad->GetCanvas()->cd();
  gPad->SetLogz(1);
  gPad->Update();
  if( args._how_much >= 3 ) {
    gPad->SaveAs( Form( "figs/%s_lz.eps", ph->GetName() ) );
    gPad->SaveAs( Form( "iwin/%s_lz.png", ph->GetName() ) );
    gPad->SaveAs( Form( "pads/%s_lz.root", ph->GetName() ) );
  }
  return ph;
}


//-----------------------------------------------------------------------------------------

TH2* jet_studies::online( const on& args )
{
  TH2* ph = (TH2*) plot( on(args).abs_what( 200 ).var2( 20 ) );
  return ph;
}


// ---------------------------------------------------------------------------------------------
// Wrappers that meant to add a kind of a ROOT-ish safe cast (see below :-( ) - Note changed "howMuch" default!
TH1F* jet_studies::getTH1F( const on& args )
{
  TH1* ph = plot( args );
  if( ph ) {
    if( ! ph->InheritsFrom ("TH1F") ) {
      cerr<<"Bug! plot "<<ph->GetName()<<" (from "<<args<<") is not a TH1F"<<endl;
      return 0;
    }
  }
  return (TH1F*) ph;
}


TH1D* jet_studies::getTH1D (const on& args )
{
  TH1* ph = plot( args );
  if( ph ) {
    if( ! ph->InheritsFrom ("TH1D") ) {
      cerr<<"Bug! plot "<<ph->GetName()<<" (from "<<args<<") is not a TH1D"<<endl;
      return 0;
    }
  }
  return (TH1D*) ph;
}

TH2F* jet_studies::getTH2F( const on& args )
{
  TH1* ph = plot( args );
  if( ph ) {
    if( ! ph->InheritsFrom ("TH2F") ) {
      cerr<<"Bug! plot "<<ph->GetName()<<" (from "<<args<<") is not a TH2F"<<endl;
      return 0;
    }
  }
  return (TH2F*) ph;
}

// ---------------------------------------------------------------------------------------------
// e.g. "njets {pthat>50}" --->  "pthat>50"
TString extract_cut( const TString& title, int debug = 0 ) {
  TString sCut;
  // ss is edited to simplify figuring out locations in title (so # of characters is preserved, etc.)
  TString ss( title );
  ss.ReplaceAll( "_{T}", "_%@#" ); // reduces number of nested brackets
  int iOpenLastInnerNestedBracket;
  while( (iOpenLastInnerNestedBracket = ss.Index( TRegexp( "{[^{}]*}[^{]*}[^{}]*$" ) ) ) > 0 ) {
    int iCloseLastInnerNextedBracket = ss.Index( TRegexp( "}[^{}]*}[^{}]*$" ), iOpenLastInnerNestedBracket );
    if( debug > 1 ) cout<<ss<<": "<<iOpenLastInnerNestedBracket<<" - "<<iCloseLastInnerNextedBracket<<endl;
    if( iCloseLastInnerNextedBracket <= 0 ) break;
    ss.Replace( iCloseLastInnerNextedBracket, 1, "_" );
    ss.Replace( iOpenLastInnerNestedBracket, 1, "_" );
  }
  int iOpenBracket = ss.Index( TRegexp ( "{[^}]*}[^{}]*$" ) );// That is, the start of the last curly brackets
  int iCloseBracket = title.Last( '}' );
  if( iOpenBracket >= 0 && iCloseBracket > iOpenBracket ) {
    sCut = title( 1+iOpenBracket, -1 + iCloseBracket - iOpenBracket ); // ROOT's brilliant hiding spot for substr
  }
  if( debug ) cout<<ss<<": "<<iOpenBracket<<" - "<<iCloseBracket<<"; title: "<<title<<" --> "<<sCut<<endl;
  return sCut;
}

TString after_cut( const TString& title ) {
  TString after;
  TString ss( title );
  ss.ReplaceAll( "_{T}", "_%@#" ); // otherwise it messes up the reg. exp.
  Ssiz_t iStart = ss.Index( TRegexp ( "{[^}]*}[^{}]*$" ) );// That is, the start of the last curly brackets
  Ssiz_t iEnd = title.Last( '}' );
  bool found = false;
  if( iStart>= 0 && iEnd > iStart ) found = true;
  if( ! found ) {
    iEnd = -1;
    Ssiz_t index;
    cout<<"Before while: "<<ss<<endl;
    while( (index = ss.Index( ", ", 1+iEnd )) >= 0 ) {
      iEnd = index;
      found = true;
    }
    if( iEnd >= 0 ) {found = true; --iEnd;}
  }
  if( found ) after = title( 1+iEnd, -1 + title.Length() - iEnd ); // ROOT's brilliant hiding spot for substr
  return after;
}

// e.g. "njets   {pthat>50}   for me" --->  "njets, for me"
TString remove_cut (const TString& title) {
  TString out (title);
  TString ss (title);
  ss.ReplaceAll ("_{T}", "_%@#"); // otherwise it messes up the reg. exp.
  int iOpenBracket = ss.Index (TRegexp ("{[^}]*}[^{}]*$"));// That is, the start of the last curly brackets
  int iCloseBracket = title.Last('}');
  if (iOpenBracket >= 0 && iCloseBracket > iOpenBracket) {
    TString start  = stripEnd (title(0,iOpenBracket));
    TString end = stripStart (title(1+iCloseBracket, title.Length()));
    out = join_text (start, end, ", ");
    //const char *pc = title.Data();
    //sCut = TString (pc + iOpenBracket + 1);
    //sCut.Remove (-1 + iCloseBracket - iOpenBracket);
  }
  return out;
}

// e.g. "njets   {pthat>50}   for me", "bla" --->  "njets {bla} for me"
TString replace_cut( const TString& title, const TString& new_cut ) {
  TString out( title );
  TString ss( title );
  ss.ReplaceAll ("_{T}", "_%@#"); // otherwise it messes up the reg. exp.
  int iOpenBracket = ss.Index (TRegexp ("{[^}]*}[^{}]*$"));// That is, the start of the last curly brackets
  int iCloseBracket = title.Last('}');
  if (iOpenBracket >= 0 && iCloseBracket > iOpenBracket) {
    TString start  = stripEnd (title(0,iOpenBracket));
    TString end = stripStart (title(1+iCloseBracket, title.Length()));
    if( ! end.BeginsWith(",") ) end.Insert( 0, " " );
    out = join_text (start, new_cut, " ");
    add_to_text( out, end, "");
  }
  return out;
}

// Optional 2nd stage processing of histograms - messes up nEntries, so trying to avoid it...
// ---------------------------------------------------------------------------------------------
// the "create on demand" logic is not yet implemented for slices
bool jet_studies::slice( const on& args )
{
  if( args._debug ) cout<<"Slice what: "<<args._what<<endl;
  TH2F* h2 = getTH2F( on(args).how_much( 0 ).less_debug() );
  if( h2 == 0 ) return false;

  int myDebug = args._debug % 10;
  
  TCanvas *c1 = 0;
  // Rene Brun's stop gag measure circa 2000. Anything better available?
  c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height ); // a bit higher than usual
  gPad->GetCanvas()->cd(); // get us up to the canvas
  gPad->Clear();
  gPad->SetLogy(0);
  gPad->SetLogx(0);
  gPad->SetLogz(0);
  gStyle->SetPalette( 1 );

  bool gridx = 1, gridy = 1, logx = 0, logy = 0, logz = 0;
  int optStat = 1111, optTitle = 1;
  float statxl = 0.65, statyl = 0.75;
  int iFirst = 1, iLast = 1;
  double ymin = -666;
  TString drawOpt (""); // don't use hist, as error bars are often important

  bool init = args._what < 0;

  TString hname = h2->GetName();
  TString xname = h2->GetXaxis()->GetTitle();
  TString yname = h2->GetYaxis()->GetTitle();
  TString ynameLower( yname ); ynameLower.ToLower();
  TString curname( hname + "_" );
  TString curtitle = h2->GetTitle();

  TString oldCut = extract_cut( curtitle );

  bool varBins = false;
  float ylow=-666, yhigh=-666;
  if (ynameLower.Contains ("pt") || ynameLower.Contains ("eta")) {
    cerr<<"pT and eta slices should now be done using build in ranges, not by slicing!"<<endl;
    return false;
  }
  /*if (yname.Contains ("pt") && how > 0) {
    varBins = true;
    float ptEdges [g_nj_nPtBins+1] = {0, 10, 20,  30, 50, 80,  130, 210, 340,  550, 900, 1500,  3000, 9999}; 
    if (how > g_nj_nPtBins) {cerr<<"Problem: how > "<<g_nj_nPtBins<<endl; return false;}
    curname += Form ("s%d", how);
    ylow  = ptEdges [how - 1];
    yhigh = ptEdges [how];
  }
  if (yname == "abs(jtdeta)" && how > 0) {
    varBins = true;
    if (how > g_nj_nEtaBins) {cerr<<"how > nEtaBins = "<<g_nj_nEtaBins<<endl; return false;}
    curname += Form ("s%d", how);
    ylow  = (how == 1) ? 0 : g_nj_etaEdges [how - 2];
    yhigh =                  g_nj_etaEdges [how - 1];
  }
  if( ( yname == "jtdeta" || yname == "jteta" ) && how > 0) {
    varBins = true;
    if (how > 2*g_nj_nEtaBins) {cerr<<"how > 2*nEtaBins = "<<2*g_nj_nEtaBins<<endl; return false;}
    curname += Form ("s%d", how);
    if (how > g_nj_nEtaBins) {
      ylow  = (how == 1+g_nj_nEtaBins) ? 0 : g_nj_etaEdges [how - g_nj_nEtaBins - 2];
      yhigh =                                g_nj_etaEdges [how - g_nj_nEtaBins - 1];
    } else {
      ylow  =                              - g_nj_etaEdges [g_nj_nEtaBins - how    ];
      yhigh = (how == g_nj_nEtaBins) ? 0 : - g_nj_etaEdges [g_nj_nEtaBins - how - 1];
    }
    } */
  if( varBins ) {
    iFirst = h2->GetYaxis()->FindBin(ylow);
    iLast  = h2->GetYaxis()->FindBin(yhigh) - 1; // so each bin appears only once!
    if( myDebug ) cout<<"ylow: "<<ylow<<" yhigh: "<<yhigh<<" --> "<<iFirst<<" - "<<iLast<<endl;
    if( iFirst >= 1 )              ylow  = h2->GetYaxis()->GetBinLowEdge(iFirst);
    if( iLast <= h2->GetNbinsY() ) yhigh = h2->GetYaxis()->GetBinLowEdge(1+iLast);
    if( oldCut ) {
      curtitle = Form ("%s {%.0f<%s<%.0f, %s}", xname.Data(), ylow, yname.Data(), yhigh, oldCut.Data());
    } else {
      curtitle = Form ("%s {%.0f<%s<%.0f}", xname.Data(), ylow, yname.Data(), yhigh);
    }
  }

  if( ylow < 0 ) {cout<<"This kind of slicing is NYI. yname: "<<yname<<endl; return false;}

  // is auxilliary ready for this h2?
  TH1* hAux = (TH1*) gDirectory->Get( Form( "%s_py", h2->GetName() ) );
  if( init || hAux == 0 ) { // if not, or if reinit forced, make it:
    delete hAux; // C++ standard says delete does nothing if hBase is 0
    hAux = h2->ProjectionY ();
  }

  // do we need an automatic init for this slice?
  TH1* hBase = (TH1*) gDirectory->Get (curname);
  if( hBase == 0 ) init = 1;

  // init
  if( init ) {
    delete hBase; // C++ standard says delete does nothing if hBase is 0
    if( myDebug ) cout<<"DBG "<<hname<<"->ProjectionX ("<<curname<<", "<<iFirst<<", "<<iLast<<")"<<endl;
    hBase = h2->ProjectionX( curname, iFirst, iLast );
    hBase->SetTitle( curtitle );
  }
  hAux->GetXaxis()->SetRange( iFirst, iLast );
  _doubles[ std::string(curname+"mean"   ) ] = hAux->GetMean();
  _doubles[ std::string(curname+"meanerr") ] = hAux->GetMeanError();
  if( myDebug > 1 ) cout<<"range: "<<iFirst<<" to "<<iLast<<" of "<<hAux->GetName()
		       <<" --> "<<hAux->GetMean()<<" @ "<<hAux->GetMeanError()<<endl;

  //bool xIsPt = xname.Contains("pt") && ! xname.Contains(TRegexp("pt.* /.*pt")); *//*
  //bool yIsPt = yname.Contains("pt") && ! yname.Contains(TRegexp("pt.* /.*pt"));*//*
  //if (xIsPt && !yIsPt) logx = true;
    
  gStyle->SetOptTitle(optTitle);
  gStyle->SetOptStat(optStat); 
  //    if (yTitleOffset > 0) h1->SetTitleOffset (yTitleOffset, "Y");
  if( ymin != -666 ) hBase->SetMinimum( ymin );
  gPad->SetLogx(logx);
  gPad->SetLogy(logy);
  gPad->SetLogz(logz);
  gPad->SetGridx(gridx);
  gPad->SetGridy(gridy);
  hBase->SetMaximum(); // unset
  hBase->SetMaximum((logy ? 10 : 1.3)*hBase->GetMaximum()); // get natural max, and increase (need space for stat box)
  hBase->SetLineColor   ( _default_hist_color );
  hBase->SetMarkerColor ( _default_hist_color );
  hBase->Draw(drawOpt);
  resizeStatBox (c1, hBase, statxl, statyl);
  // if this won't mess up log view
  if( logy==0 && hBase->GetMinimum() > 0 && hBase->GetMinimum() < 0.4 *hBase->GetMaximum() ) hBase->SetMinimum(0);
  // and draw twice.
  hBase->Draw( drawOpt );

  // after all the options
  gPad->GetCanvas()->cd();
  gPad->Update();
  if( args._write_level ) {
    gPad->SaveAs( Form( "figs/%s.eps", curname.Data() ) );
    gPad->SaveAs( Form( "iwin/%s.png", curname.Data() ) );
  }
  return true;
}

// -------------------------------------------------------------------------------------------
TH1D* jet_studies::get_slice( const on& args )
{
  TString name( build_name( args._what, args._var2, args._cut, iEta( args ), iPt( args ), args._suffix ) );
  if( args._slice >= 0 ) name += Form( "_s%d", args._slice );
  TH1D* h1 = (TH1D*) gDirectory -> Get( name );
  if( h1 == 0 ) {cout<<"Can't find TH1D histogram \""<<name<<"\""<<endl;}
  return h1;
}

// ---------------------------------------------------------------------------------------------
bool jet_studies::slices( const on& args )
{
  TH2F* h2 = getTH2F( on(args).how_much( 0 ).less_debug() );
  if( h2 == 0 ) return false;

  bool OK = true;

  TString yname = h2->GetYaxis()->GetTitle();
  // NYI:  if( how == 0 ) {
  int nSlices = -1;
  if( yname.Contains("pt") ) nSlices = _n_pT_bins;
  if( yname.Contains("eta") ) {
    if( yname.Contains("abs") ) {
      nSlices = _n_eta_bins;
    } else {
      nSlices = 2*_n_eta_bins;
    }
  }
  if( nSlices < 0 ) {cerr<<"Don't know into how many slices to slice that"<<endl; return false;}
  for( int iSlice = 1; iSlice <= nSlices; ++iSlice ) {
    bool curOK = slice( on(args).slice( iSlice ) );
    if( !curOK ) OK = false;
  }
  return OK;
}

// ---------------------------------------------------------------------------------------------
void jet_studies::get_slice_hists( vector<TH1*> &hists, const on& args )
{
  cout<<"Looking for variable histograms, until one is not found...";
  for( int isl=1; true;++isl ) {
    TH1* h1 = get_slice( on( args ).slice( isl ) );
    if( h1 == 0 ) break;
    if( h1->GetEffectiveEntries() > args._min_entries ) hists.push_back( h1 );
  }
}

void jet_studies::get_hists( vector<TH1*> &hists, vector<TH1*> &xHists, TString& title, vector<TString> &captions,
			     const on& args )
{
  title = "";
  hists.clear();
  xHists.clear();
  captions.clear();
  int eta = iEta( args );
  int pt = iPt( args );
  bool scan_eta = eta < 0;
  bool scan_pT = pt < 0;
  int iFirst = -1;
  if( scan_pT == scan_eta ) {
    cerr<<"get_hists needs exactly one negative input out of iEta: "<<eta<<" & iPt: "<<pt<<endl; 
    return;
  }

  if( scan_pT ) {
    range eta_range = eta_range_at( eta );
    if( eta > 0 && eta_range.isEmpty() ) {cerr<<"Warning: unknown eta range in "<<_name<<": "<<eta<<endl; return;}
    int nBin = 0;
    int iWhatX = args._aux_probe ? 34 : 35; // what to plot (with many bins) for the x-axis remapping, normal is tagpt
    if( args._aux_probe ) iWhatX = 34; // probe jet pT
    if( pt == -1 ) nBin = _n_coarse_pT_bins;
    if( pt == -10 || pt == -30 || pt == -50 ) { 
      nBin = _n_pT_bins;
      if( pt == -30 ) { // no use scanning bins where we run out of physical (tag) jets
	if( eta_range.from > 2.5 ) nBin -= 2; 
	if( eta_range.from > 3 ) nBin -= 2; 
      }
    }
    if( pt == -210 || pt == -230 || pt == -250 || pt == -180 ) {
      nBin = _n_pT_bins_2TeV;
    }
    if( pt == -10 || pt == -210 ) {iWhatX = 34;}
    if( pt == -50 || pt == -250 ) {iWhatX = 36;}
    if( nBin <= 0 ) {cerr<<"get_hists ERROR. Unknown iPt index: "<<pt<<endl; return;}
    if( args._debug ) cout<<"DBG - pt: "<<pt<<" -> nBin: "<<nBin<<", iWhatX: "<<iWhatX<<endl;
    for( int ib=0; ib < nBin; ++ib ) {
      TH1* h1 = getTH1F( on( args ).pT( ib-pt ).how_much( 0 ) );
      if( h1 == 0 ) break;
      if( args._debug ) cerr<<h1->GetName()<<" has "<<h1->GetEntries()<<" entries, "
			    <<"and "<<h1->GetEffectiveEntries()<<" [ "
			    <<getEffectiveEntriesWithOverflows( h1 ) <<" ] effective entries"<<endl;
      if( h1->GetEntries() > 4 * h1->GetEffectiveEntries() ) h1->SetOption("E1");
      if( ( args._min_entries >= 0 && getEffectiveEntriesWithOverflows( h1 ) > args._min_entries ) ||
	  ( args._min_entries < 0 && h1->GetEntries() > - args._min_entries ) ) {
	if( ! args._no_aux ) {
	  TH1* haux = getTH1F( on(args).abs_what( iWhatX ).pT( ib - pt ).how_much( 0 ) );
	  if( haux == 0 ) {cerr<<"Bug? can't get aux of "<<h1->GetName()<<endl; break;}
	  xHists.push_back( haux );
	}
	hists.push_back( h1 );
	captions.push_back( pT_range_at( ib-pt ).caption );
	if( hists.size() == 1 ) iFirst = ib - pt;
      } // if should be used
    }
  } else { // scan eta
    range pT_range = pT_range_at( pt );
    if( pt > 0 && pT_range.isEmpty() ) {cerr<<"Warning: unknown pT range in "<<_name<<": "<<pt<<endl; return;}
    int nBin = 0;
    int iWhatX = 32; // remapping to jtdeta (#eta), so plot it with many bins
    if( eta == -1 || eta == -11 || eta == -21 ) nBin = _n_coarse_eta_bins;
    if( eta == -100 || eta == -300 || eta == -500 ) nBin = _n_eta_bins;
    if( eta == -1 || eta == -100 ) iWhatX = 33; // in these case it's to |#eta|
    if( nBin <= 0 ) {cerr<<"get_hists ERROR. Unknown iEta index: "<<eta<<endl; return;}
    for( int ib=0; ib < nBin; ++ib ) {
      TH1* h1 = getTH1F( on( args ).pT( ib - eta ).how_much( 0 ) );
      if( h1 == 0 ) break;
      if( args._debug ) cerr<<h1->GetName()<<" has "<<h1->GetEffectiveEntries()<<" [ "
			    <<getEffectiveEntriesWithOverflows( h1 ) <<" ] effective entries"<<endl;
      if( getEffectiveEntriesWithOverflows( h1 ) > args._min_entries ) {
	if( ! args._no_aux ) {
	  TH1* haux = getTH1F( on( args ).abs_what( iWhatX ).pT( ib - eta ).how_much( 0 ) );
	  if( haux == 0 ) {cerr<<"Bug? can't get aux of "<<h1->GetName()<<endl; break;}
	  xHists.push_back( haux );
	} 
	hists.push_back( h1 );
	captions.push_back( eta_range_at( ib-eta ).caption );
	if( hists.size() == 1 ) iFirst = ib - eta;
      } // if should be used
    }
  }
  // Figure out the title
  if( hists.size() > 0 ) {
    TString oldCut = extract_cut( hists[0]->GetTitle() );
    int eta0 = scan_eta ? iFirst : eta;
    int pt0  = scan_pT  ? iFirst : pt;
    TString sRegion = joint_caption( eta_range_at( TMath::Abs(eta0) ), pT_range_at( TMath::Abs(pt0) ) );
    Ssiz_t iFound = oldCut.Index( sRegion );
    if( args._debug || iFound < 0 ) cout<<"oldCut: "<<oldCut<<", iFirst: "<<iFirst<<", eta0: "<<eta0
					<<", pt0: "<<pt0<<" -> sRegion: "<<sRegion<<" -> "<<iFound<<endl;
    if( iFound < 0 ) {cerr<<"Bug! Can't parse title cuts"<<endl; return;}
    TString restOfOldCut = oldCut( 0, iFound ) + oldCut( iFound + sRegion.Length(), oldCut.Length() );
    range sNewRegion = scan_pT ? eta_range_at( TMath::Abs (eta) ) : pT_range_at( TMath::Abs (pt) );
    TString newCut = join_text( restOfOldCut, sNewRegion.caption );
    title = stripStart( stripEnd( replace_cut( hists[0]->GetTitle(), newCut ) ) );
    if( args._debug ) cout<<"restOfOldCut: "<<restOfOldCut<<", sNewRegion: "<<sNewRegion.caption<<" -> newCut: "<<newCut
			  <<" -> title: "<<title<<endl;
    if( iFound < 0 ) {cerr<<"Bug! Can't parse title cuts"<<endl; return;}
  }
  return;
}


// ---------------------------------------------------------------------------------------------

TGraphErrors* jet_studies::get_remap_graph( const on& args )
{
  TGraphErrors* pgr = 0;
  bool no_fit = args._func <= 0;
  TString func_suffix = no_fit ? "" : Form( "_%d", args._func );
  
  // do we need an automatic init?
  TString grName( build_name( args._what, args._var2, args._cut, iEta( args ), iPt( args ), 
			      args._suffix + func_suffix, "m" ) );
  pgr = (TGraphErrors*) gDirectory->Get( grName );
  if( pgr && ! args._init ) return pgr;
  
  // something's missing, prepare everything
  if( args._debug % 10 ) cout<<"DBG creating the remapping. grName: "<<grName<<" -> pgr? "<<bool(pgr != 0)<<endl;
  if( pgr ) gDirectory->Delete( grName ); // we don't own it since it was added to gDirectory!
  
  vector<TH1*> hists, xHists;
  vector<TString> captions;
  TString title;
  get_hists( hists, xHists, title, captions, on( args ).less_debug() );
  int nHist = hists.size();
  cout<<"Found "<<nHist<<" histograms, title: "<<title<<endl;
  if( nHist < 1 ) return 0;
  if( xHists.size() != hists.size() ) {cerr<<"Bug! size mismatch hists vs. xHists"<<endl; return 0;}
  int myDebug = args._debug % 10;

  Double_t *grx = new Double_t [nHist], *gry = new Double_t [nHist];
  Double_t *ex = new Double_t [nHist], *ey = new Double_t [nHist]; // for the graph

  for( int ih=0; ih<nHist; ++ih ) {
    TH1* ph  = hists[ih];
    TH1* phx = xHists[ih]; 
    ex  [ih] = phx->GetMeanError();
    grx [ih] = phx->GetMean();
    if( ih > 0 && grx[ih] <= grx[ih-1] ) cerr<<"WARNING! remapping is not monotonous at bin #"<<ih<<endl;
    ey  [ih] = ph->GetMeanError();
    gry [ih] = ph->GetMean();
  }
  if( myDebug > 1 ) cout<<"DBG: prepared vectors"<<endl;

  // Figure out the title
  if( title.Length() > 0 && title(0) != ' ' ) title = " "+title;
  TString grTitle( "remapping" + title );

  pgr = new TGraphErrors( nHist, grx, gry, ex, ey );
  delete[] grx;
  delete[] gry;
  delete[] ex;
  delete[] ey;
  if( myDebug > 1 ) cout<<"Made graph with "<<nHist<<" data points"<<endl;
  pgr->SetName( grName );
  pgr->SetTitle( grTitle );
  pgr->GetXaxis()->SetTitle( Form("<%s>", xHists[0]->GetXaxis()->GetTitle() ) );
  pgr->GetYaxis()->SetTitle( Form("<%s>",  hists[0]->GetXaxis()->GetTitle() ) );
  pgr->SetLineColor   ( _default_hist_color );
  pgr->SetMarkerColor ( _default_hist_color );
  if( myDebug > 1 ) cout<<"Set it to name: "<<grName<<endl;
  gDirectory->Add( pgr ); // so it won't disappear when the canvas is cleared / deleted!
  
  //if (nHist < 2) {cerr<<"Can't fit with only 1 bin"<<endl; return 0;}
  
  return pgr;
}

// ---------------------------------------------------------------------------------------------
TF1* jet_studies::fit_remapping( const on& args )
{
  int iFunc = args._func;

  TString funcSuffix ( Form( "_%d", iFunc ) );
  TString grName( build_name( args._what, args._var2, args._cut, iEta( args ), iPt( args ), 
			      args._suffix + funcSuffix, "m" ) );
  TGraphErrors* pgr = (TGraphErrors*) gDirectory->Get( grName );
  if( pgr == 0 ) {cerr<<"Can't find graph to fit ("<<grName<<")."<<endl; return false;}

  TString funcName( build_name( args._what, args._var2, args._cut, iEta( args ), iPt( args ), 
				args._suffix + funcSuffix, "f" ) );
  TF1* pFunc = pgr->GetFunction( funcName );
  if( pFunc && ! args._init ) return pFunc;

  // something's missing, prepare everything
  if( args._debug ) cout<<"DBG fitting the remapping. funcName: "<<funcName<<" -> pFunc? "<<bool(pFunc != 0)<<endl;
  // is it even possible to get rid of a graph's function?
  
  int nHist = pgr->GetN();
  if( nHist < 2 ) {cerr<<"Can't fit with only 1 bin"<<endl; return 0;}
  
  if( iFunc < 1 || iFunc > 7 ) {cerr<<"Fit function type iFunc: "<<iFunc<<" is NYI"<<endl; return 0;}
  TString exponent( "[0]+[1]*log(x)" );
  for( int iterm=2; iterm <= iFunc; ++iterm ) exponent += Form( "+[%d]*log(x)**%d", iterm, iterm );
  TString formula( Form( "x-exp(%s)", exponent.Data() ) );
  if( args._debug ) cout<<"DBG will fit: "<<formula<<endl;

  Double_t* grx = pgr->GetX();
  double maxx = 1.1*grx[ nHist-1 ] - 0.1*grx[ nHist-2 ];
  double minx = 50; // below 10 method breaks down, but dijets are suspect up to 50

  pFunc = new TF1( funcName.Data(), formula.Data(), minx, maxx );
  pFunc->SetParameter( 0, 1 );
  pFunc->SetParameter( 1, 1 );
  for( int iterm=2; iterm <= iFunc; ++iterm ) pFunc->SetParameter( iterm, 0.0001 );
  pgr->Fit( pFunc, "", "", minx, maxx );
  return pFunc; // or do I need to retrieve the pointer from the graph?
}

// ---------------------------------------------------------------------------------------------

bool jet_studies::plot_remap( const on& args )
{
  int iFunc = args._func;
  int eta = iEta( args );
  range eta_range = eta_range_at( eta );
  if( eta > 0 && eta_range.isEmpty() ) {cerr<<"Warning: unknown eta range in prepRemap: "<<eta<<endl; return false;}
  int remap_to_what = 34, pT = -30; // 34 = finely binned jtpt

  TGraphErrors* pge = get_remap_graph( on(args).abs_what( remap_to_what ).pT( pT ).less_debug() );
  if( pge == 0 ) return 0;
  if( iFunc > 0 ) {
    TF1* pf = fit_remapping( on(args).abs_what( remap_to_what ).pT( pT ).less_debug() );
    if( pf == 0 ) return 0;
    pf->SetLineWidth( _fit_width );
  }

  //->SetMinimum(0);
  //double min = getGraphsGraphicalMin (gae);
  gPad->SetGridx(true);
  gPad->SetGridy(true);
  pge->Draw("AP");
  double min1 = 0.9*getGraphsGraphicalMin( pge );
  pge->SetMinimum( min1 );
  double max1 = 1.1*getGraphsGraphicalMax( pge );
  pge->SetMaximum( max1 );
  pge->Draw("AP");
  double max = TMath::Min( max1, pge->GetXaxis()->GetXmax() );
  double min = TMath::Max( min1, pge->GetXaxis()->GetXmin() );
  if( args._debug > 8 ) cout<<"DBG min1: "<<min1<<", x-axis min: "<<pge->GetXaxis()->GetXmin()<<" -> "<<min
			    <<" (via hist: "<<pge->GetHistogram()->GetXaxis()->GetXmin()<<")."<<endl;
  if( args._debug > 8 ) cout<<"DBG max1: "<<max1<<", x-axis max: "<<pge->GetXaxis()->GetXmax()<<" -> "<<max
			    <<" (via hist: "<<pge->GetHistogram()->GetXaxis()->GetXmax()<<")."<<endl;
  TLine* lxy = new TLine( min, min, max, max );
  lxy->SetLineColor( kRed );
  lxy->Draw();
  
  gPad->SetLogx();
  gPad->SetLogy();
  
  gPad->Update();
  relocateStatBox( .55, .18, .95, .48 );
  
  // after all the options
  gPad->GetCanvas()->cd();
  gPad->Update();
  if( args._write_level ) {
    TString sFunc;
    if( iFunc > 0 ) sFunc = Form( "_%d", iFunc );
    gPad->SaveAs( Form( "figs/%s%s.eps", pge->GetName(), sFunc.Data() ) );
    gPad->SaveAs( Form( "iwin/%s%s.png", pge->GetName(), sFunc.Data() ) );
    gPad->SaveAs( Form( "pads/%s%s.root", pge->GetName(), sFunc.Data() ) );
  }
  return true;
}

// ---------------------------------------------------------------------------------------------
// Scans on either iEta or iPt, see getHists above
//
TGraphAsymmErrors * jet_studies::get_ineffs( const on& args )
{
  bool init = args._init;
  float cut_value = args._cut_value;
  bool pass_above = args._pass_above;
  bool rejections = args._rejection;

  int iDebugMe = args._debug % 10;
  if( iDebugMe ) {
    cout<<"make (in)effs for: "<<args._what;
    if( args._fin != 0 ) cout<<", fin: "<<args._fin;
    cout<<endl;
  }

  // do we need an automatic init?
  TString sCV( formatInNaturalPrecision(cut_value, 3).ReplaceAll('.','_'));
  TString prefix;
  if( ! rejections ) prefix = "e";
  if( args._aux_probe ) prefix += "a";
  TString grName( build_name( args._what, 0, args._cut, iEta( args ), iPt( args ), 
			      (pass_above ? "G" : "L") + sCV + args._suffix, prefix ) );
  TGraphAsymmErrors* pgr = (TGraphAsymmErrors*) gDirectory->Get( grName );
  if( pgr == 0 ) init = true;

  if( ! init ) return pgr;

  // need to recreate graph
  delete pgr; // C++ standard says delete does nothing if pgr is 0
  if( iDebugMe ) cout<<"DBG creating "<<grName<<endl;
 
  TGraphErrors* remap = 0;
  TString xLabel( "p_{T}" );
  if( args._remap ) {
    xLabel = "<p_{T}>";
    if( iPt( args ) >= 0 ) {cerr<<"Only remappings to jet pT implemented so far (so need iPt<0 & iEta>0)."
	                          " iPt=-30 is common (scans tag jet pT)"<<endl; return 0;}
    remap = get_remap_graph( on( args ).what( 34 ).func( 0 ).less_debug() ); // remap to jtpt
    if( remap == 0 ) {cerr<<"Aborting. Can't find any relevant remapping."<<endl; return 0;}
    if( iDebugMe > 1 || TString( remap->GetName() ).Contains("_") ) 
      cout<<"DBG using remap hist. "<<remap->GetName()<<endl; 
  }

  vector<TH1*> hists, xHists;
  vector<TString> captions;
  TString title;
  get_hists( hists, xHists, title, captions, on( args ).less_debug() );
  int nHist = hists.size();
  cout<<"Found "<<nHist<<" histograms, title: "<<title<<endl;
  if( nHist < 1 ) return 0;
  if( xHists.size() != hists.size() ) {cerr<<"Bug! size mismatch hists vs. xHists"<<endl; return 0;}

  int nBins = hists[0]->GetNbinsX();
  int cutBin = hists[0]->FindBin(cut_value);
  if( cutBin < 1 || cutBin > nBins ) {cerr<<"cut_value ("<<cut_value<<") is outside histogram x-axis"<<endl; return 0;}

  if( hists[0]->GetBinLowEdge( 1 ) == -0.5 && hists[0]->GetBinLowEdge( 2 ) == 0.5 
      && TMath::Abs( -0.5 + cut_value - TMath::Floor( cut_value ) ) > 0.01 ) {
    cerr<<"\n=========================================================\n"
	<<"\nWarning! For integer variables (as indicated by binning),"
	<<"\n         use cut values ending in .5 (at bin boundaries)!"
	<<"\n\n========================================================="<<endl;
  }
  
  bool OK;
  Double_t *grx = new Double_t [nHist], *gry = new Double_t [nHist], *exl = new Double_t [nHist];
  Double_t *exh = new Double_t [nHist], *eyl = new Double_t [nHist], *eyh = new Double_t [nHist]; // for the graph
  int nTooSoft = 0;
  for( int ih=0; ih<nHist; ++ih ) {
    TH1* ph = hists[ih];
    TH1* phx = xHists[ih];
    exl [ih] = exh [ih] = phx->GetMeanError(); // a bit of an approximation when using remapping...
                                               // also neglects remapping uncertainties
    if( args._remap ) {
      grx [ih] = remap->Eval( phx->GetMean() );
      if( iDebugMe > 5 ) cout<<"DBG ih: "<<ih<<" mean: "<<phx->GetMean()<<" --> "<<grx[ih]<<endl;
      //
      // NOTE: if remapping no longer uses the same events (e.g. fancy fitting separate for good and bad)
      //       will need a more sophisticated treatment in case things get soft again
      //
      if( ih > 0 && grx[ih-1] > 0 && grx[ih] <= grx[ih-1] ) cerr<<"WARNING! remapping is not monotonous @"<<ih<<endl;
      if( grx[ih] < args._min_pT ) ++nTooSoft;
    } else {
      grx [ih] = phx->GetMean();
    }
    if( iDebugMe > 8 ) cout<<"DBG "<<ih<<") "<<ph->GetName()<<" --> "<<grx[ih]<<"    (nTooSoft: "<<nTooSoft<<")"<<endl; 

    Double_t ineff, dineff;
    OK = passingFraction( *ph, cut_value, ineff, dineff, rejections != pass_above );
    if( ! OK ) {cerr<<"Bug! Can't calculate (in)eff for "<<ph->GetName()<<", cut_value: "<<cut_value<<endl; return 0;}

    gry [ih] = ineff;
    makeAsymmEffErrors( ineff, dineff, eyl [ih], eyh [ih] );
    if( iDebugMe > 8 ) cout<<"DBG ineff: "<<formValErr( ineff, dineff )
			   <<" --> "<<gry[ih]<<" +"<<eyh[ih]<<" -i"<<eyl[ih]<<endl; 
  }

  if( nHist - nTooSoft < 1) {cerr<<"Can't make (in)eff. graph: "
				 <<"all histograms are too soft for remap to exist"<<endl; return 0;}

  // figure out graph's title
  TString sCuts( extract_cut( title ) );
  TString sAfter( after_cut( title ) );
  bool setIneff = hists[0]->GetXaxis()->GetNbins() == 2 && pass_above && cut_value == 0.5;
  if( iDebugMe ) cout<<"setIneff? "<<setIneff
		     <<"; title from get_hists: "<<title<<" -> cuts: "<<sCuts<<" & also: "<<sAfter<<endl;
  TString grTitle;
  if( setIneff ) {
    if( sCuts.Length() > 0 && sCuts != ' ' ) {sCuts.Insert( 0, " {" ); sCuts+="}";}
    grTitle = Form( "%s%s%s", hists[0]->GetXaxis()->GetTitle(), sCuts.Data(), sAfter.Data() );
  } else {
    if( sCuts.Length() > 0 && sCuts(0) != ' ' ) sCuts.Insert( 0, " " );
    TString sValueCut( Form( "%s%s", (rejections == pass_above) ? "<" : ">", formatInNaturalPrecision( cut_value, 3 ).Data() ) );
    grTitle = Form( "P(%s%s)%s%s", 
		    hists[0]->GetXaxis()->GetTitle(), 
		    sValueCut.Data(),
		    sCuts.Data(), sAfter.Data() );
  }

  TGraphAsymmErrors *gae = new TGraphAsymmErrors( nHist-nTooSoft, grx+nTooSoft, gry+nTooSoft, 
						  exl+nTooSoft, exh+nTooSoft, eyl+nTooSoft, eyh+nTooSoft );
  delete[] grx;
  delete[] gry;
  delete[] exl;
  delete[] exh;
  delete[] eyl;
  delete[] eyh;
  gae->SetName( grName );
  gae->SetTitle( grTitle );
  gae->GetXaxis()->SetTitle( xLabel );
  double low_x =  gae->GetXaxis()->GetXmin();
  double high_x = gae->GetXaxis()->GetXmax();
  if( low_x <= 0 || high_x / low_x < 100 ) gae->GetXaxis()->SetMoreLogLabels( true );
  gae->GetYaxis()->SetTitle( rejections ? "Rejection" : "Efficiency" );
  gae->SetLineColor  ( _default_hist_color );
  gae->SetMarkerColor( _default_hist_color );
  if( iDebugMe > 1 ) cout<<"Made "<<grName<<" with "<<nHist-nTooSoft<<" (out of "<<nHist<<") data points. Titles: "
			 <<gae->GetTitle()<<"/"<<gae->GetXaxis()->GetTitle()<<"/"<<gae->GetYaxis()->GetTitle()<<endl;
  gDirectory->Add( gae ); // so it won't disappear when the canvas is cleared / deleted!
  return gae;
}


// ---------------------------------------------------------------------------------------------
bool jet_studies::plot_ineff( const on& args )
{
  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas

  TGraphAsymmErrors *gae = get_ineffs( args );
  if( gae == 0 ) return false;
  TString xname( gae->GetXaxis()->GetTitle() );
  TString yname( gae->GetYaxis()->GetTitle() );
  if( args._debug > 12 ) {
    cout<<"Found "<<gae->GetName()<<" at "<<(int)gae<<", dir: "<<gDirectory->GetPath()<<" with titles: "
	<<gae->GetTitle()<<"/"<<gae->GetXaxis()->GetTitle()<<"/"<<gae->GetYaxis()->GetTitle()<<endl;
  }

  float max = getGraphsGraphicalMax( gae );
  Float_t oldOff  = gStyle->GetTitleYOffset();
  Float_t oldMarg = gStyle->GetPadLeftMargin();
  gPad->Clear();
  if( args._debug > 5 ) cout<<"max: "<<max<<endl;
  /*if (max < 0.003 && max > 0.001 && fin != 2) { // old hack
    gStyle->SetTitleYOffset(1.9);
    gStyle->SetPadLeftMargin(0.22);
    gPad->UseCurrentStyle();
    } */
  gStyle->SetTitleYOffset(1.0); // new hack
  gStyle->SetOptFit();
  gPad->SetLogy(0);
  gPad->SetLogx(1);  // since ROOT 5.24 or so, this line triggers some memory management bug that deletes the axis titles...
                     // hence "xname" is extracted earlier. Failed to reproduce bug in simplified code :-(
  gPad->SetLogz(0);
  gPad->SetGridx();
  gPad->SetGridy();
  gae->SetMinimum(0);
  gae->GetXaxis()->SetTitle( xname ); // workaround for the ROOT wierdness
  gae->GetYaxis()->SetTitle( yname ); //   "              "         "
  //double min = getGraphsGraphicalMin (gae);
  gae->Draw("AP");

  if( xname.Contains( TRegexp( "^<.*p.*>$") ) || xname.Contains( "p_{T}" ) ) {
    gPad->SetLogx();

    TString funcName( Form( "%sf", gae->GetName() ) );
    TF1* pFunc0 = new TF1( funcName+"0", "[0]", args._min_pT, 4000 );
    pFunc0->SetParameter( 0, 0.0001 );
    gae->Fit( pFunc0, "R" );
    gae->Fit( pFunc0, "R" );
    cout<<"Fit results for function: "<<pFunc0->GetName()<<" are chi^2/NDF: "
      <<pFunc0->GetChisquare()<<" / "<<pFunc0->GetNDF()<<endl;
    float prob0 = TMath::Prob( pFunc0->GetChisquare(), pFunc0->GetNDF() );
    
    TF1* pFunc = new TF1( funcName, "[0]+[1]*exp(x*[2])", args._min_pT, 4000 );
    pFunc->SetParameter( 0, 0.0001 );
    pFunc->SetParameter( 1, 0.1 );
    pFunc->SetParameter( 2, -0.1 );
    gae->Fit( pFunc, "R" );
    gae->Fit( pFunc, "R" );
    cout<<"Fit results for function: "<<pFunc->GetName()<<" are chi^2/NDF: "
      <<pFunc->GetChisquare()<<" / "<<pFunc->GetNDF()<<endl;
    float prob = TMath::Prob( pFunc->GetChisquare(), pFunc->GetNDF() );

    TF1* pFuncL = new TF1( funcName+"L", "[0]+[1]*log(x)", args._min_pT, 4000 );
    pFuncL->SetParameter( 0,  0.0001 );
    pFuncL->SetParameter( 1, -0.0001 );
    gae->Fit( pFuncL, "R" );
    gae->Fit( pFuncL, "R" );
    cout<<"Fit results for function: "<<pFuncL->GetName()<<" are chi^2/NDF: "
      <<pFuncL->GetChisquare()<<" / "<<pFuncL->GetNDF()<<endl;
    float probL = TMath::Prob( pFuncL->GetChisquare(), pFuncL->GetNDF() );

    int iBest = 0;
    if( prob0 > prob && prob0 > probL ) {
      gae->Fit( pFunc0, "R" );
    } else {
      iBest = 1;
      gae->SetMaximum( max*1.3 ); // some extra space on top, for larger stat box
      if( prob > probL ) {
	iBest = 2;
	gae->Fit( pFunc, "R" );
      } 	
    }
   // doesn't work    relocateStatBox (.55, (prob0 > prob) ? .85 : 0.75, .95, 1);
    resizeStatBox( c1, gae, .55, iBest ? .75 : .85, .95, 1 ); 

  } // that's the only fitting option so far.
  
  // after all the options
  gPad->GetCanvas()->cd();
  gPad->Update();
  if( args._write_level >= 1 ) {
    gPad->SaveAs( Form( "figs/%s.eps",  gae->GetName() ) );
    gPad->SaveAs( Form( "iwin/%s.png",  gae->GetName() ) );
    gPad->SaveAs( Form( "pads/%s.root", gae->GetName() ) );
  }
  gStyle->SetTitleYOffset( oldOff );
  gStyle->SetPadLeftMargin( oldMarg );
  return true;
}

// ---------------------------------------------------------------------------------------------

bool jet_studies::xsecs( const on& args )
{
  float fin = args._fin;
  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas
  TString sin;
  int what = 1;
  if( fin > 0 && fin < 10 ) {what = 11; sin += "soft";}
  if( fin == 3 ) what = 59;
  if( fin == 4 ) what = 107;
  bool doLoose = fin < 2 || fin > 5;
  TH1* got1a = plot( on( args ).abs_what( what ).cut( 24 ).how_much( 0 ) );
  if( got1a == 0 ) return false;
  TH1* got1b = 0;
  if( doLoose ) {
    got1b = plot( on( args ).abs_what( what ).cut( 42 ).how_much( 0 ) );
    if( got1b == 0 ) return false;
  }
  TH1* got1c = plot( on( args ).abs_what( what ).cut( 35 ).how_much( 0 ) );
  if( got1c == 0 ) return false;
  
  TH1 *h1a = got1a, *h1b = got1b, *h1c = got1c; // redirection to allow adding rebinning if needed

  // get them all to use the same units & scale (mb)
  adjustUnits( *h1a, -3 );
  int scale = adjustUnits( *h1a );
  cout<<"DBG scale: "<<scale<<endl;
  if( scale == -666 ) {
    cerr<<"BUG failed to adjustUnits for "<<h1a->GetName()<<", can't continue."<<endl; 
    return false;
  }
  if( h1b ) adjustUnits( *h1b, -3 + scale);
  adjustUnits( *h1c, -3 + scale);

  double sigMax = TMath::Max( h1a->GetMaximum(), h1c->GetMaximum() );
  if( h1b ) sigMax = 1.2 * TMath::Max( sigMax, h1b->GetMaximum() );
  if( sigMax > 0 ) {
    h1a->SetMaximum( sigMax ); h1c->SetMaximum( sigMax ); if( h1b ) h1b->SetMaximum( sigMax );
  }

  double sigMin = TMath::Min( minPositiveEntry( *h1a ), minPositiveEntry( *h1c ) );
  if( h1b ) sigMin = TMath::Min( sigMin, minPositiveEntry( *h1b ) );
  sigMin *= 0.8; 
  if( sigMin > 0 ) {
    h1a->SetMinimum( sigMin ); h1c->SetMinimum( sigMin ); if( h1b )h1b->SetMinimum( sigMin );
  }
  
  gPad->Clear();
  gPad->SetLogy(1);
  gPad->SetLogx(0);
  gPad->SetLogz(0);
  gPad->SetGridy();
  gPad->SetGridx();
  gStyle->SetOptStat(0);

  h1a->SetLineColor(kGreen+2);
  h1a->SetLineWidth(2);
  h1c->SetLineColor(kRed+1);
  h1c->SetLineWidth(2);
  if( h1b ) {
    h1b->Draw("hist"); // only one without offsets
    h1a->Draw("samehist");
  } else {
    h1a->Draw("hist");
  }
  h1c->Draw("samehist");
  gStyle->SetOptStat( 0 );
  // force redraw without stat box
  h1c->Draw("samehist");
  gStyle->SetOptStat( 0 );

  // after all the options
  gPad->GetCanvas()->cd();
  gPad->Update();
  if( args._write_level >= 1 ) {
    TString name( h1a->GetName() );
    name.ReplaceAll ("C24", "");
    if( sin.Length() > 0 && ! sin.BeginsWith( "_" ) ) sin.Prepend( '_' );
    gPad->SaveAs( Form( "figs/xsecs_%s%s.eps", name.Data(), sin.Data() ) );
    gPad->SaveAs( Form( "iwin/xsecs_%s%s.png", name.Data(), sin.Data() ) );
    gPad->SaveAs( Form( "pads/xsecs_%s%s.root", name.Data(), sin.Data() ) );
  }
  return true;
}

bool jet_studies::plotXsecs( const on& args )
{
  bool ok;
  for( int i=1; i<= 5; ++i ) {
    ok = xsecs( on( args ).eta( i ).pT( 0 ).fin( 0 ) );
    if( ! ok ) return false;
    ok = xsecs( on( args ).eta( i ).pT( 0 ).fin( 1 ) );
    if( ! ok ) return false;
  }
  return true;
}

// ---------------------------------------------------------------------------------------------
// make plots like Ken's - see cut_list_at for default lists of cuts
bool jet_studies::xsec_by_cuts( const on& args, const vector<int> user_cuts, int units )
{
  vector<int> cuts ( user_cuts.begin(), user_cuts.end() );
  if( user_cuts.size() <= 0 ) {
    if( args._iList == 0 ) {
      cerr<<"Neither cuts, nor an iList were give to xsec_by_cuts"<<endl; 
      return false;
    }
    cuts = cut_list_at( args._iList );
  }

  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas

  on local_args( args );
  int what = args._what;
  if( what == 0 ) local_args._what = 314;
  // add graphical costumization as needed
  // ...

  vector<TH1*> hists;
  vector<TString> captions;
  for( unsigned int i=0; i<cuts.size(); ++i ) {
    TH1* hist = plot( local_args.cut( cuts[i] ).how_much( (i==0) ? 
							  2 : // so graphical information (e.g. logy) is available
							  0 ) );
    if( hist == 0 ) {
      cerr<<"Aborting! Failed to get histogram #"<<i<<endl;
      return false;
    }
    if( args._debug > 12 ) {cerr<<"DBG cut ["<<i<<"]="<<cuts[i]<<" -> hist name: "<<hist->GetName()
				<<" & max: "<<hist->GetMaximum()<<endl;}
    if( i > 0 ) hist->SetMaximum(); // only first one gets its maximum respected
    hists.push_back( hist );
    TString title( extract_cut( hist->GetTitle() ) );
    if( args._debug > 13 ) cout<<"DBG extracted title["<<i<<"]: "<<title<<endl;
    if( title.IsWhitespace() ) title = "All";
    captions.push_back( title );
  }

  // get them all to use the same units & scale (mb)
  bool units_given = TMath::Abs( units ) < 100;
  if( units_given ) adjustUnits( *hists[0], units );
  int units0 = currentUnits( *hists[0] );
  if( args._debug ) cout<<"DBG units of #0: "<<units0<<endl;
  if( units0 == -666 ) {
    if( units_given ) {
      cerr<<"BUG failed to adjustUnits for "<<hists[0]->GetName()<<", can't continue."<<endl; 
      return false;
    }
    // else move on - no units exist, and none asked for
  } else {
    for( unsigned int i=1; i<cuts.size(); ++i ) {
      adjustUnits( *hists[i], units0 );
    }
  }

  // set matching and appropriate min & max
  double y_max = hists[0]->GetMaximum(), y_min = minPositiveEntry( *hists[0] );
  for( unsigned int i=1; i<cuts.size(); ++i ) {
    y_max = TMath::Max( y_max, hists[i]->GetMaximum() );
    y_min = TMath::Max( y_min, minPositiveEntry( *hists[i] ) );
  }
  y_max *= 1.1; 
  y_min *= 0.9;
  for( unsigned int i=0; i<cuts.size(); ++i ) {
    hists[i]->SetMaximum( y_max );
    hists[i]->SetMinimum( y_min );
  }

  TString file_prefix( "xbc" );
  if( args._iList > 0 ) file_prefix = Form( "%s%d_", file_prefix.Data(), args._iList );
  return overlay_hists( hists, captions, "", args, file_prefix, true, 0.55, 0.65, 0.4 );
}

bool jet_studies::xsec_by_cuts_all_regions( const on& args, const vector<int> cuts, int units )
{
  bool ok;
  for( int i=0; i<= 5; ++i ) {
    ok = xsec_by_cuts( on( args ).eta( i ), cuts, units);
    if( ! ok ) return false;
  }
  return true;
}

vector<int> jet_studies::cut_list_at( int index )
{
  vector<int> cuts;
  if( index == 1 ) {
    cuts.push_back( 0 ); cuts.push_back( 56 ); cuts.push_back( 55 );
  }
  if( index == 2 ) {
    cuts.push_back( 0 ); cuts.push_back( 60 ); cuts.push_back( 62 );
  }
  if( index == 3 ) {
    cuts.push_back( 56 ); cuts.push_back( 58 ); cuts.push_back( 65 ); cuts.push_back( 59 );
  }
  if( index == 4 ) { // Ken's example
    cuts.push_back( 70 ); cuts.push_back( 80 ); cuts.push_back( 81 );
    cuts.push_back( 71 ); cuts.push_back( 73 ); cuts.push_back( 76 );
  }
  if( index == 5 ) {
    cuts.push_back( 0 ); cuts.push_back( 70 );
  }
  if( index == 6 ) {
    cuts.push_back( 56 ); cuts.push_back( 80 );
  }
  if( index == 7 ) {
    cuts.push_back( 200 ); cuts.push_back( 203 );
  }
  if( index == 8 ) {
    cuts.push_back( 210 ); cuts.push_back( 213 );
  }
  if( index == 9 ) {
    cuts.push_back( 220 ); cuts.push_back( 223 );
  }
  if( index == 10 ) {
    cuts.push_back( 200 ); cuts.push_back( 210 ); cuts.push_back( 220 ); cuts.push_back( 230 );
  }
  if( index == 11 ) {
    cuts.push_back( 225 ); cuts.push_back( 235 );
  }
  if( index == 12 ) {
    cuts.push_back( 220 ); cuts.push_back( 226 );
  }
  if( index == 13 ) {
    cuts.push_back( 240 ); cuts.push_back( 220 );
  }
  if( index == 14 ) {
    cuts.push_back( 241 ); cuts.push_back( 261 );
  }
  if( index == 15 ) { // little improvement - these aren't leading jets
    cuts.push_back( 70 ); cuts.push_back( 81 );cuts.push_back( 79 );
    cuts.push_back( 82 );
  }
  if( index == 16 ) {
    cuts.push_back( 5 ); cuts.push_back( 55 );cuts.push_back( 64 );
    cuts.push_back( 67 );
  }
  if( index == 17 ) { // like 16, but for GB
    cuts.push_back( 220 ); cuts.push_back( 227 );cuts.push_back( 228 );
    cuts.push_back( 260 );
  }
  if( index == 18 ) { // GB + jet IDs
    cuts.push_back( 220 ); cuts.push_back( 222 );cuts.push_back( 223 );
    cuts.push_back( 266 ); cuts.push_back( 267 );
  }
  if( index == 19 ) { // QP + jet IDs
    cuts.push_back( 240 ); cuts.push_back( 253 );cuts.push_back( 254 );
    cuts.push_back( 255 ); cuts.push_back( 256 );
  }
  if( index == 20 ) { // f_{g}>0.6 + jet IDs
    cuts.push_back( 100 ); cuts.push_back( 134 );cuts.push_back( 135 );
    cuts.push_back( 136 ); cuts.push_back( 137 );
  }

  if( cuts.size() == 0 ) {cerr<<"ERROR. Don't know cut_list #"<<index<<endl;}
  return cuts;
}

bool jet_studies::xsec_by_cuts( const on& args, int units )
{
  vector<int> cuts;
  return xsec_by_cuts( args, cuts, units );
}

bool jet_studies::xsec_by_cuts_all_regions( const on& args, int units )
{
  vector<int> cuts;
  return xsec_by_cuts_all_regions( args, cuts, units );
}

// compare deltaPhi dist. (but not normalization) with various vetoes
bool jet_studies::dphis( const on& args, int rebin )
{
  int iDebug = args._debug;
  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas

  int what = 21;
  if( args._fin == 1 ) what = 24;
  TH1* got1a = plot( on( args ).abs_what( what ).cut( 24 ).how_much( 0 ) );
  if( got1a == 0 ) return false;
  TH1* got1b = plot( on( args ).abs_what( what ).cut( 42 ).how_much( 0 ) );
  if( got1b == 0 ) return false;
  TH1* got1c = plot( on( args ).abs_what( what ).cut( 35 ).how_much( 0 ) );
  if( got1c == 0 ) return false;
  if( iDebug ) cerr<<"got them"<<endl;

  TH1 *h1a = 0, *h1b = 0, *h1c = 0, *h1atmp = 0, *h1ctmp = 0;
  if( rebin <= 1 ) {
    h1atmp = (TH1*) got1a->Clone( Form( "%sN", got1a->GetName() ) );
    h1b    = (TH1*) got1b->Clone( Form( "%sN", got1b->GetName() ) );
    h1ctmp = (TH1*) got1c->Clone( Form( "%sN", got1c->GetName() ) );
  } else {
    h1atmp = got1a->Rebin( rebin, Form( "%sR%d", got1a->GetName(), rebin ) );
    h1b    = got1b->Rebin( rebin, Form( "%sR%d", got1b->GetName(), rebin ) );
    h1ctmp = got1c->Rebin( rebin, Form( "%sR%d", got1c->GetName(), rebin ) );
  }
  if( iDebug ) cerr<<"cloned/rebinned at: "<<int(h1atmp)<<", ct: "<<int(h1ctmp)<<endl;
  h1a = cloneWithOffset( *h1atmp,  0.1, Form( "%sO", h1atmp->GetName() ) );
  h1c = cloneWithOffset( *h1ctmp, -0.1, Form( "%sO", h1ctmp->GetName() ) );
  if( h1a == 0 || h1c == 0 ) {cerr<<"Clone with offset failed!"<<endl; return false;}
  if( iDebug ) cerr<<"offset clones ready"<<endl;
  normalize( *h1a );
  normalize( *h1b );
  normalize( *h1c );
  h1a->GetYaxis()->SetTitle( "Fraction" );
  h1b->GetYaxis()->SetTitle( "Fraction" );
  h1c->GetYaxis()->SetTitle( "Fraction" );
  h1a->SetMaximum(); // unset it
  h1b->SetMaximum(); // unset it
  h1c->SetMaximum(); // unset it
  h1a->GetListOfFunctions()->RemoveAll();
  h1b->GetListOfFunctions()->RemoveAll();
  h1c->GetListOfFunctions()->RemoveAll();

  double sigMax = 1.5 * TMath::Max( TMath::Max( maxValMinusNSigma( *h1a, 1 ),
						maxValMinusNSigma( *h1b, 1 ) ),
				    maxValMinusNSigma( *h1c, 1 ) );
  for( int ib = 1; ib <= h1a->GetNbinsX()/5; ++ib ) {
    double maxb = 1.2 * TMath::Max( TMath::Max( h1a->GetBinContent( ib ), 
						h1b->GetBinContent( ib ) ),
				    h1c->GetBinContent( ib ) );
    if( maxb > sigMax ) sigMax = maxb;
    if( iDebug > 10 ) cerr<<"ib: "<<ib<<", maxb: "<<maxb<<" -> sigMax: "<<sigMax<<endl;
  }
  if( iDebug > 1 ) cerr<<"sigMax: "<<sigMax<<", h1a's max: "<<h1a->GetMaximum()
		       <<", h1b's max: "<<h1b->GetMaximum()
		       <<", h1c's max: "<<h1c->GetMaximum()<<endl;
  h1a->SetMaximum( sigMax ); h1b->SetMaximum( sigMax ); h1c->SetMaximum( sigMax );
  h1a->SetMinimum( 0 ); h1b->SetMinimum( 0 ); h1c->SetMinimum( 0 );

  gPad->Clear();
  gPad->SetLogy(0);
  gPad->SetLogx(0);
  gPad->SetLogz(0);
  gPad->SetGridy();
  gPad->SetGridx();
  gStyle->SetOptStat(0);

  h1a->SetLineColor( kGreen+2 );
  h1a->SetMarkerColor( kGreen+2 );
  h1a->SetMarkerStyle( 22 );
  h1a->SetLineWidth( 2 );
  h1b->SetMarkerStyle( 21 );
  h1b->SetMarkerSize( 0.7 );
  h1c->SetLineColor( kRed+1 );
  h1c->SetMarkerColor( kRed+1 );
  h1c->SetLineWidth( 2 );
  gStyle->SetOptStat( 0 );
  h1a->Draw("E");
  h1b->Draw("sameE");
  h1c->Draw("sameE");

  // after all the options
  gPad->GetCanvas()->cd();
  gPad->Update();
  if( args._write_level >= 1 ) {
    TString name (h1a->GetName());
    name.ReplaceAll ("C24", "j");
    gPad->SaveAs( Form( "figs/dphis_%s.eps", name.Data() ) );
    gPad->SaveAs( Form( "iwin/dphis_%s.png", name.Data() ) );
    gPad->SaveAs( Form( "pads/dphis_%s.root", name.Data() ) );
  }
  return true;
}

bool jet_studies::plot_dphis( const on& args )
{
  bool ok;
  for( int iEta=1; iEta<= 5; ++iEta ) {
    for( int iPt=31; iPt<=40; ++iPt ) {
      int rebin = 1;
      if( iPt <= 37 ) rebin = 2;
      if( iPt <= 35 ) rebin = 4;
      if( iPt <= 33 ) rebin = 6;
      if( iPt <= 32 ) rebin = 10;
      ok = dphis( on( args ).eta( iEta ).pt( iPt ), rebin );
      if( ! ok ) return false;
    }
  }
  return true;
}

// compare deltaPhi dist. & normalization with and without the default veto
bool jet_studies::dvj( const on& args, int rebin )
//bool jet_studies::dvj( int iEta, int iPt, TTree& tree, const TString suffix, 
//		       int write_level, int rebin, int iDebug, float fin )
{
  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas

  int what = 21;
  if( args._fin == 1 ) what = 24;
  TH1* got1a = plot( on( args ).abs_what( what ).oneD().cut( 27 ).how_much( 0 ) );
  if( got1a == 0 ) return false;
  TH1* got1b = plot( on( args ).abs_what( what ).oneD().cut( 24 ).how_much( 0 ) );
  if( got1b == 0 ) return false;

  TH1 *h1a = 0, *h1b = 0;
  if( rebin <= 1 ) {
    h1a = got1a;
    h1b = got1b;
  } else {
    h1a = got1a->Rebin( rebin, Form( "%sR%d", got1a->GetName(), rebin) );
    h1b = got1b->Rebin( rebin, Form( "%sR%d", got1b->GetName(), rebin) );
    h1a->SetMaximum(); // unset it
    h1b->SetMaximum(); // unset it
  }

  double sigMax = 2 * maxValMinusNSigma( *h1a, 2 );
  if( args._debug > 1 ) cerr<<"sigMax: "<<sigMax<<", h1b's max: "<<h1b->GetMaximum()<<endl;
  if( sigMax > 0 && sigMax < h1b->GetMaximum() ) {h1b->SetMaximum( sigMax ); h1a->SetMaximum( sigMax );}

  gPad->Clear();
  gPad->SetLogy(0);
  gPad->SetLogx(0);
  gPad->SetLogz(0);
  gPad->SetGridy();
  gPad->SetGridx();
  gStyle->SetOptStat(0);

  h1b->SetLineColor( kGreen+2 );
  h1b->SetLineWidth( 2 );
  h1b->SetMinimum( 0 );
  h1b->Draw("hist");
  h1a->Draw("same"); // how do I prevent this from plotting the stats box?
  //  relocateStatBox(2,2,2,2); // atleast move it out of the frame - this only works from the CINT prompt
  resizeStatBox( c1, h1a, 2, 2, 2, 2 );
  h1a->Draw("same"); // force repainting (no, updated doesn't cut it, and no, stat box stays to the side - voodoo)

  // after all the options
  gPad->GetCanvas()->cd();
  gPad->Update();
  if( args._write_level >= 1 ) {
    TString name (h1a->GetName());
    name.ReplaceAll ("C27", "j");
    gPad->SaveAs( Form( "figs/dv%s.eps",  name.Data() ) );
    gPad->SaveAs( Form( "iwin/dv%s.png",  name.Data() ) );
    gPad->SaveAs( Form( "pads/dv%s.root", name.Data() ) );
  }
  return true;
}

bool jet_studies::bin_bias( const on& args )
{
  int pt = iPt( args );

  if (pt < 10 || pt > 50) {cerr<<"Must give a jtpt / tagpt pT range, i.e., 10-50"<<endl; return false;}

  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas

  int basePt = pt;
  if (pt >= 30) basePt -= 20;

  TH1* h1a = plot( on( args ).oneD().pt( basePt ).how_much( 0 ) );
  if( h1a == 0 ) return false;
  TH1* h1b = plot( on( args ).oneD().pt( basePt+20 ).how_much( 0 ) );
  if( h1b == 0 ) return false;

  gPad->Clear();
  gPad->SetLogy(0);
  gPad->SetLogx(0);
  gPad->SetLogz(0);
  gStyle->SetOptStat(0); // sometimes needed to clear stat objects
  h1a->Draw();
  h1b->Draw();
  gStyle->SetOptStat("nemMeR");

  h1a->SetMaximum(); // undo any previous setting
  Stat_t max = 1.3*h1a->GetMaximum();
  h1a->SetMaximum(max);
  h1a->SetMinimum(0);
  h1b->SetLineColor (kGreen+2);
  h1b->SetLineWidth (2);
  h1b->SetFillStyle( 0 );
  h1a->Draw("");
  resizeStatBox( c1, h1a, .6, .7, .99, .9, "statsa", kMagenta+4 );
  h1b->Draw("hist");
  resizeStatBox (c1, h1b, .2,.7,.59, .9, "statsb", kGreen+2);
  h1a->Draw("Esame");

  // after all the options
  gPad->GetCanvas()->cd();
  gPad->Update();
  if( args._write_level >= 1 ) {
    TString hname( h1b->GetName() );
    if( hname.BeginsWith("j") ) hname.Remove( 0, 1 );
    gPad->SaveAs( Form( "figs/bb%s.eps", hname.Data() ) );
    gPad->SaveAs( Form( "iwin/bb%s.png", hname.Data() ) );
  }
  return true;
}
   
bool jet_studies::trigger_bias( const on& args )
{
  int pt = iPt( args );
  if( pt < 10 || pt > 50 ) {cerr<<"Must give a jtpt / tagpt pT range, i.e., 10-50"<<endl; return false;}
  if( args._what > 100 ) {cerr<<"Parsing of plot #s >= 100 NYI"<<endl; return false;}

  int basePt = pt;
  if( pt >= 30 ) basePt -= 20;

  TH1* h1a = plot( on( args ).oneD().cut( 6 ).pT( basePt ) );
  if (h1a == 0) return false;
  TH1* h1b = plot( on( args ).oneD().cut( 0 ).pT( basePt+20 ) );
  if (h1b == 0) return false;

  gPad->Clear();
  gPad->SetLogy(0);
  gPad->SetLogx(0);
  gPad->SetLogz(0);
  clear_stat_box( *h1a );
  clear_stat_box( *h1b );

  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas

  gStyle->SetOptStat("nemMeR");

  h1a->SetMaximum(); // undo any previous setting
  Stat_t max = 1.3*h1a->GetMaximum();
  h1a->SetMaximum(max);
  h1a->SetMinimum(0);
  h1b->SetLineColor( kGreen+2 );
  h1b->SetLineWidth( 2 );
  h1b->SetFillStyle( 0 );
  if( args._debug ) cout<<"h1a lists: "
			<<(h1a->GetListOfFunctions() ? h1a->GetListOfFunctions()->GetEntries() : -1 )<<endl;
  //  h1a->Draw("");
  // if( args._debug ) cout<<"h1a lists: "
  //			<<(h1a->GetListOfFunctions() ? h1a->GetListOfFunctions()->GetEntries() : -1 )<<endl;
  h1b->Draw("hist");
  resizeStatBox( c1, h1b, .2,.7,.59, .9, "statsb", kGreen+2 );
  h1a->Draw("Esame");
  resizeStatBox( c1, h1a, .6, .7, .99, .9, "statsa", _default_hist_color );
  if( args._debug ) cout<<"h1a lists: "
			<<(h1a->GetListOfFunctions() ? h1a->GetListOfFunctions()->GetEntries() : -1 )
			<<" name: "<<( (h1a->GetListOfFunctions() && h1a->GetListOfFunctions()->GetEntries() ) ? 
				       h1a->GetListOfFunctions()->At(0)->GetName() : "NONE" )<<endl;
  if( args._debug ) cout<<"h1b lists: "
			<<(h1b->GetListOfFunctions() ? h1b->GetListOfFunctions()->GetEntries() : -1 )
			<<" name: "<<( (h1b->GetListOfFunctions() && h1b->GetListOfFunctions()->GetEntries() ) ? 
				       h1b->GetListOfFunctions()->At(0)->GetName() : "NONE" )<<endl;

  // after all the options
  gPad->GetCanvas()->cd();
  gPad->Update();
  if( args._write_level >= 3 ) {
    TString hname (h1b->GetName());
    if( hname.BeginsWith("j") ) hname.Remove( 0, 1 );
    gPad->SaveAs( Form( "figs/ntb%s.eps", hname.Data() ) );
    gPad->SaveAs( Form( "iwin/ntb%s.png", hname.Data() ) );
  }
  return true;
}
   
bool jet_studies::trigger_pT_bias( const on& args )
{
  return plot( on( args ).abs_what( 24 ).oneD().cut( 6 ) );
}


bool jet_studies::old_trigger_bias( const on& args )
{
  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas

  if( iEta( args ) != 0 ) {cerr<<"Warning! iEta != 0 introduces an asymmetry between tag and probe..."<<endl;}
  if( iPt( args ) < 50 || iPt( args ) >= 70 ) {
    cerr<<"Abort! Need 50 <= iPt < 70 (symmetry in jtpt & tagpt)"<<endl; 
    return false;
  }

  TH1* h1b = plot( on( args ).abs_what( 24 ).oneD().cut( 0 ).how_much( 0 ).less_debug() );
  if( h1b == 0 ) return false;
  double Nall, dNall;
  countEntries( h1b, Nall, dNall );

  TH1* h1a = plot( on( args ).abs_what( 24 ).oneD().cut( 4 ).how_much( 2 ).less_debug() ) ;
  if( h1a == 0 ) return false;
  double N1, dN1;
  countEntries( h1a, N1, dN1 );

  int nBins = h1a->GetNbinsX();
  int cutBin = h1a->FindBin(0);
  if( cutBin < 1 || cutBin > nBins ) {cerr<<"Bug! cut_value=0 is outside x-axis of "<<h1a->GetName()<<endl; return 0;}

  double frac, dfrac;
  bool OK = calcEffTot( N1, Nall, dN1, dNall, frac, dfrac );
  if( OK ) {
    TText *ptxt = new TText();
    ptxt->SetTextSize(0.06);   ptxt->SetTextFont(62);
    ptxt->SetTextColor(kBlue); ptxt->SetTextAlign(12);
    ptxt->DrawTextNDC (.2, .8, Form("P(Ntag<2)=%s", formValErr( frac, dfrac) ) );
  }

  double p0, dp0;
  OK = passingFraction( *h1a, 0, p0, dp0, true, args._debug );
  if( OK ) {
    TText *ptxt = new TText();
    ptxt->SetTextSize(0.06);        ptxt->SetTextFont(62);
    ptxt->SetTextColor(kMagenta+3); ptxt->SetTextAlign(12);
    ptxt->DrawTextNDC (.2, .7, Form("P(tag leads)=%s", formValErr( p0, dp0 ) ) );
  }

  gPad->GetCanvas()->cd();
  gPad->Update();
  if( args._write_level ) {
    TString hname( h1a->GetName() );
    if( hname.BeginsWith("j") ) hname.Remove( 0, 1 );
    gPad->SaveAs( Form( "figs/tb%s.eps", hname.Data() ) );
    gPad->SaveAs( Form( "iwin/tb%s.png", hname.Data() ) );
  }
  return true;
}
   
   
bool jet_studies::scan_and_overlay( const on& args , int rebin )
{
  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas
  c1->SetWindowSize( 900, 700 );

  int what = args._what;

  vector<TH1*> hists, xHists, orgHists;
  vector<TString> captions;
  TString title;
  float top_fraction = 0.42;
  if( what == 99 ) top_fraction = 0.1;
  on local_args( args );
  if( iEta( args ) >= 0 && iPt( args ) == 0 ) local_args.pT( -30 );
  get_hists( orgHists, xHists, title, captions, local_args.less_debug().aux_probe( false ).no_aux( false ) );
  bool logy = gPad->GetLogy();
  unsigned int nHist = orgHists.size();
  cout<<"Found "<<nHist<<" histograms, title: "<<title<<endl;
  if( nHist < 1 ) return false;
  const int myDebug = args._debug % 10;

  // prepare hists, rebinning when needed
  for( unsigned int ih=0; ih<nHist; ++ih ) {
    TH1* curOrg = orgHists[ih];
    if( rebin <= 1 ) {
      hists.push_back( curOrg );
    } else {
      char* newName = Form( "%sR%d", curOrg->GetName(), rebin );
      hists.push_back( curOrg->Rebin( rebin, newName ) );
      if( args._debug > 2 ) cerr<<"ih: "<<ih<<" newName: "<<newName<<" |hists|: "<<hists.size()<<std::flush
				<<" max: "<<(*hists.rbegin())->GetMaximum()<<endl;
      (*hists.rbegin())->SetMaximum(); // unset
    }
  }
  // get each one a sensible graphical maximum
  for( unsigned int ih=0; ih<nHist; ++ih ) {
    set_top_fraction( *hists[ih], top_fraction, logy, args._debug/10 );
  }
  // what's the maximum?
  Stat_t maxY = -1;
  Stat_t minY = 0.6;
  for( unsigned int ih=0; ih<nHist; ++ih ) {
    Stat_t curMaxY = hists[ih]->GetMaximum();
    if( curMaxY > maxY ) maxY = curMaxY;
    // allow logy to work with 0s
    if( logy && hists[ih]->GetMaximum() <= 0) {
      Stat_t curMinY = 1E-3 * hists[ih]->GetMaximum();
      if( curMinY < minY ) minY = curMinY;
    }
  }
  if( myDebug > 0 ) cout<<"maxY: "<<maxY<<", minY: "<<minY<<endl;

  gStyle->SetOptStat(0);

  int nLeg = TMath::Min( 3, 1 + int( nHist ) / 7);
  int nPerLeg = 1 + (nHist-1) / nLeg;
  float legmaxy = 0.9, legminx=0.15, leg_line_height = 0.1, leg_min_y = 0.6;
  float legwidth = 0.85 / nLeg;
  if( what == 77 ) {
    legminx = 0.5;
    legwidth = 0.45 / nLeg;
  }
  if( what == 99 ) {
    if( legwidth > 0.45 ) legwidth = 0.45;
    leg_line_height = 0.15;
    leg_min_y = 0.35;
  }
  float leglowy = legmaxy - leg_line_height*nPerLeg;
  if( leglowy < leg_min_y ) leglowy = leg_min_y;

  vector<TLegend*> legends;
  for( int il=0; il<nLeg; ++il ) {
    TLegend* pleg = new TLegend( legminx + il*legwidth, leglowy, legminx+(0.8+il)*legwidth, legmaxy );
    if( pleg == 0 ) {cerr<<"Can't allocate legend #"<<il<<endl; return false;}
    legends.push_back( pleg );
  }
  if( myDebug > 0 ) cout<<"prepared "<<nLeg<<" =? "<<legends.size()<<" legends"<<endl;
  
  // yet another memory leak. This one to control the title
  TH1* clone = (TH1*) hists[0]->Clone();
  clone->SetTitle( title );
  hists[0]=clone;

  for( unsigned int ih=0; ih<nHist; ++ih ) {
    TString drawOpt = (ih > 0) ? "samehist" : "hist";
    hists[ih]->SetMinimum( logy ? minY : 0 );
    hists[ih]->SetMaximum( maxY );
    if( TString( hists[ih]->GetOption() ) == "E1" ) drawOpt += "E1";
    hists[ih]->Draw( drawOpt );
    hists[ih]->SetLineColor ( (ih < n_colors() ) ? _gradual_colors[ih] : 200 + ih - n_colors() );
    hists[ih]->SetLineWidth ( (ih < n_colors() ) ? _widths[ih] : 1 );
    hists[ih]->SetFillStyle( 0 );
    if( ih % 2 ) hists[ih]->SetLineStyle( 2 );
 
    if( myDebug > 5 ) cout<<ih<<"Chose color: "<<hists[ih]->GetLineColor()<<std::flush
			  <<", il: "<<(int(ih/nPerLeg))<<std::flush
			  <<", cap: "<<captions[ih]<<endl;
    legends.at( int( ih/nPerLeg ) )->AddEntry( hists[ih], captions[ih], "l" );
  }
  for( int il=0; il<nLeg; ++il ) legends.at( il )->Draw();

  if( args._write_level && nHist > 0 ) {
    TString hname( hists[0]->GetName() );
    if( hname.BeginsWith("j") ) hname.Remove( 0, 1 );
    // keep index being run over, as it shows the starting point
    gPad->SaveAs( Form( "figs/so%s.eps", hname.Data() ) );
    gPad->SaveAs( Form( "iwin/so%s.png", hname.Data() ) );
  }
  return true;
}

// --------------------------------------------------------------------------------------------------------
bool jet_studies::overlay_hists( vector<TH1*> org_hists, vector<TString> captions, const TString& title,
				 const on& args, const TString& file_prefix, bool contrast,
				 float leg_min_x, float leg_min_y, float total_legs_width,
				 float leg_line_height, float max_leg_width, 
				 int rebin, int user_logy )
{
  unsigned int nHist = org_hists.size();
  if( nHist == 0 ) {
    cerr<<"WARNING! no histograms were given to overlay_hists"<<endl;
    return false;
  }
  if( args._debug ) cout<<"Found "<<nHist<<" histograms, title: "<<title<<endl;

  bool user_captions = captions.size() > 0;
  if( user_captions && captions.size() != nHist ) {
    cerr<<"BUG! # of captions ("<<captions.size()<<") mismatches # of hists ("<<nHist
	<<"). overlay_hists aborts."<<endl;
    return false;
  }

  unsigned int pallete_size = contrast ? n_contrasting_colors() : n_colors();

  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas
  c1->SetWindowSize( 900, 700 );

  //on local_args( args );
  //if( iEta( args ) >= 0 && iPt( args ) == 0 ) local_args.pT( -30 );
  bool logy = gPad->GetLogy();
  if( user_logy > 0 ) logy = 1;
  if( user_logy < 0 ) logy = 0;

  // prepare hists, rebinning when needed
  vector<TH1*> hists;
  for( unsigned int ih=0; ih<nHist; ++ih ) {
    TH1* cur_org = org_hists[ih];
    if( rebin <= 1 ) {
      hists.push_back( cur_org );
    } else {
      char* new_name = Form( "%sR%d", cur_org->GetName(), rebin );
      hists.push_back( cur_org->Rebin( rebin, new_name ) );
      if( args._debug > 2 ) cerr<<"ih: "<<ih<<" new_name: "<<new_name<<" |hists|: "<<hists.size()<<std::flush
				<<" max: "<<(*hists.rbegin())->GetMaximum()<<endl;
      (*hists.rbegin())->SetMaximum(); // unset
    }
  }

  // what's the maximum?
  Stat_t maxY = -1;
  Stat_t minY = 0.6;
  for( unsigned int ih=0; ih<nHist; ++ih ) {
    Stat_t curMaxY = hists[ih]->GetMaximum();
    if( curMaxY > maxY ) maxY = curMaxY;
    // allow logy to work with 0s
    if( logy && hists[ih]->GetMaximum() <= 0) { // was this supposed to be GetMinimum?
      Stat_t curMinY = 1E-3 * hists[ih]->GetMaximum();
      if( curMinY < minY ) minY = curMinY;
    }
    if( args._debug > 9 ) cout<<"ih: "<<ih<<" curMaxY: "<<curMaxY<<", logy: "<<logy<<", minY: "<<minY<<endl;
  }
  if( args._debug > 0 ) cout<<"maxY: "<<maxY<<", minY: "<<minY<<endl;

  gStyle->SetOptStat(0);

  int nLeg = TMath::Min( 3, 1 + int( nHist ) / 7);
  int nPerLeg = 1 + (nHist-1) / nLeg;
  float legmaxy = 0.9;

  if( total_legs_width < 0 ) {cerr<<"BUG! total_legs_width is negative"<<endl; return false;}
  float legwidth = total_legs_width / nLeg;
  if( max_leg_width > 0 && legwidth > max_leg_width ) legwidth = max_leg_width;

  float leglowy = legmaxy - leg_line_height*nPerLeg;
  if( leglowy < leg_min_y ) leglowy = leg_min_y;

  vector<TLegend*> legends;
  for( int il=0; il<nLeg; ++il ) {
    TLegend* pleg = new TLegend( leg_min_x + il*legwidth, leglowy, leg_min_x+(0.8+il)*legwidth, legmaxy );
    if( pleg == 0 ) {cerr<<"Can't allocate legend #"<<il<<endl; return false;}
    legends.push_back( pleg );
  }
  if( args._debug > 0 ) cout<<"prepared "<<nLeg<<" =? "<<legends.size()<<" legends"<<endl;
  
  // yet another memory leak. This one to control the title
  TH1* clone = (TH1*) hists[0]->Clone( Form( "%s_oh", hists[0]->GetName() ) );
  clone->SetTitle( title );
  hists[0]=clone;

  for( unsigned int ih=0; ih<nHist; ++ih ) {
    TString drawOpt = (ih > 0) ? "samehist" : "hist";
    hists[ih]->SetMinimum( logy ? minY : 0 );
    hists[ih]->SetMaximum( maxY );
    hists[ih]->Draw( drawOpt );
    int color;
    if( ih < pallete_size ) {
      if( contrast ) {
	color = _contrasting_colors[ ih ];
      }	else {
	color = _gradual_colors[ ih ];
      }
    } else {
      color = 200 + ih - pallete_size;
    }
    hists[ih]->SetLineColor( color );
    hists[ih]->SetLineWidth( (ih < pallete_size ) ? _widths[ih] : 1 );
    hists[ih]->SetFillStyle( 0 );
    if( ih % 2 ) {
      hists[ih]->SetLineStyle( 2 );
      if( contrast ) hists[ih]->SetLineWidth( 1 + hists[ih]->GetLineWidth() );
    } 
 
    if( args._debug > 5 ) cout<<ih<<"Chose color: "<<hists[ih]->GetLineColor()<<std::flush
			      <<", il: "<<(int(ih/nPerLeg))<<std::flush
			      <<", cap: "<<captions[ih]<<endl;
    legends.at( int( ih/nPerLeg ) )->AddEntry( hists[ih], captions[ih], "l" );
  }
  for( int il=0; il<nLeg; ++il ) legends.at( il )->Draw();

  if( args._write_level && nHist > 0 ) {
    TString hname( hists[0]->GetName() );
    if( hname.BeginsWith("j") ) hname.Remove( 0, 1 );
    if( hname.EndsWith("_oh") ) hname.Remove( hname.Length()-3, 3 );
    // keep index being run over, as it shows the starting point
    gPad->SaveAs( Form( "figs/%s%s.eps", file_prefix.Data(), hname.Data() ) );
    gPad->SaveAs( Form( "iwin/%s%s.png", file_prefix.Data(), hname.Data() ) );
  }
  return true;
}

float entriesToMarkerSize( const Double_t entries )
{
  if( entries < 20 ) return 1;
  if( entries < 50 ) return 0.7;
  if( entries < 150 ) return 0.6;
  if( entries < 500 ) return 0.5;
  return 0.4; // smaller than this won't plot
}

void chooseMarkerSize( TH1& hist ) // meant for TH2s, but to avoid unnecesary casts...
{
  Double_t N = hist.GetEntries();
  if( N < 2000 ) {
    hist.SetMarkerStyle( 20 );
    hist.SetMarkerSize( entriesToMarkerSize( N ) );
  } else { // these smaller marker types don't respect the size parameter
    if( N < 5000  ) hist.SetMarkerStyle( 7 );
    if( N < 15000 ) hist.SetMarkerStyle( 6 );
    hist.SetMarkerStyle( 1 );
  }
  return;
}

bool plotTwo( TH1& ha, TH1& hb, bool subtract = false,
	      bool logVal = false, bool write = true, int colorScheme = 0, int iDebug = 0,
	      float fin = -666., const TString statOpt = "nemr")
{
  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", 550, 400 );
  gPad->GetCanvas()->cd(); // get us up to the canvas

  gPad->Clear();
  gPad->SetLogy(0);
  gPad->SetLogx(0);
  gPad->SetLogz(0);
  gStyle->SetOptStat(1);

  int nDim = ha.GetDimension();
  if( nDim != hb.GetDimension() ) {cerr<<"plotTwo failed! input histograms have different dimensions"<<endl; return false;}

  TH1* htmp = &ha;
  if( subtract ) {
    
    cout<<"Cloning..."<<endl;
    htmp = (TH1*) ha.Clone( Form("%s_cl", ha.GetName() ));
    cout<<"subtracting..."<<endl;
    htmp->Add( &hb, -1 );

  }

  if( nDim == 1 ) {

    gPad->SetLogy( logVal );
    htmp->SetMaximum(); // undo any previous setting
    hb.SetMaximum(); // undo any previous setting
    Stat_t max = 1.3*TMath::Max( htmp->GetMaximum(), hb.GetMaximum());
    htmp->SetMaximum(max);
    htmp->SetMinimum(0);
    hb.SetMaximum(max);
    hb.SetMinimum(0);
    if( logVal ) {
      htmp->SetMinimum();
      hb.SetMinimum();
      Double_t min = TMath::Min( htmp->GetMinimum(), hb.GetMinimum() );
      if( min <= 0 ) {
	Double_t Neff1 = 10 * TMath::Max( 10., htmp->GetEffectiveEntries() );
	Double_t Neff2 = 10 * TMath::Max( 10., hb.GetEffectiveEntries() );
	Double_t minmax = TMath::Min( htmp->GetMaximum() / Neff1, hb.GetMaximum() / Neff2);
	Double_t maxmax = TMath::Max( htmp->GetMaximum(), hb.GetMaximum() );
	min = TMath::Min( 0.6, TMath::Min( minmax, 1E-3 * maxmax ) );
	if( iDebug > 1 ) cerr<<"min: "<<min<<" Neffs: "<<Neff1<<", "<<Neff2
			     <<"; Maxs: "<<htmp->GetMaximum()<<", "<<hb.GetMaximum()
			     <<" -> mimax: "<<minmax<<"; maxmax: "<<maxmax<<" -> min: "<<min<<endl;
	htmp->SetMinimum( min );
	hb.SetMinimum( min );
      }
    }
    if( fin != -666. ) {
	htmp->SetMinimum( fin );
	hb.SetMinimum( fin );
    }
      
    htmp->SetLineColor (kMagenta+4);
    htmp->SetFillColor (kYellow);
    hb.SetLineColor (kGreen+3);
    hb.SetFillColor (kGreen+2);
    hb.SetFillStyle (1001);
    hb.SetLineWidth (2);
    // dispose of old stat boxes
    gStyle->SetOptStat( 0 );
    // can't get the stat boxes to work anymore :-(
    TH1* clone = (TH1*) hb.DrawClone("hist");
    htmp->Draw("");
    // so we can set new options
    gStyle->SetOptStat( statOpt );
    // create the stat box, so it can be moved around
    clone->Draw("hist");
    if( iDebug ) { 
      cout<<"1 clone: "; print_list_of_functions( *clone );
      cout<<"1 htmp: ";print_list_of_functions( *htmp );
    }
    resizeStatBox (c1, clone, .48,.7,.73, .9, "statsb", kGreen+3);
    if( iDebug ) { 
      cout<<"2 clone: "; print_list_of_functions( *clone );
      cout<<"2 htmp: ";print_list_of_functions( *htmp );
    }
    htmp->Draw("histsame");
    if( iDebug ) { 
      cout<<"3 clone: "; print_list_of_functions( *clone );
      cout<<"3 htmp: ";print_list_of_functions( *htmp );
    }
    resizeStatBox (c1, htmp, .74, .7, .99, .9, "statsa", kMagenta+4);
    if( iDebug ) { 
      cout<<"4 clone: "; print_list_of_functions( *clone );
      cout<<"4 htmp: ";print_list_of_functions( *htmp );
    }
    hb.SetFillStyle(0);
    hb.Draw("samehist");
    if( iDebug ) { 
      cout<<"5 clone: "; print_list_of_functions( *clone );
      cout<<"5 htmp: ";print_list_of_functions( *htmp );
    }

  } else if( nDim == 2 ) {
    
    gStyle->SetOptStat( 0 );
    hb.SetMarkerColor (colorScheme ? kOrange : kGreen+2);
    chooseMarkerSize( hb );
    chooseMarkerSize( *htmp );
    htmp->SetMarkerColor( colorScheme ? kGreen+2 : kRed+1 );
    cout<<"Drawing..."<<endl;
    TString drawOpt;
    if( (colorScheme % 5) >= 3 ) drawOpt += "hcolz"; 
    if( (colorScheme % 5) == 3 ) htmp->SetMarkerColor( kMagenta+4 );
    if( iDebug ) cerr<<"DBG colorScheme: "<<colorScheme
		     <<" -> drawOpt: "<<drawOpt<<", color: "<<htmp->GetMarkerColor()<<endl;
    htmp->Draw( drawOpt );
    cout<<"htmp drawn"<<endl;
    //    cout<<"Cloned"<<endl;
    //resizeStatBox (c1, htmp, .74, .75, .99, 1., "statsa", kRed+2);
    hb.Draw( "same" );
    cout<<"h2b drawn"<<endl;
    //resizeStatBox (c1, h2b, .48,.75,.73, 1., "statsb", kGreen+3);

  } else {
    cerr<<"Bug - unexpected nDim"<<endl;
    return false;
  }


  // after all the options
  gPad->GetCanvas()->cd();
  cout<<"Updating..."<<endl;
  gPad->Update();
  if( write ) {
    cout<<"Parsing..."<<endl;
    TString hnameA (ha.GetName());
    if (hnameA.BeginsWith("j")) hnameA.Remove(0,1);
    TString hnameB (hb.GetName());
    if (hnameB.BeginsWith("j")) hnameB.Remove(0,1);
    cout<<"Saving..."<<endl;
    gPad->SaveAs(Form ("figs/c%s_%s.eps", hnameA.Data(), hnameB.Data()));
    gPad->SaveAs(Form ("iwin/c%s_%s.png", hnameA.Data(), hnameB.Data()));
    if( subtract ) gPad->SaveAs( Form( "pads/c%s_%s.root", hnameA.Data(), hnameB.Data() ) );
  }
  return true;
}
   
bool plotTwo( const TString& name1, const TString& name2, bool subtract = false,
	      bool logVal = false, bool write = true, int colorScheme = 0, 
	      int iDebug = 0, float fin = -666., const TString statOpt = "nemr")
{
  TH1* h1b = (TH1*) gDirectory->Get( name2 );
  if (h1b == 0) {cerr<<"Can't find histogram "<<name1<<endl; return false;}

  TH1* h1a = (TH1*) gDirectory->Get( name1 );
  if (h1a == 0) {cerr<<"Can't find histogram "<<name2<<endl; return false;}

  return plotTwo( *h1a, *h1b, subtract, logVal, write, colorScheme, iDebug, fin, statOpt );
}

bool jet_studies::plot_two( const on& args, int what2, bool subtract, bool logVal,
			    int colorScheme, const TString statOpt  )
//bool jet_studies::plot_two( int what1, int what2, int iEta, int iPt, TTree& tree, 
//			    const TString suffix, bool subtract, bool logVal, bool write, 
//			    int colorScheme, int iDebug, float fin, const TString statOpt )
{
  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas

  TH1* h1b = plot( on( args ).how_much( 0 ).less_debug() );
  if( h1b == 0 ) return false;

  TH1* h1a = plot( on( args ).abs_what( what2 ).how_much( 2 ).less_debug() );
  if( h1a == 0 ) return false;

  return plotTwo( *h1a, *h1b, subtract, logVal, args._write_level, colorScheme, 
		  args._debug % 10, args._fin, statOpt );
}

bool jet_studies::old_plot_two( const on& args, int what2, bool logVal, int colorScheme )
{
  TCanvas *c1 = (TCanvas *) gROOT->GetListOfCanvases()->First();
  if( ! c1 ) c1 = new TCanvas( "c1", "c1", _default_canvas_width, _default_canvas_height );
  gPad->GetCanvas()->cd(); // get us up to the canvas

  TH1* h1b = plot( on( args ).how_much( 0 ) );
  if( h1b == 0 ) return false;

  TH1* h1a = plot( on( args ).abs_what( what2 ).how_much( 2 ) );
  if( h1a == 0 ) return false;

  gPad->Clear();
  gPad->SetLogy(0);
  gPad->SetLogx(0);
  gPad->SetLogz(0);
  gStyle->SetOptStat(1);

  int nDim = h1a->GetDimension();
  if( nDim == 1 ) {

    gPad->SetLogy( logVal );
    h1a->SetMaximum(); // undo any previous setting
    Stat_t max = 1.3*h1a->GetMaximum();
    h1a->SetMaximum(max);
    h1a->SetMinimum(0);
    if( logVal ) {
      h1a->SetMinimum();
      h1b->SetMinimum();
      Double_t min = TMath::Min( h1a->GetMinimum(), h1b->GetMinimum() );
      if( min <= 0 ) {
	Double_t minmax = TMath::Min( h1a->GetMaximum(), h1b->GetMaximum() );
	Double_t maxmax = TMath::Max( h1a->GetMaximum(), h1b->GetMaximum() );
	min = TMath::Min( 0.6, TMath::Min( 0.1 * minmax, 1E-3 * maxmax ) );
	h1a->SetMinimum( min );
	h1b->SetMinimum( min );
      }
    }
    h1a->SetLineColor (kMagenta+4);
    h1a->SetFillColor (kYellow);
    h1b->SetLineColor (kGreen+3);
    h1b->SetFillColor (kGreen+2);
    h1b->SetLineWidth (2);
    h1a->Draw("hist");
    resizeStatBox (c1, h1a, .74, .75, .99, 1., "statsa", kMagenta+4);
    h1b->Draw("samehist");
    resizeStatBox (c1, h1b, .48,.75,.73, 1., "statsb", kGreen+3);

  } else if( nDim == 2 ) {
    
    TH2F* h2a = getTH2F( args );
    TH2F* h2b = getTH2F( on( args ).abs_what( what2 ) );
    if( h2a == 0  ||  h2b == 0 ) {cerr<<"Bug! can't get TH2Fs"<<endl; return false;}

    // meant for scatter plots, where h1a includes h1b
    h2b->SetMarkerColor (colorScheme ? kOrange : kGreen+2);
    cout<<"Cloning..."<<endl;
    TH2F* htmp = (TH2F*) h2a->Clone("htmp_plotTwo");
    cout<<"Cloned"<<endl;
    htmp->SetMarkerColor( colorScheme ? kGreen+2 : kRed+1 );
    cout<<"subtracting..."<<endl;
    htmp->Add( h2b, -1 );
    cout<<"subtracted"<<endl;
    htmp->Draw("");
    cout<<"htpm drawn"<<endl;
    //    cout<<"Cloned"<<endl;
    //resizeStatBox (c1, htmp, .74, .75, .99, 1., "statsa", kRed+2);
    h2b->Draw("same");
    cout<<"h2b drawn"<<endl;
   //resizeStatBox (c1, h2b, .48,.75,.73, 1., "statsb", kGreen+3);

  } else {
    cerr<<"Bug - unexpected nDim"<<endl;
    return false;
  }


  // after all the options
  gPad->GetCanvas()->cd();
  cout<<"Updating..."<<endl;
  gPad->Update();
  if( args._write_level ) {
    cout<<"Parsing..."<<endl;
    TString hnameA( h1a->GetName() );
    if( hnameA.BeginsWith("j") ) hnameA.Remove( 0, 1 );
    TString hnameB( h1b->GetName() );
    if( hnameB.BeginsWith("j") ) hnameB.Remove( 0, 1 );
    cout<<"Saving..."<<endl;
    gPad->SaveAs( Form( "figs/p%s_%s.eps", hnameA.Data(), hnameB.Data() ) );
    gPad->SaveAs( Form( "iwin/p%s_%s.png", hnameA.Data(), hnameB.Data() ) );
  }
  return true;
}
   
// -------------------------------------------------------------------------------------------
// 
void oj ()
{
  cout<<"Loading the range utils:"<<std::flush;
  gROOT->LoadMacro("rangeUtils.c+");
  cout<<" OK.  "<<endl;
}

