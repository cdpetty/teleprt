#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

char* getComponent(char component[], char* path){
    int iterator;
    char* pathPointer = path;
    for(iterator = 0; *pathPointer != '/' && *pathPointer; iterator++, pathPointer++);
//    printf("Iterator: %d\n", iterator);
//    strncpy(component, path, iterator);
    memcpy(component, path, iterator);
    component[iterator] = '\0';
//    printf("Component: %s\n", component);
//    fflush(0);
    return (*pathPointer == '\0') ? pathPointer : ++pathPointer;
//    if (*pathPointer == '\0'){
//        printf("returning pathpointer: %s\n", pathPointer);
//        return pathPointer;
//    } else {
//        printf("returning pathpointer: %s\n", ++pathPointer);
////        return ++pathPointer;
//        return pathPointer;
//    }
}

//char* getComponent(char component[], char* path){
//    int iterator;
//    for(iterator = 0; *path != '/' && *path; component[iterator] = *path++, iterator++){}
//    component[iterator] = '\0';
//    if (*path == '\0'){
//        return path;
//    } else {
//        return ++path;
//    }
//}

//char* getComponent(char component[], char* path){
////    printf("Called get component with compoentn %s and path %s", component, path);
//    memccpy(component, path, '/', strlen(path));
//    component[strlen(component)-1] = '\0';
////    printf("\ncomponent: %s\n", component);
//    return (*(path + strlen(component))) ? path + strlen(component) + 1 : path + strlen(component);
//}


int doesMatch(char component[], char* name){
    char* tmp = component;
    return strncmp(tmp, name, strlen(tmp));
}

char* getDir(char component[], char* name){

    DIR *directory;
    struct dirent *de;

    if (!(directory = opendir(name))){
        fputs("TP error: cannot open directory", stderr);
        exit(1);
    }

    while ( (de = readdir(directory)) ){
        if (de->d_type == DT_DIR) {
//            puts(de->d_name);
            if (doesMatch(component, de->d_name) == 0) {
                closedir(directory);
                return de->d_name;
            }
        }
    }
    return NULL;
}

int main(int argc, char **argv, char **envp){

    if (argc == 1){
        puts(getenv("HOME"));
    } else {
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
            char* newDir = getDir(component, realPath);
            if (newDir){
                strcat(realPath, newDir);
            } else {
                puts(".");
                return 0;
            }
        }

        puts(realPath);
    }
    return 0;
}
