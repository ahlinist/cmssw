
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Alignment/CommonAlignment/interface/Alignable.h"
#include "Alignment/CommonAlignmentParametrization/interface/CompositeAlignmentDerivativesExtractor.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"

#include "Alignment/CommonAlignmentParametrization/interface/CompositeAlignmentParameters.h"


//__________________________________________________________________________________________________
CompositeAlignmentParameters::
CompositeAlignmentParameters(const AlgebraicVector& par, const AlgebraicSymMatrix& cov,
			     const Components& comp) :
  AlignmentParameters(0,par,cov) ,
  theComponents(comp) 
{}


//__________________________________________________________________________________________________
CompositeAlignmentParameters::
CompositeAlignmentParameters(const AlgebraicVector& par, const AlgebraicSymMatrix& cov,
			     const Components& comp, const AlignableDetToAlignableMap& alimap,
			     const Aliposmap& aliposmap, const Alilenmap& alilenmap) :
  AlignmentParameters(0,par,cov) ,
  theComponents(comp) ,
  theAlignableDetToAlignableMap(alimap),
  theAliposmap(aliposmap),
  theAlilenmap(alilenmap)
{}


//__________________________________________________________________________________________________
CompositeAlignmentParameters::
CompositeAlignmentParameters(const DataContainer& data,
			     const Components& comp, const AlignableDetToAlignableMap& alimap,
			     const Aliposmap& aliposmap, const Alilenmap& alilenmap) :
  AlignmentParameters(0,data) ,
  theComponents(comp) ,
  theAlignableDetToAlignableMap(alimap),
  theAliposmap(aliposmap),
  theAlilenmap(alilenmap)
{}


//__________________________________________________________________________________________________
CompositeAlignmentParameters::~CompositeAlignmentParameters() 
{}


//__________________________________________________________________________________________________
CompositeAlignmentParameters* 
CompositeAlignmentParameters::clone( const AlgebraicVector& par, 
				     const AlgebraicSymMatrix& cov) const
{
  CompositeAlignmentParameters* cap = 
    new CompositeAlignmentParameters(par,cov,components());

  if ( userVariables() )
    cap->setUserVariables(userVariables()->clone());

  return cap;
}


//__________________________________________________________________________________________________
CompositeAlignmentParameters* 
CompositeAlignmentParameters::cloneFromSelected( const AlgebraicVector& par, 
						 const AlgebraicSymMatrix& cov) const
{
  return clone(par,cov);
}


//__________________________________________________________________________________________________
CompositeAlignmentParameters* 
CompositeAlignmentParameters::clone( const AlgebraicVector& par, 
				     const AlgebraicSymMatrix& cov,
				     const AlignableDetToAlignableMap& alimap, 
				     const Aliposmap& aliposmap,
				     const Alilenmap& alilenmap ) const
{
  CompositeAlignmentParameters* cap = 
    new CompositeAlignmentParameters(par,cov,components(),alimap,aliposmap,alilenmap);

  if ( userVariables() )
    cap->setUserVariables(userVariables()->clone());

  return cap;
}


//__________________________________________________________________________________________________
CompositeAlignmentParameters* 
CompositeAlignmentParameters::cloneFromSelected( const AlgebraicVector& par, 
						 const AlgebraicSymMatrix& cov, 
						 const AlignableDetToAlignableMap& alimap, 
						 const Aliposmap& aliposmap,
						 const Alilenmap& alilenmap) const
{
  return clone(par,cov,alimap,aliposmap,alilenmap);
}


//__________________________________________________________________________________________________
CompositeAlignmentParameters::Components 
CompositeAlignmentParameters::components() const
{ 
  return theComponents;
}


//__________________________________________________________________________________________________
// full derivatives for a composed object
AlgebraicMatrix
CompositeAlignmentParameters::derivatives( const std::vector<TrajectoryStateOnSurface>& tsosvec,
					   const std::vector<AlignableDet*>& alidetvec ) const
{
  std::vector<AlignableDetOrUnitPtr> detOrUnits;
  this->convert(alidetvec, detOrUnits);

  return this->derivatives(tsosvec, detOrUnits);
}

