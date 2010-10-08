/*
Name: PerfMonService.cc
Author: Daniele Francesco Kruse
E-mail: daniele.francesco.kruse@cern.ch
Version: 0.9 (16/02/2010)

This code is responsible for monitoring the performance of modules of CMSSW using the PMU.

compile linking it with libpfm!
*/

#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "DataFormats/Provenance/interface/ModuleDescription.h"
#include "IgHookTrace.h"
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <perfmon/pfmlib.h>
#include <perfmon/pfmlib_core.h>
#include <perfmon/pfmlib_intel_nhm.h>
#include <vector>
#include <map>
#include <utility>
#include <sstream>
#include <perfmon/perfmon.h>
#include <perfmon/perfmon_dfl_smpl.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <getopt.h>
#include <time.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>
#include <zlib.h>

#include <algorithm>
#include <list>
#include <cmath>
#include <sys/stat.h>
#define cpuid(func,eax,ebx,ecx,edx)	__asm__ __volatile__ ("cpuid": "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (func));

#define MAX_EVT_NAME_LEN 256
#define NUM_PMCS PFMLIB_MAX_PMCS
#define NUM_PMDS PFMLIB_MAX_PMDS
#define FMT_NAME PFM_DFL_SMPL_NAME
#define BPL (sizeof(uint64_t)<<3)
#define LBPL 6

#define SYM_NAME_MAX_LENGTH 10000
#define MAX_OUTPUT_FILENAME_LENGTH 1024
#define MAX_EVENT_NAME_LENGTH 500
#define MAX_PREFIX_NAME_LENGTH 1024
#define FILENAME_MAX_LENGTH 1024

#define MAX_NUMBER_OF_PROGRAMMABLE_COUNTERS 4

static pfarg_pmd_t pd_smpl[NUM_PMDS];
static uint64_t collected_samples, collected_partial;
static int ctx_fd;
static pfm_dfl_smpl_hdr_t *hdr;
static uint64_t ovfl_count;
static size_t entry_size;
static unsigned int num_smpl_pmds;
static std::vector<std::map<std::string, std::map<unsigned long, unsigned int> > > samples(MAX_NUMBER_OF_PROGRAMMABLE_COUNTERS); //a map of modules each containing numbers of samples of their addresses
static std::vector<std::map<std::string, std::vector<unsigned long int> > > results(MAX_NUMBER_OF_PROGRAMMABLE_COUNTERS); //a map of modules and their result values across multiple events
static uint64_t last_overflow;
static uint64_t last_count;    
static std::string current_module;
static int sp[MAX_NUMBER_OF_PROGRAMMABLE_COUNTERS];

namespace edm 
{
 namespace service 
 {
  class PerfMonService
  {
   public:
    PerfMonService(const ParameterSet& iPS, ActivityRegistry& iRegistry);
    ~PerfMonService();
    void postBeginJob();
    void postEndJob();
    void preEventProcessing(const edm::EventID& iID, const edm::Timestamp& iTime);
    void postEventProcessing(const Event& e, const EventSetup&);
    void preModule(const ModuleDescription& desc);
    void postModule(const ModuleDescription& desc);

   private:
    pfmlib_input_param_t inp;
    pfmlib_output_param_t outp;
    pfarg_ctx_t ctx;
    pfarg_pmd_t pd[NUM_PMDS];
    pfarg_pmc_t pc[NUM_PMCS];
    pfarg_load_t load_arg;
    int fd;
    unsigned int i;
    int ret;
    void startpm();
    void stoppm(const ModuleDescription& desc);
    void finalizepm();
    std::string event_str[MAX_NUMBER_OF_PROGRAMMABLE_COUNTERS];
    std::string prefix;
    std::string family;
    char event_cstr[MAX_NUMBER_OF_PROGRAMMABLE_COUNTERS][MAX_EVENT_NAME_LENGTH];
    char prefix_cstr[MAX_PREFIX_NAME_LENGTH];
    unsigned int ph_ev_count;
    bool inv[MAX_NUMBER_OF_PROGRAMMABLE_COUNTERS];
    bool nehalem;
    bool core;
    bool westmere;
    unsigned int cmask[MAX_NUMBER_OF_PROGRAMMABLE_COUNTERS];
    unsigned int start_at_event;
    pfmlib_core_input_param_t params;
    pfmlib_nhm_input_param_t nhm_params;
    int used_counters_number;
    
