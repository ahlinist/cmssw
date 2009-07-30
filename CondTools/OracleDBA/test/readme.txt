templateForDropbox.txt is a template for the meta data file to be used when exporting payloads into the ORCON/ORCOFF DBs.
Suppose you want to export some payloads, in the tag <tagname> stored in an sqlite file (*.db), into the ORCON DB. So, what you have to do is:
1. rename the sqlite file into <tagname>.db;
2. download the template meta data file;
3. rename it <tagname>.txt;
4. modify it according to this structure:

destDB <connectionstring>
inputtag <inputtagname>
tag <tagname>
since <begin>
till <end>
usertext <userlog>

   where
	<connectionstring> is the connection string,
	<inputtagname> is the name of the tag in the sqlite file you want to export (leave blank if identical to tagname),
	<tagname> is the name of the tag to be exported,
	<begin> is the begin time (first since), optional (leave blank if not needed),
	<end> is the end time (last till), optional (leave blank if not needed), 
	<userlog> is the text string you want to add in the exportation log in the POPCONLOG database account. 
5. generate a time-based UUID (unique string identifier), based on the system clock plus the system's ethernet hardware address, if present, and save it as a shell variable:
   (csh, tcsh)> set u=`uuidgen -t`
   (bash)$ u=`uuidgen -t`
6. rename the sqlite and metadata files by "glueing" the unique string, preceded by an @:
   mv <tagname>.db <tagname>@${u}.db
   mv <tagname>.txt <tagname>@${u}.txt 
7. upload the sqlite file <tagname>@${u}.db into the dropbox available for your subdetector;
8. upload the meta data file <tagname>@${u}.txt into the same dropbox.

Before putting the files into the dropbox check that both sqlite and meta data files have the same name, with the leading part (i.e. the part BEFORE the @) identical to the name of the tag you want to export, <tagname>, otherwise the payloads will not be exported and an error will be returned by the shell.

renameFiles.sh is a bash script that renames automatically both the sqlite and the metadata files according to the dropbox rules. Its usage is the following:
./renameFiles.sh <sqlitefile> <metadatafile> <tagname>

where 
      <sqlitefile> is the name of the sqlite file (.db)
      <metadatafile> is the name of the metadata file (.txt)
      <tagname> is the name of the tag to be exported by the dropbox.
