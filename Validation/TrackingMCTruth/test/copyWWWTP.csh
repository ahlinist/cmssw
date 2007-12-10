#! /bin/csh
setenv RELEASE $CMSSW_VERSION

if ( ! -d /afs/cern.ch/cms/performance/tracker/activities/validation/$RELEASE/ ) mkdir /afs/cern.ch/cms/performance/tracker/activities/validation/$RELEASE/

setenv WWWDIRObj /afs/cern.ch/cms/performance/tracker/activities/validation/$RELEASE/TrackingParticles

if (! -d $WWWDIRObj) mkdir $WWWDIRObj

setenv WWWDIR /afs/cern.ch/cms/performance/tracker/activities/validation/$RELEASE/TrackingParticles

mkdir $WWWDIR/eps

mkdir $WWWDIR/gif
echo "...Copying..."

mv *.eps $WWWDIR/eps

mv *.gif $WWWDIR/gif

echo "...Done..."
