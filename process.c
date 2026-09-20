#include "process.h"

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

int enum_process(Process procs[]){
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");

    if (dir == NULL){
        perror("Unable to open /proc");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    while ((entry = readdir(dir)) != NULL){
        if (!is_num_string(entry->d_name))
            continue;

        char sta_file[64];
        char cmd_file[64];

        snprintf(sta_file, sizeof(sta_file),
                 "/proc/%s/status", entry->d_name);

        snprintf(cmd_file, sizeof(cmd_file),
                 "/proc/%s/cmdline", entry->d_name);

        int sta_fd = open(sta_file, O_RDONLY);
        int cmd_fd = open(cmd_file, O_RDONLY);

        if (sta_fd < 0 || cmd_fd < 0){
            if (sta_fd >= 0)
                close(sta_fd);
            if (cmd_fd >= 0)
                close(cmd_fd);
            continue;
        }

        procs[i].pid = atoi(entry->d_name);

        char lines[8192];

        int tt = read(sta_fd, lines, sizeof(lines) - 1);

        if (tt <= 0){
            close(sta_fd);
            close(cmd_fd);
            continue;
        }

        lines[tt] = '\0';

        char *p = strstr(lines, "PPid:");
        char *sta = strstr(lines, "State:");
        char *th = strstr(lines, "Threads:");
        char *vm = strstr(lines, "VmSize:");

        if (p != NULL)
            sscanf(p, "PPid: %d", &procs[i].ppid);

        if (sta != NULL)
            sscanf(sta, "State: %c", &procs[i].state);

        if (th != NULL)
            sscanf(th, "Threads: %d", &procs[i].threads);

        if (vm != NULL)
            sscanf(vm, "VmSize: %d", &procs[i].vmsize);

        tt = read(cmd_fd,
                  procs[i].cmdline,
                  sizeof(procs[i].cmdline) - 1);

        if (tt > 0)
            procs[i].cmdline[tt] = '\0';
        else
            procs[i].cmdline[0] = '\0';

        close(sta_fd);
        close(cmd_fd);

        i++;
    }

    closedir(dir);

    return i;
}

void fetch_proc_info(Process procs[], int count){
    printf("PID\t\tPPID\t\tSTATE\t\tTHREADS\t\t"
           "VMSIZE(Kb)\t\tCMDLINE\n");

    for (int i = 0; i < count; i++){
        printf("%d\t\t%d\t\t%c\t\t%d\t\t"
               "%d\t\t%s\n",
               procs[i].pid,
               procs[i].ppid,
               procs[i].state,
               procs[i].threads,
               procs[i].vmsize,
               procs[i].cmdline);
    }
}