    bool sampling;
    int detect_unavail_pmu_regs(int fd, pfmlib_regmask_t *r_pmcs, pfmlib_regmask_t *r_pmds);
    int detect_unavail_pmcs(int fd, pfmlib_regmask_t *r_pmcs){return detect_unavail_pmu_regs(fd, r_pmcs, NULL);}
    void pfm_bv_set(uint64_t *bv, uint16_t rnum){bv[rnum>>LBPL] |= 1UL << (rnum&(BPL-1));}
    int pfm_bv_isset(uint64_t *bv, uint16_t rnum){return bv[rnum>>LBPL] & (1UL <<(rnum&(BPL-1))) ? 1 : 0;}
    void pfm_bv_copy(uint64_t *d, uint64_t *j, uint16_t n){if(n<=BPL) *d = *j; else {memcpy(d, j, (n>>LBPL)*sizeof(uint64_t));}}
    static void process_smpl_buf(pfm_dfl_smpl_hdr_t *hdr, size_t entry_size);
    static void sigio_handler(int /*n*/, struct siginfo */*info*/, struct sigcontext */*sc*/);
    void start_smpl(const ModuleDescription& desc);
    void stop_smpl(const ModuleDescription& desc);
    void finalize_smpl();
    pfm_dfl_smpl_arg_t buf_arg;
    pfarg_load_t load_args;
    void *buf_addr;
    unsigned num_counters;
    unsigned int max_pmd;
    pfmlib_options_t pfmlib_options;
  };
   
   // PerfMonService() - Constructor
   // const ParameterSet& iPS     : set of parameters passed by the python configuration file to the service
   // ActivityRegistry& iRegistry : register of the various hooks used by the service
   // takes care of registering the hooks used, initializes the perfmon library and initializes
   // the fields of the class using the parameters passed by the python configuration file to the service
  PerfMonService::PerfMonService(const ParameterSet& iPS, ActivityRegistry& iRegistry)     
  {
   int ax,bx,cx,dx;
   cpuid(1,ax,bx,cx,dx);
   int sse4_2_mask = 1 << 20;
   int nehalem_flag = cx & sse4_2_mask;
   iRegistry.watchPostBeginJob(this,&PerfMonService::postBeginJob);
   iRegistry.watchPostEndJob(this,&PerfMonService::postEndJob);
   iRegistry.watchPreProcessEvent(this,&PerfMonService::preEventProcessing);
   iRegistry.watchPostProcessEvent(this,&PerfMonService::postEventProcessing);
   iRegistry.watchPreModule(this,&PerfMonService::preModule);
   iRegistry.watchPostModule(this,&PerfMonService::postModule);
   if(pfm_initialize() != PFMLIB_SUCCESS)
   {
    fprintf(stderr, "ERROR: Unable to initialize perfmon2 library\naborting...\n");
    exit(1);
   }
   ph_ev_count = 0;
   used_counters_number = 0;
   event_str[0] = iPS.getUntrackedParameter<std::string>("EVENT0", event_str[0]);
   event_str[1] = iPS.getUntrackedParameter<std::string>("EVENT1", event_str[1]);
   event_str[2] = iPS.getUntrackedParameter<std::string>("EVENT2", event_str[2]);
   event_str[3] = iPS.getUntrackedParameter<std::string>("EVENT3", event_str[3]);
   for(int i=0; i<MAX_NUMBER_OF_PROGRAMMABLE_COUNTERS; i++) 
   {
    if(event_str[i].length()>0) used_counters_number++;
   }
   family = iPS.getUntrackedParameter<std::string>("FAMILY", family);
   nehalem = false;
   core = false;
   westmere = false;
   if(family.compare("CORE")==0) core = true;
   else if(family.compare("NEHALEM")==0) nehalem = true;
   else if(family.compare("WESTMERE")==0) westmere = true;
   else
   {
    fprintf(stderr, "ERROR: Unsupported processor family \"%s\"\naborting...\n", family.c_str());
    exit(1);
   }
   if(core && nehalem_flag)
   {    
    fprintf(stderr, "ERROR: Processor family specified (\"%s\") incompatible with current architecture\naborting...\n", family.c_str());
    exit(1);
   }
   prefix = iPS.getUntrackedParameter<std::string>("PREFIX", prefix);
   for(int i=0; i<MAX_NUMBER_OF_PROGRAMMABLE_COUNTERS; i++) 
   {
    strcpy(event_cstr[i], event_str[i].c_str());
   }
   strcpy(prefix_cstr, prefix.c_str()); 
   inv[0] = iPS.getUntrackedParameter<bool>("INV0", inv[0]);
   inv[1] = iPS.getUntrackedParameter<bool>("INV1", inv[1]);
   inv[2] = iPS.getUntrackedParameter<bool>("INV2", inv[2]);
   inv[3] = iPS.getUntrackedParameter<bool>("INV3", inv[3]);
   cmask[0] = iPS.getUntrackedParameter<int>("CMASK0", cmask[0]);
   cmask[1] = iPS.getUntrackedParameter<int>("CMASK1", cmask[1]);
   cmask[2] = iPS.getUntrackedParameter<int>("CMASK2", cmask[2]);
   cmask[3] = iPS.getUntrackedParameter<int>("CMASK3", cmask[3]);
   sp[0] = iPS.getUntrackedParameter<int>("SP0", sp[0]);
   sp[1] = iPS.getUntrackedParameter<int>("SP1", sp[1]);
   sp[2] = iPS.getUntrackedParameter<int>("SP2", sp[2]);
   sp[3] = iPS.getUntrackedParameter<int>("SP3", sp[3]);
   sampling = iPS.getUntrackedParameter<bool>("SAMPLE", sampling);
   start_at_event = iPS.getUntrackedParameter<int>("START_AT_EVENT", start_at_event);
  }

