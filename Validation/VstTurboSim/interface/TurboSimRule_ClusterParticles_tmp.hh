
bool TurboSimRule_ClusterParticles::train( TSObjectEvent &event ) {

   /// p.o. - parton object     r.o. - reco object
   /// p.p. - parton particle   r.p. - reco particle

   /// foreach configuration of parton objects such that #p.o. <= #r.p.
   while ( 

   /**
    * Every reconstructed object is associated with the closest parton object.
    * (every cause has an effect)
    * 
    * Iterate through recoItems and compute the closest partonItem with 
    * nearest neighbor algorithm. Distance heuristic is solid angle. 
    */
   vector< vector < double > > 
      distances( event.partonItems().size(), 
		 vector< double >( event.recoItems().size(), 
				   0.0 ) );
   vector< int > P2R( event.partonItems().size(), 0 );
   vector< int > R2P( event.recoItems().size(), 0 );
   double min;

   for ( vector< TurboSimPartonObject >::const_iterator partonObj =
	    event.partonItems().begin(), int p = 0;
	 partonObj != event.partonItems().end();
	 partonObj++, p++ )
      for ( vector< TurboSimRecoObject >::const_iterator recoObj =
	       event.recoItems().begin(), int r = 0;
	    recoObj != event.recoItems().end();
	    recoObj++, r++ )
	 distances[p][r] = distance( partonObj->at( 0 ).lorentzVector(),
				     recoObj->at( 0 ).lorentzVector() );
   for ( int p = 0; p < distances.size(); p++ ) {
      min = distances[p][0];
      P2R[p] = 0;
      for ( int r = 1; r < distances[p].size(); r++ )
	 if ( distances[p][r] < min ) {
	    min = distances[p][r];
	    P2R[p] = r;
	 }
   }
   for ( int r = 0; r < distances[0].size(); r++ ) {
      min = distances[0][r];
      R2P[r] = 0;
      for ( int p = 1; p < distances.size(); p++)
	 if ( distances[p][r] < min ) {
	    min = distances[p][r];
	    R2P[r] = p;
	 }
   }
	    

   for ( vector< TurboSimRecoObject >::iterator recoObj = 
	    event.recoItems().begin();
	 recoObj != event.recoItems().end();
	 recoObj++ )
      for ( vector< TurboSimParticle >::iterator r = recoObj->begin();
	    r != recoObj->end();
	    r++ )
	 if ( not ( *r == DefinitionOf::UNCL ) ) {
	    
	    double minDistance = _maxMinDistance, 
	           distance;
	    vector< TurboSimPartonObject >::const_iterator closestPartonObj = 
	       event.partonItems().end();
	 
	    for ( vector< TurboSimPartonObject >::const_iterator partonObj = 
		     event.partonItems().begin();
		  partonObj != event.partonItems().end();
		  partonObj++ )
	       for ( vector< TurboSimParticle>::const_iterator p = partonObj->begin();
		     p != partonObj->end();
		     p++ ) {

		  // compute distance between recoItem ( particle ) and
		  // partonItem ( particle ), *r and *p
		  distance = TSRule::deltaAngle( r->lorentzVector(),
						 p->lorentzVector(),
						 DELTA_ANGLE );
		  // if distance is a minimum . . .
		  if ( distance < minDistance ) {
		     minDistance = distance;
		     closestPartonObj = partonObj;
		  }
	       }
	 
	    // if a close object was found, associate it with the reco object
	    if ( closestPartonItem != event.partonItems().end() ) {
	       recoObj->setPartonObject( &( *closestPartonObj ) );
	       closestPartonObj->setRecoObject( &( *recoObj ) );
	    }
	 } // end if ( not unclustered energy )  
	 else {
	    
	 }

   /** 
    * Now look at separation in momentum space --
    *  
    */ 
   for ( vector< TurboSimPartonObject >::const_iterator i = 
	    event.partonItems().begin();
	 i != event.partonItems().end();
	 i++ )
      for ( vector < TurboSimPartonObject >::const_iterator j =
	       i;
	    j != event.partonItems().end();
	    j++ ) {

	 HepLorentzVector pip, pjp; // 4-momentum of i^th, j^th parton cluster
	 for ( vector< TurboSimParticle >::const_iterator k = i->begin(); 
	       k != i->end(); 
	       k++ )
	    pip += k->lorentzVector();
	 for ( vector< TurboSimParticle >::const_iterator k = j->begin();
	       k != j->end();
	       k++ )
	    pjp += k->lorentzVector();

	 // mij is mass, a measure of spread in momentum space
	 // two massless particles of energy E, momentum p (= E/c),
	 // can have mass of m = 2E/c^2, maximally ( if p1 = -p2 )
	 double mij = ( pip + pjp ).m();
	 
	 // if momentum is not spread
	 if ( ( i->size() == 1 ) &&
	      ( j->size() == 1 ) &&
	      ( mij < 30 ) ) {
	    // cluster objects
	    string s = i->at( 0 ).type() + j->at( 0 ).type();
	    clustersInThisMassRange_total[s][ int( mij )]++;
	    // 
	    double eip = pip.e(); // total energy of i^th parton object
	    double ejp = pjp.e(); // total energy of j^th parton object
	    
	    double eir = 0;
	    double ejr = 0;
	 }
      }
   
   double rankPair( const TSObjectEvent &event,
		    int i, int j ) {
      double rank = 1;

      HepLorentzVector 
	 vi = event.partonItems()[i].lorentzVector(),
	 vj = event.partonItems()[j].lorentzVector();

      // spread in position space
      rank *= ( 1 - deltaAngle( vi, vj, TSRule::DELTA_ANGLE ) / Math::M_PI );

      double 
	 pzij = ( vi + vj ).pz(),
	 eij = ( vi + vj ).e();
      
      // spread in momentum space
      rank *= (1 - ( eij - pzij ) / ( eij + pzij ) ); 
      
      

   static void generateSets( int size ) {
      sets.clear();
      sets.resize( size );
      for ( int s = 0; s < sets.size(); s++ )
	 subsets( ~0, s + 1, s + 1, sets[s] );
   }
   
   void subsets( int use, 
		 int n, 
		 int bitsAvailable,
		 vector< vector< pair< int, int > > > &sets ) {
      
      // use is a bitgenerator that tell us what indices to 
      // include on this pass:
      // 0 - indice used already -- don't include it in vector
      // 1 - indice not used yet -- iterate through it
      
      vector< pair< int, int > > tmp;
      pair< int, int > tmpPair;
      int bits;
   
      // consider all as singles
      // i is index of particle
      for ( int i = 0; i < n; i++ )
	 if ( use & ( 1 << i ) ) {
	    tmpPair.first = i;
	    tmpPair.second = -1;
	    tmp.push_back( tmpPair );
	 }
      sets.push_back( tmp );
      tmp.clear();
      
      // end recursion
      // no pairing to do
      if ( bitsAvailable == 1 )
	 return; 
      // only one pair
      if ( bitsAvailable == 2 ) {
	 tmpPair.first = tmpPair.second = -1;
	 for ( int i = 0; i < n; i++ )
	    if ( use & ( 1 << i ) )
	       if ( tmpPair.first == -1 )
		  tmpPair.first = i;
	       else {
		  tmpPair.second = i;
		  break;
	       }
	 tmp.push_back( tmpPair );
	 sets.push_back( tmp );
	 return;
      }
      
      // recursively consider one pair
      // in remaining available bits
      for ( int i = 0; i < n; i++ ) {
	 if ( not ( use & ( 1 << i ) ) )
	    continue;
	 for ( int j = i + 1; j < n; j++ ) {
	    if ( not ( use & ( 1 << j ) ) )
	       continue;
	    bits = ( 1 << i ) + ( 1 << j );
	    
	    // the two bits that are set go into a pair
	    // and the rest of the bits are processed
	    // recursively
	    
	    // we loose a use bit on each j loop
	    
	    // process rest of bits
	    // new use = ( ( not bits ) and use )
	    int oldNumSets = sets.size();
	    subsets( ( ~bits ) & use, 
		     n, 
		     bitsAvailable - 2,
		     sets );
	    
	    // add the pair to each added set
	    tmpPair.first = i; tmpPair.second = j;
	    for ( int s = oldNumSets; s < sets.size(); s++ ) {
	       sets[s].push_back( tmpPair );
	    }
	 }
      }
   }

