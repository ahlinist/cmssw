#ifndef __TurboSimRule_ClusterParticles_h
#define __TurboSimRule_ClusterParticles_h

#include "Validation/VstTurboSim/interface/TurboSimRule.hh"

#define PAIRING_BINS 5 // # of bins in pairing matrix per pairing type
#define PAIRING_SEPARATION_RESOLUTION 0.15  // radians


class TurboSimRule_ClusterParticles : public TurboSimRule {

 public:

   TurboSimRule_ClusterParticles( double keepSinglets = 2.0 ) 
      :
      TurboSimRule( "Cluster and match parton particles to reco particles" ),
      _keepSinglets( keepSinglets )
   {
      InitializePairingStats();
   }

   bool train( TurboSimEvent &event );
   bool read( istream & );
   bool apply( TurboSimEvent &event ) const;
   std::string toString() const;

private:

   void InitializePairingStats();

   double _keepSinglets;

   typedef map< pair< TurboSimParticleType,
		      TurboSimParticleType >,
                vector< int > > pairingStatsCountType;
   typedef map< pair< TurboSimParticleType,
		      TurboSimParticleType >,
		vector< double > > pairingStatsFractionType;

   pairingStatsCountType clustersInRange_total,
                         clustersInRange_paired;
   pairingStatsFractionType clusterFractionToPair;

};

ostream & operator<<( ostream &, TurboSimRule_ClusterParticles const & );
istream & operator>>( istream &, TurboSimRule_ClusterParticles & );

#endif