AlgebraicMatrix
CompositeAlignmentParameters::derivatives( const std::vector<TrajectoryStateOnSurface>& tsosvec,
					   const std::vector<AlignableDetOrUnitPtr>& alidetvec ) const
{
  std::vector<Alignable*> alivec;
  for (std::vector<AlignableDetOrUnitPtr>::const_iterator it=alidetvec.begin(); it!=alidetvec.end(); ++it)
    alivec.push_back(alignableFromAlignableDet(*it));
  
  CompositeAlignmentDerivativesExtractor extractor(alivec,alidetvec,tsosvec);
  return extractor.derivatives();
}

//__________________________________________________________________________________________________
AlgebraicVector 
CompositeAlignmentParameters::correctionTerm( const std::vector<TrajectoryStateOnSurface>& tsosvec,
					      const std::vector<AlignableDet*>& alidetvec) const
{
  std::vector<AlignableDetOrUnitPtr> detOrUnits;
  this->convert(alidetvec, detOrUnits);

  return this->correctionTerm(tsosvec, detOrUnits);
}

//__________________________________________________________________________________________________
AlgebraicVector 
CompositeAlignmentParameters::correctionTerm( const std::vector<TrajectoryStateOnSurface>& tsosvec,
					      const std::vector<AlignableDetOrUnitPtr>& alidetvec) const
{
  std::vector<Alignable*> alivec;
  for (std::vector<AlignableDetOrUnitPtr>::const_iterator it=alidetvec.begin(); it!=alidetvec.end(); ++it )
    alivec.push_back(alignableFromAlignableDet(*it));
  
  CompositeAlignmentDerivativesExtractor extractor(alivec,alidetvec,tsosvec);
  return extractor.correctionTerm();
}
 	 
//__________________________________________________________________________________________________ 	 
// assume all are selected
AlgebraicMatrix
CompositeAlignmentParameters::selectedDerivatives( const std::vector<TrajectoryStateOnSurface>& tsosvec,
						   const std::vector<AlignableDet*>& alidetvec) const
{ 
  return derivatives(tsosvec,alidetvec);
}
//__________________________________________________________________________________________________ 	 
// assume all are selected
AlgebraicMatrix
CompositeAlignmentParameters::selectedDerivatives( const std::vector<TrajectoryStateOnSurface>& tsosvec,
						   const std::vector<AlignableDetOrUnitPtr>& alidetvec) const
{ 
  return derivatives(tsosvec,alidetvec);
}

//__________________________________________________________________________________________________ 	 
// only one (tsos,AlignableDet) as argument [for compatibility with base class]
AlgebraicMatrix 
CompositeAlignmentParameters::derivatives( const TrajectoryStateOnSurface &tsos, 
					   const AlignableDetOrUnitPtr &alidet) const
{
  std::vector<TrajectoryStateOnSurface> tsosvec;
  std::vector<AlignableDetOrUnitPtr> alidetvec;
  tsosvec.push_back(tsos);
  alidetvec.push_back(alidet);
  return derivatives(tsosvec,alidetvec);
}
 	
//__________________________________________________________________________________________________ 
// assume all are selected
AlgebraicMatrix 
CompositeAlignmentParameters::selectedDerivatives( const TrajectoryStateOnSurface &tsos, 
						   const AlignableDetOrUnitPtr &alidet ) const
{ 
  return derivatives(tsos,alidet);
}
 	 

// Derivatives ----------------------------------------------------------------
// legacy methods
// full derivatives for a composed object
AlgebraicMatrix
CompositeAlignmentParameters::derivativesLegacy( const std::vector<TrajectoryStateOnSurface> &tsosvec, 
						 const std::vector<AlignableDet*>& alidetvec ) const
{
  // sanity check: length of parameter argument vectors must be equal
  if (alidetvec.size() != tsosvec.size())
  {
    edm::LogError("BadArgument") << " Inconsistent length of argument vectors! ";
    AlgebraicMatrix selderiv(1,0);
    return selderiv;
  }

  std::vector<AlgebraicMatrix> vecderiv;
  int nparam=0;

  std::vector<TrajectoryStateOnSurface>::const_iterator itsos=tsosvec.begin();
  for( std::vector<AlignableDet*>::const_iterator it=alidetvec.begin(); 
       it!=alidetvec.end(); ++it, ++itsos ) 
  {
    AlignableDet* ad = (*it);
    Alignable* ali = alignableFromAlignableDet(ad);
    AlignmentParameters* ap = ali->alignmentParameters();
    AlgebraicMatrix thisselderiv = ap->selectedDerivatives(*itsos,ad);
    vecderiv.push_back(thisselderiv);
    nparam += thisselderiv.num_row();
  }

  int ipos=1;
  AlgebraicMatrix selderiv(nparam,2);
  for ( std::vector<AlgebraicMatrix>::const_iterator imat=vecderiv.begin();
	imat!=vecderiv.end(); ++imat ) 
  {
    AlgebraicMatrix thisselderiv=(*imat);
    int npar=thisselderiv.num_row();
    selderiv.sub(ipos,1,thisselderiv);
    ipos += npar;
  }

  return selderiv;
}