   // startpm()
   // initializes all the necessary structures to start the actual counting, calling pfm_start()
  void PerfMonService::startpm()
  {
   memset(&ctx,0, sizeof(ctx));
   memset(&inp,0, sizeof(inp));
   memset(&outp,0, sizeof(outp));
   memset(pd, 0, sizeof(pd));
   memset(pc, 0, sizeof(pc));
   memset(&load_arg, 0, sizeof(load_arg));
   memset(&params, 0, sizeof(params));
   memset(&nhm_params, 0, sizeof(nhm_params));

   for(int i=0; i<used_counters_number; i++) 
   {
    ret = pfm_find_full_event(event_cstr[i], &inp.pfp_events[i]);
    if(ret != PFMLIB_SUCCESS)
    {
     fprintf(stderr, "ERROR: cannot find event: %s\naborting...\n", event_cstr[i]);
     exit(1);
    }
   }
   inp.pfp_dfl_plm = PFM_PLM3; 
   inp.pfp_event_count = 4;
   for(int i=0; i<used_counters_number; i++) 
   {
    if(inv[i])
    {
     (params.pfp_core_counters[i]).flags |= PFM_CORE_SEL_INV;
     (nhm_params.pfp_nhm_counters[i]).flags |= PFM_NHM_SEL_INV;
    }
    if(cmask[i]>0)
    {
     (params.pfp_core_counters[i]).cnt_mask = cmask[i];
     (nhm_params.pfp_nhm_counters[i]).cnt_mask = cmask[i];
    }
   }
   if(nehalem || westmere)
   {
    ret = pfm_dispatch_events(&inp, &nhm_params, &outp, NULL);
   }
   else
   {
    ret = pfm_dispatch_events(&inp, &params, &outp, NULL);
   }
   if(ret != PFMLIB_SUCCESS)
   {
    fprintf(stderr, "ERROR: cannot dispatch events: %s\naborting...\n", pfm_strerror(ret));
    exit(1);
   }
   for(unsigned int i=0; i<outp.pfp_pmc_count; i++)
   {
    pc[i].reg_num = outp.pfp_pmcs[i].reg_num;
    pc[i].reg_value = outp.pfp_pmcs[i].reg_value;
   }
   for(unsigned int i=0; i<outp.pfp_pmd_count; i++)
   {
    pd[i].reg_num = outp.pfp_pmds[i].reg_num;
    pd[i].reg_value = 0;
   }
   fd = pfm_create_context(&ctx, NULL, 0, 0);
   if(fd == -1)
   {
    fprintf(stderr, "ERROR: Context not created\naborting...\n");
    exit(1);
   }
   if(pfm_write_pmcs(fd, pc, outp.pfp_pmc_count) == -1)
   {
    fprintf(stderr, "ERROR: Could not write pmcs\naborting...\n");
    exit(1);
   }
   if(pfm_write_pmds(fd, pd, outp.pfp_pmd_count) == -1)
   {
    fprintf(stderr, "ERROR: Could not write pmds\naborting...\n");
    exit(1);
   }
   load_arg.load_pid = getpid();
   if(pfm_load_context(fd, &load_arg) == -1)
   {
    fprintf(stderr, "ERROR: Could not load context\naborting...\n");
    exit(1);
   }
   
   pfm_start(fd, NULL);   
  }

