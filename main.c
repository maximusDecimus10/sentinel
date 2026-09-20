#include "process.h"
#include "tree.h"
int main(int argc, char* argv[]){
    if(argc<2) return -1;
    Process proc[1024];
    int count = enum_process(proc);
    if(strncmp(argv[1], "proc", 4)==0){
        fetch_proc_info(proc, count);
    }else if(strncmp(argv[1], "tree", 4)==0){
        write_tree(proc, count);
    }else{
        printf("There is no argument like %s.\n Try -h or --help to explore all the functionalities.\n", argv[1]);
    }
    return 0;
}
