#include <vector>
#include <string>
#include <cfloat>
#include <numeric>
#include <getopt.h>
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstTurboSim/interface/TurboSimMorphisms.hh"
#include "Validation/VstTurboSim/interface/TurboSimMorphisms2.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonEvent.hh"
#include "Validation/VstMathUtils/interface/Math.hh"
#include "CLHEP/Vector/ThreeVector.h"
#include <cstdlib>
#include <stdio.h>

using namespace std;


class Identifier {

   friend istream& operator>>(istream& fin, Identifier& identifier) {
      fin >> identifier.lineNumber >> identifier.eventType >> identifier.runNumber;
      return(fin);
   }
   
   friend ostream& operator<<(ostream& fout, const Identifier& identifier) {
      fout << identifier.lineNumber << " " << identifier.eventType << " " << identifier.runNumber << " ";
      return(fout);
   }

public:
  string getString() const { return(Math::ftoa(lineNumber)+"_"+eventType+"_"+runNumber); };
  int lineNumber;
  string eventType, runNumber;
  
};



TurboSimMorphisms2::TurboSimMorphisms2() {

   objectPrintFormat = "";

   angleMeasureStick = DELTA_ANGLE;
   angleMeasureFat = KT_DURHAM;
   clusterDistanceStick = ( getenv( "clusterDistanceStick" ) == NULL ? 
			    ( angleMeasureStick == KT_DURHAM ? 0.001 : 0.25) : 
			    atof( getenv( "clusterDistanceStick" ) ) );
   clusterDistanceFat = ( getenv("clusterDistanceFat")==NULL ? 
			  (angleMeasureFat==KT_DURHAM ? 0.002 : 0.5) :  
			  atof(getenv("clusterDistanceFat")) );
   swallowDistance = ( getenv("swallowDistance")==NULL ? 
		       (angleMeasureFat==KT_DURHAM ? 0.001 : 0.2) : 
		       atof(getenv("swallowDistance")) );
   mergeMass = ( getenv("mergeMass")==NULL ? 15 : atof(getenv("mergeMass")) );
   mergeEnergy = ( getenv("mergeEnergy")==NULL ? 10 : atof(getenv("mergeEnergy")) );
   ignoreEnergy = ( getenv("ignoreEnergy")==NULL ? 7.5 : atof(getenv("ignoreEnergy")) );
   maximumNumberOfMorphismTableRowsAbleToBeHeldInMemoryAtOneTime = 500000;
   maxPartonObjectsOnOneLine = 2;

}


double TurboSimMorphisms2::deltaAngle( const CLHEP::HepLorentzVector& v1, 
				       const CLHEP::HepLorentzVector& v2, 
				       AngleMeasure angleMeasureMethod ) {

   if ( angleMeasureMethod == DEFAULT )
      angleMeasureMethod = angleMeasureFat;

   double theta1 = v1.theta();
   double phi1 = v1.phi();
   double theta2 = v2.theta();
   double phi2 = v2.phi();

   double dotProduct, angle, ans;

   switch ( angleMeasureMethod ) {
   
      // Delta Radius (solid angle)   
      case DELTA_R:
	 return Math::deltaR( phi1, Math::theta2eta( theta1 ), phi2, Math::theta2eta( theta2 ) );
	 break;
  
      // Delta Angle
      case DELTA_ANGLE:
	 dotProduct = 
	    sin(theta1)*cos(phi1)*sin(theta2)*cos(phi2) +
	    sin(theta1)*sin(phi1)*sin(theta2)*sin(phi2) +
	    cos(theta1)*cos(theta2);
	 dotProduct = min( 0.999, dotProduct ); // handle 1 + numerical error
	 angle = fabs( acos( dotProduct ));
	 return angle; 
	 break;

      // Manhattan Distance
      case MANHATTAN_DISTANCE:
	 return Math::addInQuadrature( Math::deltaphi( phi1, phi2 ), theta1 - theta2 );
	 break;

      // Energy Difference (magnitude of momentum)
      case KT_DURHAM: 
	 ans = pow( (v1 + v2).m() / 200, 2. );
	 if (( v1.e() < 0.1 ) || 
	     ( v2.e() < 0.1 ))
	    ans = 99;
	 return ans;
    
      default:
	 assert( false );
	 return 0;
   }
}

string getFileName( const string &targetTurboSimMorphismsFileName, 
		    const vector<string> &tag, 
		    const string &type ) {
   
   string ans = targetTurboSimMorphismsFileName + "_";

   // type contains a specific type of event, i.e. 'singlet_reco2parton'
   if ( type != "" )
      ans += type + "_";

   // tag -- type of event. i.e. 'bb' or 'e+e-'
   for ( size_t i = 0; i < tag.size(); i++)
      ans += tag[i];

   ans += ".txt";
   return ans;

}


/*****  Create the TurboSim Morphisms Table  *****/