   // stoppm()
   // const ModuleDescription& desc : description of the module that just finished its execution (we are only interested in its name)
   // stops the counting calling pfm_stop() and stores the counting results into the "results" map
  void PerfMonService::stoppm(const ModuleDescription& desc)
  {
   pfm_stop(fd);
   if(pfm_read_pmds(fd, pd, inp.pfp_event_count) == -1)
   {
    fprintf(stderr, "ERROR: Could not read pmds\naborting...\n");
    exit(1);
   }
   
   for(int i=0; i<used_counters_number; i++) 
   {
    results[i][desc.moduleName()+"_"+desc.moduleLabel()].push_back(pd[i].reg_value);
   }

   close(fd);
  }

   // finalizepm()
   // called when all the countings of the current event are finished, it dumps the results
   // into the output file corresponding to the event being counted
  void PerfMonService::finalizepm()
  {
   char filename[MAX_OUTPUT_FILENAME_LENGTH];
   char to_cat[50];
   FILE *outfile;
   for(int i=0; i<used_counters_number; i++) 
   {
    bzero(filename, MAX_OUTPUT_FILENAME_LENGTH);
    sprintf(filename, "%s_%s", prefix_cstr, event_cstr[i]);
    for(int j=0; j<(int)strlen(filename); j++)
    {
     if(filename[j]==':')
     {
      filename[j]='-';
     }
    }
    bzero(to_cat, 50);   
    if(inv[i])
    {
     strcpy(to_cat, "_INV_1");
    }
    if(cmask[i]>0)
    {
     sprintf(to_cat, "%s_CMASK_%d", to_cat, cmask[i]);
    }
    sprintf(filename, "%s%s.txt", filename, to_cat);
    outfile = fopen(filename, "w");
    if(nehalem)
    {
     fprintf(outfile, "NHM ");
    }
    else if(westmere)
    {
     fprintf(outfile, "WSM ");
    }
    else if(core)
    {
     fprintf(outfile, "CORE ");
    }
    fprintf(outfile, "%s %d %d %d\n", event_cstr[i], cmask[i], inv[i], sp[i]);
    for(std::map<std::string, std::vector<unsigned long int> >::iterator it=(results[i]).begin(); it!=(results[i]).end(); it++)
    {
     fprintf(outfile, "%s\n", (it->first).c_str());
     for(std::vector<unsigned long int>::iterator j=(it->second).begin(); j!=(it->second).end(); j++)
     {
      fprintf(outfile, "%lu\n", *j);
     }
    }
    fclose(outfile);
   }
  }

   // ~PerfMonService() - Destructor
   // calls the finalizer of the monitoring service: finalizepm() in the case of counting or finalize_smpl() in the case of sampling
  PerfMonService::~PerfMonService()
  {
   if(!sampling)
   {
    finalizepm();
   }
   else
   {
    finalize_smpl();
   }
  }

