/*
Name: create_config_files_all.cpp
Author: Daniele Francesco Kruse
E-mail: daniele.francesco.kruse@cern.ch
Version: 0.9 (16/02/2010)

This program takes a XML configuration file and generates the python 
configuration files to be passed to cmsRun, in order to use the PerfmonService of CMSSW
both for counting and sampling. It also generates the "*_runs.py" file responsible for
calling all the others.

compile linking it with libxerces-c (-lxerces-c)
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <map>
#include <string>

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/ContentHandler.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>

#define MAX_EVENTS_NO 4
#define MAX_EVENT_SETS 100
#define MAX_EVENT_NAME_LENGTH 200
#define ORIG_BUF_MAX_LENGTH 100000
#define FILENAME_MAX_LENGTH 1024
#define APPENDIX_MAX_LENGTH 20000
#define PROPERTY_STR_MAX_LENGTH 200
#define VALUE_STR_MAX_LENGTH 200
#define ANALYSIS_NAME_MAX_LENGTH 200
#define PROC_FAMILY_MAX_LENGTH 200

using namespace xercesc;

// gen_file()
// const char *original_buffer   : original buffer containing the configuration for cmsRun
// const char *original_filename : filename of the original python configuration file 
// const char *prefix            : destination directory and prefix for result files
// const char *eventN            : name of the N-th event to monitor
// int invN                      : inv mask of the N-th counter
// unsigned int cmaskN           : counter mask of the N-th counter
// unsigned int spN              : sampling period of the N-th counter (use 0 if counting)
// unsigned int start_at         : specifies at which event number PerfmonService should start monitoring
// char *generated_filename      : name of generated python configuration file
// const char *family            : processor family (CORE or NEHALEM or WESTMERE)
// generates the python configuration file, containing the original part ("original_buffer") plus the parameters to be used by the PerfmonService of CMSSW
int gen_file(const char *original_buffer, const char *original_filename, const char *prefix,
             const char *event0, int inv0, unsigned int cmask0, unsigned int sp0,
             const char *event1, int inv1, unsigned int cmask1, unsigned int sp1,
             const char *event2, int inv2, unsigned int cmask2, unsigned int sp2,
             const char *event3, int inv3, unsigned int cmask3, unsigned int sp3,
             unsigned int start_at, char *generated_filename, const char *family)
{
 char to_append[APPENDIX_MAX_LENGTH];
 bzero(to_append, APPENDIX_MAX_LENGTH);
 char num_str[4];
 char filename[FILENAME_MAX_LENGTH];
 bzero(filename, FILENAME_MAX_LENGTH);
 int sampling = 0;
 if(sp0>0 || sp1>0 || sp2>0 || sp3>0) sampling = 1;
 strcpy(to_append, "\n\nprocess.PerfMonService=cms.Service(\"PerfMonService\",\n\tEVENT0=cms.untracked.string(\"");
 strcat(to_append, event0);
 strcat(to_append, "\"),\n\tEVENT1=cms.untracked.string(\"");
 strcat(to_append, event1);
 strcat(to_append, "\"),\n\tEVENT2=cms.untracked.string(\"");
 strcat(to_append, event2);
 strcat(to_append, "\"),\n\tEVENT3=cms.untracked.string(\"");
 strcat(to_append, event3);
 strcat(to_append, "\"),\n\tPREFIX=cms.untracked.string(\"");
 strcat(to_append, prefix);
 if(prefix[strlen(prefix)-1]!='/') strcat(to_append, "/");
 sampling ? strcat(to_append, "_S\"),") : strcat(to_append, "_C\"),");
 strcat(to_append, "\n\tINV0=cms.untracked.bool(");
 inv0 ? strcat(to_append, "True") : strcat(to_append, "False");
 strcat(to_append, "),\n\tINV1=cms.untracked.bool(");
 inv1 ? strcat(to_append, "True") : strcat(to_append, "False");
 strcat(to_append, "),\n\tINV2=cms.untracked.bool(");
 inv2 ? strcat(to_append, "True") : strcat(to_append, "False");
 strcat(to_append, "),\n\tINV3=cms.untracked.bool(");
 inv3 ? strcat(to_append, "True") : strcat(to_append, "False");
 strcat(to_append, "),\n\tFAMILY=cms.untracked.string(\"");
 strcat(to_append, family);
 strcat(to_append, "\"),\n\tCMASK0=cms.untracked.int32(");
 sprintf(num_str, "%d", cmask0);
 strcat(to_append, num_str); 
 strcat(to_append, "),\n\tCMASK1=cms.untracked.int32(");
 sprintf(num_str, "%d", cmask1);
 strcat(to_append, num_str); 
 strcat(to_append, "),\n\tCMASK2=cms.untracked.int32(");
 sprintf(num_str, "%d", cmask2);
 strcat(to_append, num_str); 
 strcat(to_append, "),\n\tCMASK3=cms.untracked.int32(");
 sprintf(num_str, "%d", cmask3);
 strcat(to_append, num_str); 
 strcat(to_append, "),\n\tSP0=cms.untracked.int32(");
 sprintf(num_str, "%d", sp0);
 strcat(to_append, num_str); 
 strcat(to_append, "),\n\tSP1=cms.untracked.int32(");
 sprintf(num_str, "%d", sp1);
 strcat(to_append, num_str); 
 strcat(to_append, "),\n\tSP2=cms.untracked.int32(");
 sprintf(num_str, "%d", sp2);
 strcat(to_append, num_str); 
 strcat(to_append, "),\n\tSP3=cms.untracked.int32(");
 sprintf(num_str, "%d", sp3);
 strcat(to_append, num_str); 
 strcat(to_append, "),\n\tSAMPLE=cms.untracked.bool(");
 sampling ? strcat(to_append, "True") : strcat(to_append, "False");
 strcat(to_append, "),\n\tSTART_AT_EVENT=cms.untracked.int32(");
 sprintf(num_str, "%d", start_at);
 strcat(to_append, num_str);
 strcat(to_append, ")\n)\n");
 
 strncpy(filename, original_filename, strlen(original_filename)-3);
 strcat(filename, "_");
 int i;
 char char_str[3];
 for(i=0; i<(int)strlen(event0); i++)
 {
  if(event0[i] == ':')
  {
   char_str[0] = '_';
   char_str[1] = '_';
   char_str[2] = '\0';
  }
  else
  {
   char_str[0] = event0[i];
   char_str[1] = '\0';
   char_str[2] = '\0';
  }
  strcat(filename, char_str);
 }
 if(inv0)
 { 
  strcat(filename, "_INV_1");
 }
 if(cmask0)
 {
  strcat(filename, "_CMASK_");
  sprintf(num_str, "%d", cmask0);
  strcat(filename, num_str);
 }
 sampling ? strcat(filename, "_S.py") : strcat(filename, "_C.py");

 int fd = open(filename, O_CREAT|O_TRUNC|O_WRONLY, 0777);
 if(fd==-1)
 {
  printf("ERROR: Could not open new file %s\n", filename);
  return 1;
 }
 ssize_t written = write(fd, original_buffer, strlen(original_buffer));
 if(written!=(int)strlen(original_buffer))
 {
  printf("ERROR: Could not write into file %s\n", filename);
  return 1;
 } 
 written = write(fd, to_append, strlen(to_append));
 if(written!=(int)strlen(to_append))
 {
  printf("ERROR: Could not write into file %s\n", filename);
  return 1;
 } 
 int ret = close(fd);
 if(ret==-1)
 {
  printf("ERROR: Could not close file %s\n", filename);
  return 1;
 }
 strcpy(generated_filename, filename);
 return 0;
}

// class ParseHandler : class responsible for parsing the XML config file
class ParseHandler : public DefaultHandler
{
 private:
  XMLCh *ATTR_NAME;
  XMLCh *ATTR_RUN_CONFIG;
  XMLCh *ATTR_OUTPUT_DIR;
  XMLCh *ATTR_START_AT_EVENT;
  XMLCh *ATTR_PARALLEL;
  XMLCh *ATTR_CMASK;
  XMLCh *ATTR_INVMASK;
  XMLCh *ATTR_SMPL_PERIOD;
  XMLCh *ATTR_FAMILY;

 public:
  char events[MAX_EVENT_SETS][MAX_EVENTS_NO][MAX_EVENT_NAME_LENGTH];
  int cmasks[MAX_EVENT_SETS][MAX_EVENTS_NO];
  int invmasks[MAX_EVENT_SETS][MAX_EVENTS_NO];
  int sampling_periods[MAX_EVENT_SETS][MAX_EVENTS_NO];
  int parallel;
  int start_at_event;
  char py_conf_file[FILENAME_MAX_LENGTH];
  char output_dir[FILENAME_MAX_LENGTH];
  char analysis_name[ANALYSIS_NAME_MAX_LENGTH];
  char processor_family[PROC_FAMILY_MAX_LENGTH];
  int cur_set;
  int cur_event;

 public:
  ParseHandler()
  {
   for(int i=0; i<MAX_EVENT_SETS; i++)
   {
    for(int j=0; j<MAX_EVENTS_NO; j++)
    {
     bzero(events[i][j], MAX_EVENT_NAME_LENGTH);
     cmasks[i][j] = 0;
     invmasks[i][j] = 0;
     sampling_periods[i][j] = 0;
    }
   }
   parallel = 0;
   start_at_event = 0;
   cur_set = 0;
   cur_event = 0;
   bzero(py_conf_file, FILENAME_MAX_LENGTH);
   bzero(output_dir, FILENAME_MAX_LENGTH);
   bzero(processor_family, PROC_FAMILY_MAX_LENGTH);
   ATTR_NAME = XMLString::transcode("NAME");
   ATTR_RUN_CONFIG = XMLString::transcode("RUN_CONFIG");
   ATTR_OUTPUT_DIR = XMLString::transcode("OUTPUT_DIR");
   ATTR_START_AT_EVENT = XMLString::transcode("START_AT_EVENT");
   ATTR_PARALLEL = XMLString::transcode("PARALLEL");
   ATTR_CMASK = XMLString::transcode("CMASK");
   ATTR_INVMASK = XMLString::transcode("INVMASK");
   ATTR_SMPL_PERIOD = XMLString::transcode("SMPL_PERIOD");
   ATTR_FAMILY = XMLString::transcode("FAMILY");
  }
  ~ParseHandler()
  {
   try
   {
    XMLString::release(&ATTR_NAME);
    XMLString::release(&ATTR_RUN_CONFIG);
    XMLString::release(&ATTR_OUTPUT_DIR);
    XMLString::release(&ATTR_START_AT_EVENT);
    XMLString::release(&ATTR_PARALLEL);
    XMLString::release(&ATTR_CMASK);
    XMLString::release(&ATTR_INVMASK);
    XMLString::release(&ATTR_SMPL_PERIOD);
    XMLString::release(&ATTR_FAMILY); 
   }
   catch(...)
   {
    printf("Error releasing!!\n");
   }     
  }
  void startDocument(){}
  void endDocument(){}
  void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const Attributes& attrs)
  {
   char *elem_name = XMLString::transcode(localname);
   if(!strcmp(elem_name, "PFM_CONFIG"));
   else if(!strcmp(elem_name, "PROPERTIES"))
   {
    char *ATTR_NAME_char = XMLString::transcode(attrs.getValue(ATTR_NAME));
    char *ATTR_RUN_CONFIG_char = XMLString::transcode(attrs.getValue(ATTR_RUN_CONFIG));
    char *ATTR_OUTPUT_DIR_char = XMLString::transcode(attrs.getValue(ATTR_OUTPUT_DIR));
    strcpy(analysis_name, ATTR_NAME_char);
    strcpy(py_conf_file, ATTR_RUN_CONFIG_char);
    strcpy(output_dir, ATTR_OUTPUT_DIR_char);
    try
    {
     XMLString::release(&ATTR_NAME_char);
     XMLString::release(&ATTR_RUN_CONFIG_char);
     XMLString::release(&ATTR_OUTPUT_DIR_char);
    }
    catch(...)
    {
     printf("Error releasing!!\n");
    }   
   }
   else if(!strcmp(elem_name, "PROCESSOR"))
   {
    char *ATTR_FAMILY_char = XMLString::transcode(attrs.getValue(ATTR_FAMILY));
    strcpy(processor_family, ATTR_FAMILY_char);
    try
    {
     XMLString::release(&ATTR_FAMILY_char);
    }
    catch(...)
    {
     printf("Error releasing!!\n");
    }   
   }
   else if(!strcmp(elem_name, "CONFIG"))
   {
    char *ATTR_START_AT_EVENT_char = XMLString::transcode(attrs.getValue(ATTR_START_AT_EVENT));
    char *ATTR_PARALLEL_char = XMLString::transcode(attrs.getValue(ATTR_PARALLEL));
    start_at_event = atoi(ATTR_START_AT_EVENT_char);
    parallel = atoi(ATTR_PARALLEL_char);
    try
    {
     XMLString::release(&ATTR_START_AT_EVENT_char);
     XMLString::release(&ATTR_PARALLEL_char); 
    }
    catch(...)
    {
     printf("Error releasing!!\n");
    }      
   }
   else if(!strcmp(elem_name, "EVENTS"));
   else if(!strcmp(elem_name, "EVENT_SET"));
   else if(!strcmp(elem_name, "EVENT"))
   {
    char *ATTR_NAME_char = XMLString::transcode(attrs.getValue(ATTR_NAME));
    char *ATTR_CMASK_char = XMLString::transcode(attrs.getValue(ATTR_CMASK));
    char *ATTR_INVMASK_char = XMLString::transcode(attrs.getValue(ATTR_INVMASK));
    char *ATTR_SMPL_PERIOD_char = XMLString::transcode(attrs.getValue(ATTR_SMPL_PERIOD));
    strcpy(events[cur_set][cur_event], ATTR_NAME_char);
    cmasks[cur_set][cur_event] = atoi(ATTR_CMASK_char);
    invmasks[cur_set][cur_event] = atoi(ATTR_INVMASK_char);
    sampling_periods[cur_set][cur_event] = atoi(ATTR_SMPL_PERIOD_char);
    try
    {
     XMLString::release(&ATTR_NAME_char);
     XMLString::release(&ATTR_CMASK_char);
     XMLString::release(&ATTR_INVMASK_char);
     XMLString::release(&ATTR_SMPL_PERIOD_char);
    }
    catch(...)
    {
     printf("Error releasing!!\n");
    }   
   }
   else
   {
    printf("ERROR: unknown element <%s>\nAborting...\n", elem_name);
    try
    {
     XMLString::release(&elem_name);
    }
    catch(...)
    {
     printf("Error releasing!!\n");
    }   
    exit(1);
   }
   try
   {
    XMLString::release(&elem_name);
   }
   catch(...)
   {
    printf("Error releasing!!\n");
   }   
  }
  void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
  {
   char *elem_name = XMLString::transcode(localname);
   if(!strcmp(elem_name, "PFM_CONFIG"));
   else if(!strcmp(elem_name, "PROPERTIES"));
   else if(!strcmp(elem_name, "PROCESSOR"));
   else if(!strcmp(elem_name, "CONFIG"));
   else if(!strcmp(elem_name, "EVENTS"));
   else if(!strcmp(elem_name, "EVENT_SET"))
   {
    cur_set++;
    cur_event = 0;
   }
   else if(!strcmp(elem_name, "EVENT"))
   {
    cur_event++;
   }
   else
   {
    printf("ERROR: unknown element <%s>\nAborting...\n", elem_name);
    try
    {
     XMLString::release(&elem_name);
    }
    catch(...)
    {
     printf("Error releasing!!\n");
    }   
    exit(1);
   }
   try
   {
    XMLString::release(&elem_name);
   }
   catch(...)
   {
    printf("Error releasing!!\n");
   }   
  }
  void characters(const XMLCh* const chars, const unsigned int length){}
};

// main()
// takes as input the XML configuration file that contains all the monitoring parameters. 
// then generates the python configuration files to be passed to cmsRun, in order to use the PerfmonService of CMSSW
// both for counting and sampling, it also generates the "*_runs.py" file responsible for calling all the others
int main(int argc, char *argv[])
{
 if(argc != 2)
 {
  printf("\n\nUSAGE: %s conf_file.xml\n\n", argv[0]);
  return 1;
 }

 XMLPlatformUtils::Initialize();
 if(access(argv[1], 04))
 {
  printf("\n\nFile %s doesn't exist!!\n\n", argv[1]);
  return 1;
 }
 SAX2XMLReader* pParser = XMLReaderFactory::createXMLReader();
 ParseHandler handler;
 pParser->setContentHandler(&handler);
 pParser->setErrorHandler(&handler);
 try
 {
  pParser->parse(argv[1]);
 }
 catch(...)
 {
  puts("Parse error!");
 }

 int fd = open(handler.py_conf_file, O_RDONLY);
 char c;
 int i=0;
 char original_buffer[ORIG_BUF_MAX_LENGTH];
 bzero(original_buffer, ORIG_BUF_MAX_LENGTH);
 while(read(fd, &c, 1) && i<ORIG_BUF_MAX_LENGTH-1)
 {
  original_buffer[i] = c;
  i++;
 }
 original_buffer[i] = 0;
 close(fd);
 
 int core_count = get_nprocs();

 char filename[FILENAME_MAX_LENGTH];
 char to_append[APPENDIX_MAX_LENGTH];
 bzero(filename, FILENAME_MAX_LENGTH);
 bzero(to_append, APPENDIX_MAX_LENGTH);
 strcpy(to_append, "");
 strcat(to_append, "import os\n\n");

 for(int i=0; i<handler.cur_set; i++)
 {
  gen_file(original_buffer, handler.py_conf_file, handler.output_dir, 
           handler.events[i][0], handler.invmasks[i][0], handler.cmasks[i][0], handler.sampling_periods[i][0],
           handler.events[i][1], handler.invmasks[i][1], handler.cmasks[i][1], handler.sampling_periods[i][1],
           handler.events[i][2], handler.invmasks[i][2], handler.cmasks[i][2], handler.sampling_periods[i][2],
           handler.events[i][3], handler.invmasks[i][3], handler.cmasks[i][3], handler.sampling_periods[i][3],
           handler.start_at_event, filename, handler.processor_family);
  strcat(to_append, "os.system(\"cmsRun ");
  strcat(to_append, filename);
  if(handler.parallel)
  {   
   if((i+1)%(core_count-1)==0)
   {
    strcat(to_append, "\")\n");
    strcat(to_append, "os.system(\"sleep 60\")\n");
   }
   else
   {
    strcat(to_append, "&\")\n");
   }
  }
  else
  {
   strcat(to_append, "\")\n");
  }
 }
 
 bzero(filename, FILENAME_MAX_LENGTH);
 strncpy(filename, handler.py_conf_file, strlen(handler.py_conf_file)-3);
 strcat(filename, "_runs.py");
 fd = open(filename, O_CREAT|O_TRUNC|O_WRONLY, 0777); 
 write(fd, to_append, strlen(to_append));
 close(fd);
 return 0;
}
