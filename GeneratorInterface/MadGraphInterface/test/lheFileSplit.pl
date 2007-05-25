#!/usr/bin/perl
#>dkcira, 2007.03.30: splits a LHE (MadGraph) input_lhe_file in smaller lhe files with split_size number of events in them

# read in parameters
my $nrparams = $#ARGV + 1;
if( "$nrparams" eq "3"){
  $input_lhe_file = @ARGV[ 0 ]; $split_size = @ARGV[ 1 ]; $split_directory=@ARGV[ 2 ];
}elsif( "$nrparams" eq "2"){
  $input_lhe_file = @ARGV[ 0 ]; $split_size = @ARGV[ 1 ]; $split_directory="split_files";
}else{
  print "\nUsage: $0 input_lhe_file split_size\n";
  exit 1;
}

print "$0 input_lhe_file=$input_lhe_file split_size=$split_size\n";
# check if output directory exists
if( -d "$split_directory"){
  print "Producing split files in existing directory: $split_directory\n";
}else{
  # create the output directory
  if (mkdir ("$split_directory", 0777)){
    print "Created output directory: $split_directory\n";
  } else {
    print "Stopping here. Output directory could not be created: $split_directory\n";
    exit 1;
  }
}
$split_file_stem=$input_lhe_file; $split_file_stem=~s/\.lhe//; # derive stem from the input file name

# chop head, end of  file, and events part
@header={}; @events={}; $endoffile="<\/LesHouchesEvents>";
open(FIL1,$input_lhe_file) || die "can't open file $input_lhe_file: $!\n";
while(<FIL1>){
  if (/<LesHouchesEvents version=/ .. /<\/init>/){ push(@header,$_)}
  if (/<event>/ .. /<\/event>/){
    if(/<event>/){ @theeventlines={}; }
    push(@theeventlines,$_);
    if(/<\/event>/){
      shift(@theeventlines); # remove HASH... element, could be machine specific
      $stringtheeventlines=join("",@theeventlines); push(@events,$stringtheeventlines);
    }
  }
}
close (FIL1);
shift(@header); # remove HASH... element, could be machine specific
shift(@events); # remove HASH... element, could be machine specific
print "header size = $#header\n";
print "events size = $#events\n";

# create the splitted files
$filecounter=0;
@split_events={};
foreach $theevent (@events){
  ++$eventcounter;
  push(@split_events,"$theevent");
  if($eventcounter == $split_size ){
    ++$filecounter;
    $split_filename="$split_directory/${split_file_stem}_split${filecounter}_size${split_size}.lhe";
    open(SPLIT,"> $split_filename") || die "can't open file $split_filename: $!\n";
    $stringheader = join("",@header); printf SPLIT "$stringheader";
    shift(@split_events); # remove HASH... element, could be machine specific
    $stringsplit_events = join("",@split_events); printf SPLIT "$stringsplit_events";
    printf SPLIT "$endoffile";
    close (SPLIT);
    print "$split_filename\n";
    $eventcounter=0;
    @split_events={}
  }
}

# create a smaller split file with the remainder of events
if($eventcounter != 0){
  ++$filecounter;
  $split_filename="$split_directory/${split_file_stem}_split${filecounter}_size${eventcounter}.lhe";
  open(SPLIT,"> $split_filename") || die "can't open file $split_filename: $!\n";
  $stringheader = join("",@header); printf SPLIT "$stringheader";
  shift(@split_events); # remove HASH... element, could be machine specific
  $stringsplit_events = join("",@split_events); printf SPLIT "$stringsplit_events";
  printf SPLIT "$endoffile";
  close (SPLIT);
  print "$split_filename\n";
}

exit 0;