   // process_smpl_buf()
   // pfm_dfl_smpl_hdr_t *hdr : pointer to header of the buffer containing addresses sampled during the sampling process
   // size_t entry_size       : size of each entry, used to navigate through the various entries
   // called when the sampling buffer is full, saves the samples into memory ("samples" map)
  void PerfMonService::process_smpl_buf(pfm_dfl_smpl_hdr_t *hdr, size_t entry_size)
  {   
   pfm_dfl_smpl_entry_t *ent;
   size_t pos, count;
   uint64_t entry;
   if(hdr->hdr_overflows == last_overflow && hdr->hdr_count == last_count)
   {
    printf("skipping identical set of samples...\n");
    return;	
   }
   count = hdr->hdr_count;
   ent = (pfm_dfl_smpl_entry_t *)(hdr+1);
   pos = (unsigned long)ent;
   entry = collected_samples;
   while(count--)
   {   
    if(ent->ovfl_pmd<=3)
    {
     ((samples[ent->ovfl_pmd])[current_module])[(unsigned long)(ent->ip)]++;
    }
    pos += entry_size;
    ent = (pfm_dfl_smpl_entry_t *)pos;
    entry++;
   }
   collected_samples = entry;
   last_overflow = hdr->hdr_overflows;
   if (last_count != hdr->hdr_count && (last_count || last_overflow == 0))
   {
    collected_partial += hdr->hdr_count;
   }
   last_count = hdr->hdr_count;
  }

   // sigio_handler()
   // int n                 : signal number of the signal being delivered
   // struct siginfo *info  : pointer to a siginfo_t structure containing info about the signal
   // struct sigcontext *sc : context of the signal, NULL in our case
   // signal handler used to catch sampling buffer overflows. When they occur it calls the process_smpl_buf() function
  void PerfMonService::sigio_handler(int /*n*/, struct siginfo */*info*/, struct sigcontext */*sc*/)
  {
   pfarg_msg_t msg;
   int fd = ctx_fd;
   int r;
   if(fd != ctx_fd)
   {
    fprintf(stderr, "ERROR: handler does not get valid file descriptor\naborting...\n");
    exit(1);
   }
   if(pfm_read_pmds(fd, pd_smpl+1, 1) == -1)
   {
    fprintf(stderr, "ERROR: pfm_read_pmds: %s\naborting...\n", strerror(errno));
    exit(1);
   }
   while(true)
   {
    r = read(fd, &msg, sizeof(msg));
    if(r!=sizeof(msg))
    {
     if(r==-1 && errno==EINTR)
     {
      printf("read interrupted, retrying\n");
      continue;
     }
     fprintf(stderr, "ERROR: cannot read overflow message: %s\naborting...\n", strerror(errno));
     exit(1);
    }
    break;
   }
   switch(msg.type)
   {
    case PFM_MSG_OVFL: // the sampling buffer is full
     process_smpl_buf(hdr, entry_size);
     ovfl_count++;
     if(pfm_restart(fd))
     {
      if(errno!=EBUSY)
      {
       fprintf(stderr, "ERROR: pfm_restart error errno %d\naborting...\n",errno);
       exit(1);
      }
      else
      {
       printf("pfm_restart: task probably terminated \n");
      }
     }
     break;
    default: 
     fprintf(stderr, "ERROR: unknown message type %d\naborting...\n", msg.type);
     exit(1);
     break;
   }
  }