void TurboSimMorphisms2::create( std::string partonFileName, 
				 std::string recoFileName, 
				 std::string targetTurboSimMorphisms2FileName) {

   objectPrintFormat = "(m)-e-costheta-phi(rad)"; // initialize as lepton
                                                  // colider -- will change if
                                                  // hadron.
   double tol = 0.001; // constant for printing

   // maps associate keys with values
   // associate filenames with output streams
   // One file for each type of partonObject and partonObject pairing.
   // Explanatory note about the many files:
   //   morphismsFiles_out - morphism tables (final output)
   //     filename format: turboSimMorphsims_[ 1 ].txt_[ 2 ].txt
   //                      [ 1 ] = # of events processed: 01 1 all
   //                      [ 2 ] = parton objects: b bb bj ... tau+ tau+tau+
   //                              (OR pairingFraction) 
   //   singletMorphismsFiles_parton2reco_out - temporary files
   //     filename format: turboSimMorphsism_[ 1 ].txt_[ 2 ]parton2reco.txt
   //   singletMorphismsFiles_reco2parton_out - temporary files
   //     filename format: turboSimMorphsism_[ 1 ].txt_[ 2 ]reco2parton.txt
   map< vector< string>, ofstream* > morphismsFiles_out; 
   map< vector< string>, ofstream* > singletMorphismsFiles_parton2reco_out;
   map< vector< string>, ofstream* > singletMorphismsFiles_reco2parton_out;

   // QuaeroItemFile provides interface to files containing many events
   QuaeroItemFile<QuaeroPartonEvent> partonEventFile( partonFileName );
   QuaeroItemFile<QuaeroEvent> recoEventFile( recoFileName );   

   // Temporary storage for building events from constitutent particles
   // Events may contain several QuaeroItemComponents (e.g. particles)
   // A QuaeroParticle is "e" or "j" or "b", etc.
   QuaeroPartonEvent partonEvent;
   QuaeroEvent recoEvent;   //QuaeroEvent == QuaeroRecoEvent< QuaeroRecoObject >

   // Storage for grouping particles from the same event
   // into pairs. Example: ( ("j") . ("b" . "e+") . ("e-") )
   // partonObjectsList.length() == recoObjectsList.length(),
   // but partonObjectsList[i].length() not necessarily equal
   // to recoObjectsList[i].length()
   // QuaeroPartonObject == QuaeroRecoObject
   // QuaeroRecoObject "is-a" QuaeroParticle "is-a" QuaeroItemComponent
   vector< vector< QuaeroPartonObject > > partonObjectsList;
   vector< vector< QuaeroRecoObject > > recoObjectsList;

   // Statistics for reconstructing objects in lookup. They are gathered in the
   // MorphismsTable creation, and used in the transformation of parton to
   // recon objects. Two dimensional matrices: Rows: particle pairs (string). 
   // Columns: Angle ( steps of 0.15 radians ). 
   // clustersIn are counts (int). fractionOf has doubles
   // corresponding to probabilities. For any given element, [i][j],
   // fractionOfClustersInThisAngleRangeToPair[i][j] =
   //    clustersInThisAngleRange_paired[i][j] / clustersInThisAngleRange_total[i][j]
   map< string, map< int, int > > clustersInThisAngleRange_total, 
                                  clustersInThisAngleRange_paired;
   map< string, map< int, double > > fractionOfClustersInThisAngleRangeToPair;
   
   // Possible Objects
   vector< string > objectTypes;
   objectTypes.push_back("e");
   objectTypes.push_back("mu");
   objectTypes.push_back("tau");
   objectTypes.push_back("ph");
   objectTypes.push_back("j");
   objectTypes.push_back("b");
   // additionally, "uncl", or unclustered energy

   // Set up statistical matrices:
   // iterate over all possible objectType pairs 
   // i.e.: e+e+, e+e-, . . ., e+b, . . . , bj, bb 
   // and add to the associated map the mapping (k . 0)
   // for all k, 0 <= k < 5, where k is steps of 0.15 radians.
   for ( size_t i = 0; i < objectTypes.size(); i++ )
      for ( size_t j = 0; j < objectTypes.size(); j++ )
         for ( size_t k = 0; k < 4; k++ ) {
            clustersInThisAngleRange_total[ objectTypes[i] + objectTypes[j] ][k] = 0;
            clustersInThisAngleRange_paired[ objectTypes[i] + objectTypes[j] ][k] = 0;
            fractionOfClustersInThisAngleRangeToPair[ objectTypes[i] + objectTypes[j] ][k] = 0;
         }


   // output to user
   system("date");
   cout << "Reading events . . . " << endl; 
   
   int counter = 0;


   // Read in data from files one object at a time
   cout << "Initial Clustering . . . " << endl;
   while ( partonEventFile.nextEvent( partonEvent ) ) {

//      cout << "parton " << partonEvent << endl;
      // Read in Reco Event
      if ( !recoEventFile.nextEvent( recoEvent ) ) {
         cerr << "\tParton Events File and Reco Events File contain different number of events." << endl
              << "Fatal Error." << endl;
         exit( EXIT_FAILURE ); 
      }

//      cout << "reco   " << recoEvent << endl;

      // Ensure objects are from same run
      if ( ( partonEvent.getEventType() != recoEvent.getEventType() ) ||
	   ( partonEvent.getRunNumber() != recoEvent.getRunNumber() ) ) {
	 cerr << "\tError!  Event mismatch between line #" << counter << " in " 
	      << partonFileName << " and " << recoFileName << endl
	      << "\t  partonEvent:  " << partonEvent << endl
	      << "\t    recoEvent:  " << recoEvent << endl
	      << "Fatal Error." << endl; 
	 exit( EXIT_FAILURE );
      }

      // Clear object lists.
      partonObjectsList.clear();
      recoObjectsList.clear();
      
      if ( counter == 0 )
         if ( partonEvent.hadronMachine() )
            objectPrintFormat = "(m)-pt-eta-phi(deg)"; // hadron collider
     
      counter++;
      //cout << endl << "\tProcessing Event " << counter << ": " << flush;
 
      // More error checking
      if ( ( partonEvent.getObjects().size() == 0 ) ) {
         if ( recoEvent.getObjects().size() == 0 ) 
            continue; // If event is empty, don't include it.
      // continue to next iteration of while loop 
         else {
            cout << "\tWarning! Event in line #" << counter 
                 << " has no parton-level objects:" << endl 
                 << "\t   Parton:" << partonEvent << endl 
                 << "\t But does have reconstructed-level objects:" << endl
                 << "\t   Reco:  " << recoEvent << endl;
         }
      }

      // Decide how to cluster objects in event (pairing).
      // (i.e. which parton objects are the cause of which reco objects.)
      // Input: partonEvent, recoEvent
      // Output: partonObjectsList, recoObjectsList, clustersIn (x 2)
      clusterObjects( partonEvent.getObjects(), recoEvent.getObjects(), 
		      partonObjectsList, recoObjectsList,
		      clustersInThisAngleRange_total, 
		      clustersInThisAngleRange_paired );
//      cout << "objects clustered. " << flush;

      // Assert that clusterObjects( ... ) provided one-to-one mapping of
      // parton objects (or pairs) to reco objects (singles, pairs, triplets, etc.).
      assert( partonObjectsList.size() == recoObjectsList.size() );
      
      // Iterate through the returned ObjectsLists and output the result to the
      // TurboSimMorphisms table
      for ( size_t i = 0; i < partonObjectsList.size(); i++ ) {
         if ( chargeConjugationRequiredQ( partonObjectsList[ i ] ) ) {
            chargeConjugate( partonObjectsList[ i ] );  // Modifies parameter
            chargeConjugate( recoObjectsList[ i ] );
         }

	 
	 // Read the partonObject type. Can be a pair or singlet.
	 vector< string > tag = getTag( partonObjectsList[ i ] );
	 
	 // Don't care about unclustered energy -- (should not have survived
	 // the cut in clusterObjects anyway).
	 if ( tag == vector< string >( 1, "uncl" ) )
	    continue;

	 // If we have not created the tag specific file yet, create it
	 if ( morphismsFiles_out.find( tag ) == morphismsFiles_out.end() ) {
	    morphismsFiles_out[ tag ] = 
	       new ofstream( getFileName( targetTurboSimMorphisms2FileName, tag, "" ).c_str() );
	    *( morphismsFiles_out[ tag ] ) << objectPrintFormat << endl << endl;
	 }
	 
	 if ( !( ( partonObjectsList[i].size() == 1 ) &&
		 ( partonObjectsList[i][0].getObjectType() == "uncl" ))) {

	 // print run data to file
	 *( morphismsFiles_out[ tag ] ) << "* " << counter << " "
					<< flush;
	 *( morphismsFiles_out[ tag ] ) << partonEvent.getEventType() << " " 
					<< partonEvent.getRunNumber() << " " 
					<< flush;
	 *( morphismsFiles_out[ tag ] ) << partonEvent.getZVtx() 
					<< flush;
	 
	 
	 // print parton objects data
	 	 
	 for ( size_t k = 0; k < partonObjectsList[ i ].size(); k++ ) {
	    *( morphismsFiles_out[ tag ] ) << "  " 
					   << partonObjectsList[i][k].print(objectPrintFormat,
									    tol) 
					   << flush;
	 }
	 *( morphismsFiles_out[ tag ] ) << " ;   -> ";
	 
	 // print reco objects data to file
	 for ( size_t k = 0; k < recoObjectsList[ i ].size(); k++ ) {
	    *( morphismsFiles_out[ tag ] ) << "  " 
					   << recoObjectsList[i][k].print(objectPrintFormat,
									  tol) 
					   << flush;
	  }
	 *( morphismsFiles_out[ tag ] ) << " ; " << endl;
	 
	 }

	 // if the parton object maps directly to the reco object (+ or - uncl)
	 // (i.e. singlets, where the reco object can have some uncl. energy
	 //       and the parton object is the same type of object as the 
	 //       reco object.)
	 if ( ( partonObjectsList[ i ].size() == 1 ) && 
	      ( ( recoObjectsList[ i ].size() == 1 ) ||
		( ( recoObjectsList[ i ].size() == 2 ) &&
		  ( recoObjectsList[ i ][ 1 ].getObjectType() == "uncl" ) ) ) &&
	      ( partonObjectsList[ i ][ 0 ].getObjectType() 
		== recoObjectsList[ i ][ 0 ].getObjectType() ) ) {
	    
	    // if stream does not exist, open / create it
	    if ( singletMorphismsFiles_reco2parton_out.find( tag ) == 
		 singletMorphismsFiles_reco2parton_out.end() )
	       singletMorphismsFiles_reco2parton_out[ tag ] = 
		  new ofstream( getFileName( targetTurboSimMorphisms2FileName, 
					     tag, 
					     "singlet_reco2parton" ).c_str() );
	    
	    // print data
	    *( singletMorphismsFiles_reco2parton_out[ tag ] ) 
	       << "* " << partonEvent.getZVtx() << " "  // z vertex 
	       << recoObjectsList[ i ][ 0 ].print( objectPrintFormat ) << " ; -> " // reco objects
	       << partonObjectsList[ i ][ 0 ].print(objectPrintFormat) << " ; " << endl; // parton objects
	    
	    // if parton obj is a singlet (not of unclustered energy)
	    // TODO: isn't this condition always satisfied by outer if statement?
	    if ( ( partonObjectsList[ i ].size() == 1 ) && 
		 ( partonObjectsList[ i ][ 0 ].getObjectType() != "uncl" ) ) {
	       // if stream does not exist, create / open it
	       if ( singletMorphismsFiles_parton2reco_out.find( tag ) ==
		    singletMorphismsFiles_parton2reco_out.end() )
		  singletMorphismsFiles_parton2reco_out[ tag ] = 
		     new ofstream( getFileName( targetTurboSimMorphisms2FileName, 
						tag, 
						"singlet_parton2reco" ).c_str() );
	       // print data
	       // TODO: if this is parton2reco, the order of the printing is 
	       // backward.
	       *( singletMorphismsFiles_parton2reco_out[ tag ] ) 
		  << "* " << partonEvent.getZVtx() << " " //  z vertex
		  << recoObjectsList[i][0].print(objectPrintFormat) << " ; -> " // reco objects
		  << partonObjectsList[i][0].print(objectPrintFormat) << " ; " << endl; // parton objects
	    } 
	 } // close if (direct mapping of singlet -> singlet)
      } // end for -- iterate over all parton objects
//      cout << "SM got here " << endl;
   } // end while -- read in events from file
   
   cout << "Initial Clustering Done. " << endl;

   if ( counter == 0 )
      cerr << "Warning: no events read from " << partonFileName << " " << recoFileName << endl;
   else
      cout << "Read " << counter << endl;

   // -------------------------
   // Now take care of doublets
   // -------------------------
   
   // 
   //interpolatePairingFractions( clustersInThisAngleRange_total,
   //				clustersInThisAngleRange_paired ); 

   // initialize a file stream for the pairing statistics matrix
   ofstream fpairingFraction( getFileName( targetTurboSimMorphisms2FileName, 
					   vector< string >( 0 ), 
					   "pairingFraction" ).c_str());

   // print pairing statistics to file
   for ( map< string, map< int, double > >::iterator i = fractionOfClustersInThisAngleRangeToPair.begin(); 
	 i != fractionOfClustersInThisAngleRangeToPair.end(); 
	 i++ )
      for ( map< int, double >::iterator j = ( i->second ).begin(); // j = i + 1
	    j != ( i->second ).end(); 
	    j++) {
	 if ( clustersInThisAngleRange_total[ i->first ][ j->first ] > 0 )
	    j->second = (double) clustersInThisAngleRange_paired[ i->first ][ j->first ] / 
	                (double) clustersInThisAngleRange_total[ i->first ][ j->first ];
	 fpairingFraction << i->first << " \t " << j->first << " \t " << j->second << endl;
      }

   // close stream
   fpairingFraction.close();

   // Close and destroy morphisms out-file-streams.
   // Initialize in-file-streams for morphisms to 0
   map< vector< string >, ifstream* > morphismsFiles_in;
   for ( map< vector< string >, ofstream* >::iterator i = 
	    morphismsFiles_out.begin(); 
	 i != morphismsFiles_out.end(); 
	 i++ ) {
      // close out-file-stream
      ( i->second )->close(); 
      delete ( i->second ); 
      i->second = NULL;
      // initialize in-file-stream
      morphismsFiles_in[ i->first ] = NULL;
   }

   // Close and destroy singletMorphisms out-file streams.
   // Initialize singletMorphisms in-file streams to new ifstream
   map< vector< string >, ifstream* > singletMorphismsFiles_parton2reco_in;
   for ( map<vector<string>,ofstream*>::iterator i=
	    singletMorphismsFiles_parton2reco_out.begin(); 
	 i != singletMorphismsFiles_parton2reco_out.end(); 
	 i++ ) {
      // close out-file
      ( i->second )->close(); 
      delete ( i->second ); 
      i->second = NULL;
      // initialize in-file
      singletMorphismsFiles_parton2reco_in[ i->first ] = 
	 new ifstream( getFileName( targetTurboSimMorphisms2FileName ,
				    i->first,
				    "singlet_parton2reco" ).c_str());
      
   }

   // Close and destroy singletMorphisms out-file streams
   // Initialize singletMorphisms in-file streams 
   map< vector< string>, ifstream* > singletMorphismsFiles_reco2parton_in;
   for ( map<vector<string>,ofstream*>::iterator i =
	    singletMorphismsFiles_reco2parton_out.begin();
	 i != singletMorphismsFiles_reco2parton_out.end(); 
	 i++) {
      ( i->second )->close(); 
      delete ( i->second ); 
      i->second = NULL;
      singletMorphismsFiles_reco2parton_in[i->first] =
	 new ifstream( getFileName ( targetTurboSimMorphisms2FileName,
				     i->first,
				     "singlet_reco2parton" ).c_str());
   }
   
   if ( getenv( "nodoubletree" ) == NULL ) {
      
      counter = 0;

      // output to user
      system("date +%T");
      cout << "Handling doublets . . . " << endl; 
      
      // Objects of transient data used to read events in from files
      QuaeroRecoObject recoObject, o1;
      QuaeroPartonObject partonObject;
     
      // MultiDimensional Binary Trees for Singlets
      // for morphing Reco to Parton
//      QuaeroParticleMorphismsSingle< QuaeroRecoObject, QuaeroPartonObject > quaeroMorphRP;
      // for morphing Parton to Reco
//      QuaeroParticleMorphismsSingle< QuaeroPartonObject, QuaeroRecoObject > quaeroMorphPR;
	

      // debuging destructor
      if ( true ) {
	 cout << "QuaeroObject destructor test . . . " << flush; 
	 QuaeroRecoObject tmp;
      }
      cout << "destructor okay." << endl;

      if ( true ) {
	 cout << "Quaero MorphismsDouble destructor test . . . " << flush;
	 QuaeroParticleMorphismsDouble< QuaeroPartonObject, 
	    QuaeroRecoObject > tmp;
      }
      cout << "destructor okay." << endl;

      if ( true ) {
	 cout << "Quaero MorphismsSingle destructor test . . . " << flush;
	 QuaeroParticleMorphismsSingle< QuaeroPartonObject, 
	    QuaeroRecoObject > tmp;
      }
      cout << "destructor okay." << endl;


      for( map< vector< string >, ifstream* >::const_iterator i = 
              singletMorphismsFiles_reco2parton_in.begin();
           i != singletMorphismsFiles_reco2parton_in.end(); 
           ++i ) {
      // MultiDimensional Binary Trees for Singlets
      // for morphing Reco to Parton
         QuaeroParticleMorphismsSingle< QuaeroRecoObject, QuaeroPartonObject > quaeroMorphRP;
      // for morphing Parton to Reco
         QuaeroParticleMorphismsSingle< QuaeroPartonObject, QuaeroRecoObject > quaeroMorphPR;
	 
         vector< string > tag = i->first;
         string tagString = "";
         for ( size_t k = 0; k < tag.size(); k++ )
            tagString += tag[ k ];
         ifstream& fin = *( i->second );
         assert( fin );
         string s;
         bool successfulRead = true;

         // read in table file
         while ( fin >> s ) {	    
            // read in an entry
            if ( s != "*" )
               cout << "s = " << s << endl;
            assert( s == "*" );


            // center of vertex
            double zvtx;
            fin >> zvtx;

            QuaeroPartonObject pO;
            QuaeroRecoObject rO;
	    
            // reco object
//            successfulRead = recoObject.read( fin, objectPrintFormat );
            successfulRead = rO.read( fin, objectPrintFormat );
            assert( successfulRead );
	    
	    fin >> s; assert( s == ";" );
	    fin >> s; assert( s == "->" );
	    
            // parton object
//            successfulRead = partonObject.read(fin,objectPrintFormat);
            successfulRead = pO.read(fin,objectPrintFormat);
            assert( successfulRead );

            fin >> s; assert( s == ";" );

            // add entry to recon particles singlet tree
//            quaeroMorphRP.Add( recoObject, vector< QuaeroPartonObject >( 1, partonObject ), zvtx );
            quaeroMorphRP.Add( rO, vector< QuaeroPartonObject >( 1, pO ), zvtx );
	 
         } // end while (read in events from TODO:(temporary?) singletMorphism files)
         fin.close();

     cout << "morphismsFiles_in " << morphismsFiles_in.size() << endl;

//     int iknt=0;
	 
	 // foreach morphism file that contains parton pairs
	 for ( map< vector< string >, ifstream* >::const_iterator j = morphismsFiles_in.begin(); 
	       j != morphismsFiles_in.end(); ++j ) {

        
            // if this file contains parton singlets, (i.e. there is only one 
            // parton in each row on the left-hand side of the table), continue
            // to next file.
            if ( ( j->first ).size() <= 1 ) 
               continue;

            // output to user
            std::string fileName(getFileName( targetTurboSimMorphisms2FileName, j->first, "" ));
/*            char * cstr;
            cstr = new char [fileName.size()+1];
            strcpy (cstr, fileName.c_str()); */

            std::string fileNameModified(getFileName( targetTurboSimMorphisms2FileName, j->first, "modified" ));

            cout << fileName << " * " << fileNameModified << endl;

//            system("sleep 1");
            ifstream fin2( fileName.c_str() );

            if (!fin2.good()) {
               cout << " Cannot open file " << fileName  << endl;
            } else {
               cout << "opened " << fileName << endl;
            }

            ofstream fout( fileNameModified.c_str(),ios_base::app );

            if (!fout.good()) {
               cout << " Cannot open modified file " << fileNameModified  << endl;
            } else {
               cout << "opened " << fileNameModified << endl;
            }

               
 
            string s; // tmp var
            fin2 >> s; assert( s == objectPrintFormat ); fout << s << endl << endl;
            Identifier identifier;
            double zvtx;

            // read in morphism file & write to new file: 'modified'
            while ( fin2 >> s ) {

               cout << s;
               assert ( s == "*" ); fout << "* ";
               fin2 >> identifier; fout << identifier << " ";
               fin2 >> zvtx; fout << zvtx << " ";

               // parton object unchanged: stream in and stream out.
               while ( partonObject.read( fin2, objectPrintFormat ) )
                  fout << partonObject.print( objectPrintFormat ) << " ";
	       
	       fout << " ; ";
	       fin2 >> s; assert( s == "->" ); fout << " -> ";
	       
               // read in reco object(s). 
               // iterate through reco objects
               // o1 is a RECO object
               QuaeroRecoObject oo1;
               while ( oo1.read ( fin2, objectPrintFormat ) ) {
		  
                  bool cc = chargeConjugationRequiredQ( vector< QuaeroRecoObject >( 1, oo1 ) );
                  if ( cc )
                     oo1.chargeConjugate(); // modifies o1
                  string identifierString = identifier.getString();
		  
                  // find the closest
                  // link is a PARTON object
                  vector< QuaeroPartonObject > link = quaeroMorphRP.findClosest( oo1, zvtx, identifierString );
		  
                  if ( link.size() == 1 ) {
                     // o2 is a PARTON object
                     QuaeroPartonObject o2 = link[ 0 ];
                     if ( cc )
                        o2.chargeConjugate();
                     // we print a PARTON object in a reco object field to the file!
                     // And not only a parton object, but an object from a
                     // different event!
                     fout << o2.print ( objectPrintFormat ) << "  ";
                  }
                  else {
                     if ( cc )
                        oo1.chargeConjugate();
                     // we print a RECO object in reco object field to the file
                     // this is the original reco object (i.e. from the same
                     // event, so this is okay)
                     fout << oo1.print ( objectPrintFormat ) << "  ";			  
                  }
               }
               fout << " ; " << endl;
            } // end while read in morphism file and output changes
            fout.close();
            cout << "closed fout " << endl;
            fin2.close();
            cout << "closed fin2 " << endl;
/*            ifstream f1(fileNameModified.c_str(), fstream::binary);
            ofstream f2(fileName.c_str(), fstream::trunc|fstream::binary);
            f2 << f1.rdbuf();
            f1.close();
            f2.close(); */
            rename(fileNameModified.c_str(), fileName.c_str() );

//            system( ( "mv " + getFileName( targetTurboSimMorphisms2FileName, j->first, "modified" ) +

            cout << "at the end of this loop " << endl;
         } // end foreach morphism file containing doublet parton events
//MRENNA         system( ("rm " + getFileName( targetTurboSimMorphisms2FileName, tag, "singlet_reco2parton" ) ).c_str());
         
         remove( getFileName( targetTurboSimMorphisms2FileName, tag, "singlet_reco2parton" ).c_str());
         cout << "and at the end of this loop " << endl;
      }  // end foreach singlet_reco2parton (temporary) file
      
      cout << "Reading singlet_parton2reco tmp files: " << flush;
      // foreach singlet_parton2reco temporary file
//SM
      cout << singletMorphismsFiles_parton2reco_in.size() << endl;


      for( map<vector<string>,ifstream*>::const_iterator i = singletMorphismsFiles_parton2reco_in.begin(); 
           i != singletMorphismsFiles_parton2reco_in.end();  ++i) {


      // MultiDimensional Binary Trees for Singlets
      // for morphing Reco to Parton
	QuaeroParticleMorphismsSingle< QuaeroRecoObject, QuaeroPartonObject > quaeroMorphRP;
      // for morphing Parton to Reco
	QuaeroParticleMorphismsSingle< QuaeroPartonObject, QuaeroRecoObject > quaeroMorphPR;

	 
	 // read singlet_parton2reco file in quaeroMorphPR tree
	 vector< string > tag = i->first;
	 cout << "reading: " << flush;
	 cout << tag[0] << " " << flush;
	 ifstream& fin = *( i->second );
	 string s;	 
	 while ( fin >> s ) {
	    assert( s == "*" );
	    double zvtx;
	    fin >> zvtx;
	    partonObject.read( fin, objectPrintFormat );
	    fin >> s; assert(s==";");
	    fin >> s; assert(s=="->");	  
	    recoObject.read(fin,objectPrintFormat);
	    fin >> s; assert(s==";");
	    quaeroMorphPR.Add(partonObject, vector<QuaeroRecoObject>(1,recoObject), zvtx);
	 }
	 fin.close();
	 cout << "read file. " << flush;
	 // foreach morphism file (some modified already)
	 for ( map<vector<string>,ifstream*>::const_iterator j = morphismsFiles_in.begin(); 
	       j != morphismsFiles_in.end(); ++j) {
	    if ( j->first.size() <= 1 ) // want parton singlets only 
	                                // ( "ph" ) is okay, ( "j" . "j") is not
	       continue;

	    // fout -- target stream for (twice) modified table.
	    // fin2 -- input stream for (once modified) original table.
	    ofstream fout (getFileName(targetTurboSimMorphisms2FileName,j->first,"modified").c_str());
	    ifstream fin2 (getFileName(targetTurboSimMorphisms2FileName,j->first,"").c_str());
	    string s;
	    fin2 >> s; assert(s==objectPrintFormat); fout << s << endl << endl;
	    Identifier identifier;
	    vector<QuaeroPartonObject> partonObjects;
	    double zvtx;
	    
	    // read in events from morphism file 
	    while( fin2 >> s) {
	       assert( s == "*" ); fout << "* ";
	       fin2 >> identifier; fout << identifier << " ";
	       fin2 >> zvtx; fout << zvtx << " ";
	       // read in all parton objects 
	       // store them in partonObjects array
	       // write them back to new file
	       while ( partonObject.read(fin2,objectPrintFormat)) {
		  fout << partonObject.print(objectPrintFormat) << " ";
		  partonObjects.push_back(partonObject);
	       }

	       fout << " ; ";
	       fin2 >> s; assert(s=="->"); fout << " -> ";

	       // read in all reco objects 
	       // write them back to output file
	       // track lines with no reco events
	       bool thereAreZeroRecoObjectsInThisLine = true;
	       while ( o1.read(fin2,objectPrintFormat)) {
		  fout << o1.print(objectPrintFormat) << " ";
		  thereAreZeroRecoObjectsInThisLine = false;
	       }

	       // if there were none, lets find a few
	       if ( thereAreZeroRecoObjectsInThisLine ) {
		  // foreach parton object we are trying to morph
		  for ( size_t k = 0; k < partonObjects.size(); k++ ) {
		     string identifierString = identifier.getString();
		     // look it up in the morph table
		     // link is RECO object
		     
		     vector<QuaeroRecoObject> link 
			= quaeroMorphPR.findClosest( partonObjects[ k ], 
						     zvtx, 
						     identifierString);
		     
                     // if it maps to nothing, or unclustered energy :(
                     // assign it to itself.
                     if ( ( link.empty() ) || 
                          ( ( link.size() == 1 ) && 
                            ( link[ 0 ].getObjectType() == "uncl") ) ) 
                        fout << partonObjects[k].print(objectPrintFormat) << " ";
                  }
               }
               fout << " ; " << endl;
            }
            fout.close();
            fin2.close();
/*            system(("mv "+getFileName(targetTurboSimMorphisms2FileName,j->first,"modified")+
                    " "+getFileName(targetTurboSimMorphisms2FileName,j->first,"")).c_str()); */
            rename(getFileName(targetTurboSimMorphisms2FileName,j->first,"modified").c_str(),
                   getFileName(targetTurboSimMorphisms2FileName,j->first,"").c_str()); 
         }
//         system(("rm "+getFileName(targetTurboSimMorphisms2FileName,tag,"singlet_parton2reco")).c_str());
         remove(getFileName(targetTurboSimMorphisms2FileName,tag,"singlet_parton2reco").c_str());
      } // end foreach singlet_parton2reco file
      cout << "done" << endl;
   }
   
   cout << "cleaning up" << endl;
   for(map<vector<string>,ifstream*>::iterator i=singletMorphismsFiles_parton2reco_in.begin();
       i!=singletMorphismsFiles_parton2reco_in.end(); ++i) {
      i->second->close(); delete(i->second); i->second = 0;
   }
   
   for(map<vector<string>,ifstream*>::iterator i=singletMorphismsFiles_reco2parton_in.begin(); 
       i!=singletMorphismsFiles_reco2parton_in.end(); ++i) {
      i->second->close(); delete(i->second); i->second = 0;
   }
   cout << "done cleanup. " << endl;
   return;
}

