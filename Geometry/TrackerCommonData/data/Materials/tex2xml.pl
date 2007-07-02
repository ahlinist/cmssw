#!/usr/local/bin/perl

## Usage:
## cat yourtexfile | ./tex2xml > newmaterials.xml  

while (<>)
{
    if(/ Norm. mixture density \[g\/cm\$\^3\$] &\s* ([\d\.]+)\s*/ )
  {
   $density=$1;
    print "\n";
    print "  <CompositeMaterial name=\"$material\" density=\"$density*g/cm3\" symbol=\" \" method=\"mixture by weight\">\n";
    $i=0;
    $tot=0;
     while($i<$ncomp)
    {
     print "    <MaterialFraction fraction=\"".$compsperc[$i]."\">\n";
     print "      <rMaterial name=\"materials:".$compsname[$i]."\"/>\n";
     print "    </MaterialFraction>\n";
     $tot+=$compsperc[$i];
     $i++;
   }  
    print "  </CompositeMaterial>\n";
    print STDERR "Total fraction is: ".$tot."\n"; 
    if(abs($tot-1) > 0.001 ) 
    {
     die "ERROR: total fraction is ".$tot; 
    }
   $flushed=1; 
 }

if(/Material name:\s*([\w\\]+)/) #new mixture
    // OLD:    if(/Material name: ([\w\\]+)/) #new mixture
  {
    $flushed=0;
    @compsname = ();
    @compsperc = ();
    $ncomp =0;
    $material = $1; 
    $material =~ s/\\//g;
    #print "New material: ".$material."\n";
    $inmaterial=1;
  }
  if(/^(\s*[0-9]+\s*)&(.*)&(.*)&(.*)&(.*)&(.*)&(.*)&(.*)&(.*)&(.*)&(.*)&(.*)/)
  {
    $compsname[$ncomp] = $3;
    $compsperc[$ncomp] = $7;
    $compsperc[$ncomp] =~ s/\s*//g;
    $compsperc[$ncomp] /= 100;
    $compsname[$ncomp] =~ s/^\s*//;
    $compsname[$ncomp] =~ s/\s*$//;
    $compsname[$ncomp] =~ s/\\//g;
     $ncomp++;
 }
  if(/\\end{tabular}/)
  { 
   $inmaterial=0;
   $density=-1;
  }
 

}
