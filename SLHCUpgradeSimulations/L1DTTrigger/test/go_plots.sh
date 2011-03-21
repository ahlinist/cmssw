
function CheckForFile()
{
    if [ ! -f ${filename} ]
        then
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        echo "  Error: the file:"
        echo "      ${filename}"
        echo "  Does not exist"
        echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    exit 1
    fi
}


function CheckForArg()
{
    case ${WORD} in
        *[!0-9]*|"")
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo "  (additional) args must be Pt numerics"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    printf "\nUsage: %s: [-f filename] [-n number of events] args\n" $(basename $0) >&2
    echo
    exit 2
    esac
}


#-----------------------------------------------------------------------------------------
#!/bin/bash
#
if [ $# -lt 1 ]; then
    printf "\n  Usage:\n\t%s [-f filename] [-n number of events] args\n" $(basename $0) >&2
    printf "\n\tOptional file should contain a sequence of Pt values,\n"
    printf "\tthat may also be given as \"args\":"
    printf "\n\tthe latter are optional if file is given.\n\n"
    exit 1
fi
#
fflag=
filename=
nevents=-1
while getopts 'f:n:h' OPTION
  do
  case $OPTION in
      f)  fflag=1
	  filename="$OPTARG"
          #CheckForFile
	  ;;
      n)  nevents="$OPTARG"
	  ;;
      h)  printf "Usage: %s: [-f filename] [-n number of events] args\n" $(basename $0) >&2
	  exit 2
	  ;;
  esac
done

shift $(($OPTIND - 1)) # this let us shift over all parsed arguments

for WORD in "$@"
  do
  CheckForArg
done

if [ "$fflag" ]
    then
    printf "\nOption -f specified with filename = $filename\n"
    printf "Additional rguments are: %s\n" "$*"
    else
    printf "\nArguments are: %s\n" "$*"
fi
printf "Number of events to generate for each case is %s\n\n" $nevents

if [ "$fflag" ]
    then
    for entry in $(cat $filename)
      do
      sed -e s/PT/$entry/g -e s/Max/$nevents/g \
	  < dtSeededTrackletsPlots_cfg.py > ${entry}_GeV_plots_to_go_cfg.py
      echo -e "\nstarting $Pt GeV/c Pt plots" 
      cmsRun ${entry}_GeV_plots_to_go_cfg.py
      rm -f ${entry}_GeV_plots_to_go_cfg.py
    done
fi

for Pt
  do
  sed -e s/PT/$Pt/g -e s/Max/$nevents/g \
      < dtSeededTrackletsPlots_cfg.py > ${Pt}_GeV_plots_to_go_cfg.py
  echo -e "\nstarting $Pt GeV/c Pt plots" 
  cmsRun ${Pt}_GeV_plots_to_go_cfg.py
  rm -f ${Pt}_GeV_plots_to_go_cfg.py
done

echo 