   // start_smpl()
   // const ModuleDescription& desc : description of the module that is just starting its execution (we are only interested in its name)
   // initializes all the necessary structures to start the sampling, calling pfm_self_start()
  void PerfMonService::start_smpl(const ModuleDescription& desc)
  {
   current_module = desc.moduleName()+"_"+desc.moduleLabel();
   ovfl_count = 0;
   num_smpl_pmds = 0;
   last_overflow = ~0; 
   max_pmd = 0;
   memset(&pfmlib_options, 0, sizeof(pfmlib_options));
   pfmlib_options.pfm_debug   = 0;
   pfmlib_options.pfm_verbose = 0;
   pfm_set_options(&pfmlib_options);
   ret = pfm_initialize();
   if(ret != PFMLIB_SUCCESS)
   {
    fprintf(stderr, "ERROR: Cannot initialize library: %s\naborting...\n", pfm_strerror(ret));
    exit(1);
   }
   struct sigaction act;
   memset(&act, 0, sizeof(act));
   act.sa_handler = (sig_t)sigio_handler;
   sigaction(SIGIO, &act, 0);
   memset(&ctx, 0, sizeof(ctx));
   memset(&buf_arg, 0, sizeof(buf_arg));
   memset(&inp,0, sizeof(inp));
   memset(&outp,0, sizeof(outp));
   memset(pd_smpl, 0, sizeof(pd_smpl));
   memset(pc, 0, sizeof(pc));
   memset(&load_args, 0, sizeof(load_args));
   pfm_get_num_counters(&num_counters); 
   memset(&params, 0, sizeof(params));
   memset(&nhm_params, 0, sizeof(nhm_params));

   for(int i=0; i<used_counters_number; i++) 
   {
    ret = pfm_find_full_event(event_cstr[i], &inp.pfp_events[i]);
    if(ret != PFMLIB_SUCCESS)
    {
     fprintf(stderr, "ERROR: cannot find event: %s\naborting...\n", event_cstr[i]);
     exit(1);
    }
   }
   inp.pfp_dfl_plm = PFM_PLM3; 
   inp.pfp_event_count = 4;
   for(int i=0; i<used_counters_number; i++) 
   {
    if(inv[i])
    {
     (params.pfp_core_counters[i]).flags |= PFM_CORE_SEL_INV;
     (nhm_params.pfp_nhm_counters[i]).flags |= PFM_NHM_SEL_INV;
    }
    if(cmask[i]>0)
    {
     (params.pfp_core_counters[i]).cnt_mask = cmask[i];
     (nhm_params.pfp_nhm_counters[i]).cnt_mask = cmask[i];
    }
   }
   if(nehalem || westmere)
   {
    ret = pfm_dispatch_events(&inp, &nhm_params, &outp, NULL);
   }
   else
   {
    ret = pfm_dispatch_events(&inp, &params, &outp, NULL);
   }
   if(ret != PFMLIB_SUCCESS)
   {
    fprintf(stderr, "ERROR: cannot configure events: %s\naborting...\n", pfm_strerror(ret));
    exit(1);
   }
   for(unsigned int i=0; i<outp.pfp_pmc_count; i++)
   {
    pc[i].reg_num   = outp.pfp_pmcs[i].reg_num;
    pc[i].reg_value = outp.pfp_pmcs[i].reg_value;
   }
   for(unsigned int i=0; i<outp.pfp_pmd_count; i++)
   {
    pd_smpl[i].reg_num = outp.pfp_pmds[i].reg_num;
    if(i)
    {
     pfm_bv_set(pd_smpl[0].reg_smpl_pmds, pd_smpl[i].reg_num);
     if(pd_smpl[i].reg_num>max_pmd)
     {
      max_pmd = pd_smpl[i].reg_num;
     }
     num_smpl_pmds++;
    }
   }
   for(int i=0; i<used_counters_number; i++) 
   {
    pd_smpl[i].reg_flags |= PFM_REGFL_OVFL_NOTIFY | PFM_REGFL_RANDOM;
    pfm_bv_copy(pd_smpl[i].reg_reset_pmds, pd_smpl[i].reg_smpl_pmds, max_pmd);
    pd_smpl[i].reg_value = (uint64_t)(sp[i] * -1);
    pd_smpl[i].reg_short_reset = (uint64_t)(sp[i] * -1);
    pd_smpl[i].reg_long_reset = (uint64_t)(sp[i] * -1);
    pd_smpl[i].reg_random_seed = 5; //tocheck
    pd_smpl[i].reg_random_mask = 0xff; //tocheck
   }
   entry_size = sizeof(pfm_dfl_smpl_entry_t)+(num_smpl_pmds<<3);
   ctx.ctx_flags = 0;
   buf_arg.buf_size = 3*getpagesize()+512;
   ctx_fd = pfm_create_context(&ctx, (char *)FMT_NAME, &buf_arg, sizeof(buf_arg));
   if(ctx_fd==-1)
   {
    if(errno==ENOSYS)
    {
     fprintf(stderr, "ERROR: Your kernel does not have performance monitoring support!\naborting...\n");
     exit(1);
    }
    fprintf(stderr, "ERROR: Can't create PFM context %s\naborting...\n", strerror(errno));
    exit(1);
   }
   buf_addr = mmap(NULL, (size_t)buf_arg.buf_size, PROT_READ, MAP_PRIVATE, ctx_fd, 0);
   if(buf_addr==MAP_FAILED)
   {
    fprintf(stderr, "ERROR: cannot mmap sampling buffer: %s\naborting...\n", strerror(errno));
    exit(1);
   }
   hdr = (pfm_dfl_smpl_hdr_t *)buf_addr;
   if(PFM_VERSION_MAJOR(hdr->hdr_version)<1)
   {
    fprintf(stderr, "ERROR: invalid buffer format version\naborting...\n");
    exit(1);
   }
   if(pfm_write_pmcs(ctx_fd, pc, outp.pfp_pmc_count))
   {
    fprintf(stderr, "ERROR: pfm_write_pmcs error errno %s\naborting...\n", strerror(errno));
    exit(1);
   }
   if(pfm_write_pmds(ctx_fd, pd_smpl, outp.pfp_pmd_count))
   {
    fprintf(stderr, "ERROR: pfm_write_pmds error errno %s\naborting...\n", strerror(errno));
    exit(1);
   }
   load_args.load_pid = getpid();
   if(pfm_load_context(ctx_fd, &load_args))
   {
    fprintf(stderr, "ERROR: pfm_load_context error errno %s\naborting...\n", strerror(errno));
    exit(1);
   }
   ret = fcntl(ctx_fd, F_SETFL, fcntl(ctx_fd, F_GETFL, 0) | O_ASYNC);
   if(ret == -1)
   {
    fprintf(stderr, "ERROR: cannot set ASYNC: %s\naborting...\n", strerror(errno));
    exit(1);
   }
   ret = fcntl(ctx_fd, F_SETOWN, getpid());
   if(ret == -1)
   {
    fprintf(stderr, "ERROR: cannot setown: %s\naborting...\n", strerror(errno));
    exit(1);
   }
   pfm_self_start(ctx_fd);
  }
  
