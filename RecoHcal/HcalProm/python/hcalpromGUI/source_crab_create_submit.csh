#first do:
#ssh lxplus
#bsub -Is -q cmsinter xterm &
#cmsenv
source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.csh
setenv STAGE_SVCCLASS cmscaf
crab -create
crab -submit -c
bjobs