/*****  Read the TurboSim Morphisms file and build the multivariate binary tree  *****/

void TurboSimMorphisms2::readFromFile(string filename) {
   ifstream fin(filename.c_str());
   string marker; 
   double zvtx;
   int partonTrainingLineNumber = 0;
   string eventType, runNumber, identifier;
   int k = 0;
   bool skipFakes = false;
   if(getenv("skipFakes")!=NULL)
      {
	 cout << "(skipping fakes)" << flush;
	 skipFakes = true;
      }
   if(fin)
      {
	
	// Read the object format:  lepton or hadron machine
	fin >> objectPrintFormat;
	while(fin >> marker)
    {
       k++;
       if(k%100000==0)
	      cout << k << " " << flush;
       assert(marker=="*");
       fin >> partonTrainingLineNumber >> eventType >> runNumber;
       //  identifier = runNumber;
       identifier = eventType + " " + runNumber;
       fin >> zvtx; // vertex position
	    
       QuaeroPartonObject partonObject;
       vector<QuaeroPartonObject> partonObjects;
       QuaeroRecoObject fsObject;
       vector<QuaeroRecoObject> fsObjects;
	       
       bool parton_NoLeptons = true;
       while(partonObject.read(fin,objectPrintFormat))
       {
          if(partonObject.getObjectType()!="uncl")
		     partonObjects.push_back(partonObject);
          if((partonObject.getObjectType()=="j")||
             (partonObject.getObjectType()=="b")||
             (partonObject.getObjectType()=="uncl")||
             (partonObject.getObjectType()=="ph"))
		     ;
          else
		     parton_NoLeptons = false;
          
          identifier += " " + partonObject.print(objectPrintFormat);
       }
	       
       fin >> marker;
       assert(marker=="->");
       identifier += " -> ";
	       
       bool fs_ContainsLepton = false;
       while(fsObject.read(fin,objectPrintFormat))
       {
          fsObjects.push_back(fsObject);
          if((fsObject.getObjectTypeSansSign()=="e")||
             (fsObject.getObjectTypeSansSign()=="mu")||
             (fsObject.getObjectTypeSansSign()=="tau"))
		     fs_ContainsLepton = true;
          //identifier += ( "/" + fsObject.getObjectType() + 
          //	   "/" + Math::ftoa( fsObject.getFourVector().e() ) );
          identifier += " " + fsObject.print(objectPrintFormat);
       }
	       
       if(skipFakes)
          if(parton_NoLeptons && fs_ContainsLepton)
             continue; // skip fakes
	       
       assert((partonObjects.size()<=3));
       //cout << "TurboSimMorphisms2::readFromFile:identifier = " << identifier << endl;
       if ( partonObjects.size() == 1 ) {
          if ( partonObjects[0].getFourVector().perp()<0.002 )
             continue;
          else
          {
		     turboSimMorphismsForestSingle.Add(partonObjects[0], fsObjects, zvtx, identifier);
          }
       }
       if(partonObjects.size()==2)
          turboSimMorphismsForestDouble.Add(partonObjects[0], partonObjects[1], fsObjects, zvtx, identifier);
       if(partonObjects.size()==3)
          turboSimMorphismsForestTriple.Add(partonObjects, fsObjects, zvtx, identifier);
	       
    }
      }
   else
   {
      cout << "warning, " << filename << " does not exist." << endl;
   }
   fin.close();
   return;
}


