#include "SLHCUpgradeSimulations/L1Trigger/interface/WindowFinder.h"
#include <iostream>

cmsUpgrades::WindowFinder::WindowFinder( const cmsUpgrades::StackedTrackerGeometry *aGeometry , double aPtScalingFactor , double aIPwidth , double aRowResolution , double aColResolution )
:	mGeometry(aGeometry), 
	mPtScalingFactor( aPtScalingFactor ), 
	mIPwidth( aIPwidth ),
	mRowResolution( aRowResolution ),
	mColResolution( aColResolution ),
	mLastId(0) 
{
//		std::cout<<"mPtScalingFactor:" << mPtScalingFactor << " & mIPwidth:" << mIPwidth << std::endl;
}


cmsUpgrades::WindowFinder::~WindowFinder()
{
}	

void cmsUpgrades::WindowFinder::dumphit( const StackedTrackerDetId & anId , unsigned int hitIdentifier , const double & aInnerRow , const double & aInnerColumn )
{
	MeasurementPoint mp	( aInnerRow + (0.5*mRowResolution), aInnerColumn + (0.5*mColResolution) ); // Centre of the pixel.
	LocalPoint innerLP 	= reinterpret_cast< const PixelGeomDetUnit* > (mGeometry	-> idToDetUnit( anId , hitIdentifier ))->topology().localPosition( mp )  ;
	std::cout << innerLP << std::endl;
}


