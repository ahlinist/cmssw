#!/bin/tcsh
#####################
# Excited electrons
#
setenv example excitedElectrons
cd $RELEASE_DIR/Quaero/tests/
rm tmp_$example.txt >& /dev/null
if ( $extent == "short" )
  setenv fLambdas "0.0001 0.0003 0.001"
  setenv mEstars  "100 150 200 250"
endif
if ( $extent == "long" )
  setenv fLambdas "0.0001 0.0002 0.0005 0.001"
  setenv mEstars  "100 125 150 175 200 225 250"
endif
foreach fLambda ( `echo $fLambdas` )
  foreach mEstar ( `echo $mEstars` )
    perl runTestJob_allColliders.pl $passwd "excitedElectrons" $mEstar $fLambda 0 0   | tail -1 | awk '{print $2}' >> tmp_$example.txt
  end
end
./keepTabsOnQuaeroJobs.tcsh `cat tmp_$example.txt`
source setupCDF.tcsh
python drawLogLcurve_2d.py "e\*" `head -1 tmp_$example.txt` `wc tmp_$example.txt | awk '{print $1}'` > all_${example}_logL.txt
root drawLogLcurve_2d.C
mv model_tag.eps $RELEASE_DIR/Quaero/doc/quaero_all/all_${example}_logL_2d.eps
echo "$example is done" > tmp_$$.txt; /usr/lib/sendmail knuteson@fnal.gov < tmp_$$.txt; rm tmp_$$.txt;
#####################

