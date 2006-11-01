#ifndef JetFlavour_H
#define JetFlavour_H

// class to identify origin of a jet

//  Author:             Christian.Weiser@cern.ch
//  Ported to CMSSW by: Andrea.Bocci@cern.ch
//  Last Update:        12/07/2006

#include <vector>
#include <cmath>

#include "DataFormats/Math/interface/LorentzVector.h"

class JetFlavour {

public:
  typedef HepLorentzVector LorentzVector;

  // define association criterium via constructor

  //
  // access info
  //
  int  mainFlavour                  ( void ) const { return m_mainFlavour; }
  int  heaviestFlavour              ( void ) const { return m_heaviestFlavour; }
  int  minimumDeltaRFlavour         ( void ) const { return m_minimumDeltaRFlavour; }

  int  mainOrigFlavour              ( void ) const { return m_mainOrigFlavour; }
  int  originFlavour                ( void ) const { return m_originFlavour; }
  int  initialFlavour               ( void ) const { return m_initialFlavour; }

  bool initialPartonSplitsToC       ( void ) const { return m_initialPartonSplitsToC; }
  bool initialPartonSplitsToB       ( void ) const { return m_initialPartonSplitsToB; }

  double pMainParton                ( void ) const { return m_pMainParton; }
  double pClosestParton             ( void ) const { return m_pClosestParton; }
  double pHeaviestParton            ( void ) const { return m_pHeaviestParton; }
  double deltaRMainParton           ( void ) const { return m_deltaRMainParton; }
  double deltaRClosestParton        ( void ) const { return m_deltaRClosestParton; }
  LorentzVector vec4MainParton      ( void ) const { return m_Vec4MainParton; }
  LorentzVector vec4ClosestParton   ( void ) const { return m_Vec4ClosestParton; }
  LorentzVector vec4HeaviestParton  ( void ) const { return m_Vec4HeaviestParton; }

  LorentzVector vec4SummedPartons   ( void ) const { return m_Vec4SummedPartons; }
  LorentzVector vec4OriginParton    ( void ) const { return m_Vec4OriginParton; }

  // if based on heavy hadrons
  bool hasBottomHadronInCone        ( void ) const { return ( m_mainFlavour == 5 ); }
  bool hasCharmHadronInCone         ( void ) const { return ( m_mainFlavour == 4 ); }
  bool hasStrangeHadronInCone       ( void ) const { return ( m_mainFlavour == 3 ); }
  bool hasHeavyHadronInCone         ( void ) const { return ( m_mainFlavour == 5 || m_mainFlavour == 4 ); }

  bool hasDown                      ( void ) const { return m_hasDown; }
  bool hasUp                        ( void ) const { return m_hasUp; }
  bool hasStrange                   ( void ) const { return m_hasStrange; }
  bool hasCharm                     ( void ) const { return m_hasCharm; }
  bool hasBottom                    ( void ) const { return m_hasBottom; }
  bool hasGluon                     ( void ) const { return m_hasGluon; }

  int  nDown                        ( void ) const { return m_nDown; }
  int  nUp                          ( void ) const { return m_nUp; }
  int  nStrange                     ( void ) const { return m_nStrange; }
  int  nCharm                       ( void ) const { return m_nCharm; }
  int  nBottom                      ( void ) const { return m_nBottom; }
  int  nGluon                       ( void ) const { return m_nGluon; }

  int  numberOfSources              ( void ) const { return m_numberOfSources; }
  std::vector<int> flavourSources   ( void ) const { return m_flavourSources; }

//set methods:

  void mainFlavour 		 (const int a)  {m_mainFlavour = a; }
  void heaviestFlavour		 (const int a)  {m_heaviestFlavour = a; }
  void minimumDeltaRFlavour	 (const int a)  {m_minimumDeltaRFlavour = a; }

  void mainOrigFlavour		 (const int a)  {m_mainOrigFlavour = a; }
  void originFlavour		 (const int a)  {m_originFlavour = a; }
  void initialFlavour		 (const int a)  {m_initialFlavour = a; }

  void initialPartonSplitsToC	(const bool a)  {m_initialPartonSplitsToC = a; }
  void initialPartonSplitsToB	(const bool a)  {m_initialPartonSplitsToB = a; }

  void pMainParton  	      (const double a)  {m_pMainParton = a; }
  void pClosestParton	      (const double a)  {m_pClosestParton = a; }
  void pHeaviestParton	      (const double a)  {m_pHeaviestParton = a; }
  void deltaRMainParton	      (const double a)  {m_deltaRMainParton = a; }
  void deltaRClosestParton        (const double a)  {m_deltaRClosestParton = a; }
  void vec4MainParton      (const LorentzVector a)  {m_Vec4MainParton = a; }
  void vec4ClosestParton   (const LorentzVector a)  {m_Vec4ClosestParton = a; }
  void vec4HeaviestParton  (const LorentzVector a)  {m_Vec4HeaviestParton = a; }

  void vec4SummedPartons   (const LorentzVector a)  {m_Vec4SummedPartons = a; }
  void vec4OriginParton    (const LorentzVector a)  {m_Vec4OriginParton = a; }

  // if based on heavy hadrons

  void hasDown		       (const bool a)  {m_hasDown = a; }
  void hasUp 		       (const bool a)  {m_hasUp = a; }
  void hasStrange		       (const bool a)  {m_hasStrange = a; }
  void hasCharm		       (const bool a)  {m_hasCharm = a; }
  void hasBottom		       (const bool a)  {m_hasBottom = a; }
  void hasGluon		       (const bool a)  {m_hasGluon = a; }

  void nDown			 (const int a)  {m_nDown = a; }
  void nUp 			 (const int a)  {m_nUp = a; }
  void nStrange			 (const int a)  {m_nStrange = a; }
  void nCharm			 (const int a)  {m_nCharm = a; }
  void nBottom			 (const int a)  {m_nBottom = a; }
  void nGluon			 (const int a)  {m_nGluon = a; }

  void numberOfSources		 (const int a)  {m_numberOfSources = a; }
  void flavourSources   (const std::vector<int> a)  {m_flavourSources = a; }

private:

  // info members
  // for both methods
  int    m_mainFlavour;         // main is the hardest
  int    m_heaviestFlavour;     // heaviest does not include gluons
  int    m_minimumDeltaRFlavour;

  int    m_originFlavour;       // the one of the status==3 mother
  int    m_mainOrigFlavour;     // mother flavour of main parton

  double m_pMainParton;         // mom. of main parton
  double m_pClosestParton;      // mom. of closest parton
  double m_pHeaviestParton;     // mom. of heaviest parton
  double m_deltaRMainParton;    // deltaR of main parton
  double m_deltaRClosestParton; // deltaR of closest parton

  LorentzVector m_Vec4MainParton;
  LorentzVector m_Vec4ClosestParton;
  LorentzVector m_Vec4HeaviestParton;

  LorentzVector m_Vec4SummedPartons;
  LorentzVector m_Vec4OriginParton;

  bool m_heavyHadronBased;
  bool m_partonBased;

  bool m_hasDown;
  bool m_hasUp;
  bool m_hasStrange;
  bool m_hasCharm;
  bool m_hasBottom;
  bool m_hasGluon;

  int m_nDown;
  int m_nUp;
  int m_nStrange;
  int m_nCharm;
  int m_nBottom;
  int m_nGluon;

  int m_numberOfSources;

  std::vector<int> m_flavourSources;

  int m_initialFlavour;        // when matching to initial partons only

  bool m_initialPartonSplitsToC;
  bool m_initialPartonSplitsToB;

};
#endif

