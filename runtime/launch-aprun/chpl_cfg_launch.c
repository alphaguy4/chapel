#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"

#define baseSysFilename ".chpl-sys-"
char sysFilename[FILENAME_MAX];


// TODO: Un-hard-code this stuff:

static int getNumCoresPerLocale(void) {
  FILE* sysFile;
  int coreMask;
  int bitMask = 0x1;
  int numCores;
  pid_t mypid;
  char* numCoresString = getenv("CHPL_LAUNCHER_CORES_PER_LOCALE");

  if (numCoresString) {
    numCores = atoi(numCoresString);
    if (numCores != 0)
      return numCores;
  }

#ifndef DEBUG_LAUNCH
  mypid = getpid();
#else
  mypid = 0;
#endif
  sprintf(sysFilename, "%s%d", baseSysFilename, (int)mypid);
  char* command = chpl_glom_strings(2, "cnselect -Lcoremask > ", sysFilename);
  system(command);
  sysFile = fopen(sysFilename, "r");
  if (fscanf(sysFile, "%d\n", &coreMask) != 1 || !feof(sysFile)) {
    chpl_error("unable to determine number of cores per locale; please set CHPL_LAUNCHER_CORES_PER_LOCALE", 0, 0);
  }
  coreMask >>= 1;
  numCores = 1;
  while (coreMask & bitMask) {
    coreMask >>= 1;
    numCores += 1;
  }
  fclose(sysFile);
  sprintf(command, "rm %s", sysFilename);
  system(command);
  return numCores;
}

char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  sprintf(baseCommand, "aprun %s -d%d -n%d -N1 %s_real", 
          ((verbosity < 2) ? "-q" : ""), getNumCoresPerLocale(), numLocales, 
          argv[0]);

  size = strlen(baseCommand) + 1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_malloc(size, sizeof(char*), "aprun command buffer", -1, "");
  
  sprintf(command, "%s", baseCommand);
  for (i=1; i<argc; i++) {
    strcat(command, " '");
    strcat(command, argv[i]);
    strcat(command, "'");
  }

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}

void chpl_launch_sanity_checks(int argc, char* argv[], const char* command) {
  // Do sanity checks just before launching.
  struct stat statBuf;
  char realName[256];
  int retVal;

  retVal = snprintf(realName, 256, "%s_real", argv[0]);
  if (retVal < 0 || retVal >= 256) {
    chpl_internal_error("error generating back-end filename");
  }

  // Make sure the _real binary exists
  if (stat(realName, &statBuf) != 0) {
    char errorMsg[256];
    sprintf(errorMsg, "unable to locate file: %s", realName);
    chpl_error(errorMsg, -1, "<internal>");
  }
}

void chpl_launch_cleanup(void) {
}