//__________________________________________________________________________________________________
// assume all are selected
AlgebraicMatrix
CompositeAlignmentParameters::selectedDerivativesLegacy( const std::vector<TrajectoryStateOnSurface> &tsosvec, 
							 const std::vector<AlignableDet*>& alidetvec ) const
{ 
  return derivativesLegacy(tsosvec,alidetvec);
}


//__________________________________________________________________________________________________
// only one (tsos,AlignableDet) as argument [for compatibility with base class]
AlgebraicMatrix 
CompositeAlignmentParameters::derivativesLegacy( const TrajectoryStateOnSurface& tsos, 
						 AlignableDet* alidet ) const
{
  std::vector<TrajectoryStateOnSurface> tsosvec;
  std::vector<AlignableDet*> alidetvec;
  tsosvec.push_back(tsos);
  alidetvec.push_back(alidet);
  return derivativesLegacy(tsosvec,alidetvec);

}


//__________________________________________________________________________________________________
// assume all are selected
AlgebraicMatrix 
CompositeAlignmentParameters::selectedDerivativesLegacy( const TrajectoryStateOnSurface& tsos, 
							 AlignableDet* alidet ) const
{ 
  return derivativesLegacy(tsos,alidet);
}


//__________________________________________________________________________________________________
// finds Alignable corresponding to AlignableDet
Alignable* 
CompositeAlignmentParameters::alignableFromAlignableDet(AlignableDetOrUnitPtr adet) const
{

  AlignableDetToAlignableMap::const_iterator iali =
    theAlignableDetToAlignableMap.find(adet);
  if ( iali!=theAlignableDetToAlignableMap.end() ) return (*iali).second;
  else return 0;

}


//__________________________________________________________________________________________________
AlgebraicVector
CompositeAlignmentParameters::parameterSubset( const std::vector<Alignable*>& vec ) const
{
  const unsigned int nali = vec.size();
  int ndim = 0;

  std::vector<int> posvec;
  std::vector<int> lenvec;

  posvec.reserve( nali );
  lenvec.reserve( nali );

  // iterate over input vector of alignables to determine size of result vector
  if ( !extractPositionAndLength( vec, posvec, lenvec, ndim ) ) return AlgebraicVector();

  // OK, let's do the real work now
  AlgebraicVector result( ndim );

  int resi = 0;
  for ( unsigned int iali = 0; iali < nali; ++iali )
  {
    int posi = posvec[iali];
    int leni = lenvec[iali];

    for ( int ir = 0; ir < leni; ++ir )
      result[resi+ir] = theData->parameters()[posi-1+ir];

    resi += leni;
  }

  return result;
}


//__________________________________________________________________________________________________
// extract covariance matrix for a subset of alignables
AlgebraicSymMatrix 
CompositeAlignmentParameters::covarianceSubset( const std::vector<Alignable*>& vec ) const
{
  const unsigned int nali = vec.size();
  int ndim = 0;

  std::vector<int> posvec;
  std::vector<int> lenvec;

  posvec.reserve( nali );
  lenvec.reserve( nali );

  // iterate over input vectors of alignables
  // to determine dimensions of result matrix
  if ( !extractPositionAndLength( vec, posvec, lenvec, ndim ) ) return AlgebraicSymMatrix();

  // OK, let's do the real work now
  AlgebraicSymMatrix result( ndim );

  int resi = 0;
  for ( unsigned int iali = 0; iali < nali; ++iali )
  {
    int posi = posvec[iali];
    int leni = lenvec[iali];

    int resj = 0;
    for ( unsigned int jali = 0; jali <= iali; ++jali )
    {   
      int posj = posvec[jali];
      int lenj = lenvec[jali];

      for ( int ir = 0; ir < leni; ++ir )
	for ( int ic = 0; ic < lenj; ++ic )
	  result[resi+ir][resj+ic] = theData->covariance()[posi-1+ir][posj-1+ic];

      resj += lenj;
    }
    resi += leni;
  }

  return result;
}


