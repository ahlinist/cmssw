#! /bin/bash
#---------------------------------------------------------------------
# setup environment and user parameters
#vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CHANGEREADONLY=0
NO_ARGS=0
# ------------------------------------------------------------------------
# changepassword ()
# Parameters: USER($1) CONNECT($2), OLDPASS($3), NEWPASS($4)
# Returns: 0 on success
# ------------------------------------------------------------------------
changepassword() {
 local USER=$1
 local CONNECT=$2
 local OPA=$3
 local NPA=$4
 local QUERY="ALTER USER ${USER} IDENTIFIED BY ${NPA} REPLACE ${OPA}"
 echo ${QUERY}
 #echo "Connecting ${CONNECT}"
 cat<<End-of-message | sqlplus -s /nolog
  connect ${CONNECT};
  set heading off feedback off verify off term off;
  ${QUERY};
  disconnect;
  exit;
End-of-message
 #echo "change from ${OPA}"
 #echo "to ${NPA}"
 return 0
 }
# ------------------------------------------------------------------------
# main
# ------------------------------------------------------------------------
if [ $# -eq ${NO_ARGS}  ] 
then
  echo "Usage: `basename $0` -h oracleservice -o oldpassword -n newpassword [-r]"
  exit 1
fi
while getopts ":rh:o:n:" Option
do
  case $Option in
    r ) CHANGEREADONLY=1
        echo "change password for readonly accounts option -$Option"
        ;;
    h ) ORACLESERVICE=${OPTARG}
        echo "Oracle service option -$Option : ${ORACLESERVICE}"
        ;;
    o ) OLDPASS=${OPTARG}
        echo "Old password option -$Option : ${OLDPASS}"
        ;;
    n ) NEWPASS=${OPTARG}
        echo "New password option -$Option : ${NEWPASS}"
        ;;
    * ) echo "Unimplemented option chosen";;
  esac
done 
shift $(($OPTIND -1))
#list of schema owners
OWNERS="   
GENERAL
CSC
DT
ECAL
HCAL
PIXEL
PRESH
RPC
STRIP
ALIGNMENT
HLT
" 
echo
for OWNER in ${OWNERS} ; do
   if [ ${CHANGEREADONLY} -eq 0 ]
   then
      ACCOUNT=CMS_COND_${OWNER}
   else
      ACCOUNT=CMS_COND_${OWNER}_R
   fi
   CONTACT=${ACCOUNT}/${OLDPASS}@${ORACLESERVICE}
   echo ${CONTACT}
   changepassword ${ACCOUNT} ${CONTACT} ${OLDPASS} ${NEWPASS}   
done
echo "done"
exit 0