void TurboSimMorphisms2::morph(string turboSimMorphismsFile, 
			       vector<string> inputPartonFiles, 
			       string outputRecoFile)
{
   // system("date");
   cout << "morphing... " << flush;
   QuaeroPartonEvent partonEvent;
   map<vector<string>,ofstream*> partonClusterFiles;
   map<vector<string>,ofstream*> reco_needSecondPass;
   ofstream fout_all(getFileName(outputRecoFile,vector<string>(0),"every").c_str());
   int counter = 0;
   objectPrintFormat = "(m)-e-costheta-phi(rad)"; // lepton collider
   
   map<string, map<int,double> > fractionOfClustersInThisAngleRangeToPair;
   ifstream fpairingFraction(getFileName(turboSimMorphismsFile,vector<string>(0),"pairingFraction").c_str());
   string objectPairType;
   int angleStep; double f;
   while(fpairingFraction >> objectPairType)
      {
	 fpairingFraction >> angleStep >> f;
	 fractionOfClustersInThisAngleRangeToPair[objectPairType][angleStep] = f;
      }
   fpairingFraction.close();
   
   
   // Read in all parton-level events, and partition the parton object clusters into individual files
   for(size_t i=0; i<inputPartonFiles.size(); i++)
      {
	 QuaeroItemFile<QuaeroPartonEvent> fparton(inputPartonFiles[i].c_str());      
	 while(fparton.nextEvent(partonEvent))
	    {
	       if(counter==0)
		  if(partonEvent.hadronMachine())
		     objectPrintFormat = "(m)-pt-eta-phi(deg)"; // hadron collider
	       
	       counter++;
	       vector<QuaeroPartonObject> partonObjects = partonEvent.getObjects();
	       vector<vector<QuaeroPartonObject> > partonObjectsList;
	       
	       clusterObjects(partonObjects, partonObjectsList, fractionOfClustersInThisAngleRangeToPair);
	       fout_all << "* " << counter << " " 
			<< partonEvent.getEventType() << " " 
			<< partonEvent.getRunNumber() << " " 
			<< partonEvent.getWeight() << " " 
			<< partonEvent.getCollisionType() << " " 
			<< partonEvent.getRootS() << " " 
			<< partonEvent.getZVtx() << " ;\n";
	       for(size_t i=0; i<partonObjectsList.size(); i++)
		  {
		     bool cc = chargeConjugationRequiredQ(partonObjectsList[i]);
		     if(cc)
			chargeConjugate(partonObjectsList[i]);
		     vector<string> tag = getTag(partonObjectsList[i]);
		     if(partonClusterFiles.find(tag)==partonClusterFiles.end())
			partonClusterFiles[tag]= new ofstream(getFileName(outputRecoFile,tag,"cluster").c_str());
		     if(cc)
			chargeConjugate(partonObjectsList[i]); // ... and back again
		     *(partonClusterFiles[tag]) << "* " << counter << " " 
						<< partonEvent.getEventType() << " " 
						<< partonEvent.getRunNumber() << " " 
						<< partonEvent.getWeight() << " " 
						<< partonEvent.getCollisionType() << " " 
						<< partonEvent.getRootS() << " " 
						<< partonEvent.getZVtx() << " ";
		     for(size_t j=0; j<partonObjectsList[i].size(); j++)
			*(partonClusterFiles[tag]) << partonObjectsList[i][j].print(objectPrintFormat) << " ";
		     *(partonClusterFiles[tag]) << " ; " << endl;
		  }
	    }
      }
   fout_all.close();

   //closing the partonClusterFiles
   for(map<vector<string>,ofstream*>::iterator i=partonClusterFiles.begin(); i!=partonClusterFiles.end(); i++)
      {
	 i->second->close(); delete(i->second); i->second = 0;
      }

   int totalNumberOfEvents = counter;
   Identifier identifier;
   string collisionType;
   double weight, rootS, zvtx;
   
   // Read in each individual file, using the table to turn the parton-level objects into reco-level objects
   vector<string> completeRecoFileNames;
   completeRecoFileNames.push_back(getFileName(outputRecoFile,vector<string>(0),"every"));
   ofstream dbg( "useMorphisms.debug" );
   ofstream dbgSearch ( "searchTraces.debug" );
   for(int k=1; k<=2; k++)     
      {
	 for(map<vector<string>,ofstream*>::iterator i=partonClusterFiles.begin(); 
	     i!=partonClusterFiles.end(); i++)
	    {
	       vector<string> tag = i->first;
	       string partonFileName = (k==1 ? getFileName(outputRecoFile,tag,"cluster") : 
					getFileName(outputRecoFile,tag,"needSecondPass"));				   
	       ifstream finParton(partonFileName.c_str());
	       string recoCompleteFileName = getFileName(outputRecoFile,tag,"reco_complete"+Math::ftoa(k));
	       completeRecoFileNames.push_back(recoCompleteFileName);
	       ofstream foutRecoComplete(recoCompleteFileName.c_str());
	       turboSimMorphismsForestSingle.clear();
	       turboSimMorphismsForestDouble.clear();
	       turboSimMorphismsForestTriple.clear();
	       for(size_t l=0; l<tag.size(); l++)
		  cout << tag[l];
	       cout << " " << flush;
	       this->readFromFile(getFileName(turboSimMorphismsFile,tag,""));
	       cout << endl << endl;
	       string s;
	       while(finParton >> s)
		  {
		     vector<QuaeroPartonObject> partonObjects;
		     QuaeroPartonObject partonObject;
		     assert(s=="*"); 
		     finParton >> identifier >> weight >> collisionType >> rootS >> zvtx; 
		     while(partonObject.read(finParton,objectPrintFormat))
			partonObjects.push_back(partonObject);
		     
		     assert(partonObjects.size()<=3);
		     vector<QuaeroRecoObject> result;
		     string comment="";
		     string comment1="";
		     string searchTrace = "";
		     
		     bool cc = chargeConjugationRequiredQ(partonObjects);
		     if(cc)
			chargeConjugate(partonObjects);
		     
		     if(k==2)
			assert(partonObjects.size()==1);
		     if(partonObjects.size()==1)
			result = turboSimMorphismsForestSingle.findClosest(partonObjects[0], zvtx, comment, searchTrace);
		     if(partonObjects.size()==2)
			result = turboSimMorphismsForestDouble.findClosest(partonObjects[0], 
									   partonObjects[1], 
									   zvtx, 
									   comment);
		     if(partonObjects.size()==3)
			result = turboSimMorphismsForestTriple.findClosest(partonObjects, zvtx, comment);
		     if(partonObjects.size()>=4)
			cout << "Multiparticle gathering -- giving up." << endl;

		     // dhf - information for useMorphisms.debug
		     //----
		     dbg <<  "parton_particle: " << identifier.eventType << " " << identifier.runNumber << " ";
		     if ( partonObjects.size() > 0 )
		       dbg << partonObjects[0].print(objectPrintFormat) << " " ;
		     if ( partonObjects.size() > 1 )
		       dbg << partonObjects[1].print(objectPrintFormat) << " " ;
		     dbg << "transformed_by/" << comment << "/"
			    << " into_reco_particle: " << " ";
		     for ( size_t j = 0; j < result.size(); j++ )
		       dbg << " " << result[j].print(objectPrintFormat); 
		     
		     dbg << endl << endl;;
			 
		     //----

		     // dhf - information for searchTraces.debug
		     dbgSearch << identifier.runNumber 
			       << "/";
		     if ( partonObjects.size() > 0 )
			dbgSearch << partonObjects[0].getObjectType() << "/" 
				  << partonObjects[0].getFourVector().e() << "/";
		     if ( partonObjects.size() > 1 )
			dbgSearch << partonObjects[1].getObjectType() << "/" 
				  << partonObjects[1].getFourVector().e() << "/";
		     dbgSearch << searchTrace << endl;

		     if(cc) 
			chargeConjugate(result);
		     
		     if(getenv("debug")!=NULL)
			{
			   cout << identifier << "  ";
			   for(size_t j=0; j<partonObjects.size(); j++)
			      cout << partonObjects[j].print(objectPrintFormat) << "  ";
			   cout << "TO  ";
			   for(size_t j=0; j<result.size(); j++)
			      cout << result[j].print(objectPrintFormat) << "  ";
			   cout << "FROM  " << comment << endl;
			}
		     
		     
		     if(partonObjects.size()==1)
			{
			   foutRecoComplete << "* " 
					    << identifier << " " 
					    << weight << " " 
					    << collisionType << " " 
					    << rootS << " " 
					    << zvtx << " ";
			   for(size_t i=0; i<result.size(); i++)
			      foutRecoComplete << result[i].print(objectPrintFormat) << " ";
			   foutRecoComplete << " ; " << endl;
			}
		     else
			{
			   for(size_t i=0; i<result.size(); i++)
			      {
				 vector<string> tag = getTag( vector< QuaeroRecoObject >( 1, result[ i ] ) );
				 if(reco_needSecondPass.find(tag)==reco_needSecondPass.end())
				    reco_needSecondPass[tag] = 
				       new ofstream(getFileName(outputRecoFile,tag,"needSecondPass").c_str());
				 *(reco_needSecondPass[tag]) << "* " 
							     << identifier << " " 
							     << weight << " " << collisionType 
							     << " " << rootS << " " 
							     << zvtx << " ";
				 *(reco_needSecondPass[tag]) << result[i].print(objectPrintFormat) 
							     << " ; " << endl;
			      }
			}
		  }
	       finParton.close();
	       // system(("rm "+partonFileName).c_str());
	       foutRecoComplete.close();
	    }
      } // for k = 1 to 2
   
   
   for(map<vector<string>,ofstream*>::iterator i=reco_needSecondPass.begin(); 
       i!=reco_needSecondPass.end(); i++)
      {
	 i->second->close(); delete(i->second); i->second = 0;
      }
   
   // sort reco_complete2 files by line number
   for( size_t i = 0; i < completeRecoFileNames.size(); i++ ) {
      // remove leading *
      system( string( "sed 's/* //' " + completeRecoFileNames[i] + " > tmp.txt" ).c_str() );
      // sort
      system( string( "sort -n tmp.txt > tmp2.txt" ).c_str() );
      // reinsert leading *
      system( string( "sed 's/\\(.*\\)/* \\1/' tmp2.txt > " + completeRecoFileNames[i] ).c_str() );
   }
   remove("tmp.txt");
   remove("tmp2.txt");
//   system( "rm tmp.txt tmp2.txt" );

   map<ifstream*,int> counterInEachCompleteRecoFile;
   map<ifstream*,string> filenames;
   for(size_t i=0; i<completeRecoFileNames.size(); i++)
      {
	 string s;
	 ifstream* p = new ifstream(completeRecoFileNames[i].c_str());
	 filenames[p] = completeRecoFileNames[i];
	 if(*p >> s)
	    {
	       assert(s=="*");
	       int counter;
	       *p >> counter; counterInEachCompleteRecoFile[p] = counter;
	    }
      }
   

   // collect clusters into events.
   // each event has an ID -- a "counter". For
   // each counter, search for clusters with the
   // same counter, and add these objects to the
   // growing event. When all counters have been found,
   // print entire event to outputRecoFile.
   ofstream fout(outputRecoFile.c_str());
   // foreach event
   for ( int outCounter = 1;  // outCounter is ID of event currently being collected
	 outCounter <= totalNumberOfEvents; 
	 outCounter++) {
      bool stillGoing = true; // still has objects to add
      string eventType, runNumber, s;
      double zvtx;
      vector<QuaeroRecoObject> objects;
      QuaeroRecoObject o;
      while ( stillGoing ) {
	 stillGoing = false;
	 // particles are order in file by their counter ID
	 // counterInEachCompleteRecoFile maps a file to the
	 // counter of the top particle in that file.
	 // foreach cluster file
	 for ( map< ifstream*, int >::iterator i = 
		  counterInEachCompleteRecoFile.begin(); 
	       ! counterInEachCompleteRecoFile.empty() && 
		  i != counterInEachCompleteRecoFile.end() ; 
	       ++i )
	    // if counter in file matches event counter
	    if ( i->second == outCounter ) {
	       // read in a cluster
	       *( i->first ) >> eventType >> runNumber >> weight >> collisionType >> rootS >> zvtx;
	       while ( o.read( *(i->first), objectPrintFormat ) )
		  objects.push_back(o);
	       stillGoing = true;
	       // update file counter
	       if ( *( i->first ) >> s ) {
		  int newCount;
		  assert( s == "*" );
		  *( i->first ) >> newCount;
		  if ( newCount < counterInEachCompleteRecoFile[ i->first ] ) {
		     cout << "in file " << filenames[ i->first ] << ": ";
		     cout << "new counter: " << newCount << " ";
		     cout << "old counter: " << counterInEachCompleteRecoFile[ i->first ] << endl;
		  }
		  counterInEachCompleteRecoFile[ i->first ] = newCount;
	       }
	       // if cluster file is empty
	       else {
		  i->first->close();
		  delete( i->first );
		  counterInEachCompleteRecoFile.erase(i);
		  break;
	       } // else (if file empty)
	    } // foreach clusters file
      } // while ( there are still particles in this event ) 
      QuaeroEvent e(eventType, runNumber, weight, objects, collisionType, rootS, zvtx);
      fout << e << endl;
   } // foreach event 
   fout.close();
   cout << endl;

   cout << "Final Counter Info: " << endl;
   for ( vector< string >::const_iterator i =
	    completeRecoFileNames.begin();
	 i != completeRecoFileNames.end();
	 ++ i )
      cout << "File: " << *i << endl;
   for ( map< ifstream*, int >::iterator i = 
	    counterInEachCompleteRecoFile.begin(); 
	 i != counterInEachCompleteRecoFile.end() ; 
	 ++i ) 
      cout << "File: " << filenames[ i->first ]  << " " 
	   << "Counter: " << i->second << endl;

   // dhf - debug information
   dbg.close();
   dbgSearch.close();

   return;
}


