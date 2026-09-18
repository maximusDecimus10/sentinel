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
    while (*str != '\0') {
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

    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    printf("PID\t\tPPID\n");

    while ((entry = readdir(dir)) != NULL) {
        if (!is_num_string(entry->d_name))
            continue;
        char name[50];
        snprintf(name, sizeof(name),
                 "/proc/%s/status", entry->d_name);
        int fd = open(name, O_RDONLY);
        if (fd == -1)
            continue;
        char line[256];
        int tt = read(fd, line, sizeof(line) - 1);
        if (tt > 0) {
            line[tt] = '\0';
            char *p = strstr(line, "PPid:");
            if (p != NULL) {
                int ppid;
                if(sscanf(p, "PPid:\t%d", &ppid)==1){printf("%s\t\t%d\n", entry->d_name, ppid);}
            }
        }
        close(fd);
    }
    closedir(dir);
}
int main(){
    fetch_proc_info();
    return 0;
}
