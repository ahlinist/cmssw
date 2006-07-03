#!/bin/bash


pool_build_object_relational_mapping -f blobmappingfile_noise.xml    -d CondFormatsSiStripObjects -c sqlite_file:/tmp/testSiStripNoisesBuilder_blob.db	        -u me -p mypass  
pool_build_object_relational_mapping -f blobmappingfile_pedestal.xml -d CondFormatsSiStripObjects -c sqlite_file:/tmp/testSiStripPedestalsBuilder_blob.db	-u me -p mypass    
pool_build_object_relational_mapping -f blobmappingfile_pednoise.xml -d CondFormatsSiStripObjects -c sqlite_file:/tmp/testSiStripPedestalsNoisesBuilder_blob.db -u me -p mypass  

