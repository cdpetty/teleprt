#include <stdio.h>
#include <dirent.h>
#include <string.h>

char* getComponent(char component[], char* path){
    int iterator = 0;
    for(iterator; *path != '/' && *path; component[iterator] = *path++, iterator++){}
    component[iterator] = '\0';
    if (*path == '\0'){
        return path;
    } else {
        return ++path;
    }
}

int doesMatch(char component[], char* name){
    char* tmp = component;
    return strncmp(tmp, name, strlen(tmp));
}

char* getDir(char component[], char* name){

    DIR *directory;
    struct dirent *de;

    if (!(directory = opendir(name))){
//        printf("Error opening directory");
    }

    while (de = readdir(directory)){
        if (de->d_type == DT_DIR) {
            if (doesMatch(component, de->d_name) == 0) {
                closedir(directory);
                return de->d_name;
            }
        }
    }

}

int main(int argc, char **argv, char **envp){

    char component[30];
    char* path = *(++argv);

    char realPath[70];
    bzero(realPath, sizeof(realPath));
    if (*path == '/'){
        strcat(realPath, "/.");
        path++;
    } else {
        strcat(realPath, ".");
    }

    while (*path) {
        path = getComponent(component, path);
        strcat(realPath, "/");
        strcat(realPath, getDir(component, realPath));
    }

    puts(realPath);
    return 0;
}