   // stop_smpl()
   // const ModuleDescription& desc : description of the module that just finished its execution (we are only interested in its name)
   // stops the sampling and calls process_smpl_buf() one last time to process all the remaining samples
  void PerfMonService::stop_smpl(const ModuleDescription& desc)
  {
   pfm_self_stop(ctx_fd);	
   process_smpl_buf(hdr, entry_size);
   close(ctx_fd);
   ret = munmap(hdr, (size_t)buf_arg.buf_size);
   if(ret)
   {
    fprintf(stderr, "ERROR: cannot unmap buffer: %s\naborting...\n", strerror(errno));
    exit(1);
   }
   return;
  }

   // finalize_smpl()
   // processes the sampling results in order to find library and offset of each sampled address, using the symbol() and tosymbol() functions,
   // and then dumps the new found information into gzipped output files, to be processed later
  void PerfMonService::finalize_smpl()
  {
   char filename[MAX_OUTPUT_FILENAME_LENGTH];
   bzero(filename, MAX_OUTPUT_FILENAME_LENGTH);
   char to_cat[50];
   gzFile outfile;
   int err;
   for(int i=0; i<used_counters_number; i++) 
   {
    sprintf(filename, "%s_%s", prefix_cstr, event_cstr[i]);
    for(int j=0; j<(int)strlen(filename); j++)
    {
     if(filename[j]==':')
     {
      filename[j]='-';
     }
    }
    bzero(to_cat, 50);   
    if(inv[i])
    {
     strcpy(to_cat, "_INV_1");
    }
    if(cmask[i]>0)
    {
     sprintf(to_cat, "%s_CMASK_%d", to_cat, cmask[i]);
    }
    sprintf(filename, "%s%s.txt.gz", filename, to_cat);
    outfile = gzopen(filename, "wb");
    if(outfile!=NULL)
    {
     if(nehalem)
     {
      gzprintf(outfile, "NHM ");
     }
     else if(westmere)
     {
      gzprintf(outfile, "WSM ");
     }
     else if(core)
     {
      gzprintf(outfile, "CORE ");
     }
     if(gzprintf(outfile, "%s %d %d %d\n", event_cstr[i], cmask[i], inv[i], sp[i]) < (int)strlen(event_cstr[i]))
     {
      fprintf(stderr, "ERROR: gzputs err: %s\naborting...\n", gzerror(outfile, &err));
      exit(1);
     }
     for(std::map<std::string, std::map<unsigned long, unsigned int> >::iterator it=samples[i].begin(); it!=samples[i].end(); it++)
     {
      unsigned long long sum = 0;
      for(std::map<unsigned long, unsigned int>::iterator jt=(it->second).begin(); jt!=(it->second).end(); jt++)
      {
       sum += jt->second;
      }
      if(gzprintf(outfile, "%s%%%llu\n", (it->first).c_str(), sum) < (int)((it->first).length()))
      {
       fprintf(stderr, "ERROR: gzputs err: %s\naborting...\n", gzerror(outfile, &err));
       exit(1);
      }
      for(std::map<unsigned long, unsigned int>::iterator jt=(it->second).begin(); jt!=(it->second).end(); jt++)
      {
       char sym_name[SYM_NAME_MAX_LENGTH];
       bzero(sym_name, SYM_NAME_MAX_LENGTH);
       const char *libName;
       const char *symbolName;
       int libOffset = 0;
       int offset = 0;
       void *sym_addr = IgHookTrace::tosymbol((void *)(jt->first));
       if(sym_addr != NULL)
       {
        bool success = IgHookTrace::symbol(sym_addr, symbolName, libName, offset, libOffset);
        if(success)
        {
         if(symbolName!=NULL && strlen(symbolName)>0)
         {
  	  strcpy(sym_name, symbolName);
	  strcat(sym_name, " ");
         }
         else
         {
          strcpy(sym_name, "??? ");
         }
         if(libName!=NULL && strlen(libName)>0)
         {
          strcat(sym_name, libName);
  	  strcat(sym_name, " ");      
         }
         else
         {
          strcat(sym_name, "??? ");
         }
         sprintf(sym_name, "%s%d ", sym_name, libOffset);
         if(strlen(sym_name)<=0)
         {
          fprintf(stderr, "ERROR: Symbol name length is zero\naborting...\n");
          exit(1);
         }
        }
        else
        {
         strcpy(sym_name,"??? ??? 0 ");
        }
       }
       else
       {
        strcpy(sym_name,"??? ??? 0 ");
       }
       if(gzprintf(outfile, "%s %d\n", sym_name, jt->second) < (int)strlen(sym_name))
       {
        fprintf(stderr, "ERROR: gzputs err: %s\n", gzerror(outfile, &err));
        exit(1);
       }
      }
     }
    }
    else
    {
     fprintf(stderr, "ERROR: Could not open file: %s\naborting...\n", filename);
     exit(1);
    }
    gzclose(outfile);
   }
  }

