#!/bin/bash
numArgs=$#;
if [ $numArgs -lt 1 ]; then
    echo "Usage: local-rpm-env.sh CMSSW_X_Y_Z ";
    exit -1;
fi
export PATH=${PATH}:$PWD/EventFilter/Deployment/scripts
export SCRAM_ARCH=slc4onl_ia32_gcc346
sver=`cat  /opt/cmssw/$SCRAM_ARCH/etc/default-scramv1-version`
cat > EventFilter/Deployment/scripts/scramv1 <<EOF
/opt/cmssw/slc4onl_ia32_gcc346/lcg/SCRAMV1/$sver/bin/scram \$@
EOF
chmod +x EventFilter/Deployment/scripts/scramv1
