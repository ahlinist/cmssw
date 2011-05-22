BASE=${1:-..}
ln -s $BASE/makeVGammaSkim_cfg.py
ln -s $BASE/makeVGammaSkim_crab.cfg
ln -s $BASE/dimuonSkim_glite_multicrab.cfg
ln -s $BASE/dimuonSkim_condor_multicrab.cfg
ln -s $BASE/../../scripts/makeMulticrabConfig.py
for f in $BASE/options_*.cfg; do
    ln -s $f
done