//__________________________________________________________________________________________________
// extract covariance matrix elements between two subsets of alignables
AlgebraicMatrix 
CompositeAlignmentParameters::covarianceSubset( const std::vector<Alignable*>& veci, 
						const std::vector<Alignable*>& vecj ) const
{
  int ndimi=0;
  int ndimj=0;

  std::vector<int> posveci;
  std::vector<int> lenveci;
  std::vector<int> posvecj;
  std::vector<int> lenvecj;

  posveci.reserve( veci.size() );
  lenveci.reserve( veci.size() );
  posvecj.reserve( vecj.size() );
  lenvecj.reserve( vecj.size() );

  // iterate over input vectors of alignables
  // to determine dimensions of result matrix
  if ( !extractPositionAndLength( veci, posveci, lenveci, ndimi ) ) return AlgebraicSymMatrix();
  // vector vecj
  if ( !extractPositionAndLength( vecj, posvecj, lenvecj, ndimj ) ) return AlgebraicSymMatrix();

  // OK, let's do the real work now
  AlgebraicMatrix result( ndimi, ndimj );

  int resi = 0;
  for ( unsigned int iali = 0; iali < veci.size(); ++iali )
  {
    int posi = posveci[iali];
    int leni = lenveci[iali];

    int resj = 0;
    for ( unsigned int jali = 0; jali < vecj.size(); ++jali )
    {   
      int posj = posvecj[jali];
      int lenj = lenvecj[jali];

      for ( int ir = 0; ir < leni; ++ir )
	for ( int ic = 0; ic < lenj; ++ic )
	  result[resi+ir][resj+ic] = theData->covariance()[posi-1+ir][posj-1+ic];

      resj += lenj;
    }
    resi += leni;
  }

  return result;
}


//__________________________________________________________________________________________________
// Extract position and length of parameters for a subset of Alignables.
bool
CompositeAlignmentParameters::extractPositionAndLength( const std::vector<Alignable*>& alignables,
							std::vector<int>& posvec,
							std::vector<int>& lenvec,
							int& length ) const
{
  length = 0;

  for ( std::vector<Alignable*>::const_iterator it = alignables.begin(); it != alignables.end(); ++it ) 
  {
    // check if in components 
    if ( std::find( theComponents.begin(), theComponents.end(), *it ) == theComponents.end() ) 
    {
      edm::LogError( "NotFound" ) << "@SUB=CompositeAlignmentParameters::extractPositionAndLength"
				  << "Alignable not found in components!";
      return false;
    }

    // get pos/length
    Aliposmap::const_iterator iposmap = theAliposmap.find( *it );
    Alilenmap::const_iterator ilenmap = theAlilenmap.find( *it );
    if ( iposmap == theAliposmap.end() || ilenmap == theAlilenmap.end() ) 
    {
      edm::LogError( "NotFound" ) << "@SUB=CompositeAlignmentParameters::extractPositionAndLength"
				  << "position/length not found for Alignable in maps!";
      return false;
    }
    posvec.push_back( iposmap->second );
    lenvec.push_back( ilenmap->second );
    length += ilenmap->second;
  }

  return true;
}

//__________________________________________________________________________________________________
void CompositeAlignmentParameters::convert(const std::vector<AlignableDet*> &input,
					   std::vector<AlignableDetOrUnitPtr> &output) const
{
  output.clear();
  output.reserve(input.size());
  //   Does not compile due to strange const-conversion problems, but why?
  //   for (std::vector<AlignableDet*>::iterator it = input.begin(), iEnd = input.end();
  //        it != iEnd; ++it) {
  //     output.push_back(AlignableDetOrUnitPtr(*it));
  //   }
  for (unsigned int i = 0; i < input.size(); ++i) {
    output.push_back(AlignableDetOrUnitPtr(input[i]));
  }
}
