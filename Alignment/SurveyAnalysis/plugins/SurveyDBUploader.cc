#include "Alignment/CommonAlignment/interface/Alignable.h"
#include "Alignment/CommonAlignment/interface/SurveyDet.h"
#include "Alignment/SurveyAnalysis/interface/SurveyInputBase.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/Alignment/interface/Alignments.h"
#include "CondFormats/Alignment/interface/SurveyErrors.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "Alignment/SurveyAnalysis/plugins/SurveyDBUploader.h"

SurveyDBUploader::SurveyDBUploader(const edm::ParameterSet& cfg):
  theValueRcd( cfg.getParameter<std::string>("valueRcd") ),
  theErrorRcd( cfg.getParameter<std::string>("errorRcd") ),
  theValues(0),
  theErrors(0)
{
}

void SurveyDBUploader::endJob()
{
  theValues = new SurveyValues;
  theErrors = new SurveyErrors;

  theValues->m_align.reserve(65536);
  theErrors->m_surveyErrors.reserve(65536);

  getSurveyInfo( SurveyInputBase::detector() );

  edm::Service<cond::service::PoolDBOutputService> poolDbService;

  if( poolDbService.isAvailable() )
  {
    if ( poolDbService->isNewTagRequest(theValueRcd) )
      poolDbService->createNewIOV<SurveyValues>
	(theValues, poolDbService->endOfTime(), theValueRcd);
    else
      poolDbService->appendSinceTime<SurveyValues>
	(theValues, poolDbService->currentTime(), theValueRcd);

    if ( poolDbService->isNewTagRequest(theErrorRcd) )
      poolDbService->createNewIOV<SurveyErrors>
	(theErrors, poolDbService->endOfTime(), theErrorRcd);
    else
      poolDbService->appendSinceTime<SurveyErrors>
	(theErrors, poolDbService->currentTime(), theErrorRcd);
  }
  else
    throw cms::Exception("ConfigError")
      << "PoolDBOutputService is not available";
}

void SurveyDBUploader::getSurveyInfo(const Alignable* ali)
{
  const std::vector<Alignable*>& comp = ali->components();

  unsigned int nComp = comp.size();

  for (unsigned int i = 0; i < nComp; ++i) getSurveyInfo(comp[i]);

  const SurveyDet* survey = ali->survey();

  const align::PositionType& pos = survey->position();
  const align::RotationType& rot = survey->rotation();

  SurveyValue value( CLHEP::Hep3Vector( pos.x(), pos.y(), pos.z() ),
		     CLHEP::HepRotation
		     ( CLHEP::HepRep3x3( rot.xx(), rot.xy(), rot.xz(),
					 rot.yx(), rot.yy(), rot.yz(),
					 rot.zx(), rot.zy(), rot.zz() ) ),
		     ali->id() );

  SurveyError error( ali->alignableObjectId(),
		     ali->id(),
		     survey->errors() );

  theValues->m_align.push_back(value);
  theErrors->m_surveyErrors.push_back(error);
}

// Plug in to framework

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SurveyDBUploader);
