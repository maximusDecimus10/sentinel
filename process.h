#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct
{
    int pid;
    int ppid;
    char state;
    int threads;
    int vmsize;
    char cmdline[1024];
} Process;

int enum_process(Process procs[]);
void fetch_proc_info(Process procs[], int count);

#endif
