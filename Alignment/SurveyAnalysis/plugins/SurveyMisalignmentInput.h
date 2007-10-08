#ifndef Alignment_SurveyAnalysis_SurveyMisalignmentInput_h
#define Alignment_SurveyAnalysis_SurveyMisalignmentInput_h

/** \class SurveyMisalignmentInput
 *
 *  Class to misaligned tracker from DB.
 *
 *  $Date: 2007/07/10 19:15:19 $
 *  $Revision: 1.2 $
 *  \author Chung Khim Lae
 */
// user include files

#include "Alignment/SurveyAnalysis/interface/SurveyInputBase.h"
#include "Alignment/SurveyAnalysis/interface/SurveyInputTextReader.h"
#include "FWCore/Framework/interface/ESHandle.h"

class AlignableSurface;
class Alignments;

class SurveyMisalignmentInput:
  public SurveyInputBase
{
public:
	
  SurveyMisalignmentInput(
			  const edm::ParameterSet&
			  );
	
  /// Read ideal tracker geometry from DB
  virtual void beginJob(
			const edm::EventSetup&
			);
	
private:
	
  SurveyInputTextReader::MapType uIdMap;

  std::string textFileName;

  edm::ESHandle<Alignments> alignments;
	
  /// Add survey info to an alignable
  void addSurveyInfo(Alignable*);

  /// Get alignable surface from misalignments.db
  AlignableSurface getAlignableSurface(align::ID);
	
};

#endif
