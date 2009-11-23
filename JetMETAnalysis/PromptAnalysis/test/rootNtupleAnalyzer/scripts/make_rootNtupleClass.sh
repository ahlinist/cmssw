#!/bin/sh

usage ()
{
        echo ""
        echo "Usage:   $0 -d directory -t TTreeName "
        echo "where:   directory is the location of the *.root files to be analyzed"
        echo "Example: $0 -d /home/data/santanas/output -t treeName/tree"
        echo ""
        echo "      Note: the option \"-d directory\" can be replaced by \"-f filename\" in case of a single root file to be analyzed"
        echo "            Example: $0 -f data/input/testTree.root -t promptanaTree/tree"
        echo ""
        exit 1;
}

if [ $# -le 3 ]; then usage; fi;
while [ $# -gt 0 ]; # till there are parameters
do
  case "$1" in
    -f) FILENAME="$2"; shift ;;
    -d) DIRNAME="$2"; shift ;;
    -t) TTREENAME="$2"; shift ;;
    *) usage ;;
  esac
  shift  # get following parameters
done

if [ ! -z "${FILENAME}" ] && [ ! -z "${DIRNAME}" ] ; then
  usage;
  exit;
fi

cd `dirname $0`/../ ; # go to the directory rootNtupleAnalyzer/

if [ ! -z "${FILENAME}" ] ; then
  FILES=${FILENAME}
elif [ ! -z "${DIRNAME}" ] ; then
  FILES=`ls ${DIRNAME}/*.root`
fi

cat > temporaryMacro.C <<EOF
{
  TChain c("$TTREENAME");
EOF

for FILE in $FILES
do
  echo "  c.Add(\"${FILE}\"); " >> temporaryMacro.C
done

cat >> temporaryMacro.C <<EOF
  c.MakeClass("rootNtupleClass");
}
EOF

root -l -q temporaryMacro.C

rm temporaryMacro.C
if [ -f "rootNtupleClass.h" ] && [ -f "rootNtupleClass.C" ]; then
    echo "Moving rootNtupleClass.h/C to ./include/ and ./src/ directories ..."
    mv -i rootNtupleClass.h include/
    mv -i rootNtupleClass.C src/
    #if [ -f "include/rootNtupleClass.h" ] && [ -f "src/rootNtupleClass.C" ]; then echo "... done."; fi;

    #echo "Creating src/analysisClass.C ..."
    #cp -i src/analysisClass_template.C src/analysisClass.C

    echo "done";    
else
    echo "Error: files rootNtupleClass.h/C have not been created."
fi







