#ifndef SimDataFormats_BTauMC_MCParticleInfo_h
#define SimDataFormats_BTauMC_MCParticleInfo_h

  /**
   * class to retrieve particle info from Lund Code
   */


class MCParticleInfo {

public:
  MCParticleInfo(){}
  MCParticleInfo( int );

  ~MCParticleInfo();

  void reset () ;

  /**
   * fill Info
   */
  void setCode( int ) ; // this also decodes all particle related information and sets the
                        // corresponding variables

  /**
   * print info
   */
  void print() const ;

  int  lundCode                 () const { return lundCode_ ; }
  int  lundCodeOfHeaviestQuark  () const { return lundCodeOfHeaviestQuark_ ; }

  bool isHadron       () const { return isHadron_ ; }
  bool isBottomHadron () const { return isBottomHadron_ ; }
  bool isCharmHadron  () const { return isCharmHadron_ ; }
  bool isStrangeHadron() const { return isStrangeHadron_ ; }
  bool hasStrangeness () const { return hasStrangeness_ ; }
  bool isBaryon       () const { return isBaryon_ ; }

  bool isParton () const { return isParton_ ; }
  bool isD      () const { return isD_      ; }
  bool isU      () const { return isU_      ; }
  bool isS      () const { return isS_      ; }
  bool isC      () const { return isC_      ; }
  bool isB      () const { return isB_      ; }
  bool isGluon  () const { return isGluon_  ; }
  bool isQuark  () const { return isQuark_  ; }
  bool isLepton () const { return isLepton_ ; }

private:
  bool isInitialised;        // properly initialised?
  // particle information
  int  lundCode_;                 // Lund Particle Code of the particle
  int  lundCodeOfHeaviestQuark_ ; // heaviest quark in the hadron
  // for hadrons
  bool isHadron_;            // is a hadron
  bool isBottomHadron_;      // b-hadron?  --|
  bool isCharmHadron_;       // c-hadron?    |-- refers to heaviest quark only!!
  bool isStrangeHadron_;     // s-hadron?  --|
  bool hasStrangeness_;      // carries strangeness
  bool isBaryon_;            // is a baryon
  // for partons
  bool isParton_ ;
  bool isD_      ;
  bool isU_      ;
  bool isS_      ;
  bool isC_      ;
  bool isB_      ;
  bool isGluon_  ;
  bool isQuark_  ;
  // for light charged leptons
  bool isLepton_ ;

};

#endif // SimDataFormats_BTauMC_MCParticleInfo_h
