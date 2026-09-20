#include <stdio.h>
#include "tree.h"
#include "process.h"
void print_proc_name(Process proc){
    printf("%s", proc.cmdline);
}

void write_procs(Process proc[], int count, int parent_pid, int depth){
    for (int i = 0; i < count; i++){
        if (proc[i].ppid == parent_pid){
            for (int j = 0; j < depth; j++)
                printf("    ");
            printf("|-- ");
            print_proc_name(proc[i]);
            printf(" (PID %d)\n", proc[i].pid);
            write_procs(proc, count, proc[i].pid, depth + 1);
        }
    }
}

void write_tree(Process proc[], int count){
    for (int i = 0; i < count; i++){
        if (proc[i].ppid == 0){
            print_proc_name(proc[i]);
            printf(" (PID %d)\n", proc[i].pid);
            write_procs(proc, count, proc[i].pid, 1);
        }
    }
}