void TurboSimMorphisms2::clusterObjects(const vector< QuaeroPartonObject >& _partonObjects, 
					vector< vector< QuaeroPartonObject > >& partonObjectsList,
					map<string, map< int, double > >& fractionOfClustersInThisAngleRangeToPair)

/* ABSTRACT: 
     Takes in a list of parton objects and determines whether to pair them.
     Returns a list of parton object groups. Many groups will have only one
     item.

   INPUTS:
     _partonObjects - vector of individual parton objects. Example:
              ( ph, e, j, b )
     fractionOfClustersInThisAngleRangeToPair - mapped matrix with data of
         type double corresponding to the probability that two objects should
	 be paired in the Morphisms table. Example:
               Energy Range:  1  2  3  4  . . . 30          
               Pair Type:
                     "jj"   [.4 .6 .2 .3  . . .    ]
		     "jph"  [.1 .2 .5 .1  . . .    ]
                     "je"   [.5 .3 .7 0   . . .    ]
                     . . .          |  
                                    |
                .7 is probability that a j and e of combined Pt ~3 should
                be treated as a pair. (--> .3 probability they should be two
                objects)
   OUTPUTS:
     partonObjectsList - vector of single and pairs of parton objects.
         Example: ( ( ph ) . ( e . j ) . ( b ) )
     void.
*/

