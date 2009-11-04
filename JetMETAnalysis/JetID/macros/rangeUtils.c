#ifndef AH_RANGEUTILS
#define AH_RANGEUTILS
/*
   Contain 

   1) a class called range, which wraps a TCut that's supposed to select a simple
   range in one variable or its absolute value with information on how to display 
   that cut in filenames and figure captions, and on what the edges are 
   (for further automated processing)

     - cutting on abs(var) is supported by ::isAbs and by findRanges 
     
     - "char *" accesors available where useful (for TString::Form()).
     
     - most of the work is in initializing these variables. See next class.

   2) a class called rangeMap which is a map of int --> ranges with additional functionality:

     - a few utilities to use this from the CINT/ROOT interpreter

     - a method to initialize ranges corresponding to bins, which takes care
       (with quite a few option) to make concise captions and cuts.

   Possible improvements:

   - add bookkeeping for initWithBins (first bin, nBins, var).
     This will force this to become an independent class, preferably with a member map.
     See comments below on why. This requires a bit of work, choosing which std::map like
     interface to keep.

     - sure looks like var would've been useful

     - also keep track of available sequencs (e.g. define "2nd" sequence)

*/

//For alcic:
#include <iostream>
#include "TMath.h"
#include "TROOT.h"
#include "TCut.h"
//#include <climits>
#include <limits>
#include <string>
//#include <sstream>
#include <map>
#include <vector>
#include <iterator>

#include "myMathUtils.c"
//#include "histUtils.c"

using std::cout;
using std::endl;
using std::cerr;
using std::vector;

// possibility: convert to a class, as fields should not change after construction
struct range {
  TCut cut;
  float from; // somewhat redundant, but allows later parsing, e.g. for aggregating histograms
  float to;   //   ditto
  TString name;
  TString caption;

  range () : cut(""), name (""), caption ("") {from = to = 0;}

  range (const TCut& xcut, const float xfrom, const float xto, const TString& xname, const TString& xcaption) 
    : cut (xcut), name (xname), caption (xcaption)
  {
    from = xfrom;
    to = xto;
  }

  range (const char* xcut, const float xfrom, const float xto, const char* xname, const char* xcaption) 
    : cut (xcut), name (xname), caption (xcaption)
  {
    from = xfrom;
    to = xto;
  }

  range (TString& xcut, const float xfrom, const float xto, const char* xname, TString& xcaption) 
    : cut (xcut), name (xname), caption (xcaption)
  {
    from = xfrom;
    to = xto;
  }

  bool isAbs () const {return TString(cut.GetTitle()).Contains("abs(");}
  bool isEmpty () const {return from == 0 && to == 0 && strlen(cut.GetTitle()) == 0
			   && name.Length() == 0 && caption.Length() == 0;}
  
  const char* cname () {return name.Data();}
  const char* ccap () {return caption.Data();}

  friend std::ostream& operator<< (std::ostream& outStream, const range& range);
};
  
std::ostream& operator<< (std::ostream& outStream, const range& r)
{
  return outStream<<"name: "<<r.name<<", cap: "<<r.caption
		  <<", ("<<r.from<<" - "<<r.to<<") = "<<r.cut.GetTitle();
}

// Note: the "inheriting from std::map is evil" argument is about ~std::map being non-virtual
//       but this is fine since the derived class has NO data members. It's just added functionality.
class rangeMap : public std::map<int, range> 
{
 private:
  typedef std::map<int, range> map_t; // probably not needed, but I use it for now

 public:
  
  void dump ();

  // CINT has problems using the map, hence this helper method, which might as well avoid exceptions painful in CINT
  range rangeAt (const int index);

  // negative return = failure
  // minX - maxX = the range (on the variable) to find
  // if is (not) abs will only examine bins that are (not) abs (see range::isAbs above). 
  // minI - maxI = can restrict the range to indices in this range (e.g. the region where the right var is used).
  int findRanges (const float minX, const float maxX, 
		  const bool abs = true, const int minI = INT_MIN, const int maxI = INT_MAX,
		  const bool debug = false);

  // - bins will be stored as ranges starting from firstIndex (and ending in firstIndex+nBins-1)
  // - var is what will be used in the TCut
  // - varInCaptions is how the var will be presented in captions (i.e. TLatex)
  // - the must be 1+nBins binEdges in the float[], which must be ascending.
  //   - std::numeric_limits<float>::max() is treated as +infinity!
  // - negateEdges is to cut on a negative range given positive bin edges
  // - nDigits is for myMathUtils' formatInNaturalPrecision function
  // - if captions are given (non-NULL) it must be TString[] with nBins entries.
  bool initWithBins (const int firstIndex, const TString& var, const TString& varInCaptions, 
		     const TString& namePrefix, const int nBins, const float* const binEdges, 
		     const bool cutOnZero = false, const bool negateEdges = false, const int nDigits = 2, 
		     const TString* const captions = 0, const TString& captionSuffix = "");


  // inefficienct access. Should be used only to parse user's interactive commands.
  // returns INT_MIN when it fails
  int name2index( const TString& name ) const;
  int caption2index( const TString& name ) const;
};

void rangeMap::dump () 
{
  for (map_t::const_iterator it=begin(); it != end(); ++it) {
    const range& r = it->second;
    cout<<it->first<<") "<<r<<endl;
  }
}

