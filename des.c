#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
int is_num_string(const char *str){
    if (str == NULL || *str == '\0')
        return 0;

    while (*str != '\0'){
        if (!isdigit((unsigned char)*str))
            return 0;
        str++;
    }

    return 1;
}

void fetch_proc_info(){
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");

    if (dir == NULL){
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    printf("PID\t\tPPID\t\tSTATE\t\tTHREADS\t\t"
           "VMSIZE(Kb)\t\tCMDLINE\n");

    while ((entry = readdir(dir)) != NULL){
        if (!is_num_string(entry->d_name))
            continue;

        char sta_name[50];
        char cmdline_name[50];

        snprintf(cmdline_name, sizeof(cmdline_name),
                 "/proc/%s/cmdline", entry->d_name);

        snprintf(sta_name, sizeof(sta_name),
                 "/proc/%s/status", entry->d_name);

        int fd = open(sta_name, O_RDONLY);
        int cmd_fd = open(cmdline_name, O_RDONLY);



        if (cmd_fd == -1)
            continue;
        if (fd == -1)
            continue;
        char cmdline[1024];
        char line[8192];

        int cmdtt = read(cmd_fd, cmdline, sizeof(cmdline) - 1);
        int tt = read(fd, line, sizeof(line) - 1);

        if (cmdtt > 0) {
            cmdline[cmdtt] = '\0';
        }

        if (tt > 0){
            cmdline[cmdtt] = '\0';
            line[tt] = '\0';

            char *sta = strstr(line, "State:");
            char *p = strstr(line, "PPid:");
            char *th = strstr(line, "Threads:");
            char *vm = strstr(line, "VmSize:");

            if (p != NULL &&
                sta != NULL &&
                th != NULL &&
                vm != NULL)
            {
                int ppid;
                int threads;
                char state;
                int vmsize;

                if (sscanf(vm, "VmSize:\t\t%d", &vmsize) == 1 &&
                    sscanf(th, "Threads:\t\t%d", &threads) == 1 &&
                    sscanf(p, "PPid:\t%d", &ppid) == 1 &&
                    sscanf(sta, "State:\t%c", &state) == 1)
                {
                    printf("%s\t\t%d\t\t%c\t\t%d\t\t%d\t\t%s\n",
                           entry->d_name,
                           ppid,
                           state,
                           threads,
                           vmsize,
                           cmdline);
                }
            }
        }
        close(fd);
        close(cmd_fd);
    }
    closedir(dir);
}

int main(){
    fetch_proc_info();

    return 0;
}

