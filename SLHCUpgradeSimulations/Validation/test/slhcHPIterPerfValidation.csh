#! /bin/tcsh 

### Configuration ############################################################
set RefRelease="stdgeom_highpurity"
#set RefRelease="stdgeom"
set NewRelease="r39v28_quad31_highpurity"
#set NewRelease="r39v28_quadseeds_tag31"
set RefFile="TSVMTV_valid_histos"
set NewFile="TSVMTV_valid_histos"

# you 
set Datafiles="/uscms_data/d2/cheung/slhc/mtv/336_tag28"
#set samples=("pu00" "pu05" "pu25" "pu50")
set samples=("pu25")
#set samples=("pu00" "pu05" "pu25")
#set samples=("pu05" "pu25")
#set RefSelection="tenmuons"
#set NewSelection="tenmuons"
set RefSelection="ttbar"
set NewSelection="ttbar"
set RefRepository="/uscms_data/d2/cheung/slhc/mtv/336_tag28/results/fullsim"
set NewRepository="/uscms_data/d2/cheung/slhc/mtv/336_tag28/results/fullsim"

##############################################################################
echo "you chose option 3"
foreach sample($samples)

echo "RefFile = $Datafiles/$RefRelease/$RefSelection/$sample/$RefFile.root"
echo "NewFile = $Datafiles/$NewRelease/$NewSelection/$sample/$NewFile.root"
    cat macro/HPIterTrackValHistoPublisher.C | sed \
      -e s@NEW_FILE@$Datafiles/$NewRelease/$NewSelection/$sample/$NewFile.root@g \
      -e s@REF_FILE@$Datafiles/$RefRelease/$RefSelection/$sample/$RefFile.root@g \
      -e s@REF_LABEL@$sample@g \
      -e s@NEW_LABEL@$sample@g \
      -e s@REF_RELEASE@$RefRelease@g \
      -e s@NEW_RELEASE@$NewRelease@g \
      -e s@REFSELECTION@$RefSelection@g \
      -e s@NEWSELECTION@$NewSelection@g \
      -e s@HPIterTrackValHistoPublisher@$sample@g \
      -e s@MINEFF@0.5@g \
      -e s@MAXEFF@1.025@g \
      -e s@MAXFAKE@1.0@g \
    > ! $sample.C

    root -b -q -l $sample.C > ! macro.$sample.log
    mkdir -p $NewRepository/$NewRelease/$NewSelection/$sample

    echo "copying pdf files for sample: " $sample
    cp *.pdf $NewRepository/$NewRelease/$NewSelection/$sample

end

