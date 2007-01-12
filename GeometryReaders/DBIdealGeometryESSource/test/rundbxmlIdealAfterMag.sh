echo start
date
cd workarea
cd db
rm testIdeal.db
touch testIdeal.db
rm trbdb.out
rm twIdeal.out
rm *.log.xml
rm *.log
rm dumpSpecsIdealWrite
rm dumpGeoHistoryIdealWrite
rm dumpGeoHistoryDBBothRead
rm dumpSpecsDBBothRead
rm trbdb.out

echo start write Ideal
date
cmsRun ../../testwriteIdeal.cfg >twIdeal.out
echo end write Ideal
date

mv dumpGeoHistory dumpGeoHistoryIdealWrite
mv dumpSpecs dumpSpecsIdealWrite
echo done with all DB writes.

echo start all DB reads.
echo start DB read both
date
cmsRun ../../testreadBothDB.cfg > trbdb.out
echo done with read DB both
date

mv dumpGeoHistory dumpGeoHistoryDBBothRead
mv dumpSpecs dumpSpecsDBBothRead

echo end all DB reads
date
cd ../xml
rm trbxml.out
rm dumpGeoHistoryXMLBothRead
rm dumpSpecsXMLBothRead
rm diffgeomBoth.out

echo start XML read both
date
cmsRun ../../testreadBothXML.cfg > trbxml.out
echo end XML read both
date

mv dumpGeoHistory dumpGeoHistoryXMLBothRead
mv dumpSpecs dumpSpecsXMLBothRead
echo done with reading XML

echo doing seds to replace -0 with 0.
date
sed -i '{s/-0.0000/ 0.0000/g}' dumpGeoHistoryXMLBothRead
cd ../db
sed -i '{s/-0.0000/ 0.0000/g}' dumpGeoHistoryDBBothRead
cd ../xml

date
echo this will show if there are any inconsistencies when reading both Ideal and Magnet
diff dumpGeoHistoryXMLBothRead ../db/dumpGeoHistoryDBBothRead > diffgeomBoth.out

echo ALL DONE!