int rangeMap::name2index( const TString& name ) const
{
  int out = INT_MIN;
  for (map_t::const_iterator it=begin(); it != end(); ++it) {
    const range& r = it->second;
    if( r.name == name ) {
      if( out != INT_MIN ) {
	cerr<<"rangeMap::name2index failed since name ("<<name<<") appears more than once"<<endl;
	return INT_MIN;
      }
      out = it->first;
    }
  }
  return out;
}

int rangeMap::caption2index( const TString& caption ) const
{
  int out = INT_MIN;
  for (map_t::const_iterator it=begin(); it != end(); ++it) {
    const range& r = it->second;
    if( r.caption == caption ) {
      if( out != INT_MIN ) {
	cerr<<"rangeMap::caption2index failed since caption ("<<caption<<") appears more than once"<<endl;
	return INT_MIN;
      }
      out = it->first;
    }
  }
  return out;
}

range rangeMap::rangeAt (const int index) 
{
  if (count(index) > 0)
    return operator[](index);
  else
    return range();
}

// negative return = failure
int rangeMap::findRanges (const float minX, const float maxX, const bool abs, const int minI, const int maxI, 
			  const bool debug)
{
  static const float bigFloat = std::numeric_limits<float>::max();
  if (debug) cout<<"DBG - minI: "<<minI<<", maxI: "<<maxI<<endl;
  float fMinX = minX, fMaxX = maxX;
  if (fMinX > fMaxX) {fMaxX = fMaxX + fMinX; fMinX = fMaxX - fMinX; fMaxX -= fMinX;} // swap them

  for (map_t::const_iterator it=begin(); it != end(); ++it) {
    int index = it->first;
    if (debug) cout<<"\nDBG - index: "<<index<<std::flush;
    if (index < minI || index > maxI) continue;

    const range& cur = it->second;
    if (debug) cout<<", cur: "<<cur<<std::flush;
    if (abs != cur.isAbs()) continue;

    float width = cur.to - cur.from;
    if (width <= 0) {cerr<<"Bug! illegal range stored at index "<<index<<endl; return -2;}

    float overlap = TMath::Min (fMaxX, cur.to) - TMath::Max (fMinX, cur.from);
    bool edge = cur.to == bigFloat || cur.from == -bigFloat;
    if (debug) cout<<", overlap: "<<overlap<<", edge? "<<edge<<std::flush;

    if (edge) {

      if (overlap <= 0) continue;
      cout<<" some of "<<index<<",";

    } else {

      if (overlap < 0.01*width) continue;

      if (overlap > 0.99*width) 
	cout<<" "<<index<<",";
      else
	cout<<" "<<formatInNaturalPrecision(100*overlap/width)<<"% of "<<index<<",";
    }
  }
  cout<<endl;
  return 0;
}

bool rangeMap::initWithBins (const int firstIndex, const TString& var, const TString& varInCaptions, 
			     const TString& namePrefix, const int nBins, const float* const binEdges, 
			     const bool cutOnZero, const bool negateEdges, const int nDigits, 
			     const TString* const captions, const TString& captionSuffix)
{
  static const float bigFloat = std::numeric_limits<float>::max();
  // check inputs
  if (var.Length() <= 0) {cerr<<"Error in initWithBins - var is empty"<<endl; return false;}
  if (nBins < 2) {cerr<<"Error in initWithBins - 2 > nBins = "<<nBins<<endl; return false;}
  for (int i=1; i<nBins; ++i) {
    if (binEdges[i] <= binEdges[i-1]) {cerr<<"Error in initWithBins - eIllegal bin at indices "
					   <<i<<" - "<<i+1<<endl; return false;}
    if (binEdges[i] > bigFloat) {cerr<<"Error in initWithBins - edge is bigger than C++ "
				     <<"can handle (?!) at index "<<i<<endl; return false;}
  }

  // do it
  for (int i=0; i<nBins; ++i) {
    int outI = firstIndex + i;

    // figure out edges
    float from, to;
    if (negateEdges) {
      from = -binEdges[1+i];
      to   = -binEdges[ i ];
    } else {
      from = binEdges[ i ];
      to   = binEdges[1+i];
    }

    // figure out cuts
    TString s1,s2;
    if ( (from != 0 || cutOnZero) && TMath::Abs(from) != bigFloat)
      // "<=" instead of "=" only to prevent misnormalizations (events can't fall through the cracks)
      s1 = formatInNaturalPrecision (from, nDigits) + "<=" + var;
    if ( ( to  != 0 || cutOnZero) && TMath::Abs( to ) != bigFloat)
      s2 = var + "<" + formatInNaturalPrecision (to, nDigits);    

    // combine cuts
    if (s1.Length() > 0 && s2.Length() > 0) s1 += " && ";
    s1 += s2;

    // figure out name
    TString name (namePrefix);
    name += outI; // increment formats the int as a string & appends (unlike + which does some funky pointer addition)

    // figure out caption
    TString caption;
    if (captions) {
      caption = captions [i] + captionSuffix;
    } else {
      caption = s1;
      caption.ReplaceAll (var, varInCaptions);
      caption.ReplaceAll ("<=", "#leq");
      caption.ReplaceAll (varInCaptions + " && " + varInCaptions, varInCaptions); // shorten
    }

    // initialize the new range
    operator[](outI) = range (s1, from, to, name, caption);
  }
  return true;
}



#endif
