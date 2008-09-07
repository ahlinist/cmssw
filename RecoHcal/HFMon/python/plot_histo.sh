#!/bin/sh
#
echo Run number?
read RUNN
FILEN="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_HCAL/data/HFMon/Run$RUNN"
HTMLN="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_HCAL/data/HFMon/Run$RUNN/index.html"
if [ -d $FILEN ];
then
echo $FILEN output folder exists - continue
else
echo $FILEN output folder does not exist. 
echo Creating $FILEN
mkdir $FILEN
fi
if [ -f $HTMLN ];
then
echo $HTMLN exists - continue
else
echo $HTMLN does not exist
echo Creating $HTMLN
touch $HTMLN
fi
########
cat > $HTMLN <<EOF

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>  
  <meta content="text/html; charset=ISO-8859-1"  
 http-equiv="content-type">  
</head>
<style type="text/css"> td { font-weight: bold } </style>
<body>
<br>
<hr>
<h2><strong>RUN $RUNN &nbsp;&nbsp;&nbsp;</strong></h2>
<hr>
<tr align="left">
<td>&nbsp;&nbsp;&nbsp;<a name="HF Monitor"><h1>HF Monitor</h1></td></tr>
<tr align="left">
<td><a href="Energy_Occ.gif"><img src="Energy_Occ.gif"></a></td>
<td><a href="Occ_L.gif"><img src="Occ_L.gif"></a></td>
<td><a href="Occ_S.gif"><img src="Occ_S.gif"></a></td>
<td><a href="HFCenterOfEnergy.gif"><img src="HFCenterOfEnergy.gif"></a></td>
<td><a href="Etsum_L.gif"><img src="Etsum_L.gif"></a></td>
<td><a href="Etsum_S.gif"><img src="Etsum_S.gif"></a></td>
<td><a href="Etsum_ratio.gif"><img src="Etsum_ratio.gif"></a></td>
<br>
</body>
</html>

EOF

########

#root -l histo.C