cmsUpgrades::StackedTrackerWindow cmsUpgrades::WindowFinder::getWindow( const StackedTrackerDetId & anId , const double & aInnerRow , const double & aInnerColumn )
{

	if(anId!=mLastId){
		mLastId = anId;
		mInnerDet = const_cast< PixelGeomDetUnit* >(reinterpret_cast< const PixelGeomDetUnit* > (mGeometry	-> idToDetUnit( anId , 0 )));
		mOuterDet = const_cast< PixelGeomDetUnit* >(reinterpret_cast< const PixelGeomDetUnit* > (mGeometry	-> idToDetUnit( anId , 1 )));

		mHalfPixelLength = mInnerDet->specificTopology().pitch().second * mColResolution * 0.5;
		mSeparation = mInnerDet->surface().localZ( mOuterDet->position() );
		if (mSeparation<0)mSeparation=-mSeparation;
	}

	MeasurementPoint mp	( aInnerRow + (0.5*mRowResolution), aInnerColumn + (0.5*mColResolution) ); // Centre of the "pixel".
	LocalPoint innerLP 	=  mInnerDet ->topology().localPosition( mp )  ;
	GlobalPoint innerGP =  mInnerDet ->surface().toGlobal( innerLP ) ;
//	std::cout << "inner : " << innerLP << std::endl;

// --- window size in r/phi --- //
	double phi = asin( mPtScalingFactor * innerGP.perp() );

	double sinDelta = sin(mInnerDet->position().phi() - innerGP.phi());
	double pixelAngle = acos(  sinDelta*(mInnerDet->position().perp())/innerLP.x()    );

	double deltaXplus=mSeparation*tan(pixelAngle+phi);
	double deltaXminus=mSeparation*tan(pixelAngle-phi);

// --- window size in z --- //
	double ratioplus = mSeparation/(innerGP.perp()*cos(pixelAngle-phi));
	double ratiominus = mSeparation/(innerGP.perp()*cos(pixelAngle+phi));

	double deltaZplus	=	(ratioplus * (innerGP.z()+mHalfPixelLength+mIPwidth)) +mHalfPixelLength;
	double deltaZminus	=	(ratiominus * (innerGP.z()-mHalfPixelLength-mIPwidth)) -mHalfPixelLength;

//std::cout<<"X:"<<(innerLP.x()+deltaXplus)<<" | "<<(innerLP.x()-deltaXplus)<<" | "<<(innerLP.x()+deltaXminus)<<" | "<<(innerLP.x()-deltaXminus)<<std::endl;
//std::cout<<"Z:"<<(innerLP.y()+deltaZplus)<<" | "<<(innerLP.y()-deltaZplus)<<" | "<<(innerLP.y()+deltaZminus)<<	" | "<<(innerLP.y()-deltaZminus)<<std::endl;

// --- make boundary points in the inner reference frame --- //
	LocalPoint SW_LP_I( innerLP.x()-deltaXplus , innerLP.y()+deltaZplus , -mSeparation );
	LocalPoint NE_LP_I( innerLP.x()-deltaXminus , innerLP.y()+deltaZminus , -mSeparation );
// --- migrate into the global frame --- //
	GlobalPoint SW_GP = mInnerDet ->surface().toGlobal(SW_LP_I);
	GlobalPoint NE_GP = mInnerDet ->surface().toGlobal(NE_LP_I);
// --- migrate into the local frame of the outer det--- //
	LocalPoint SW_LP_O = mOuterDet ->surface().toLocal(SW_GP);
	LocalPoint NE_LP_O = mOuterDet ->surface().toLocal(NE_GP);
// --- convert into pixel units --- //
	std::pair<float,float> SW_Pixel = mOuterDet -> specificTopology().pixel(SW_LP_O);
	std::pair<float,float> NE_Pixel = mOuterDet -> specificTopology().pixel(NE_LP_O);

	mMinrow	= mRowResolution * floor( SW_Pixel.first / mRowResolution ); 
	mMincol	= mColResolution * floor( SW_Pixel.second / mColResolution );
	mMaxrow	= mRowResolution * floor( NE_Pixel.first / mRowResolution );
	mMaxcol 	= mColResolution * floor( NE_Pixel.second / mColResolution ); 

	if(mMinrow>mMaxrow)std::swap(mMinrow,mMaxrow);
	if(mMincol>mMaxcol)std::swap(mMincol,mMaxcol);

/*

	std::cout << "( " << aInnerRow << " , " << aInnerColumn << " ) -> ( "
	<< SW_Pixel.first <<" , "<< SW_Pixel.second <<" ) & ( "<< NE_Pixel.first <<" , "<<NE_Pixel.second <<" ) -> ( "
	<< mMinrow <<" , "<< mMincol <<" ) & ( "<< mMaxrow <<" , "<< mMaxcol <<" ) " << std::endl;

	MeasurementPoint mpa	( mMinrow+0.25, mMincol+0.25 );
//	MeasurementPoint mpa	( SW_Pixel.first, SW_Pixel.second );
	GlobalPoint gpa =  mOuterDet ->surface().toGlobal( mOuterDet ->topology().localPosition( mpa )  ) ;
	double phidiffa = gpa.phi() - innerGP.phi();
	double r1a = innerGP.perp()/100;
	double r2a = gpa.perp()/100;
	double x2a = r1a*r1a + r2a*r2a - 2*r1a*r2a*cos(phidiffa);
	double pta = 0.6*sqrt(x2a)/sin(fabs(phidiffa));
	double za = innerGP.z()- (  (gpa.z()-innerGP.z())*innerGP.perp()/mSeparation);

	MeasurementPoint mpb	( mMaxrow+0.25, mMaxcol+0.25 );
//	MeasurementPoint mpb	( NE_Pixel.first, NE_Pixel.second );
	GlobalPoint gpb =  mOuterDet ->surface().toGlobal( mOuterDet ->topology().localPosition( mpb )  ) ;
	double phidiffb = gpb.phi() - innerGP.phi();
	double r1b = innerGP.perp()/100;
	double r2b = gpb.perp()/100;
	double x2b = r1b*r1b + r2b*r2b - 2*r1b*r2b*cos(phidiffb);
	double ptb = 0.6*sqrt(x2b)/sin(fabs(phidiffb));
	double zb = innerGP.z()- (  (gpb.z()-innerGP.z())*innerGP.perp()/mSeparation);

std::cout<< "pt limits = " << pta << " & " << ptb << std::endl;
std::cout<< "z limits = " << za << " & " << zb << std::endl;
*/
	return cmsUpgrades::StackedTrackerWindow::StackedTrackerWindow (  mMinrow , mMaxrow , mMincol , mMaxcol  );
}
