setenv ORACLE_CERN /afs/cern.ch/project/oracle
if ( -r $ORACLE_CERN/script/cshrc_oracle.csh ) then
    source $ORACLE_CERN/script/cshrc_oracle.csh
endif

setoraenv -s 10103
setenv LD_LIBRARY_PATH .:/afs/cern.ch/project/oracle/@sys/prod/lib:$LD_LIBRARY_PATH
