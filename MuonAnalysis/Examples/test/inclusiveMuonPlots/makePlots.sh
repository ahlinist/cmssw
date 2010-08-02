#!/bin/bash

## Input files
DATA="inclusiveMuonPlots_Data.root"
MC="inclusiveMuonPlots_MC.class.root"

## Output directory
DIR=plots

#### Luminosities
LUMI_DATA=0.4698774
LUMI_MC=0.75779186535764376 # 54M sample
# resulting scale factors
SCALE=$(echo ${LUMI_DATA}/${LUMI_MC} | bc -l)

#### Plot selection
PLOT_LIST="all"   # "all", "pas", "dp"
DO_RELNORM=1
DO_LUMINORM=1
DO_BARREL_ENDCAPS=1

#### Plot options
# Stacks and colors for background
COMP=" --composite='Punch(1),Ghost(8),Light(207),Heavy(67)' "
# Define some other options
OPTIONS=""
OPTIONS="${OPTIONS} --overflow"     ## merge overflow bin into first and last visible bins
OPTIONS="${OPTIONS} --ratio"        ## make ratio plot
OPTIONS="${OPTIONS} --rebin-ratio"  ## rebin low-statistics regions of ratio plots
OPTIONS="${OPTIONS} --legend=legend.txt" ## define legend
OPTIONS="${OPTIONS} --pdf"          ## produce pdf files
# CMS publication spam 
SPAM="--prelim=#splitline{CMS Preliminary}{#sqrt{s} = 7 TeV}"


##### Normalization for the N(muons)plot
# need to reconstruct what's the number of minbias events
# effective cross sections of the 'good collision' selection
XS_DATA=49.17 #mb
XS_MC=49.07   #mb
# with some (arbitrary) uncertainty
XS_ERR_DATA=$(echo "${XS_DATA} * 0.02" | bc -l) # relative 2%
XS_ERR_MC=$(  echo "${XS_MC}   * 0.02" | bc -l) # relative 2%
# make a copy of the input files, fixing the bin 0
DATA0=${DATA/.root/_bin0.root}; MC0=${MC/.root/_bin0.root};
cp $DATA $DATA0; cp $MC $MC0;
python nMuonsBin0Fix.py $DATA0  $(echo "${XS_DATA}*${LUMI_DATA}*1000000" | bc -l) $(echo "${XS_ERR_DATA}*${LUMI_DATA}*1000000" | bc -l)  > /dev/null
python nMuonsBin0Fix.py $MC0    $(echo "${XS_MC}  *${LUMI_MC}  *1000000" | bc -l) $(echo "${XS_ERR_MC}  *${LUMI_MC}  *1000000" | bc -l)  > /dev/null
DATA=$DATA0; MC=$MC0;