{
   
   // Initialize partonObjectsList to empty
   partonObjectsList = vector< vector< QuaeroPartonObject > >( 0 ); 
   
   // Create modifiable copy of _partonObjects input
   vector< QuaeroPartonObject > partonObjects = _partonObjects;

   // Preprocessing -- throw out useless objects
   for( size_t i = 0; i < partonObjects.size(); i++)
      // if ( object has small Pt || object is 'unclustered energy' )
      if ( ( partonObjects[ i ].getFourVector().perp() < 3 )  || 
	   ( partonObjects[ i ].getObjectType() ==  "uncl" ) ) {
	 // remove object from list -- it is not useful
	 partonObjects.erase( partonObjects.begin() + i, 
			      partonObjects.begin() + i + 1 );
	i--;
      }

   // Preprocessing -- morph two objects into one if one is insignificant
   // foreach parton object pair, ( i . j ), i != j
   for ( size_t i = 0; i < partonObjects.size(); i++ )
      for ( size_t j = i + 1; j < partonObjects.size(); j++ ) {

	 string q = "";  // new object type (i.e. "e", "ph", or "j")
	 
	 // assign new object type if one is much more massive 
	 // (i.e. in a photon, electron pair, a photon is much more massive)
	 // only look at following pairs: (e, ph), (j, ph)
	 if ( ( partonObjects[ i ].getObjectTypeSansSign() == "e"  ) &&
	      ( partonObjects[ j ].getObjectTypeSansSign() == "ph" ) )
	    q = partonObjects[ i ].getObjectType();
	 else if ( ( partonObjects[ i ].getObjectTypeSansSign() == "ph" ) &&
		   ( partonObjects[ j ].getObjectTypeSansSign() == "e"  ) ) 
	    q = partonObjects[ j ].getObjectType();
	 else if ( ( partonObjects[ i ].getObjectTypeSansSign() == "j"  ) &&
		   ( partonObjects[ j ].getObjectTypeSansSign() == "ph" ) )
	    q = partonObjects[ i ].getObjectType();
	 else if ( (partonObjects[ i ].getObjectTypeSansSign() == "ph" ) &&
		   (partonObjects[ j ].getObjectTypeSansSign() == "j"  ) ) 
	    q = partonObjects[ j ].getObjectType();
	 
	 // if new object has been assigned, check validity
	 if ( q != "" ) {
	    if( ( ( q == "j" ) && 
		  ( objectPrintFormat == "(m)-pt-eta-phi(deg)" ) && // hadron collider
		  ( deltaAngle( partonObjects[ i ].getFourVector(), 
				partonObjects[ j ].getFourVector(), 
				DELTA_R ) < 0.5 ) ) 
		||
		( ( q.substr( 0, 1 ) == "e" ) && // q.(sans sign)
		  ( deltaAngle( partonObjects[ i ].getFourVector(),
				partonObjects[ j ].getFourVector(), 
				DELTA_ANGLE ) < 0.2) ) ) {
	       // if valid,
	       // replace objects in parton object list
	       partonObjects[i] = 
		  QuaeroPartonObject( q, 
				      partonObjects[ i ].getFourVector() + 
				      partonObjects[ j ].getFourVector() );
	       partonObjects.erase( partonObjects.begin() + j );
	       j--;
	    }
	 }
      }

   // Heart of clusterObjects
   // Much different from Morphisms1

   // Intialize partonObjectsList
   for ( size_t i = 0; i < partonObjects.size(); i++ )
      if ( find( partonObjectsList.begin(), // find( . . . ) in <algorithm>
		 partonObjectsList.end(), 
		 vector<QuaeroPartonObject>( 1, partonObjects[ i ] ) )
	   == partonObjectsList.end() ) // if partonObject not in list, 
	 partonObjectsList.push_back( vector< QuaeroPartonObject>( 1, partonObjects[ i ]) );
   
   // Pair objects
   for ( size_t i = 0; i < partonObjectsList.size(); i++ )
      for ( size_t j = i + 1; j < partonObjectsList.size(); j++ ) {

	 // if both objects are singlets
	 // and they are particle / anti-particle pair
	 if ( ( partonObjectsList[ i ].size() == 1 ) &&
	      ( partonObjectsList[ j ].size() == 1 ) ) {

	    // mij = Mass of (i + j)
	    double separation = 
	       deltaAngle( partonObjectsList[ i ][ 0 ].getFourVector(),
			   partonObjectsList[ j ][ 0 ].getFourVector(),
			   DELTA_ANGLE );
 
	    if ( separation < 0.75 ) {  // 0.75 radians or 45 degrees
	       // i and j being paired.

	       double r = drand48(); // roll dice

	       string objectPairType = 
		  partonObjectsList[ i ][ 0 ].getObjectTypeSansSign() + 
		  partonObjectsList[ j ][ 0 ].getObjectTypeSansSign(); // = "ej"

	       // pair has certain probabiliy of being clustered
	       // probability is found in fractionOfClusters matrix
	       // compare r (dice roll) to probability
	       if ( r < fractionOfClustersInThisAngleRangeToPair
		    [ objectPairType ][ int( separation / 0.15 ) ] ) {
		  // Cluster Objects
		  partonObjectsList[ i ].insert( partonObjectsList[ i ].end(),
						 partonObjectsList[ j ].begin(), 
						 partonObjectsList[ j ].end() );
		  partonObjectsList.erase(partonObjectsList.begin()+j);
		  break;
	       }
	    }
	 }
      }

   return;
   
}

