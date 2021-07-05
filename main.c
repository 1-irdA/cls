#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PARENT ".."
#define CURRENT "."

DIR* open_dir(char *path);
int is_dir_arg(char *name);
void walk(char *name);
int is_dir(struct dirent *entry);
int check_dir_name(char *name);
char * build_fullname(char *cur_full_name, char *base_name, char *entry_name);

int main(int argc, char **argv) {
        
    if (argc == 1) {
        walk(".");
    } else if (argc == 2 && is_dir_arg(argv[1])) {
    	walk(*++argv);
    }

    return 0;
}

int is_dir_arg(char *name) {
    
    struct stat info;

    if ((stat(name, &info)) == -1) {
        fprintf(stderr, "error during access to %s\n", name);
        return 1;
    }

    return S_ISDIR(info.st_mode);
}


DIR* open_dir(char * path) {
    
    DIR *folder = NULL;

    if ((folder = opendir(path)) == NULL) {
        perror("Unable to open dir");
    }

    return folder;
}

void walk(char *name) {

    struct dirent *entry = NULL;
    DIR *dir = open_dir(name);
    char *fullname = NULL;

    while ((entry = readdir(dir))) {
        fullname = (char *) malloc((strlen(entry->d_name)
				+ strlen(name)
				+ 2) * sizeof(char));
	fullname = build_fullname(fullname, name, entry->d_name);
        if (is_dir(entry)) {
	    printf("d - %s\n", entry->d_name);
	    walk(fullname);
	} else if (check_dir_name(entry->d_name) && entry->d_name[0] != '.') {
	    printf("f - %s\n", entry->d_name);
	}
    }
}

int check_dir_name(char *name) {
    return strcmp(name, PARENT) != 0
	    && strcmp(name, CURRENT) != 0;
}

int is_dir(struct dirent *entry) {
    return entry->d_type == DT_DIR 
	    && check_dir_name(entry->d_name)
	    && entry->d_name[0] != '.';
}

char * build_fullname(char *cur_fullname, char *base_name, char *entry_name) {
    strcat(cur_fullname, base_name);
    strcat(cur_fullname, "/");
    return strcat(cur_fullname, entry_name);
}