#### Now we start with the real business
# check existance of directory, if necessary clean it
test -d $DIR || mkdir $DIR
rm -rf $DIR/* 2> /dev/null;

## Now we run
DO="./inclusiveMuonPlots_step2.py"
FILES="$DATA -r $MC"

## Limited plot lists
if [[ "${PLOT_LIST}" == "pas" ]]; then
    KIN=" --select qpt,eta,phi,nMuonsCorr --exclude Sta"
    OPTIONS="${OPTIONS} --pdf"
    $DO $FILES $COMP globalMuons  $KIN -n manual,$SCALE --out $DIR/globalMuons/  ${OPTIONS/--ratio/} "$SPAM";
    $DO $FILES $COMP trackerMuons $KIN -n manual,$SCALE --out $DIR/trackerMuons/ ${OPTIONS/--ratio/} "$SPAM";
    ALL="$KIN --select tip,tipSig,globalHits,globalMuonHits,Iso.*03" 
    $DO $FILES $COMP globalMuonsEwkPt3 $ALL -n manual,$SCALE --out $DIR/tightMuons/  ${OPTIONS/--ratio/} "$SPAM";
    exit 0;
elif [[ "${PLOT_LIST}" == "dp" ]]; then
    KIN=" --select 'qpt,qp,^(p|pt|eta|phi)$,dxy,dz,nMuonsCorr' --exclude Sta"
    OPTIONS="${OPTIONS} --pdf"
    $DO $FILES $COMP globalMuons  $KIN -n manual,$SCALE --out $DIR/globalMuons/  ${OPTIONS/--ratio/} "$SPAM";
    $DO $FILES $COMP trackerMuons $KIN -n manual,$SCALE --out $DIR/trackerMuons/ ${OPTIONS/--ratio/} "$SPAM";
    ALL="$KIN --select tip,tipSig,globalHits,globalMuonHits,Iso.*03" 
    $DO $FILES $COMP globalMuonsEwkPt3 $ALL -n manual,$SCALE --out $DIR/tightMuons/  ${OPTIONS/--ratio/} "$SPAM";
    exit 0;
fi

# Full plot list but filter out some plots which are not meaningful
GLB=" --exclude-group kinSta "
TRKONLY=" --exclude-group kinSta,sta,glb "
STAONLY=" --exclude-group kin,trk,glb,iso,muid "

for M in globalMuons globalMuonsPt3 globalMuonsEwkPt3; do
    echo $M;
    if [[ "${DO_LUMINORM}" == "1" ]]; then
        $DO $FILES $COMP $GLB $M    -n manual,$SCALE --out $DIR/${M}/ ${OPTIONS};
        if [[ "${DO_BARREL_ENDCAPS}" == "1" ]]; then
            $DO $FILES $COMP $GLB ${M}B -n manual,$SCALE --out $DIR/${M}_barrel/  ${OPTIONS};
            $DO $FILES $COMP $GLB ${M}E -n manual,$SCALE --out $DIR/${M}_endcaps/ ${OPTIONS};
        fi;
    fi;
    if [[ "${DO_RELNORM}" == "1" ]]; then
        $DO $FILES $COMP $GLB $M    -n integral      --out $DIR/${M}_relnorm/  ${OPTIONS};
        if [[ "${DO_BARREL_ENDCAPS}" == "1" ]]; then
            $DO $FILES $COMP $GLB ${M}B -n integral      --out $DIR/${M}_relnorm_barrel/  ${OPTIONS};
            $DO $FILES $COMP $GLB ${M}E -n integral      --out $DIR/${M}_relnorm_endcaps/ ${OPTIONS};
        fi;
    fi;
done

for M in trackerMuons; do
    echo $M;
    if [[ "${DO_LUMINORM}" == "1" ]]; then
        $DO $FILES $COMP $TRKONLY $M    -n manual,$SCALE --out $DIR/${M}/ ${OPTIONS};
        if [[ "${DO_BARREL_ENDCAPS}" == "1" ]]; then
            $DO $FILES $COMP $TRKONLY ${M}B -n manual,$SCALE --out $DIR/${M}_barrel/  ${OPTIONS};
            $DO $FILES $COMP $TRKONLY ${M}E -n manual,$SCALE --out $DIR/${M}_endcaps/ ${OPTIONS};
        fi;
    fi;
    if [[ "${DO_RELNORM}" == "1" ]]; then
        $DO $FILES $COMP $TRKONLY $M    -n integral      --out $DIR/${M}_relnorm/  ${OPTIONS};
        if [[ "${DO_BARREL_ENDCAPS}" == "1" ]]; then
            $DO $FILES $COMP $TRKONLY ${M}B -n integral      --out $DIR/${M}_relnorm_barrel/  ${OPTIONS};
            $DO $FILES $COMP $TRKONLY ${M}E -n integral      --out $DIR/${M}_relnorm_endcaps/ ${OPTIONS};
        fi;
    fi;
done

for M in standAloneMuons standAloneMuonsVH; do
    echo $M;
    if [[ "${DO_LUMINORM}" == "1" ]]; then
        $DO $FILES $COMP $STAONLY $M -n manual,$SCALE --out $DIR/${M}/ ${OPTIONS};
        if [[ "${DO_BARREL_ENDCAPS}" == "1" ]]; then
            $DO $FILES $COMP $STAONLY ${M}B -n manual,$SCALE   --out $DIR/${M}_barrel/  ${OPTIONS};
            $DO $FILES $COMP $STAONLY ${M}E -n manual,$SCALE   --out $DIR/${M}_endcaps/ ${OPTIONS};
        fi;
    fi;
    if [[ "${DO_LUMINORM}" == "1" ]]; then
        $DO $FILES $COMP $STAONLY $M -n integral      --out $DIR/${M}_relnorm/  ${OPTIONS};
        if [[ "${DO_BARREL_ENDCAPS}" == "1" ]]; then
            $DO $FILES $COMP $STAONLY ${M}B -n integral   --out $DIR/${M}_relnorm_barrel/  ${OPTIONS};
            $DO $FILES $COMP $STAONLY ${M}E -n integral   --out $DIR/${M}_relnorm_endcaps/ ${OPTIONS};
        fi;
    fi;
done