class UnclLikelihood: public Math::FunctionObject {
   
public:
   UnclLikelihood( const vector< string >& _objectTypes, 
		   const vector< double >& _lhs_p, 
		   const vector< double >& _rhs_p ) {

      n = _lhs_p.size();
      assert( n == _rhs_p.size() );
      assert( n == _objectTypes.size() );
      lhs_p = _lhs_p;
      rhs_p = _rhs_p; 
      objectTypes = _objectTypes;
   }

   double operator()( const vector< double >& _x ) {

      double ans = 0;
      double deltaE = 1.2; // units are GeV
      double fudgeE = 4.0; // units are GeV
      vector<double> x = _x;
      assert( x.size() == n );
      for ( size_t i = 0; i < n; i++ ) {
	double p = 2.;
	if ( ( objectTypes[ i ] == "btau" ) && 
	     ( lhs_p[ i ] > rhs_p[ i ] + x[ i ] ) )
	   p = 1.; // bOrTauWithEscapedNeutrino

	ans += pow( fabs( x[ i ] + rhs_p[ i ] - lhs_p[ i ] ) /
		    Math::addInQuadrature( sqrt( ( fabs( lhs_p[ i ] ) + 
						   fabs( rhs_p[ i ] ) ) / 2 ),
					   deltaE ) ,
		    p ); // pt_x
      }
      ans += pow( fabs( accumulate( x.begin(), x.end(), 0.) ) /
		  fudgeE, 
		  2.);
      ans = sqrt( ans );
      return ans;
   } 

private:
   size_t n;
   vector< double > lhs_p, rhs_p;
   vector< string > objectTypes;

};


