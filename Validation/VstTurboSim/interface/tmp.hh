void clusterParticles( TSObjectEvent &event ) {

   /// z - Recombination Matrix
   vector< vector< double > > z;

   /// onSimplexBoundry - used to indicate that the particle
   /// belongs to (true) or does not belong to (false) the 
   /// boundries of the simplex. (i.e. z[p][r] = 0)
   vector< vector< bool > > onSimplexBoundry;


   InitializeRecombinationMatrix( z, 
				  onSimplexBoundry,
				  event.partonItems.size(),
				  event.recoItems.size() );

   MinimizeOmega( z, onSimplexBoundry, event );

   // Match parton particles to reco particles
   // Check if reco particles should be paired

}

/** 
 * Initialize {z} and {onSimplexBoundry}
 * 
 * Initialize matrices to correct size, default
 * initial values.
 *
 * @param z Recombination Matrix
 * @param b Simplex Boundry Matrix 
 * @param numP Number of parton objects
 * @param numR Number of reconstructed objects
 */
void InitializeRecombinationMatrix( vector< vector< double > > &z,
				    vector< vector< bool > > &b,
				    int numP,
				    int numR ) {

   // first column of {z} will be for unassigned energy
   // hence, numR + 1
   vector< bool > tmpB( numR + 1, false );
   vector< double > tmpZ( numR + 1, 1.0 / double( numR + 1 ) );

   z.clear();
   b.clear();

   for ( int p = 0; p < numP; p++ ) {
      z.push_back( tmpZ );
      b.push_back( tmpB );
   }

}


/**
 * see OJF, Q_minimize
 * 
 * Minimizes Omega({z}) for a given set of reco particles
 * from the existing configuration of {z}.
 *
 * Iteratively preforms minimization algorithm. Loop is terminated when no
 * z[p][r] moves in an iteration or the maximal number of iterations is 
 * exceeded. Maximal number is, by default, 1000.
 *
 * @param z     Recombination Matrix, {z}
 * @param b     Simplex Boundry Matrix
 * @param event TSObjectEvent containing the parton objects
 *              and reco objects to match.
 *
 */
void MinimizeOmega( vector< vector< double > > &z,
		    vector< vector< bool > > &b,
		    TSObjectEvent &event ) {

   int moved;

   // must scale energy first

   for ( int i = 1, i < 1000; i++ ) {
      
      moved = 0;
      
      for ( int p = 0; p < z.size(); p++ ) {
	 if ( MinimizeOmega_wrt( z[p], b[p], event ) )
	    moved++;
      }
      
      if ( moved == 0 )
	 break;
   }
}
 
/**
 * Minimizes omega for a given row of the recombination matrix.
 * (i.e. minimizes omega for a given parton object)
 *
 * @param zRow  A row of the recombination matrix
 * @param bRow  Corresponding row of the simplex boundry matrix
 * @param event TSObjectEvent containing parton and recon objects to match.
 */
bool MinimizeOmega_wrt( vector< double > &zRow,
			vector< double > &bRow,
			TSObjectEvent &event ) {
   
   double 
      dist,  //< t, a scalar, distance to move  
      step;  //< 
   vector< double > dirVector( zRow.size() );

   /// set up direction vector for this row of {z}.
   GradOmega( dirVector, zRow );
   /// snap dirVector to simplex boundries
   DMinusSnap( dirVector, bRow );
   /// 
   //\todo step not initialized??
   DEvalStep( dirVector, zRow, bRow, step );

   if ( ( step > 0 ) && ( step < inf_step ) ) {
      dist = step * dNorm( d );
      while ( dist > 0.000001 ) {
	 oldOmega = Omega();
	 // save particle
	 // move_particle
	 if ( Omega() < oldOmega )
	    return true;
	 // undo move particle
	 dist *= 0.3666; //< between 1/3 and .4
	 step *= 0.3336; //< between 1/3 and .4
      }
   }
}

void MoveParticle( vector< double> &Z,
		   vector< double> &d,
		   double step ) {
   
   // z-move-by
   // update jets???

}

/**
 * Intializes the direction vector
 *
 * @param dirVector Direction vector
 * @param zRow A row of {z}
 */
GradOmega( vector< double > &dirVector, 
	   vector< double > &zRow,
	   TSObjectEvent &event,
	   int p ) {
   
   GradY( dirVector, zRow );
   
   // need energy of parton particle . . . .
   // need event, p
   double energy = event.partonItems()[p].lorentzVector().e();
   
   dirVector[0] = 0;
   for ( int r = 1; r < zRow.size(); r++ ) {
      dirVector[r] /= Radius2;
      dirVector[r] -= energy;
   }
   
}

GradY( vector< double > &dirVector,
       vector< double > &zRow,
       TSObjectEvent &event,
       int p ) {
   
   double res, et, r024;

   dirVector[0] = 0;
   for ( r = 1; r < zRow.size(); r++ ) {
      
      // positive scalar product (dot product)
      res = max( event.recoItems()[r].lorentzVector() *
		 event.partonItems()[p].lorentzVector() ),
		 0 );
      et = max( event.recoItems()[r].lorentzVector().et(),
		EPS_Et );
      r024 = 
	 event.partonItems()[p].lorentzVector().et() / 
	 et * ( event.partonItems()[p].eta() - 
		event.recoItems()[r].eta() );

      // what is p0shmpzch??
      // = () . (~)^3 - (~). ()^3
      res += r024 * event.recoItems()[r].p0shmpzch;

      dirVector[r] = res * 2;
   }
}

double DNorm( vector< double > &D ) { 
   double r = 0;
   for ( int i = 0; i < D.size(); i++ )
      r = max( r, fabs( d[ i ] ) );
   return r;
}



   // for each parton particle 
   for ( int p = 0; p < z.size(); p++ )
      /**
       * Look only at vector of reco objects, z[p]
       * Z[r] := z[p][r]
       *
       * Shift Z to Z + tD (where Z and D are vectors)
       * change in Omega is:
       * Omega(Z + tD) = Omega(Z) + tF.D + O(t^2)
       * where:
       * vector F is a vector of funtions, F[r]
       *  such that function F[r] = dOmega(Z)/dZ[r]
       * F.D is the dot product = 
       *  Sum(F[r]*D[r], r = 1 to numRecoObj )
       * vector D describes some direction
       * t is a scalar
       *
       * Must choose t and D such that z is
       * constrained to simplex.
       *
       * F.D = Sum(F[r]*D[r], for all r )
       *     = Sum((F[r]+Fo)D[r] - (Fo)(Do))
       * where Fo
       */
      ;
}