   // reserved for future use
  void PerfMonService::postBeginJob()
  {
    
  }

   // reserved for future use
  void PerfMonService::postEndJob()
  {
      
  }

   // reserved for future use
  void PerfMonService::preEventProcessing(const edm::EventID& iID, const edm::Timestamp& iTime)
  {
      
  }

   // postEventProcessing()
   // just used to count how many events have already been processed
  void PerfMonService::postEventProcessing(const Event& e, const EventSetup&)
  {
   ph_ev_count++;   
  }

   // preModule()
   // const ModuleDescription& desc : description of the module that is just starting its execution (we are only interested in its name)
   // after the predefined event count ("ph_ev_count") has passed, calls the starter of the monitoring service: 
   // startpm() in the case of counting or start_smpl() in the case of sampling
  void PerfMonService::preModule(const ModuleDescription& desc)
  {
   if(ph_ev_count>=start_at_event && !sampling)
   {
    startpm();
   }
   else if(ph_ev_count>=start_at_event && sampling)
   {
    start_smpl(desc);
   }
  }

   // postModule()
   // const ModuleDescription& desc : description of the module that is just starting its execution (we are only interested in its name)
   // after the predefined event count ("ph_ev_count") has passed, calls the stopper of the monitoring service: 
   // stoppm() in the case of counting or stop_smpl() in the case of sampling
  void PerfMonService::postModule(const ModuleDescription& desc)
  {
   if(ph_ev_count>=start_at_event && !sampling)
   {
    stoppm(desc);
   }
   else if(ph_ev_count>=start_at_event && sampling)
   {
    stop_smpl(desc);
   }
  }
 
   // registers the service "PerfMonService"
  DEFINE_FWK_SERVICE (PerfMonService);
 }
}