void TurboSimMorphisms2::clusterObjects( const vector< QuaeroPartonObject >& _partonObjects, 
					 const vector< QuaeroRecoObject >& recoObjects, 
					 vector< vector< QuaeroPartonObject > >& partonObjectsList, 
					 vector< vector< QuaeroRecoObject > >& recoObjectsList,
					 map< string, map< int, int > >& clustersInThisAngleRange_total,
					 map< string, map< int, int > >& clustersInThisAngleRange_paired ) {
   
   // clusterObjects: (1) map reco objects to nearest parton objects
   //                 (2) determine clustering of parton objects
   //                 (3) determine pairing of parton objects
   // Arguments;
   //   Input: _partonObjects, recoObjects
   //   Output: partonObjectsList, recoObjectsList
   //   Modifies: clustersInThisAngleRange_total, clustersInThisAngleRange_paried
   // Notes:
   //   See other clusterObjects fuction for more comments about clustersInThis . . .
   //     The difference here is that clustersInThisAngleRange are _counts_, not probabilities.
   //     Therefore the data is of type int instead of double.
   //   partonObjectsList is of the form ( ( e+ ) . ( e- . j ) . ( b ) )
   //     Again, see other clusterObjects for more details.
   
   partonObjectsList = vector< vector< QuaeroPartonObject > >( 0 );
   vector< QuaeroPartonObject > partonObjects = _partonObjects;

   // Remove parton level objects with energy that is 
   for ( size_t i = 0; i < partonObjects.size(); i++ )
      if ( ( partonObjects[ i ].getFourVector().perp() < 3) || // Pt less than 3 GeV
	   ( partonObjects[ i ].getObjectType() == "uncl" ) ) {  // unclustered
	 partonObjects.erase( partonObjects.begin() + i,
			      partonObjects.begin() + i + 1 );
	 i--;
      }

   // Fill partonObjectsList
   // Remove duplicates while adding partonObjectsList
   // (problem with duplicates in data files)?
   for( size_t i = 0; i < partonObjects.size(); i++ )
      if ( find( partonObjectsList.begin(), 
		 partonObjectsList.end(), 
		 vector< QuaeroPartonObject >( 1, partonObjects[ i ] ) ) 
	   == partonObjectsList.end() )
	 partonObjectsList.push_back( vector< QuaeroPartonObject >( 1, partonObjects[ i ] ) );

   // Initialize recoObjectsList length
   recoObjectsList = vector<vector<QuaeroRecoObject> >( partonObjectsList.size() );
   if ( recoObjectsList.size() == 0 )
      return; 


   // Every reconstructed object is associated with the closest parton object
   //  (every effect has a cause)
   // ----------------------------
   // Iterate through recoObjects and compute closest partonObject
   // via nearest neighbor algorithm (distance heruistic is solid angle)
   for ( size_t i = 0; i < recoObjects.size(); i++) {   // for each recoObject
     if( ( recoObjects[ i ].getObjectType() != "jf" ) &&
	 ( recoObjects[ i ].getObjectTypeSansSign() != "jtau" ) )  // Do not consider jf or jtau objects   
       {
      if ( ( recoObjects[ i ].getObjectType() != "uncl" ) ) {  
//	 double minDistance = FLT_MAX, 
	 double minDistance = FLT_MAX,
	        distance;
	 int closestPartonObject = -1;
	 for ( size_t k = 0; k < partonObjectsList.size(); k++ )
	    for ( size_t j = 0; j < partonObjectsList[ k ].size(); j++ ) {
	       // compute distance between reco object and parton object
	       distance = deltaAngle( recoObjects[ i ].getFourVector(),
				      partonObjectsList[ k ][ j ].getFourVector(),
				      DELTA_ANGLE);  
	       // if distance is a minimum . . .
	       if ( distance < minDistance ) {
		  minDistance = distance;
		  closestPartonObject = k;
	       }
	    }
	 // if a close paron object was found, associate it with reco object
	 if ( closestPartonObject >= 0 )
	    // association established through array location
	    // reco objects have same location in recoObjectsList as their
	    // associated parton objects have in partonObjectsList
	    recoObjectsList[ closestPartonObject ].push_back( recoObjects[ i ] );
	 else // if no minimum found
	    recoObjectsList.back().push_back( recoObjects[ i ] ); // associate with "uncl"
      } // end if 
      
      // Unclustered energy is treated separately and associated with uncl 
      // ----------------------------------------------------------------
      else { // if object is unclustered energy

	 // find the unclustered energy in the parton objects
	 int unclPos = -1;
	 for ( size_t j = 0; j < partonObjectsList.size(); j++ ) {
	    assert( partonObjectsList[ j ].size() >= 1 );
	    if ( partonObjectsList[ j ][ 0 ].getObjectType() == "uncl" )
	       unclPos = j;
	 }
	 
	 if ( unclPos >= 0 )
	    // asssociate the reco and parton 'objects'
	    recoObjectsList[ unclPos ].push_back( recoObjects[ i ] );
      }  // end if unclustered energy      
       } // if not jf and not jtau
   } // end foreach( reco object )

   //cout << "singlet matching done. " << flush;

   /** 
    * At this point, partonObjectsList contains no doublets
    * every partonObject is a singlet which may or may not 
    * have a corresponding recoObject.
    * recoObjectsList can vary greatly. There can be many
    * particles / object. All reco particles are included
    * in the list. 
    * So we have single parton objects mapped to none, or 
    * one, or many reco particles.
    */

   // Now join partonObjects:
   // Look at separation in momentum space --
   // if partonObject[ i ]  --> recoObject[ i ] 
   // and partonObject[ j ] --> recoObject[ j ], ( i != j )
   // and energies are: p[ i ] = 20,  r[ i ] = 40
   //                   p[ j ] = 35,  r[ j ] = 15
   // then maybe we should join p[i] and p[j].
   // DHF - do not consider a pair if both particles
   // are the same sign: i.e. both antiparticles, 
   // or both particles. Must be particle, antiparticle
   // pair.

   double separationCut = 0.75;

   double howMuchWeLikeToKeepSinglets = 2;
   if ( getenv("TurboSimKeepSinglets") != NULL )
      howMuchWeLikeToKeepSinglets = atof( getenv("TurboSimKeepSinglets") );
   
   // for each partonObject pair: i, j  (i != j)
   for ( size_t i = 0; i < partonObjectsList.size(); i++ )
      for ( size_t j = i + 1; j < partonObjectsList.size(); j++ ) {
	 
	 // .size() restrictions to constrain parton clustering to doublets
	 // sign restriction: two particles of same sign should not be paired.
	 // i.e.: e+ e+ or e+ tau+ should not be paired.
	 // okay to pair j j, hence && sign != "" condition
	 if ( ( partonObjectsList[ i ].size() == 1 ) &&
	      ( partonObjectsList[ j ].size() == 1 ) )  {
	    
	    //cout << "found prospective pair: " 
	    //	 << partonObjectsList[i][0].getObjectType() 
	    //	 << partonObjectsList[j][0].getObjectType() 
	    //	 << ", " << flush;

	    // Find Parton Object Momentas
 	    CLHEP::HepLorentzVector pip, pjp; // 4-momentum of i^th, j^th parton cluster
	    pip = partonObjectsList[ i ][ 0 ].getFourVector();
	    pjp = partonObjectsList[ j ][ 0 ].getFourVector();
	
	    // mij is mass, a measure of spread in momentum space
	    // two massless particles of energy E, momentum p (= E/c),
	    // can have mass of m = 2E/c^2 (maximally, if p1 = -p2)
	    double separation = deltaAngle( pip, pjp, DELTA_ANGLE );

	    // if particles close together
	    if ( separation < separationCut ) { // .75 radian or 45 degrees separation
	       //cout << "clustering, " << flush;

	       // create paired tag
	       string s;
	       s = partonObjectsList[i][0].getObjectTypeSansSign() +
		  partonObjectsList[j][0].getObjectTypeSansSign();
	       
	       // cluster objects
	       clustersInThisAngleRange_total[s][int(separation / 0.15)]++;

	       //cout << "clustered, " << flush;

	       // Now determine whether to pair them or not using energy differences
	    
	       // Find parton object energies
	       double eip = pip.e();  // total energy of i^th parton cluster
	       double ejp = pjp.e();  // total energy of j^th parton cluster
	 
               // Find reco object energies
//MRENNA: is this correct?    
               double eir = 0; // total energy of i^th reco cluster 
               double ejr = 0; // total energy of j^th reco cluster
               size_t iSize = recoObjectsList[i].size();
               for ( size_t k = 0; k < iSize; k++ ) {
                  eir += recoObjectsList[i][k].getFourVector().e();
                  //   if( iSize>1 ) cout << "iSize " << k << " " << eir << endl;
               }
//                  eir = recoObjectsList[i][k].getFourVector().e();
               size_t jSize = recoObjectsList[j].size();
               for ( size_t k = 0; k < jSize; k++ ) {
                  ejr += recoObjectsList[j][k].getFourVector().e();
                  //   if( jSize>1 ) cout << "jSize " << k << " " << ejr << endl;
               }
//                  ejr = recoObjectsList[j][k].getFourVector().e();
	   
	       // q1 is how good the Parton[i]-> Reco[i] and Parton[j] -> Reco[j]
	       // make sense individually. Large q1 means don't make sense.
	       double q1 = Math::addInQuadrature( ( eip - eir ) / sqrt( eip ),
                                              ( ejp - ejr ) / sqrt( ejp ) ) / sqrt( 2. );
	    
	       // q2 is how good Parton[i] + Parton[j] -> Reco[i] + Reco[j]
	       // makes sense (together). Large q2 means _does_ make sense.
	       double q2 = fabs( ( eip + ejp ) - ( eir + ejr ) ) / sqrt( eip + ejp );
	    
	   /*    double howMuchWeLikeToKeepSinglets = 2;
	       if ( getenv("TurboSimKeepSinglets") != NULL )
		  howMuchWeLikeToKeepSinglets = 
		     atof( getenv("TurboSimKeepSinglets") ); */

	       //cout << "pairing, " << flush;
	       
	       if ( ( q1 > howMuchWeLikeToKeepSinglets ) && // if Pi -> Ri and Pj -> Rj don't make sense individually
                ( q1 > howMuchWeLikeToKeepSinglets * q2)) {  // AND Pi + Pj -> Ri + Rj makes more sense
              // then pair objects
		  
              partonObjectsList[i].insert(partonObjectsList[i].end(), 
                                          partonObjectsList[j].begin(), 
                                          partonObjectsList[j].end());
              partonObjectsList.erase(partonObjectsList.begin()+j);
              recoObjectsList[i].insert(recoObjectsList[i].end(), 
                                        recoObjectsList[j].begin(), 
                                        recoObjectsList[j].end());
              recoObjectsList.erase(recoObjectsList.begin()+j);
		  
              clustersInThisAngleRange_paired[s][int(separation / 0.15)]++;
              break;
		  
		  //cout << "paired. ";
		  
	       } // end if ( to pair objects or not ) 
	    } // end if ( to cluster objects or not )
	 } // end if ( both objects are singlets )
      } // end foreach ( parton object pair: i, j )
   return;
}
